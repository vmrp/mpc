/*
简单双向链表文件浏览
by:每天进步一点
1键输出列表、0直接键退出、
不足之处还望见凉！
*/

#include <base.h>

#define FONT 1
//字体
#define MAXLEN 250
//搜索文件名最大长度

#define MAXFILENUM 300
//搜索文件最大数

#define MoveTime 1
//长按移动时间间隔

int tw,th;
textwh("口",FALSE,FONT,&tw,&th);
//获取文字宽高

colorst TextColor;//文字颜色
TextColor.r=250;
TextColor.g=250;
TextColor.b=250;

colorst BgColor;//背景色
BgColor.r=40;
BgColor.g=20;
BgColor.b=40;


void text(char *str,int16 x,int16 y)
{
 dtext(str,x,y,TextColor.r,TextColor.g,TextColor.b,FALSE,FONT);
}

//文件链表结构
typedef struct listst
{
 struct listst *last;//上1个
 struct listst *next;//下1个
 char *name;//文件名
 uint8 mode;//文件类型
 uint32 len;//文件长度
}LISTST;
uint32 ListLen=sizeof(LISTST);//文件键表1个结点的长度

LISTST *First,*End;//前目录文件链表开头、结尾

struct 
{
 LISTST *now;//当前结点
 LISTST *one;//当前前屏幕第1个结点
 char *dir;//当前目录
 uint32 all;//目录文件数
 uint32 num;//当前文件数
 uint32 onenum;//当前屏幕第1个结点文件数
}Focus;
 Focus.dir="";
int32 timer=timercreate();//长按定时器句柄

uint16 TextH=th*2;//一个文件名距形框高
uint16 Textx=tw;//文件名x
uint16 Texty=th*2;//文件名y
uint16 HowH=SCRH-th*2;//屏幕文件列表距形框高
uint32 HowDraw=HowH/TextH;//显示文件数

//初使化键表头
void ListStart()
{
 if(First!=NULL)
 free(First);
 First=malloc(ListLen);
 memset(First,NULL,ListLen);
 
}

//搜索目录
void FindPath()
{
 ListStart();
 LISTST *to=First;

 char *buffer=malloc(MAXLEN+ListLen);

 uint32 f=findstart(Focus.dir,buffer,MAXLEN);
 findnext(f,buffer,MAXLEN);

 while(Focus.all<=MAXFILENUM)
 {
 memset(buffer,NULL,MAXLEN);
 if(-1==findnext(f,buffer,MAXLEN))
 break;

//新增1个结点
 LISTST *one;
 one=malloc(ListLen);
 memset(one,NULL,ListLen);

//搜索结果写入键表
 uint32 len=strlen(buffer)+ListLen;
 one->name=malloc(len);
 strncpy(one->name,buffer,len);
//获取文件类型、长度
 char *str=malloc(strlen(Focus.dir)+strlen(one->name)+2);
 sprintf(str,"%s/%s",Focus.dir,one->name);
 one->mode=filestate(str);
 if(one->mode==IS_FILE)
 one->len=getlen(str);
 else if(one->mode==IS_DIR)
 one->len=strlen(str);
 free(str);

//连接链表
 one->last=to;
 to->next=one;
 to=to->next;
 Focus.all++;
 }
 findstop(f);
 free(buffer);

 End=to;
}

void FindFile()
{
 ShowMsg("Loading...");
 Focus.num=Focus.all=0;

 FindPath();

 if(Focus.all<1)
 {
  ShowMsg("空文件夹");
  sleep(500);
  event(KY_DOWN,_SRIGHT,NULL);
  return;
 }
 Focus.num=1;
 First=First->next;
 DelPoint(First->last);
 Focus.now=First;
}

//画列表
void DrawList()
{
 if(Focus.all<=0)
 return;
//绘背景
 cls(BgColor.r,BgColor.g,BgColor.b);
 shaderect(0,0,SCRW,th,52643440,0,0);
 shaderect(0,SCRH-th,SCRW,th,52643440,0,0);

//绘目录或文件长度信息
 if(Focus.now->mode==IS_DIR)
 {
  if(strlen(Focus.dir)>1)
 text(Focus.dir,0,0);
  else text("File View",0,0);
 }
 else if(Focus.now->mode==IS_FILE)
 {
 char lenStr[16];
 sprintf(lenStr,"%d.%02dkb",Focus.now->len/1024,Focus.now->len%1024);
 text(lenStr,0,0);
 }
//绘 当前文件数/总文件数 信息
  char Str[13];
  sprintf(Str,"%d/%d",Focus.num,Focus.all);
  text(Str,SCRW-strw(Str),0);
  text("打开",0,SCRH-th);
  text("返回",SCRW-tw*2,SCRH-th);
 //DrawTime((SCRW-tw*2)/2,SCRH-th,TextColor.r,TextColor.g,TextColor.b);

//调整光标显示
 switch(1)
 {
  case (Focus.num==1):
  Focus.onenum=Focus.num;
  Focus.one=Focus.now;
  break;
  case (Focus.num==Focus.all):
 Focus.onenum=Focus.num;
 Focus.one=Focus.now;
 while(Focus.onenum+HowDraw-1!=Focus.all)
  {
  if(Focus.one->last!=NULL)    Focus.one=Focus.one->last;
  else break;
  Focus.onenum--;
  }
  break;
  case (Focus.onenum+HowDraw==Focus.num):
  Focus.onenum++;
  Focus.one=Focus.one->next;
  break;
  case (Focus.num==Focus.onenum-1):
  Focus.onenum=Focus.num;
  Focus.one=Focus.now;
  break;
 }

//画文件列表
 LISTST *to=Focus.one;
 uint32 n;

 rectst rect;
 rect.x=Textx;
 rect.y=th+th/2;
 rect.w=SCRW-10;
 rect.h=th*3;

 while(n<HowDraw)
 {
  if(to==Focus.now)
  {
  shaderect(0,rect.y,rect.w,TextH,13158600,3289850,0);
  dtextex(to->name,rect.x,rect.y,&rect,&TextColor,2,FONT);
  }
 else
 text(to->name,rect.x,rect.y);
 effsetcon(rect.x,rect.y+TextH,rect.w,1,128,128,128);
 rect.y+=TextH;n++;
 if(to->next!=NULL)
 to=to->next;
 else break;
 }

//右侧进度条
 uint32 zx=SCRW-10,zy,zw=10,zh;
 if(Focus.all>NULL)
 zh=HowH/Focus.all;
 zy=Focus.num*zh+th-zh;
 drect(zx,th,1,HowH,0,0,0);
 shaderect(zx+1,zy,zw,zh,179250,216640,1);

 ref(0,0,SCRW,SCRH);
}

//输出文件列表
void PrintList()
{
 printf("\n当前目录: %s\n",Focus.dir);
 LISTST *to=First;
 uint32 n=1;
 while(to!=NULL)
 {
  printf("%d.%s,len=%d\n",n,to->name,to->len);
  to=to->next;
  n++;
 }
}

//删除整个文件链表
void DelAll()
{
 LISTST *now=First;
 while(now->next!=NULL)
 {
  now=now->next;
  DelPoint(now->last);
 }
}

void DelPoint(LISTST *one)
{
 free(one->name);
 free(one);
 one=NULL;
}

//入口涵数
int init()
{
 FindFile();
 DrawList();
 return 0;
}

//按键入口函数
int event(int type, int p1, int p2)
{
 if(KY_DOWN==type)
 switch(p1)
  {
 case _0:
 exitapp();break;

 //输出列表
 case _1:
 ShowMsg("请稍候…");
 PrintList();
 ShowMsg("完成");
 sleep(500);
 DrawList();
 break;

 //打开目录
 case _SLEFT:
 case _SELECT:
 if(Focus.now->mode==IS_DIR)
 {
 uint32 len=strlen(Focus.dir)+strlen(Focus.now->name)+2;
 char *str=malloc(len);
 sprintf(str,"%s/%s",Focus.dir,Focus.now->name);
 if(Focus.dir!=NULL)
 free(Focus.dir);
 Focus.dir=str;

 DelAll();
 FindFile();
 DrawList();
 }
 else
 {
  ShowMsg("不是文件夹");
  sleep(500);
  DrawList();
 }
 break;

//返回上级目录、若无上级目录则退出
 case _SRIGHT:
 if(Focus.dir=="")
 exitapp();
 char *str=strrchr(Focus.dir,'/');
 if(str!=NULL)
 if(strlen(Focus.dir)>1)
 {
 uint32 l=strlen(str);
 memset(Focus.dir+strlen(Focus.dir)-l,NULL,l);

 DelAll();
 FindFile();
 DrawList();
 }
 else exitapp();
 break;

 case _UP:
 case _DOWN:
 LongMove(p1);
 timerstart(timer,MoveTime,p1,"LongMove",1);
 break;
 }
 else if(type==KY_UP)
 switch(p1)
 {
  case _UP:
  case _DOWN:
  timerstop(timer);
  break;
 }
 return 0;
}

//平台暂停调用
int pause(void)
{
 return 0;
}
//平台恢复调用
int resume(void)
{
    return 0;
}

//退出
void exitapp()
{
 ShowMsg("退出中…");
 timerdel(timer);
 free(Focus.dir);
 DelAll();
 exit();
}

void LongMove(int p1)
{
 if(Focus.all<1)
 return;

 switch(p1)
 {
  case _UP:
  if(Focus.num>1)
  {
   Focus.num--;
   Focus.now=Focus.now->last;
  }
  else
  {
   Focus.num=Focus.all;
   Focus.now=End;
  }
  break;

  case _DOWN:
  if(Focus.num<Focus.all)
 {
  Focus.num++;
  Focus.now=Focus.now->next;
 }
 else
 {
  Focus.num=1;
  Focus.now=First;
 }
  break;
 }

 DrawList();
}


/*绘消息提示*/
void ShowMsg(char *Msg)
{
 int w,h;uint16 x,y;
 textwh(Msg,FALSE,FONT,&w,&h);
 if(w<SCRW)x=(SCRW-w)/2;
 y=(SCRH-h)/2;
 effsetcon(x,y,w,h,50,50,50);
 dtext(Msg,x,y,255,255,250,FALSE,FONT);
 ref(x-5,y-5,w+10,h+10);
}

//获取字串宽
int strw(char *str)
{
 int w,h;
 textwh(str,FALSE,FONT,&w,&h);
 return w;
}

//获取字串高度
int strh(char *str)
{
 int w,h;
 textwh(str,FALSE,FONT,&w,&h);
 return h;
}


typedef struct
{
 uint16 year;
 uint8  month;
 uint8  day;
 uint8  hour;
 uint8  minute;
 uint8  second;
} datetime;

//绘时间
void DrawTime(int16 x,int16 y,uint8 r,uint8 g,uint8 b)
{
 datetime date;
 getdatetime(&date);
 char da[7];
 memset(da,NULL,7);
 sprintf(da,"%d:%02d",date.hour,date.minute,date.second);
 dtext(da,x,y,r,g,b,FALSE,FONT);
}
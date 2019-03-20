#include <c/FileManage/ttFileList.h>

//列表长按移动间隔时间(ms)
#define LMTime 1

//字体
#define FFONT 1

int
TextH=th,TextN=(SCRH-th*3)/TextH-1,//文件列表行数
KB=1024,//1KB大小
LTimer;//列表长按定时器句柄

FileList List;//列表

FilePoint One=0;//屏幕第一个文件结点
uint32 OneN=0;//屏幕第一个文件编号

//文件列表入口
void ListInit()
{
 LTimer=timercreate();
 List=FileListCreate();
 FindInit();
}

//搜索文件入口函数
void FindInit()
{
 ShowMsg("loading...",0);
 if(List->FileN!=0)
 FilePointAllDel(List->first);
 FindPath(List);

 if(List->FileN==0)
 {
 if(List->path=="")
 ExitApp();
 else
  {
  Show("空文件夹！",0);
  sleep(500);
  LastPath();
  }
 }
 else
 {
 DrawFMain();
 DrawFileList();
 }
}

//列表退出
void ExitList()
{
 timerdel(LTimer);
 FileListDel(List);
}

//文件浏览按键入口函数
int FVEvent(int type,int p1,int p2)
{
 if(type==KY_DOWN)
 switch(p1)
 {
 case _SRIGHT:
 LastPath();
 break;

 case _SELECT:
 if(List->now->type==Is_Dir)
 NextPath();
 else
 NextOpen();
 break;

 case _UP:
 case _DOWN:
 LMove(p1);
 timerstart(LTimer,LMTime,p1,"LMove",TRUE);
 break;
 }

 else
 if(type==KY_UP)
 switch(p1)
 {
 case _UP:
 case _DOWN:
 timerstop(LTimer);
 break;
 }
 return 0;
}


void DrawText(void *Text,uint16 x,uint16 y)
{
 dtext(Text,x,y,0,0,0,FALSE,FFONT);
}

void DrawBase(uint16 y,void *str,void *str2,void *str3)
{
 if(str!="")
 DrawText(str,0,y);
 if(str2!="")
 DrawText(str2,(SCRW-strwh(str2,1))>>1,y);
 if(str3!="");
 DrawText(str3,SCRW-strwh(str3,1),y);
}

void DrawFMain()
{
  srect(0,0,SCRW,th,148,182,255,210,210,210,0);
 drect(0,th-1,SCRW,1,0,0,0);
 srect(0,SCRH-th,SCRW,th,210,210,210,148,182,255,0);
 drect(0,SCRH-th,SCRW,1,0,0,0);

 DrawText("FileManage",0,0);
 DrawTime(SCRW-tw*2-5,0);
 DrawBase(SCRH-th,"菜单","打开","返回");
}

void DrawFileList()
{
 char str[7];
 drect(0,th,SCRW,SCRH-th*2,255,255,255);
 drect(0,th,SCRW,th,100,100,230);
 drect(0,th*2,SCRW,1,0,0,0);
 sprintf(str,"%d/%d",List->NowN,List->FileN);
 DrawBase(th,List->path,"",str);

 //调整光标显示
 switch(1)
 {
  case (List->NowN==1):
  OneN=List->NowN;
  One=List->now;
  break;
  case (List->NowN==List->FileN):
 OneN=List->NowN;
 One=List->now;
 while(OneN+TextN-1!=List->FileN)
  {
  if(One->prev!=NULL)
  One=One->prev;
  else break;
  OneN--;
  }
  break;
  case (OneN+TextN==List->NowN):
  OneN++;
  One=One->next;
  break;
  case (List->NowN==OneN-1):
  OneN=List->NowN;
  One=List->now;
  break;
 }

 FilePoint to=One;
 uint16 y=th*2,N=0;

 while(N<TextN)
 {
 bmpshowflip(Bmp,0,y,16,16,16,BM_TRANS,0,16*(to->type),0xF81F);
 DrawText(to->name,16,y);

 if(to==List->now)
 {
 shaderect(0,y,SCRW,th*2,0x55a0ff,0x0055ff,0);
 bmpshowflip(Bmp,0,y,16,16,16,BM_TRANS,0,16*(to->type),0xF81F);
 dtext(to->name,16,y,255,255,255,FALSE,FFONT);
 char lenStr[14];
 memset(lenStr,0,14);
 if(List->now->type!=Is_Dir)
 sprintf(lenStr,"%d.%02dKB",to->len/KB,to->len%KB);
 DrawBase(y+th,GetFileKindStr(to->type),"",lenStr);
 y+=(TextH<=th?th:0);
 }
 if(to->next!=NULL)
 to=to->next;
 else
 break;

 N++;
 y+=TextH;
 }
 ref(0,0,SCRW,SCRH);
}


//列表焦点移动
void LMove(int p1)
{
 switch(p1)
 {
  case _UP:
  if(List->NowN>1)
  {
   List->NowN--;
   List->now=List->now->prev;
  }
  else
  {
   List->NowN=List->FileN;
   List->now=List->last;
  }
  break;

  case _DOWN:
  if(List->NowN<List->FileN)
 {
  List->NowN++;
  List->now=List->now->next;
 }
 else
 {
  List->NowN=1;
  List->now=List->first;
 }
  break;
 }

 DrawFileList();
}

//返回上级目录
void LastPath()
{
 if(strlen(List->path)<=1)
 ExitApp();

 char *str=strrchr(List->path,'/');
 if(str!=NULL)
 if(strlen(List->path)>1)
 {
 uint32 l=strlen(str);
 memset(List->path+strlen(List->path)-l,NULL,l);
 FindInit();
 }
 else
 ExitApp();
}

//将3个字符串合成一个,返回合成后的字符串指针，申请的内存需手动释放
void* str3to1(void *str,void *str2,void *str3)
{
 int len=strlen(str)+strlen(str2)+strlen(str3)+1;
 void *ptr=(void*)malloc(len);
 memset(ptr,NULL,len);
 memcpy(ptr,str,len);
 strcat(ptr,str2);
 strcat(ptr,str3);
 return ptr;
}

//进入下级目录
void NextPath()
{
 char *name=str3to1(List->path,"/",List->now->name);

 if(List->now->type==Is_Dir)
 {
 free(List->path);
 int len=strlen(name);
 List->path=(char*)malloc(len);
 memset(List->path,NULL,len);
 strcpy(List->path,name);
 FindInit();
 }
 free(name);
}

//选择文件后的操作
void NextOpen()
{
 char *name=str3to1(List->path,"/",List->now->name);
 int8 is=0;

 switch(List->now->type)
 {
 case Is_Not:
 case Is_Zip:
 ShowMsg("暂不支持！",0);
 BackFV();
 break;
/*
 case Is_Photo:
 Jm=Jm_PV;
 is=PVInit(name);
 break;

 case Is_Book:
 Jm=Jm_BV;
 is=BVInit(name);
 break;

 case Is_Music:
 Jm=Jm_MP;
 is=MPInit(name);
 break;
*/
 case Is_Mrp:
 runmrp(name,"start.mr");
 break;
 }
 free(name);

 if(-1==is)
 {
 ShowMsg("打开失败！",0);
 sleep(500);
 BackFV();
 }
}

//返回文件浏览
void BackFV()
{
 Jm=Jm_FV;
 DrawFMain();
 DrawFileList();
}

//By:天天
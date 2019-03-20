//wildly 自定义函数库
/*
新函数及说明
strint 将整数转为字符串
strdel 删除字符串中的任意字符，选自小钱。
getmrpinformation 获取mrp信息。
mrpinformation 更改mrp信息。
readmrpfile 读取mrp内部文件。
mrpunins 卸载mrp,删除mrp生成的文件夹及文件，和mrp。
函数库，优点，写代码更方便，快捷，功能多，给力。
QQ:319965592   wildly
*/


#ifndef MPC_WILDLY_H

#define MPC_WILDLY_H

//字体大小，自定义
#define FONT 0

#include <base.h>
//这两个不需解释
#define c cls(0,0,0)
#define r ref(0,0,SCRW,SCRH)

//剩余内存
#define MAIN maine()

//总内存
#define MEM mem()

#define Metime getuptime()

int Time_Create[20];

int Edit_Create[20];

int Menu_Create[20];

int Dlg_Create[20];

//获取手机已开机 时
#define Hou hour()

//获取手机已开机 分
#define Min minute()

//获取手机已开机 秒
#define Sec second()

//系统时间 年
#define Year _year()

//月
#define Month _month()

//日
#define Day _day()

//时
#define Hour _hour()

//分
#define Minute _minute()

//获取单个字符宽
#define Texw textw()

//获取单个字符高
#define Texh texth()

//时间结构体
typedef struct
{   uint16 year;
    uint8 month;
    uint8 day;
    uint8 hour;
    uint8 minute;
}mr_time;

mr_time date;

//部分函数所需要的内存，结构体。
typedef struct
{
   char *str;
   char *buf[2];
}buffer;

buffer strsc;

//main入口，定时器创建。
int init(void)
{MR_TimeCreate();
return main();}

//字符转换，str要转换的字符，is_uni 1 GB2312转为unicode字节 0 unicode转GB2312
void* gb(char *str,int is_uni)
{
 int err,a,b;
 a=wstrlen(str);
 if(!is_uni)
 {
 u2c(str,a,&str,&a);
 return str;
 }
 else
 return c2u(str,&err,&b);
}

//复制文件，name要复制的文件，name2复制后的文件，无复制大小， 成功返回 0 失败则 -1
int copy(char *name,char *name2)
{int Len,f,fa;
if(getlen(name)<=0)return -1;
f=open(name,1);
fa=open(name2,8);
seek(fa,0,SEEK_END);
void *buf=(char*)malloc(51200);
if(buf==NULL)return -1;
while(1)
{memset(buf,NULL,51200);
seek(f,Len,SEEK_SET);
uint32 Read=read(f,buf,51200);
write(fa,buf,Read);
Len+=Read;
if(Read<51200)break;}
close(fa);free(buf);
return close(f);}

//显示bmp图片，跟moc的格式一样，返回成功0，失败-1
int bmp(char *filename,int x,int y,int w,int wm,int h,int pm,int dx,int dy)
{int f,len; uint16 *ptr;
len=getlen(filename);
if(len<=0)return -1;
if(filestate(filename)!=IS_FILE)return -1;
ptr=malloc(len);
f=open(filename,1);
read(f,ptr,len);
bmpshowflip(ptr,x,y,w,wm,h,pm,dx,dy,*ptr);
free(ptr);
return close(f);}

//删除文件夹DirPath内所有文件及文件夹，包括DirPath，成为返回0，失败返回-1
int rmdirEX(char *DirPath)
{char tDirPath[255];
 char tName[64];
 char tPath[255];
 char *pPath;
 int f,Count = 0;
if(filestate(DirPath)==8)return -1;
strcpy(tDirPath,DirPath);
 while(TRUE)
 {
  f = findstart(tDirPath,tName,64);
  findnext(f,tName,64);
  while(TRUE)
  {
   if(findnext(f,tName,64) == -1)
    break;
   sprintf(tPath,"%s/%s",tDirPath,tName);
   if(IS_DIR == filestate(tPath))
   {
    if(rmdir(tPath))
    {Count++;
     findstop(f);
     strcpy(tDirPath,tPath);
     goto Next;
    }
   }else
   if(remove(tPath))
   {findstop(f);
    goto Stop;
   }
  }
  findstop(f);
  if(Count)
  {pPath = tDirPath;
   pPath = strrchr(pPath,'/');
   memset(pPath,0,strlen(pPath));
   Count--;
  }else break;
  Next:
 }
 Stop:
 int A_T=findstart("",tName,64);
 findstop(A_T);
 return rmdir(DirPath);
}

//画空心矩形，跟rect是一样的。
void rest(int x,int y,int w,int h,int r,int g,int b)
{
dline(x,y,x,y+h-1,r,g,b);
dline(x+1,y,x+w-2,y,r,g,b);
dline(x+w-1,y,x+w-1,y+h-1,r,g,b);
dline(x+1,y+h-1,x+w-2,y+h-1,r,g,b);
}

//画进度条，mes进度长，x,y方位，w边框进度宽,h边框进度高，r,g,b边框颜色，r2,g2,b2进度颜色
void rent(int mes,int x,int y,int w,int h,int r,int g,int b,int r2,int g2,int b2)
{int cm;
if(mes<=2)cm=0; else cm=2;
rest(x,y,w,h,r,g,b);
drect(x+1,y+1,mes-cm,h-2,r2,g2,b2);
}

int maine()
{return getmemremain();}

int mem()
{return getsysmem();}

//在str字符串里查找str2字符，str字符第一次出现str2字符时 则返回0 否则返回-1
int strch(char *str,char *str2)
{if(strstr(str,str2)==NULL)return -1;
return 0;}

//简化dtext，str字符，x,y坐标
int txt(char *str,int x,int y)
{dtext(str,x,y,255,255,255,0,FONT);
return 0;}

//跟MOC的drawtxt2是一样的，不用填字体
int text(char *str, int x, int y, int w ,int h ,int r , int g , int b)
{rectst rect;
rectst ect;
colorst color;
rect.x = x;
rect.y = y;
rect.w = w;
rect.h = h;
color.r = r;
color.g = g;
color.b = b;
dtextex(str,x,y,&rect,&color,2,FONT);
return 0;}


//画渐变矩形，r,g,b第一颜色 ，ra,ga,ba 第二颜色，Fen颜色方位。
void sect(int x, int y, int w, int h, int r, int g, int b, int ra, int ga, int ba, int Fen)
{int N1 = r*0x10000+g*0x100+b, N2=ra*0x10000+ga*0x100+ba;
shaderect(x,y,w,h,N1,N2,Fen);}

//显示整数，x,y坐标
void intxt(int mes,int x,int y)
{char str[255];
dtext(sprintf(str,"%d",mes),x,y,255,255,255,0,FONT);}

//获取手机CPU，例子：printf("%d.%dMhz",10000/cpu(),1000000/cpu()-10000/cpu()*100);，获取速度很慢。
int cpu()
{int a,b;
a=getuptime();
for(b=0;b<=10000;b++);
a=getuptime()-a;
return a;}

int hour()
{int chen=Metime/1000/60/60%60;
return chen;}

int minute()
{int chen=Metime/1000/60%60;
return chen;}

int second()
{int chen=Metime/1000%60;
return chen;}

int _year()
{if(getdatetime(&date))
return -1;
return date.year;}

int _month()
{if(getdatetime(&date))
return -1;
return date.month;}

int _day()
{if(getdatetime(&date))
return -1;
return date.day;}

int _hour()
{if(getdatetime(&date))
return -1;
return date.hour;}

int _minute()
{if(getdatetime(&date))
return -1;
return date.minute;}

//创建20个定时器
void MR_TimeCreate()
{int i;
for(i=0;i<20;i++)
{Time_Create[i]=timercreate();} }

//停止定时器，data定时器的序号。
void Stop(int data)
{timerstop(Time_Create[data]);}

//启动定时器，data定时器序号，sm定时器时长 单位，times定时器回调函数，type是否循环 0不循环 1循环
void timer(int data,int sm,char *times,int type)
{timerstart(Time_Create[data],sm,TRUE,times,type);}

     /****本地UI部分****/

//创建编辑框，set编辑框序号，Name编辑框名 类型GB2312，Open编辑框内容 类型GB2312，输入格式 0正常输入 1数字输入 2密码输入 ，Max字数限制。
int editc(int set,char *Name,char *Open,int Pes,int Max)
{Edit_Create[set]=editcreate(gb(Name,1),gb(Open,1),Pes,Max);
return Edit_Create[set];}

//获取编辑框内容，set要获取信息的编辑框序号，获取的信息类型：GB2312
void *getb(int set)
{void *EditGet = gb(editget(Edit_Create[set]),0); return EditGet;}

//销毁编辑框，SetCe要销毁的编辑框序号。
void edel(int SetCe)
{editdel(Edit_Create[SetCe]);}

//编辑框事件，可不加入event
void edit_event(int type,int p,int p2)
{if(type==MR_DIALOG)
{switch(p)
{case DLG_OK:
edit_ok(); break;
case DLG_CANCEL:
edit_cancel(); break;
} } }

//获取本插件长度 单位Byte
int stdiolen()
{return getlen("stdio.h");}

int textw()
{int w,h;
textwh("陈",0,0,&w,&h);
return w;}

int texth()
{int w,h;
textwh("陈",0,0,&w,&h);
return h;}

//创建菜单，Mes菜单序号，Name菜单标题、类型GB2312，MenuMax菜单项数量
void menuc(int Mes,char *Name,int MenuMax)
{
Menu_Create[Mes]=menucreate(gb(Name,1),MenuMax);
}

//菜单项，Mes菜单序号，Name菜单项标题、类型GB2312，Menuy菜单项顺序。
void mset(int Mes,char *Name,int Menuy)
{menuset(Menu_Create[Mes],gb(Name,1),Menuy);}

//释放菜单，Mes菜单序号
void mshow(int Mes)
{menushow(Menu_Create[Mes]);}

//销毁菜单，Mes菜单序号
void mdel(int Mes)
{menudel(Menu_Create[Mes]);}

//创建一个对话框，Mes对话框序号，text对话框内容、内容格式GB2312，type对话框类型、0有“确定”，1有“确定”、“返回”，2有“返回”。
void dlgc(int Mes,char *text,int type)
{Dlg_Create[Mes]=dlgcreate("",gb(text,1),type);}

//销毁对话框，Mes对话框序号
void gdel(int Mes)
{dlgdel(Dlg_Create[Mes]);}

//创建文件，filename文件名
int newfile(char *filename)
{int f=open(filename,8);
close(f);
return f;}

     /****本地UI部分结束****/

//方位确定,正返回0
int ifrect(int x,int y,int x2,int y2,int w,int h)
{
if(x>x2-w&x<=x2&y>y2-h&y<=y2)return 0;

return -1;}

//new进度条
void where(char *t,int b,int er,int h)
{ drect(0,SCRH/2,SCRW,h,200,80,100);
drect(0,SCRH/2+1,SCRW*b/er,h-2,20,20,190);
txt(t,0,SCRH/2); ref(0,SCRH/2,SCRW,35); 
}

//将整数转为字符串,str 转换的字符串
char *strint(int32 p)
{char n[200];
sprintf(n,"%d",p);
return n;
}

//删除text字符中，dt字符，通过返回值获取,该函数调用后调用freestrbuf()，或退出时调用freestrbuf()
char *strdel(char *text,char *dt)
{if(text==NULL)return NULL;
int len=strlen(text);
strsc.str = malloc(len)+1;
memset(strsc.str,0,len);
if(dt == NULL)return strsc.str;
char *b=strstr(text,dt);
int len2=strlen(dt),len3=strlen(b);
if(len==len2||b==NULL)return strsc.str;
free(strsc.str);
strsc.str = malloc(len-len2+2);
memset(strsc.str,0,len-len2+2);
memcpy(strsc.str,text,len-len3);
strcat(strsc.str,text+len-(len3-len2));
return strsc.str;
} 

      /****mrp部分****/

     //获取'更改，mrp信息，取值。
enum
{
   _MNAME = 0,     //mrp名
   _INTERNAL,     //mrp内部名
   _PAUTHOR,     //mrp作者
   _SERIALNUMBER, //mrp 序列号
   _MHELP      //mrp 帮助
};

//不调用。。。。。
void *mrpmalloc(uint8 s,int len)
{strsc.buf[s] = malloc(len);
if(strsc.buf[s] == NULL)return NULL;
return strsc.buf[s];
}

//获取mrp信息，file mrp文件，s 获取信息的取值, 反回值获取，该函数调用后调用freestrbuf()，或退出时调用freestrbuf() 
void *getmrpinformation(char *file,int s)
{int pys,lens;
if(filestate(file) != 1 || getlen(file) < 100)return NULL;
int f = open(file,1);
if(f == -1){close(f);return NULL;}
      switch(s)
       {
         case _MNAME:
         pys = 28; lens = 22;
        break;
         case _INTERNAL:
         pys = 16; lens = 11;
        break;
         case _PAUTHOR:
      pys = 88; lens = 37;
         break;
         case _SERIALNUMBER:
      pys = 52; lens = 15;
         break;
       case _MHELP:
      pys = 128; lens = 59;
          break;
}
if(mrpmalloc(0,lens) == NULL){close(f);return NULL;}
seek(f,pys,SEEK_SET);
read(f,strsc.buf[0],lens);
close(f);
return strsc.buf[0];
}

//更改mrp信息，file mrp文件，str 更改信息，s 更改取值。
int mrpinformationwrite(char *file,char *str,int s)
{int pys,lens;
if(filestate(file) != 1 || getlen(file) < 100)return -1;
int f = open(file,4);
if(f == -1){close(f);return -1;}
    switch(s)
     {
    case _MNAME:
    pys = 28; lens = 22;
    break;
    case _INTERNAL:
    pys = 16; lens = 11;
    break;
    case _PAUTHOR:
    pys = 88; lens = 37;
    break;
    case _SERIALNUMBER:
    pys = 52; lens = 15;
    break;
    case _MHELP:
    pys = 128; lens = 59;
    break;
}
seek(f,pys,SEEK_SET);
write(f,str,strlen(str) > lens ? lens : strlen(str));
seek(f,pys+strlen(str),SEEK_SET);
write(f,"",lens-strlen(str));
close(f);
return 0;
}

//读取mrp内文件，file mrp文件，mrpfile 欲要读取的文件名，len 读取文件的长度，lookfor 0，该函数调用后调用freestrbuf()，或退出时调用freestrbuf() 
void *readmrpfile(char *file,char *mrpfile,int32 len,int lookfor)
{uint8 *buf;
readfilefrommrp(file,mrpfile,&buf,&len,lookfor);
int slen = strlen(buf);
mrpmalloc(1,slen);
memset(strsc.buf[1],0,slen);
if(strsc.buf[1] == NULL)return NULL;
memcpy(strsc.buf[1],buf,slen);
freefiledata(buf,len);
return strsc.buf[1];
}

//卸载mrp，该函数会删除mrp生成的文件夹和文件，该函数可能不准确卸载游戏比较准确,file mrp文件名，is 是否删除mrp 0否 ，1是，该函数调用后调用freestrbuf()，或退出时调用freestrbuf() 
void mrpunins(char *file,int is)
{
char *files = strdel( getmrpinformation(file) ,".mrp");
if(is)remove(file);
if(filestate(files) != 2)break;
rmdirEX(files);
}

      /****mrp部分结束****/

//释放内存，函数注释中提到调用该函数，调用函数后，就该释放内存。
void freestrbuf()
{
for(int i=0;i<2;i++)
free(strsc.buf[i]);
free(strsc.str);
}

//获取字符宽高，简化。
void strwh(char *str,int32 *sw,int32 *sh);
{
int32 w,h;
textwh(str,0,FONT,&w,&h);
*sw = w; *sh = h;
}

//申请内存，并初始化。
void *Malloc(int size)
{
void *buf = malloc(size);
memset(buf,0,size);
return buf;
}

#endif
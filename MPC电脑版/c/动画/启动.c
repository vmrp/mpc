/*
* gif图片分解播放
* By:天天
* 2012.8.3
* 图片不能太大，分解张数不能太多。
*/

#include <base.h>

#define REF ref(0,0,SCRW,SCRH)

#define FONT 1

//图片路径
#define gifName "c/动画/图1.gif"

//图片储存目录
#define path "c/动画/GIF"

//是否播放前清屏
#define IS_CLS 0

//图片播放时间
#define Time 500

uint32 FileN;//总图片个数

uint32 FileNum=0;

uint32 timer;

int tw,th;
textwh("我",FALSE,1,&tw,&th);

int init()
{
 ShowMsg("loading...");

 FileN=GifInit();

 if(FileN)
 {
 ShowMsg("播放...");
 cls(0,0,0);
 timer=timercreate();
 timerstart(timer,Time,Time,"Gif",1);
 }
 return 0;
}

int event(int type, int p1, int p2)
{
 if(KY_DOWN == type)
 {
 switch(p1)
  {
  case _SRIGHT:
  timerdel(timer);
  exit();break;

  case _1:
  break;
  }
 }
 return 0;
}

int pause(void)
{
 return 0;
}

int resume(void)
{
 return 0;
}

void Gif(int data)
{
 if(FileNum>=FileN)
 FileNum=0;

 char name[100];
 memset(name,NULL,100);
 sprintf(name,"%s/%d.gif",path,FileNum);
 if(IS_CLS)
 cls(0,0,0);
 img(name,0,0);
 drect(0,SCRH-th,SCRW,th,0,0,0);
 dtext(name,0,SCRH-th,255,255,25,FALSE,FONT);
 REF;
 FileNum++;
}



void ShowMsg(char *Msg)
{
 int w,h;uint16 x,y;
 textwh(Msg,FALSE,FONT,&w,&h);
 if(w<SCRW)x=(SCRW-w)/2;
 y=(SCRH-h)/2;
 effsetcon(x-5,y-5,w+10,h+10,100,100,100);
 dtext(Msg,x,y,255,255,250,FALSE,FONT);
 ref(x-5,y-5,w+10,h+10);
}

/*
以下程序取自
陪死神吃苹果
*/

#define IMGNAME gifName
#define SAVENAME path

int GifInit(void)
{
 ShowMsg("分解中...");

 mkdir(SAVENAME);
 int i;
 char end=0x3b;
char outname[64];
uint8 *p,*pp;
uint8 *buf;
uint8 pal;
uint8 bits;
int32 f,f1;
int len=getlen(IMGNAME);
f=open(IMGNAME,1);
buf=malloc(len);
read(f,buf,len);
close(f);
bits=buf[10]&7;
pal=buf[10]>>7;
int pallen=0,pallen1;
if(pal)
pallen=3*pow(2,bits+1);
int nlen=len-13-pallen;
int wlen;
p=buf+13+pallen;
while(nlen>1)
{
sprintf(outname,"%s/%d.gif",SAVENAME,i);
ShowMsg(outname);
f1=open(outname,12);
write(f1,buf,13+pallen);
pp=memchr(p,0x2c,nlen);
pal=pp[9]>>7;
bits=pp[9]&7;
pallen1=0;
if(pal)
pallen1=3*pow(2,bits+1);

for(pp+=11+pallen1;*pp!=0;pp+=*pp+1);
wlen=(int)(++pp-p);
write(f1,p,wlen);
write(f1,&end,1);
close(f1);
i++;
p=pp;
nlen-=wlen;
if(p[0]==end)
break;
}
free(buf);
char t[40];
sprintf(t,"分解%d张图片",i);
ShowMsg(t);
return i;
}


int pow(int a,int n)
{
int i,b=1;
for(i=0;i<n;i++)
b*=a;
return b;
}
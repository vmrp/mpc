/****
消逝mp3播放器
屏幕:240*320
by:每天进步一点
QQ:643552878
date:2012.5月
*****/

#include <base.h>
#define REF ref(0,0,SCRW,SCRH) //歌曲搜索目录
#define PATH "my music"
#define SHU 100
//播键刷新速度(次/ms)

//界面平移速度
#define PV tw/2



#define font 1
//字体
#define jtname "c/消逝mp3截图.bmp"


//背景图片
#define day "c/消逝mp3/白天.jpg"
#define night "c/消逝mp3/黑夜.jpg"
#include "c/消逝mp3/event.h"
#include "c/消逝mp3/huitu.h"#include "c/消逝mp3/filelist.h"

uint16 *bei=malloc(SCRW*SCRH*2),*scr=getscrbuf();

int SCRW2=SCRW/2;
int SCRH8=SCRH/8;

int r,g,b;
//文字颜色
int key1,key2,key3,key4;

int tw,th,th2,tw2,jm=1,jm2=jm,j=1,paly,v=1,exitpaly=0,pf=1,loop=0,xx=0;
uint8 *nowtime;
uint8 alltime;

int timef=NULL;
/*播放器界面定时器句柄*/
int anf=NULL;
/*按键定时器句柄*/

//获得数字宽高
textwh("1",0,font,&tw2,&th2);
//获得文字宽高
textwh("顶",0,font,&tw,&th);

int init()
{
 soso(2);
 hpf();
 //皮肤初始化
 
 cls(0,0,0);
 dtext("正在搜索mp3文件中…",5,SCRH/2-th*2,255,0,0,0,font);
 dtext("请稍候…",SCRW2-tw,SCRH/2,255,0,0,0,font);
 REF;
 mp3init();
 starend();
 kz(0);
 return 0;
}

void exitapp()
{
if(FileListHead !=NULL)
free(FileListHead);
FileListHead=NULL;

free(bei);
bei=NULL;
if(timef)timerdel(timef);
if(anf)timerdel(anf);

if(paly)
{ if(exitpaly==0)
 { MP3(6,"",1);
   MP3(7,"",1);
 }
}
starend();

exit();
}
int pause()
{return 0;}
int resume()
{kz();return 0;}

void t(char *te)
{drect(0,SCRH/2+30,SCRW,30,0,0,0);
dtext(te,SCRW/2,SCRH/2+30,255,5,5,0,1);
ref(0,0,SCRW,SCRH);
sleep(50);
}

void soso(int he)
{
if(he==1)
{ cls(0,0,0);
 dtext("正在检查组件……",SCRW/2-60,SCRH/2-30,255,0,0,0,1);
ref(0,0,SCRW,SCRH);

int wo;
if(getlen("c/消逝mp3/huitu.h")<=0)wo=1;
t("1/6");
if(getlen("c/消逝mp3/爱书阁.jpg")<=0)wo=1;
t("2/6");
if(getlen("c/消逝mp3/filelist.h")<=0)wo=1;
t("3/6");
if(getlen("c/消逝mp3/event.h")<=0)wo=1;
t("4/6");
if(getlen("c/消逝mp3/next.bmp")<=0)wo=1;
t("5/6");
if(getlen("c/消逝mp3/prev.bmp")<=0)wo=1;
t("6/6");

if(wo==1)
{
cls(0,0,0);
dtext("缺少组件！原文件名不正确！",10,SCRH/2,255,0,0,0,1);
dtext("请到:",5,SCRH/2+30,255,0,0,0,1);
dtext("wap.mrpej.com重新下载！",5,SCRH/2+60,255,0,0,0,1);
ref(0,0,SCRW,SCRH);
sleep(3000);exitapp();
}

}
else if(he==2)
{
/*
int wow=0;
if(key1!=getlen("c/消逝mp3/huitu.h"))wow++;
if(key2!=getlen("c/消逝mp3/filelist.h"))wow++;
if(key3!=getlen("c/消逝mp3/event.h"))wow++;
if(key4!=getlen("c/消逝mp3/消逝播放器.c"))wow++;
if(wow)wrong(wow);
*/
}
}soso(1);

int jmjs(int number)
{
int i,j,temp,count=0;
int num[4];
for(i=0;number>0;i++)
{
num[i]=number%10;
number=number/10;
if(i==3)break;
}

for(i=0;i<4;i++)
num[i]=(num[i]+5)%10;

temp=num[3];
num[3]=num[0];
num[0]=temp;

return(num[0]*1000+num[1]*100+num[2]*10+num[3]);
}
void key()
{ key1=jmjs(1769);
  key2=jmjs(9905);
  key3=jmjs(9264);
  key4=jmjs(7041);
} key();
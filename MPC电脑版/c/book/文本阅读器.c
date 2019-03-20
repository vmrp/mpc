/***
   文本阅读器
仿RX阅读器！！！
无所谓、忧乐作品！
特殊功能键:
0键切换到白天(夜晚)模式！
*键截图！
#键开启图片背景！
9键开启全屏阅读！
2键初步解决乱码！

完成日期:
2012.4.14 
************/

#include <base.h>
char *txt="c/base.h";
//(注:在目录是，要在前面加上'/')要阅读的文本路径，可自己改。


#include "c/book/history.h"
//历史记录界口
#define bt"文本阅读器"
#define photo"c/book.bmp"
//截图的图片名字。

#define tu_path"c/book/背景.jpg"
//图片背景路径。可自己添加

#define cun"c/book/history/"
//历史记录存放文件夹

int jm=1;
/*1为阅读界面
2为输入页数界面
3为记录界面*/

int32 w;
int v=156;
int32 len=255;
int32 len_=getlen(txt);
char *page=malloc(10);

int ch=1;
//控制全屏阅开关
int bj=0;
//控制背景图片开关
int y_w=0,ye[4];


int b_r,b_g,b_b;
b_r=b_g=b_b=55;
//背景颜色。

int z_r,z_g,z_b;
z_r=z_g=z_b=255;
//文字颜色

int init()
{
if(len_<0)wrong();
history();

int y=0;
while(y<=(SCRH/2-20))
{
y+=3;
if(y>(SCRH/2-50))y=y-1;

cls(0,0,0);
dtext("无所谓忧乐 作品",10,y,255,255,0,0,1);
dtext(bt,50,SCRH-y,255,0,255,0,1);
ref(0,0,SCRW,SCRH);
}

y=100;
while(y<160)
{
effsetcon(0,0,SCRW,SCRH,y,y,y);
ref(0,0,SCRW,SCRH);
y+=10;
}
 readbook();
  return 0;
}

void readbook()
{
char *wen=malloc(len);
int32 f=open(txt,1);
seek(f,w,0);
read(f,wen,len);
close(f);
    
rectst rect;
colorst color;

if(ch==0)
//开启全屏
{
rect.x=10;
rect.y=30;
rect.w=SCRW-20;
rect.h=SCRH-60;
}
color.r=z_r;
color.g=z_g;
color.b=z_b;

if(ch==1)
//关闭全屏
{
rect.x=10;
rect.y=30;
rect.w=SCRW-20;
rect.h=SCRH-50;
}
    
cls(b_r,b_g,b_b);

if(bj)
img(tu_path,0,0);
//绘背景图

if(ch==1)
{
dline(0,30,240,30,255,255,0);
char *text=(strrchr(txt,'/'));
if(text!=NULL)dtext(text,70,5,138,138,138,0,1);
else
dtext(txt,70,5,138,138,138,0,1);
char ys[10];
sprintf(ys,"%d/%d",len_/v,w/v);
dtext(ys,5,5,138,138,138,0,1);

}

dtextex(wen,rect.x,rect.y,&rect,&color,2,1);
 
ref(0,0,SCRW,SCRH);
free(wen);
}

int event(int type, int p1, int p2)
{
 if(KY_DOWN == type)
{
if(p1==_STAR)
{ printscr(photo);}


if(jm==1)switch(p1)
{
case _SRIGHT:
exit_yn();jm=0;break;

case _2:
if(w>0)w--;else w=0;
readbook();
break;

case _UP:
w-=20;readbook();break;

case _DOWN:
w+=20;readbook();
break;

case _4:
case _LEFT:
if(w>=160)
w-=v;readbook();
break;

case _6:case _RIGHT:
if(w/v<(len_/v))
w+=v;readbook();
break;

case _0:
pf();readbook();
break;

case _1:
jm=2;shu(11);
break;

case _9:
if(ch==1)ch=0;else ch=1;
readbook();
break;

case _POUND:
if(bj==1)bj=0;
else bj=1;
readbook();break;
 
}

else if(jm==2)
switch(p1)
{
case 0:case 1:
case 2:case 3:
case 4:case 5:
case 6:case 7:
case 8:case 9:
shu(p1);break;

case _SLEFT:
shu(55);break;

case _SRIGHT:
jm=1;y_w=0;
readbook();
break;
}
else if(jm==0)
switch(p1)
{
case _SLEFT:
jm=3;history_write();break;

case _SRIGHT:
jm=1;readbook();break;
}
else if(jm==3)
{
if(p1==_SLEFT)
exitapp(1);

if(p1==_SRIGHT)
exitapp(0);
}

 }
return 0;
}

int pause(void)
{return 0;}
int resume(void)
{return 0;}

void pf()
//皮肤切换！
{
if(b_r==255)
{ b_r=b_g=b_b=55;
 z_r=z_g=z_b=55;
}
else if(b_b==55)
{ b_r=b_g=b_b=255;
 z_r=z_g=z_b=0;
}

}

void wrong()
//打开失败
{
cls(0,0,0);

drect(0,100,SCRW,50,255,255,255);
drect(1,101,SCRW-2,48,0,0,0);
dtext("文件打开失败！",50,110,255,255,255,0,1);
ref(0,0,SCRW,SCRH);
sleep(150);

effsetcon(0,0,SCRW,SCRH,138,138,138);
ref(0,0,SCRW,SCRH);
sleep(3000);
exit();
}

void shu(int p1)
{
 if(p1==11)
{
effsetcon(0,0,240,320,50,50,50);
drect(0,100,240,100,255,255,255);
drect(1,101,238,98,0,0,0);
dline(0,135,240,135,255,255,0);
dline(0,160,240,160,255,255,0);
dtext("输入页数:",30,105,255,255,255,0,1);
char *sh=malloc(20);
sprintf(sh,"0~%d",len_/v);
dtext(sh,150,105,255,255,255,0,1);
free(sh);
dtext("确定",0,175,255,255,0,0,1);
dtext("取消",180,175,255,255,0,0,1);
}

else if(p1<10&&y_w<4)
{
y_w++;
ye[y_w-1]=p1;
char *sh=malloc(10);
sprintf(sh,"%d",p1);
dtext(sh,50+y_w*10,135,255,255,255,0,1);
ref(0,0,240,320);
free(sh);
}


else if(p1==55)
{
int yi;
if(y_w==1)yi=ye[0];
if(y_w==2)
yi=ye[0]*10+ye[1];
if(y_w==3)
yi=ye[0]*100+ye[1]*10+ye[2];
if(y_w==4)
yi=ye[0]*1000+ye[1]*100+ye[2]+10+ye[3];
if(yi*v<=len_)
w=yi*v;
else w=len_;

y_w=0; jm=1;
readbook();
}

ref(0,0,SCRW,SCRH);
}

void exit_yn()
{
effsetcon(0,0,SCRW,SCRH,50,50,50);
drect(0,SCRH-70,SCRW,70,255,255,255);
drect(1,SCRH-69,SCRW-2,68,0,0,0);

dline(0,SCRH-32,SCRW,SCRH-32,255,255,0);
dtext("是否退出？",SCRW/2-50,SCRH-60,255,255,255,0,1);
dtext("确定",0,SCRH-30,255,0,0,0,1);
dtext("取消",185,SCRH-30,255,0,0,0,1);
ref(0,0,SCRW,SCRH);
}

void history_write()
{
cls(0,0,0);
dtext("是否记录该次阅读进度？",0,100,255,255,255,0,1);
dtext("记录",0,SCRH-30,255,255,0,0,1);
dtext("不记",SCRW-60,SCRH-30,255,255,0,0,1);
ref(0,0,SCRW,SCRH);

}

//截图函数，摘自小钱
void printscr(char *name)
{
  uint16 *scrf = getscrbuf();
  int f = open(name, 12);
  write(f, scrf, SCRW*SCRH*2);
 close(f);

drect(0,100,SCRW,50,255,255,255);
drect(1,101,SCRW-2,48,0,0,0);
if(getlen(name)>0)
dtext("截图成功",80,105,0,255,0,0,1);
else dtext("截图失败！",80,105,255,0,0,0,1);
ref(0,0,SCRW,SCRH);
sleep(1500);
readbook();
}

void exitapp(int j)
{
free(page);
if(j)jlu();
exit();
}

//无所谓、忧乐作品
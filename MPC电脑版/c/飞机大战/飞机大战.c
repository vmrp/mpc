
#include <base.h>
#include "c/飞机大战/组件.h"
#include "c/飞机大战/触.h"
#include "c/飞机大战/MS.h"
#define FONT 1
#define R 155
#define G 255
#define B 255
#define text(txt,x,y) dtext(txt,x,y,R,G,B,0,FONT)

#define cd_big 4

char *cd[cd_big]={"开始游戏","游戏关于","游戏帮助","游戏退出"};

int potion=1;
int jm=0;
int tww,thh;
textwh("剑",FALSE,FONT,&tww,&thh);

int a,b,v,u,jc,rar,prta,s,hp,j,h,rp,ap,hpt,jdhp,max,max1,nd,xx,whk,f,l,t4,p4,fj,bsg,tn,ruk,exhp,bz,gbz,bztime,bx,by,times,fjtime,jgx,bjtme,tx,ty,tx0,ty0,getimoni,aa=0,bb=-320;
times=timercreate();
int W,H,tw,th,nw,nh;

int init(void)
{
W=240;
H=320;
tw=strw("哦",1);
th=strh("哦",1);
nw=strw("2",1);
nh=strh("2",1);
b=h=W/2-tw;a=j=H-2*th-10;hp=50;
rp=1;hpt=4;jdhp=5;max=255;max1=0;fj=5;rar=3;bsg=3;whk=4;u=p4+25;v=-20;ruk=1;exhp=1;jgx=11;getimoni=1;
cls(0,0,0);
lmc(255,255,255);
prevn();
kz();
ref(0,0,240,320);
return 0;
}

void pt(int k)
{write(f,k,l);}

void ra(int k)
{read(f,k,l);}

void dst(int xx)
{
jm=5;
cls(0, 0, 0);
bmp565("c/飞机大战/240320.bmp",0,aa,240,240,320,0,0,0);
bmp565("c/飞机大战/240320.bmp",0,bb,240,240,320,0,0,0);
aa=aa+2;bb=bb+2;
if(aa>=320)
{aa=-320;}
if(bb>=320)
{bb=-320;}
bmp565("c/飞机大战/24480.bmp",b,a,24,24,30,6,0,48);
drect(h+jgx,j-300,rp,300,max,max1,0);
t4=t4+whk;
v=v+8;
if(bjtme==1)
{
tx=tx-1;
ty=ty+8;
tx0=tx0+1;
ty0=ty0+8;
}
if(p4<b-20)
{p4=p4+2;}
else
{p4=p4-2;}
if(gbz>125)
{gbz=0;bztime=0;bz=0;hptmer();}
gbz=gbz+bztime;
if(nd==5)
{bjtme=1;whk=1;hp=hp+200;nd=nd+1;tn=1;exhp=5;getimoni=5;mox();boss();}
if(nd==7)
{whk=4;tn=0;}
if(nd==15)
{bjtme=1;whk=2;hp=hp+350;nd=nd+1;tn=1;exhp=8;getimoni=5;mox();boss();}
if(nd==17)
{whk=4;tn=0;}
if(bz==1)
{bmp565("c/飞机大战/24480.bmp",bx,by,24,24,23,6,0,74+gbz);}
if(nd==20)
{bjtme=1;whk=2;hp=hp+450;nd=nd+1;tn=1;exhp=10;getimoni=5;mox();boss();}
if(nd==22)
{whk=4;tn=0;}
if(nd==50)
{bjtme=1;whk=1;hp=hp+2450;nd=nd+1;tn=1;exhp=40;getimoni=5;mox();boss();}
if(nd==52)
{whk=4;tn=0;}
bmp565("c/飞机大战/8120.bmp",u,v,8,8,13,6,0,24);
if(bjtme==1)
{bmp565("c/飞机大战/8120.bmp",tx,ty,8,8,13,6,0,24);bmp565("c/飞机大战/8120.bmp",tx0,ty0,8,8,13,6,0,24);}
if(fjtime==0)
{bmp565("c/飞机大战/5842.bmp",p4,t4-48,58,51,42,6,0,0);}if(t4>280){p4=rand()%210;t4=rand()%13;jdhp=jdhp-getimoni;hp=50;}
if(h>=p4)
{if(h<p4+40){hp=hp-hpt;}
if(hp<=0)
{bz=1;bztime=24;bx=p4+12;by=t4-25;fjtime=1;}}
if(ap==25)
{rp=2;jgx=11;}
if(ap==40)
{rp=3;jgx=10;}
if(ap==70)
{rp=4;jgx=10;}
if(ap==100)
{max=255;max1=0;}
if(rp==2)
{hpt=6;}
if(rp==3)
{hpt=9;}
if(rp==4)
{hpt=11;}
if(max1==0)
{hpt=13;}
drect(W-50,0,50,nh,255,255,255);
drect(W-50,0,hp/exhp,nh,80,120,40);
dtext("分数:",1,H-th,255,255,255,0,1);
dtext("基地HP:",W-2*(tw+2*nw+5),H-th,255,255,255,0,1);
iprint(hp,W-50,0,128,165,128,1);
iprint(ap,2*tw+nw,H-th,255,255,0,1);
iprint(jdhp,W-2*nw,H-th,0,255,0,1);
dtext("必杀:",1,0,255,255,255,0,1);
iprint(bsg,2*tw+nw,0,200,0,0,1);
drect(85,310,50,10,255,255,255);
drect(85,310,fj*10,10,0,25,220);
if(v>310)
{u=p4+25;v=t4-48;}
if(jdhp==0)
{tbi();}
if(nd==70)
{vit();}
if(fj<1)
{tbi();}
if(u<=b+18)
{
if(u>b-10)
{
if(v<=a+18)
{
if(v>a-10)
{fj=fj-1;u=p4+25;v=t4-25;}
}
}
}
if(tx<=b+18)
{
if(tx>b-10)
{
if(ty<=a+18)
{
if(ty>a-10)
{
fj=fj-1;tx=p4+25;ty=t4-25;
}
}
}
}
if(tx0<=b+18)
{
if(tx0>b-10)
{
if(ty0<=a+18)
{
if(ty0>a-10)
{
fj=fj-1;tx0=p4+25;ty0=t4-25;
}
}
}
}
if(ty>310)
{tx=p4+25;ty=t4-25;}
if(ty0>310)
{tx0=p4+25;ty0=t4-25;}
ref(0,0,240,320);
}

void boss()
{
cls(0,0,0);
dtext("危险！！！",0,0,255,255,255,0,1);
dtext("BOSS出现小心！！！",0,16,255,255,255,0,1);
ref(0,0,240,320);
sleep(1500);
dst(0);
}

void tbi()
{
dtext("游戏失败！",(W-5*tw)/2,150,255,255,255,0,1);
ref(0,0,240,320);
sleep(2000);
timerstop(times);
timerdel(times);
exit();
}

void vit()
{
dtext("游戏胜利！",(W-5*tw)/2,150,255,255,0,0,1);
ref(0,0,240,320);
sleep(2000);
timerstop(times);
timerdel(times);
exit();
}

void bs()
{
if(bsg<=0)
{bsg=0;}
if(bsg<1)
{}
if(tn==1)
{ruk=0;}
if(tn==0)
{ruk=1;}
if(tn==0)
{
if(bsg>=1)
{
bz=1;
bztime=24;
bx=p4+12;
by=t4-25;
fjtime=1;
bsg=bsg-ruk;
}
}
else
{
}
}

void chu()
{
int t;
cls(0,0,0);
for(t=45;t<=W;t=t+rand()%80)
{
bmp565("c/飞机大战/进度条背景.bmp",0,0,240,240,320,0,0,0);
drect2(45,H/2-5,W-45*2,10);
drect(45+1,(H/2-5)+1,(t-45*2)-2,8,20,255,0);
dtext("当分达到一定数激光威力、外观",0,H/2+th,255,0,0,0,0);
dtext("有所变化！",0,H/2+th*2,255,0,0,0,0);
mkdir("gjdbw");
ref(0,0,240,320);
sleep(2000);
}
}

void cd0()
{
remove("gjdbw/gjdbw.dat");
cd1();
}

void cd1()
{
f=open("gjdbw/gjdbw.dat",12);
cd2();
close(f);
}

void cd2()
{
l=4;
pt(nd);
pt(jdhp);
pt(ap);
pt(rp);
pt(hpt);
pt(max);
pt(max1);
pt(hp);
pt(b);
pt(a);
pt(h);
pt(j);
pt(whk);
pt(p4);
pt(t4);
pt(u);
pt(v);
pt(fj);
pt(rar);
}

void dd()
{
f=open("gjdbw/gjdbw.dat",12);
dd0();
close(f);
return 0;
}

int dd0()
{
read(f,nd,4);
read(f,jdhp,4);
read(f,ap,4);
read(f,rp,4);
read(f,hpt,4);
read(f,max,4);
read(f,max1,4);
read(f,hp,4);
read(f,b,4);
read(f,a,4);
read(f,h,4);
read(f,j,4);
read(f,whk,4);
read(f,p4,4);
read(f,t4,4);
read(f,u,4);
read(f,v,4);
read(f,fj,4);
read(f,rar,4);
}


int event(int type, int p1, int p2)
{
if((type==2||type==3)&&400>320)EVENT(type,p1,p2);
if(type==0||type==1)even(type,p1,p2);
if(KY_DOWN == type&&!jm)
switch(p1)
{
case _SRIGHT:
jm=4;kz();
break;
case _SLEFT:
case _SELECT:
if(potion<5)
jm=potion;
kz();
break;
case _6:
case _RIGHT:
if(potion<cd_big)potion++;
else potion=1;
kz();
break;
case _4:
case _LEFT:
if(potion>1)potion--;
else potion=cd_big;
kz();
break;
}
else if(jm==1)
game_event(type,p1,p2);
else
if(type==KY_DOWN&&(jm==2||jm==3))
switch(p1)
{
case _SRIGHT:
jm=0;kz();break;
}
else
if(KY_DOWN==type&&jm==4)
switch(p1)
{
case _SRIGHT:
jm=0;kz();
break;
case _SLEFT:
case _SELECT:
timerstop(times);
timerdel(times);
exit();
}
else
if(KY_DOWN==type&&jm==5)
switch(p1)
{
case _SRIGHT:
timerstop(times);
jm=0;
kz();
if(p1==7){cd0();}
if(p1==8){dd();dst(0);}
break;
case _POUND:
case _0:
bs();
break;
case _LEFT:
case _4:
b=b-20;
h=h-20;
if(b<=0){b=0;}
if(h<=0){h=0;}
break;
case _RIGHT:
case _6:
b=b+20;
h=h+20;
if(b>=220){b=220;}if(h>=220){h=220;}
break;
}
return 0;
}

void drect2(int x,int y,int w,int h)
{
drect(x+1,y+1,w-2,h-2,255,255,255);
dline(x,y,x,y+h-1,0,0,255);
dline(x+1,y,x+w-2,y,0,0,255);
dline(x+w-1,y,x+w-1,y+h-1,0,0,255);
dline(x+1,y+h-1,x+w-2,y+h-1,0,0,255);
}

void hptmer()
{
p4=rand()%210;
t4=rand()%13;
ap=ap+5;
nd=nd+1;
hp=50;
exhp=1;
fjtime=0;
bjtme=0;
getimoni=1;
}

void mox()
{
tx=p4+25;
ty=t4-25;
tx0=p4+25;
ty0=t4-25;
}

void bmp565(char *name,int x,int y,int mw,int w,int h,int rop,int sx,int sy)
{
int lenxl=getlen(name),rgb;
uint16 *ptrxl=malloc(lenxl);
int fxl=open(name,4);
if(rop==6)
{
read(fxl,&rgb,2);
seek(fxl,0,0);
}
read(fxl,ptrxl,lenxl);
close(fxl);
bmpshowflip(ptrxl,x,y,mw,w,h,rop,sx,sy,rgb);
free(ptrxl);
}

int printscr(char *file)
{
remove(file);
int qp=240*320*2;
uint16 *pic=malloc(qp);
memcpy(pic,getscrbuf(),qp);
bmpshowflip(pic,0,0,240,240,320,0x600,0,0,0);
memcpy(pic,getscrbuf(),qp);

int f=open(file,8);
int16 s=19778;
int h[18]={240*320*2+70,0,70,56,240,320,1048577,3,51200,4000,4000,0,0,63488,2016,31,0,0};
write(f,&s,2);
write(f,h,68);
write(f,pic,qp);
close(f);
bmpshowflip(pic,0,0,240,240,320,0x600,0,0,0);
free(pic);
    return 0;
}


int pause(void)
{
return 0;
}

int resume(void)
{
kz();
return 0;
}


void kz()
{
switch(jm)
{
 case 0:hui_cd();break;
 case 1:game();break;
 case 2:about();break;
 case 3:help();break;
 case 4:yesexit();break;
 }
}
void hui_cd()
{
 cls(10,10,10);
 int H=320-th;
 bmp565("c/飞机大战/菜单背景.bmp",0,0,240,240,320,0,0,0);
drect(0,th,240,1,R/2,G/2,,B/2);
 text("飞机大战",(240-tw*4)/2,0);
 drect(0,H,240,1,R/2,G/2,B/2);
 text("确定",0,H);
 text("退出",240-tw*2,H);
 time(240/2-tw,H);

 char *txt=malloc(strlen(cd[potion-1])+4);
 sprintf(txt,">%s<",cd[potion-1]);

 text(txt,(240-textw(txt))/2,320-th*3);
 free(txt);
 ref(0,0,240,320);

}

int textw(char *txt)
{
 int ww,hh;
 textwh(txt,FALSE,FONT,&ww,&hh);
 return ww;
}

void yesexit()
{
 effsetcon(0,0,240,320,138,138,138);
 int H=320-th*2;
 drect(0,H,240,th*2,10,10,10);
 drect(0,H,240,1,R/2,G/2,B/2);
text("确认退出?",tw,H);
text("退出",0,H+th);
text("返回",240-tw*2,H+th);
;
ref(0,0,240,320);
}


void game()
{
timerstart(times,200,1,"dst",1);
chu();
dst(0);
}


void game_event(int type,int p1,int p2)
{
 if(type==KY_DOWN)
 switch(p1)
 {
 case _SRIGHT:
 jm=0;kz();break;
 }
}


int print(char *txt,int data)
{
char text[500];
cls(240,240,240);
sprintf(text,txt,data);
dtext(text,0,0,20,20,20,0,0,2);
ref(0,0,240,320);
return 0;
}

int iprint(int a,int x,int y,int r,int g,int b,int font)
{
char txt[30];
sprintf(txt,"%d",a);
dtext(txt,x,y,r,g,b,0,font);
return 0;
}



int timers[3];


int timer(int32 data,int32 longx,char *txt,int32 xunhuan)
{
timerstopx(data);
timerdel(timers[data]);
timers[data]=timercreate();
timerstart(timers[data],longx,data,txt, xunhuan);
return 0;
}

int timerstopx(int data)
{
timerstop(timers[data]);
timerdel(timers[data]);
return 0;
}


int strw(char *txt,int font)
{
int fw,fh;
textwh(txt,0,font,&fw,&fh);
return fw;
}

int strh(char *txt,int font)
{
int fw,fh;
textwh(txt,0,font,&fw,&fh);
return fh;
}

void playsound(char *music,int type,int zhushe,int xunhuan)
{
soundloadfile(type,music);
soundplay(type,zhushe,xunhuan);
}

int putchx=10;
int putch(char a)
{
char b[5];
if(putchx==10)
cls(240,240,240);
sprintf(b,"%c",a);
dtext(b,putchx,10,20,20,20,0,0,2);
ref(0,0,240,320);
putchx+=12;
return 0;
}void lmc(int r,int g,int b)
{
int y=0,a=SCRH;
for(y=0;y<=160;y=y+1)
{
dline(0,y,SCRW,y,r,g,b);
dline(0,a,SCRW,a--,r,g,b);
ref(0,0,240,320);
}
}

#include <base.h>
#include "c/剑圣卡斯传说/传说.c"
#include "c/剑圣卡斯传说/组件.h"
#define tu "c/剑圣卡斯传说/圣.bmp"
#define tu2 "c/剑圣卡斯传说/怪.bmp"
#define bmp_person bmpshowflip(person,Cr->x,Cr->y,100,100,125,BM_COPY,0,0,0)

void bmp_guai(int x,int y)
{
bmpshowflip(guai,x,y,100,100,125,BM_COPY,0,0,0);
}

#define cdbei "c/剑圣卡斯传说/菜单背景.jpg"

#define LV_BIG 50
//最高级,满级

#define FONT 1
//字体
#define R 155
#define G 255
#define B 255

#define REF ref(0,0,SCRW,SCRH)
//刷新
#define text(txt,x,y) dtext(txt,x,y,R,G,B,0,FONT)




char *cd[4]={"开始游戏","游戏关于","游戏帮助","游戏退出"};

int potion=1;
int jm=0;

int tw,th;
textwh("剑",FALSE,FONT,&tw,&th);

int init()
{
 game_init();
 kz();
 return 0;

}

int event(int type, int p1, int p2)
{
 if(KY_DOWN == type&&!jm)
  switch(p1)
  {
  case _SRIGHT:  jm=4;kz();
  break;

 case _SLEFT:
 if(potion<5)
 jm=potion;
 kz();break;

 case _6:
 case _RIGHT:
 if(potion<4)potion++;
 else potion=1;
 kz();
  break;

 case _4:
 case _LEFT:
 if(potion>1)potion--;
 else potion=4;
 kz();
 break;
  }
else if(jm==1)
game_event(type,p1,p2);

else
if(type==KY_DOWN&&(jm==2||jm==3))
 switch(p1)
{ case _SRIGHT:
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
 exitapp();
}

    return 0;
}
/*
int pause(void)
{return 0;}
int resume(void)
{return 0;}
*/

void exitapp()
{
 exitapp_game();
 exit();

}

void kz()
{
switch(jm)
 {
 case 0:hui_cd();break;
 case 1:game_kz();break;
 case 2:about();break;
 case 3:help();break;
 case 4:yesexit();break;
 }

}
void hui_cd()
{
 cls(10,10,10);
 img(cdbei,0,0);
 int H=SCRH-th;
 drect(0,th,SCRW,1,R/2,G/2,,B/2);
 text("菜单",SCRW/2-tw,0);
 drect(0,H,SCRW,1,R/2,G/2,B/2);
 text("确定",0,H);
 text("退出",SCRW-tw*2,H);
 time(SCRW/2-tw,H);

 char *txt=malloc(strlen(cd[potion-1])+4);
 sprintf(txt,">%s<",cd[potion-1]);

 text(txt,(SCRW-textw(txt))/2,SCRH-th*3);
 free(txt);
 REF;
}
int textw(char *txt)
{
 int w,h;
 textwh(txt,FALSE,FONT,&w,&h);
 return w;
}
void yesexit()
{
 effsetcon(0,0,SCRW,SCRH,138,138,138);
 int H=SCRH-th*2;
 drect(0,H,SCRW,th*2,10,10,10);
 drect(0,H,SCRW,1,R/2,G/2,B/2);
text("确认退出?",tw,H);
text("退出",0,H+th);
text("返回",SCRW-tw*2,H+th);
 REF;
}
/*
闪侠v1.1
by:每天进步一点
图片提供:宿敌→笨小孩
QQ:643552878
2012.6

游戏的设置在<game闪侠.h>里
*/
#include <base.h>
#include "c/闪侠/game闪侠.h"
#include "c/闪侠/组件.h"

//字体颜色
#define R 155
#define G 255
#define B 255

//背景图片
#define cdbei "c/闪侠/jpg/菜单背景.jpg"

#define REF ref(0,0,SCRW,SCRH)
//刷新

//绘字宏
#define text(txt,x,y) dtext(txt,x,y,R,G,B,0,FONT)

//菜单选项个数
#define cd_big 4

//菜单文字
char *cd[cd_big]={"开始游戏","游戏关于","游戏帮助","游戏退出"};

int potion=1;
int jm=0;
/*0主菜单,>1游戏界面,>2关于,>3帮助.>4退出*/

int tw,th;
textwh("闪",FALSE,FONT,&tw,&th);

int init()
{
 kz();
 return 0;

}

int event(int type, int p1, int p2)
{
 if(KY_DOWN == type&&!jm)
  switch(p1)
  {
  case _SRIGHT:
 jm=4;kz();
  break;

 case _SLEFT:
 if(potion<5)
 jm=potion;
 kz();break;

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
//游戏按键

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

int pause()
{
if(jm==1)game_pause();
return 0;

}
int resume()
{
 if(jm==1)game_resume();
 return 0;}

void exitapp()
{
 game_exitapp();
  exit();
}

void kz()
{
switch(jm)
 {
 case 0:hui_cd();break;
 case 1:game_init();break;
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


//游戏界面绘图
void game()
{cls(10,10,10);
 img(cdbei,0,0);
 int r=R/2,g=G/2,b=B/2;
 drect(0,th,SCRW,1,r,g,b);
 text("还未添加！",SCRW/2-tw*2,0);
 drect(0,SCRH-th,SCRW,1,r,g,b);
 text("返回",SCRW-tw*2,SCRH-th);
 REF;
}

//by:每天进步一点
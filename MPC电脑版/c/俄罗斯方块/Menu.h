
colorst TextColor;

void drawtext(char *text,int16 x,int16 y)
{
 dtext(text,x,y,TextColor.r,TextColor.g,TextColor.b,FALSE,FONT);
}

void srect(int16 x,int16 y,int16 w,int16 h,uint8 r,uint8 g,uint8 b,uint8 r1,uint8 g1,uint8 b1,uint8 mode)
{
 int a=r*0x10000+g*0x100+b,a1=r1*0x10000+g1*0x100+b1;
shaderect(x,y,w,h,a,a1,mode);
printf("%d,%d\n",a,a1);
}


enum
{
 Menu_Game,
 Menu_About,
 Menu_Help,
 Menu_Exit
};

uint8 Option=Menu_Game;

int MenuEvent(int type,int p1,int p2)
{
 if(Jm==Jm_Menu)
 if(KY_DOWN==type)
 switch(p1)
 {
  case _SRIGHT:
  Jm=Jm_Exit;DrawExit();
  break;

  case _SELECT:
  case _SLEFT:
  if(Jm!=Jm_Game)
  switch(Option)
  {
   case Menu_Game:
   Jm=Jm_Game;DrawGameMain();
   break;
   case Menu_About:
   Jm=Jm_About;DrawAbout();
   break;
   case Menu_Help:
   Jm=Jm_Help;DrawHelp();
   break;
   case Menu_Exit:
   Jm=Jm_Exit;DrawExit();
   break;
  }
  break;

  case _DOWN:
  case _8:
  if(Option<Menu_Exit)
  Option++;
  else
  Option=Menu_Game;
  DrawMenu();
  break;

  case _UP:
  case _2:
  if(Option>Menu_Game)
  Option--;
  else
  Option=Menu_Exit;
  DrawMenu();
  break;
 }
 return NULL;
}

int OtherEvent(int type,int p1,int p2)
{
 if(type==KY_DOWN)
 if(p1==_SRIGHT)
 {
 Jm=Jm_Menu;
 DrawMenu();
 }
 return NULL;
}

void MenuExit()
{
 free(updownbmp);
}


srect(0,0,SCRW,th,250,250,150,150,150,250,0);
srect(0,th,SCRW,th,150,150,250,250,250,150,0);
uint16 *updownbmp=malloc(SCRW*th*4);
memcpy(updownbmp,scr,SCRW*th*4);
#define UPBMP bmpshowflip(updownbmp,0,0,SCRW,SCRW,th,0,0,0,0)
#define DOWNBMP bmpshowflip(updownbmp,0,SCRH-th,SCRW,SCRW,th,0,th,0,0)

int16 MenuH=(SCRH-th*8)/2,MenuH2=SCRH-th,MenuX=(SCRW-tw*4)/2;

void DrawMenu()
{
 cls(255,255,255);
 UPBMP;DOWNBMP;
 drawtext("俄罗斯方块",0,0);
 drawtext("确定",0,MenuH2);
 drawtext("退出",SCRW-tw*2,MenuH2);

 srect(0,MenuH+th*Option*2,SCRW,th,200,200,200,50,50,250,0);
 drawtext("开始游戏",MenuX,MenuH);
 drawtext("关于作者",MenuX,MenuH+th*2);
 drawtext("游戏帮助",MenuX,MenuH+th*4);
 drawtext("退出游戏",MenuX,MenuH+th*6);
 ref(0,0,SCRW,SCRH);

}

void DrawAbout()
{
 cls(255,255,255);
 UPBMP;DOWNBMP;

 rectst rect;
 rect.x=0;rect.y=0;
 rect.w=SCRW;rect.h=SCRH-th;
 dtextex("关于\n游戏: 俄罗斯方块\n作者: 每天进步一点\nQQ: 643552878\n发布网站: wap.mrpej.com\n警告: 任何人不得修改破坏本游戏、转载请注明原作者",rect.x,rect.y,&rect,&TextColor,2,FONT);
 drawtext("返回",SCRW-tw*2,SCRH-th);
 ref(0,0,SCRW,SCRH);
}

void DrawHelp()
{
 cls(255,255,255);
 UPBMP;DOWNBMP;

 rectst rect;
 rect.x=0;rect.y=0;
 rect.w=SCRW;rect.h=SCRH-th;
 dtextex("帮助\n游戏界面:\n左软键:暂停/继续游戏\n右软键:返回主菜单\n左右键:移动\n上键:翻转\n下键:加速下降",rect.x,rect.y,&rect,&TextColor,2,FONT);
 drawtext("返回",SCRW-tw*2,SCRH-th);
 ref(0,0,SCRW,SCRH);
}

void DrawExit()
{
 effsetcon(0,0,SCRW,SCRH,100,100,100);
 srect(0,SCRH-th*2,SCRW,th*2,250,250,150,150,150,250,0);
 drawtext("退出游戏？",0,SCRH-th*2);
 drawtext("确定",0,SCRH-th);
 drawtext("返回",SCRW-tw*2,SCRH-th);
 ref(0,0,SCRW,SCRH);

}
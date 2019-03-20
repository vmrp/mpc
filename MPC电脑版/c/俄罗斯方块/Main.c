#include <base.h>
#define FONT 1

enum
{
 Jm_Exit,
 Jm_Game,
 Jm_Menu,
 Jm_About,
 Jm_Help
};
uint8 Jm=Jm_Menu;

uint16 *scr=getscrbuf();
int tw,th;
textwh("Íæ",FALSE,FONT,&tw,&th);

#include <c/¶íÂÞË¹·½¿é/menu.h>
#include <c/¶íÂÞË¹·½¿é/Game.h>

int init()
{
 DrawMenu();
 return 0;
}

int event(int type,int p1,int p2)
{
 if(p1==_STAR)exitapp();
// printf("Jm=%d,Option=%d\n",Jm,Option);

 switch(Jm)
 {
 case Jm_Menu:
 MenuEvent(type,p1,p2);
 break;

 case Jm_Game:
 GameEvent(type,p1,p2);
 break;

  case Jm_About:
  case Jm_Help:
  OtherEvent(type,p1,p2);
  break;

  case Jm_Exit:
  if(type==KY_DOWN)
  switch(p1)
  {
  case _SLEFT:
  exitapp();break;
  case _SRIGHT:
  Jm=Jm_Menu;DrawMenu();
  break;
  }
  break;
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

void exitapp()
{
 MenuExit();
 exit();
}

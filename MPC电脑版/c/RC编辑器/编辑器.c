#include <base.h>
#include <c/RC±à¼­Æ÷/lang.h>

rectst textRect;
textRect.x=0;
textRect.y=0;
textRect.w=SCRW;
textRect.h=SCRH;

colorst textColor;
textColor.r=250;
textColor.g=250;
textColor.b=250;



uint32 NowN=0,MaxN=28;
res *lang;

int init(void)
{
 lang=langload("c/FileManage/tt/res_lang0.rc",MaxN);
 Show();

 return 0;
}

int event(int type, int p1, int p2)
{
 if(KY_DOWN==type)
 {
 switch(p1)
  {
  case _SRIGHT:
  exitapp();
  break;

  case _4:
  case _LEFT:
  if(NowN)
  NowN--;
  Show();
  break;

  case _6:
  case _RIGHT:
  if(NowN<MaxN)  
  NowN++;
  Show();
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

void exitapp()
{
 free(lang);
 exit();
}

void Show()
{
 cls(0,0,0);
 //drect(0,30,SCRW,1,128,128,128);
 char str[10];
 sprintf(str,"%d/%d",NowN,MaxN);
 dtext(str,0,0,textColor.r,textColor.g,textColor.b,FALSE,1);
 dtextex(langStr(lang,NowN),0,30,&textRect,&textColor,3,1);
 ref(0,0,SCRW,SCRH);
}


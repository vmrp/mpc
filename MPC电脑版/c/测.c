/*
* By:ÌìÌì
* QQ:643552878
* Date:

*/

#include <base.h>

char *Char="ABCDEFGHIJKLMNOPQRSTUVWXYZ";


int init(void)
{
 cls(0,0,0);
 ref(0,0,SCRW,SCRH);

 return 0;
}

int event(int type, int p1, int p2)
{
 if(KY_DOWN==type)
 {
 DrawChar(Char+p1);
 switch(p1)
  {
  case _SRIGHT:
  exit();
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

int16 x=0,y=0;

char fu[2];

void DrawChar(void* c)
{
 memcpy(fu,c,1);
 fu[1]='\0';

 int zw,zh;
 textwh(fu,0,1,&zw,&zh);

 if(x+zw>SCRW)
 {x=0;y+=zh+5;}

 if(y+zh+5>SCRH)
 {
 x=y=0;
 cls(0,0,0);
 ref(0,0,SCRW,SCRH);
 }
 x+=zw+5;
 dtext(fu,x,y,255,255,255,0,1);
 ref(x-5,y-5,zw+10,zh+10);

}
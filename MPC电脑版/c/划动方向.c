#include "base.h"

int Msx,Msy,MsN;

int init()
{
 cls(0, 0, 0);
 dtext("滑动屏幕显示方向",0,0,255,0,0,0,1);
 ref(0,0,SCRW,SCRH);
 return 0;
}

int event(int msg, int p0, int p1)
{
 if(msg==KY_DOWN&&p0==_SRIGHT)
  {
   exit();
   return 0;
  }
 if(msg==MS_MOVE)
 {
 cls(0,0,0);
 MsN++;
 if(MsN==2)
 { MsN=0;Msx=p0;Msy=p1;
 }
if(p0<Msx)
dtext("←",0,0,225,0,0,0,0);
else if(p0>=Msx)
dtext("→",0,0,225,0,0,0,0);
if(p1>Msy)
dtext("↓",0,20,225,0,0,0,0);
else if(p1<=Msy)
dtext("↑",0,20,225,0,0,0,0);

 ref(0,0,SCRW,SCRH);
 }
    return 0;    
}
int pause()
{
    return 0;
}
int resume()
{
    return 0;
}
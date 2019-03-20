#include <base.h>

int init(void)
{
cls(255,255,255);
effsetcon(10,10,SCRW-20,SCRH-20,255,100,255);
ref(0,0,SCRW,SCRH);
return 0;
}

void py(int j)
{ int x;
 if(j<0)
 for (x=30;x<=240;x+=30)
 {
  img("tu.jpg",0,0);
  if(x>200)x-=x/30;
  jm1(x);jm2(x);
  ref(0,0,SCRW,SCRH); 
 }


 if(j>0)
 for(x=SCRW-30;x>=0;x-=30)
  { 
  if(x<40)x+=x/30;
  img("tu.jpg",0,0);
  jm1(x);jm2(x);
  ref(0,0,SCRW,SCRH);
 }

}

void jm1(int x)
{

dtext("界面1",-x+100,100,255,255,0,0,1);
effsetcon(-x+10,10,220,300,250,150,250);
}

void jm2(int x)
{

dtext("界面2",240-x+100,100,2,2,2,0,1);
effsetcon(240-x,10,220,300,500,250,0);
}

int event(int type, int p1, int p2)
{
    if(KY_DOWN == type)
    {
        switch(p1)
        {
        case _SRIGHT:
            exit();
            break;

case _LEFT:
py(-1);break;

case _RIGHT:
py(1);break;
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
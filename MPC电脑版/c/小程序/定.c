#include <base.h>

int t,w,h;

void g(int p1)
{
if(p1==_5)
{
if(w<SCRW)w=w+20;
else w=0;
}
drect(0,0,SCRW,50,0,0,0);
drect(0,0,w,50,200,100,10);

ref(0,0,SCRW,SCRH);

}

int init(void)
{g(0);
    return 0;
}

int event(int type, int p1, int p2)
{
 if(KY_DOWN == type)
  switch(p1)
  {
   case _SRIGHT:
   timerdel(t);exit();
            break;
        case _5:
    t= timercreate();
 timerstart(t,1,p1,"g",1);
     
       break;

}

 if(KY_UP==type)
 switch(p1)
{
 case _5:w=0;
timerdel(t); 

      break;
}



    return 0;
}

int pause(void)
{return 0;}

int resume(void)
{
    return 0;
}
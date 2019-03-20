#include <base.h>
int f=timercreate();
int y=20;

int init(void)
{
timerstart(f,500,500,"book",1);


return 0;
}

int event(int type, int p1, int p2)
{
    if(KY_DOWN == type)
    {
        switch(p1)
        {
        case _SRIGHT:
           if(f)timerdel(f); exit();
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

void book(int data)
{
cls(5,5,5);
dline(20,50,20,250,255,255,255);
dline(220,50,220,250,255,255,255);

dline(120,80,120,280,255,255,255);
dline(20,50,120,80,225,255,255);
dline(220,50,120,80,255,255,255);
dline(20,250,120,280,255,255,255);
dline(220,250,120,280,255,255,255);

dline(15,45,15,250,255,255,255);
dline(225,45,225,250,255,255,255);





int i;

for(i=70;i<=230;i+=20)
{dline(20,i,110,i+30,255,255,255);
dline(220,i,130,i+30,255,255,255);
}

if(y<=220)y+=10;
else y=20;

dline(y,20,y,250,255,255,255);

dline(y,20,120,80,255,255,255);
dline(y,250,120,280,255,255,255);

dtext("ºÃºÃÑ§Ï°£¡",y,0,y,rand()%256,y,0,1);



ref(0,0,240,320);
}
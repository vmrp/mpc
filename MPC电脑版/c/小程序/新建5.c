#include <base.h>
int f,th=24;


int init(void)
{f=timercreate();
timerstart(f,1,1,"time",1);    return 0;
}

int event(int type, int p1, int p2)
{
    if(KY_DOWN == type)
    {
        switch(p1)
        {
        case _SRIGHT:
 if(f)timerdel(f);exit();
            break;
        case _1:
uint32 f=open("a.bmp",8);
read(f,getscrbuf(),SCRW*SCRH*2);
close(f);
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

void time(int data)
{
//drect(0,SCRH/8+th+1,SCRW,SCRH/8*3+5,0,0,0);
int n;
for(n=1;n<=10;n++)
dline(rand()%SCRW,rand()%SCRH,SCRW-rand()%SCRW,SCRH-rand()%SCRH,rand()%255,rand()%255,rand()%255);

ref(0,0,SCRW,SCRH);
}

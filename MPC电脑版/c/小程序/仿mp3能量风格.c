#include <base.h>


int tx=0,f=timercreate();

int init(void)
{
timerstart(f,1,1,"fg",1);
printf("%d\n",f);

    return 0;
}

int event(int type, int p1, int p2)
{
    if(KY_DOWN == type)
    {
        switch(p1)
        {
        case _SRIGHT:
     timerdel(f); printf("\n%d",f);   exit();
            break;
        case _1:
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
{    return 0;
}


void fg(int data)
{

int r=rand()%265;
int g=rand()%265;
int b=rand()%265;
int i,i2;
effsetcon(0,0,SCRW,SCRH,128,128,128);
dtext("天天设计",tx,0,255,255,255,0,0);
tx+=(tx<SCRW?5:-SCRW);

int y;
for(i=0;i<=240;i+=20)
{
y=rand()%256;
drect(i,y,15,320-y,r-i,g+i,b);
}

for(i2=30;i2<=300;i2+=10)
drect(0,i2,240,1,0,0,0);
ref(0,0,240,320);
}

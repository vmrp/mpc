#include <base.h>

int32 timer[3];

int a,b,c;



void timer0(int data)
{
    sand(getuptime());
    cls(rand()%255,rand()%255,rand()%255);
    ref(0,0,SCRW,SCRH);
    printf("timer0,data=%d,a=%d\r\n",data,a++);
}
void timer1(int data)
{
    printf("timer1,data=%d,b=%d\r\n",data,b++);
}
void timer2(int data)
{
    printf("timer2,data=%d,c=%d\r\n",data,c++);
}

int init(void)
{
    timer[0]=timercreate();
    timer[1]=timercreate();
    timer[2]=timercreate();
    timerstart(timer[0],1000,1234,"timer0",1);
    timerstart(timer[1],2000,2345,"timer1",1);
    timerstart(timer[2],3000,3456,"timer2",1);
    return 0;
}

int event(int type, int p1, int p2)
{
    if(KY_DOWN == type)
    {
        switch(p1)
        {
        case _SRIGHT:
            exitapp();
            break;
        case _1:
            break;
        case _2:
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

int exitapp(void)
{
    timerdel(timer[1]);
    timerdel(timer[0]);
    timerdel(timer[2]);
    exit();
    return 0;
}
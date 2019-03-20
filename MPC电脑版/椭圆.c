#include "mrc_base.h"
#include "mrc_exb.h"


#define PI (3.1415926535897932384626433832795)

double sinx(double x)
{
    double result=x,temp=x;
    double den=x,fac=1;
    int n=1,sign=1;
    while((temp>1e-5)||(temp<-1e-5)) 
    {
        n++,fac*=n,den*=x;
        n++,fac*=n,den*=x;
        temp=den/fac;sign=-sign;
        result=sign>0?result+temp:result-temp;
    }
    return result;
}

double cosx(double x)
{
    x=1.57079-x;
    return sinx(x);
} 

int32 timer;
double x,y,degree;
int32 w,h;
uint8 r,g,b;

void draw(int32 data){
    //ÍÖÔ²·½³Ì
    x=100 + 90 * cosx(degree);
    y=100 + 30 * sinx(degree);
    degree=degree+0.05;
    if(degree>2*PI) {
        degree=0;
        r=mrc_rand();
        g=mrc_rand();
        b=mrc_rand();
    }

    mrc_drawRect((int32)x,(int32)y,w,h,r,g,b);
    mrc_refreshScreen((int32)x,(int32)y,w,h);

}

int32 mrc_init(void)
{
    w=h=10;
    r=mrc_rand();
    g=mrc_rand();
    b=mrc_rand();
    mrc_LCDCanSleep(0);
    mrc_clearScreen(0,0,0);
    mrc_refreshScreen(0,0,240,320);
    timer=mrc_timerCreate();
    mrc_timerStart(timer,10,0,draw,TRUE);
    return MR_SUCCESS;
}


int32 mrc_event(int32 code, int32 param0, int32 param1)
{
    if(code == MR_KEY_PRESS){
        mrc_exit();
    }
    return MR_SUCCESS;
}

int32 mrc_pause()
{
    return MR_SUCCESS;	
}

int32 mrc_resume()
{
    return MR_SUCCESS;
}
int32 mrc_exitApp(void)
{
    return 0;
}
int32 mrc_extRecvAppEventEx()
{
    return 0;
}
int32 mrc_extRecvAppEvent()
{
    return 0;
}
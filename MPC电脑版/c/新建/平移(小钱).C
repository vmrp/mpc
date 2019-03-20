#include <base.h>


int init(void)
{chushihua();
    beijing();
    qie(0,n);
    ref(0,0,SCRW,SCRH);
return 0;
}
void chushihua()
{
da=4;//界面最大为4个
xiao=0;//最小为0个
//0~4共5个界面
n=da;//初始化最后一个界面

char *dt="c/book.bmp";
bei=getlen(dt);
if(bei>0)
{int ff2;
ff=malloc(bei);
ff2=open(dt,4);
read(ff2,ff,bei);
close(ff2);
}
}

int n,//控制界面
bei,
da,//控制界面最大值
xiao//控制界面最小值
;
uint16 *ff;//背景指针

int jie0(int xx)
{
    effsetcon(xx+10,20,SCRW-20,SCRH-80,255,155,155);
    dtext("界面0",xx+55,60,0,0,0,0,1);

return 0;
}
int jie1(int xx)
{
    effsetcon(xx+10,20,SCRW-20,SCRH-80,155,255,155);
    dtext("界面1",xx+55,60,0,0,0,0,1);

return 0;
}
int jie2(int xx)
{
    effsetcon(xx+10,20,SCRW-20,SCRH-80,155,155,255);
    dtext("界面2",xx+55,60,0,0,0,0,1);

return 0;
}
int jie3(int xx)
{
    effsetcon(xx+10,20,SCRW-20,SCRH-80,255,155,255);
    dtext("界面3",xx+55,60,0,0,0,0,1);

return 0;
}
int jie4(int xx)
{
    effsetcon(xx+10,20,SCRW-20,SCRH-80,155,255,255);
    dtext("界面4",xx+55,60,0,0,0,0,1);

return 0;
}
void beijing()
{
if(bei>0){bmpshowflip(ff,0,0,SCRW,SCRW,SCRH,0x0000,0,0,0);}
else{cls(255,255,255);}
//背景
}
void pingyi(int j1)
{
//平移算法，勿动！
 int x,su=SCRW;
 if(j1==-1)
 { n--;
   if(n<xiao){n=da;}
 }
 if (j1==1)
 {n++;
  if(n>da){n=xiao;}
}

for(x=0;x<=SCRW;x=x+su)
{
su=su/2;
if(su<1){su=1;}
beijing();

 if(j1<0)
 { qie(x*j1+SCRW,n);
  if(n!=da)
  { qie (x*j1,n+1);}
 else
 { qie(x*j1,xiao);}
} 

 if(j1>0)
 { qie(x-SCRW,n);
   if(n!=xiao)
   { qie(x,n-1); }
 else {qie(x,da); }
}
ref(0,0,SCRW,SCRH);}

//这里可写移动完毕后的内容
}
void qie(int xx,int nn)
{//由nn控制界面nn=n
if(nn==0){jie4(xx);}
if(nn==1){jie3(xx);}
if(nn==2){jie2(xx);}
if(nn==3){jie1(xx);}
if(nn==4){jie0(xx);}
}
int event(int type, int p1, int p2)
{
    if(KY_DOWN == type)
    {
        switch(p1)
        {
        case _SRIGHT:
            free(ff);
exit();
            break;
        case _LEFT:
  pingyi(1);
            break;
     case _RIGHT:
  pingyi(-1);
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
/*小钱作品*/
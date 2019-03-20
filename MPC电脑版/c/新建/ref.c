#include<base.h>
int a,b,h,c,i,n,x;
void sx()
{
refresh(0,0,SCRW,SCRH);
}

int init(void)
{
cls(0,0,255);
re1();
cls(150,50,50);
re2();
cls(255,0,0);
re3();
cls(255,180,230);
re4();
cls(60,255,0);
re5();
cls(0,160,50);
re6();
cls(0,0,255);
re7();
cls(150,50,50);
re8();
cls(255,0,0);
re9();
cls(255,180,230);
re10();
cls(60,255,0);
re11();
cls(0,160,50);
re12();
cls(0,0,255);
re13();
cls(150,50,50);
re14();
cls(255,0,0);
return 0;
}
void re1()
{
int a;
for(a=0;a<=40;a=a+2)
{
refresh(0,0,SCRW,a);
refresh(0,40,SCRW,a);
refresh(0,80,SCRW,a);
refresh(0,120,SCRW,a);
refresh(0,160,SCRW,a);
refresh(0,200,SCRW,a);
refresh(0,240,SCRW,a);
refresh(0,280,SCRW,a);
refresh(0,320,SCRW,a);
}
}
void re2()
{
int a;
for(a=0;a<=30;a=a+2)
{
refresh(0,0,a,SCRH);
refresh(30,0,a,SCRH);
refresh(60,0,a,SCRH);
refresh(90,0,a,SCRH);
refresh(120,0,a,SCRH);
refresh(150,0,a,SCRH);
refresh(180,0,a,SCRH);
refresh(210,0,a,SCRH);
refresh(240,0,a,SCRH);
}
}
void re3()
{
for(a=0;a<=SCRW;a=a+3)
{
refresh(0,0,a,320);
refresh(0,0,a,320);
}
}
void re4()
{
int a;
for(a=1;a<=81;a=a+3)
{
refresh(0,0,a,a);
refresh(80,0,a,a);
refresh(160,0,a,a);
refresh(0,80,a,a);
refresh(160,80,a,a);
refresh(80,80,a,a);
refresh(0,160,a,a);
refresh(160,160,a,a);
refresh(80,160,a,a);
refresh(0,240,a,a);
refresh(160,240,a,a);
refresh(80,240,a,a);
refresh(80,80,a,a);
refresh(0,240,a,a);
}
}
void re5()//ÌØÊâË¢ÐÂ
{
int a,b,c;
b=SCRW/4;
c=SCRH/6;
for(a=0;a<=b;a=a+2)
{
refresh(b-a,b-a,2*a,2*a);
refresh(b*3-a,b-a,2*a,2*a);
refresh(b-a,b*3-a,2*a,2*a);
refresh(b*3-a,b*3-a,2*a,2*a);
refresh(b*2-a,c*2-a,2*a,2*a);
refresh(b-a,c*5-a/2,2*a,2*a);
refresh(b*3-a,c*5-a/2,2*a,2*a);
refresh(b*2-a,SCRH-a/2,2*a,a);
}
}
void re6()
{
for(x=0;x<=320;x=x+3)
{
refresh(0,0,35,x);
refresh(35,320-x,35,320);
refresh(70,0,35,x);
refresh(105,320-x,35,320);
refresh(140,0,35,x);
refresh(175,320-x,35,320);
refresh(210,0,35,x);
}
}
void re7()
{
for(a=0;a<=SCRW/2;a=a+1)
{
refresh(SCRW/2-a,0,1,SCRH);
refresh(SCRW/2+a,0,1,SCRH);
}
}
void re8()
{
for(a=0;a<=SCRW/2;a=a+1)
{
refresh(SCRW/2-a,SCRH/2-a*SCRH/SCRW,2*a,2*a*SCRH/SCRW);
}
}
void re9()
{
for(a=0;a<=35;a=a+1)
{
effsetcon(0,0,240,320,220+a,a,220+a);sx();
}
}
void re10()
{
int i,n,h;
n=SCRW/16;
h=SCRH/n;
n=n/2;
for(i=0;i<n;i=i+1)
{
refresh(0,h*i,SCRW,h);
sleep(150);
refresh(16*i,0,16,SCRH);
sleep(150);
refresh(0,SCRH-h*(i+1),SCRW,h);
sleep(150);
refresh(SCRW-16*(i+1),0,16,SCRH);
sleep(150);
}
sx();
}
void re11()
{
for(a=0;a<=SCRW/2;a=a+1)
{
refresh(SCRW/2-a,0,1,SCRH);
refresh(SCRW/2+a,0,1,SCRH);
}
sleep(1888);
}
void re12()
{
for(a=0;a<=SCRH/2;a=a+1)
{
refresh(0,SCRH/2-a,SCRW,1);
refresh(0,SCRH/2+a,SCRW,1);
}
sleep(2000);
}
void re13()
{
for(a=0;a<=SCRH;a=a+1)
{
refresh(0,a,SCRW,1);
refresh(SCRW,SCRH-a,SCRW,1);
}
}
void re14()
{
int a;
for(a=0;a<=320;a=a+3)
{
refresh(240-a,0,240,46);
refresh(0,46,a,46);
refresh(240-a,92,240,46);
refresh(0,138,a,46);
refresh(240-a,184,240,46);
refresh(0,230,a,46);
refresh(240-a,276,240,46);
}
}

void refresh(int x,int y,int w,int h)
{
ref(x,y,w,h);
}

int event(int type,int p,int p2)
{
exit();
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

void cls1(int r,int g,int b)
{
cls(r,g,b);
}
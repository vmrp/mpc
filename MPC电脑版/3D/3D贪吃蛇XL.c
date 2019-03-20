#include <base.h>

/*加载rc文件dfs，返回指针*/
#include "3d/snake.h"
#include "3d/XL.h"


#define scrw SCRW
#define scrh SCRH

int k,gbinit;
char *bar[5]={"开始游戏","游戏帮助","关 于","退 出","小厘"};

int init(void)
{
int logof;
uint16 *logo;
int i;
cl();

for(i=0;i<=3;i++)
dtext(bar[i],50,160+i*30,180,180,180,0,2);

logo=malloc(160*32*2);
logof=open("3d/logo.bmp",1);
read(logof,logo,160*32*2);
close(logof);
bmpshowflip(logo,(scrw-160)/2,50,160,160,32,6,0,0,2016);
free(logo);
dtext("风的影子 作品",90,90,30,100,150,0,1);

gb();
 return 0;
}

int event(int type, int p, int p2)
{
if(k==0) initevent(type,p,p2);

else if(k==1) gameevent(type,p,p2);

else if(k==2)
helpevent(type,p,p2);

else if(k==3)
aboutevent(type,p,p2);
    return 0;
}

int initevent(int type,int p,int p2)
{
if (type==0)
 {
switch(p)
  {
case _UP:
hy();gbinit--;if(gbinit<0)gbinit=3;gb();break;
case _DOWN:
hy();gbinit++;if(gbinit>3)gbinit=0;gb();
break;

case _SLEFT:
case 20:
if(gbinit==0)main();
else if(gbinit==1)help();
else if(gbinit==2)about();
else if(gbinit==3)
break;
case _SRIGHT:
break;
  }
 }

else if(type==1)
 {
switch(p)
  {
case 17:
case 20:
if(gbinit==0)k=1;
else if(gbinit==1)k=2;
else if(gbinit==2)k=3;
else if(gbinit==3)exitapp();break;

case _SRIGHT:
{exitapp();break;}

  }
 }
return 0;
}


int pause(void)
{ return 0;}

int resume(void)
{return 0;}
void cl()
{cls(50,50,50);}
void rf()
{

ref(0,0,scrw,scrh);}

void hy()
{
nrect(10,158+gbinit*30,scrw-20,30,50,50,50);
}

void gb()
{
nrect(10,158+gbinit*30,scrw-20,30,50,150,220);
rf();
}

int exitApp(){return 0;}

int exitapp(void)
{
exit();
free(ptr);
return 0;
}



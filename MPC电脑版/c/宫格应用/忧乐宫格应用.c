/*应用2~12，可自己添加！*/
#include <base.h>
#include "c/宫格应用/wen.c"
#include "c/宫格应用/help.c"
int a=1;
int jm=1;
int init(void)
{
open();
yl(); 

return 0;
}

int event(int type, int p1, int p2)
{
    if(KY_DOWN == type&&jm==1)
 {
 switch(p1)
{
case _SRIGHT:
exitapp();break;
case 1:
case 2:
case 3:
case 4:
case 5:
case 6:
case 7:
case 8:
case 9:
a=p1;
yl();break;

case _SLEFT:
case _SELECT:
star();break;


case _LEFT:
if(a>1)a--;
yl();break;

case _RIGHT:
if(a<12)a++;
yl();break;

case _UP:
if(a>3)a-=3;
yl();break;

case _DOWN:
if(a<9)a+=3;
yl();break;


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


void yl()
{
cls(0,0,0);
drect(0,0,240,30,55,55,55);
drect(0,290,240,30,55,55,55);
dtext("忧乐宫格应用",50,5,210,221,210,0,1);
dtext("启动",0,295,210,210,210,0,1);
dtext("退出",185,295,210,221,210,0,1);
int i;
for(i=30;i<=320;i+=65)
dline(0,i,240,i,255,255,0);

for(i=0;i<4;i+=1)
dline(i*80,30,i*80,290,255,255,0);
dline(238,30,238,290,255,255,0);

dtext("1.文章",5,45,255,255,255,0,1);

dtext("2.关于",90,45,255,255,255,0,1);
dtext("3.无",170,45,255,255,255,0,1);
dtext("4无",10,110,225,255,255,0,1);
dtext("5.无",90,110,255,255,255,0,1);
dtext("6,无",170,110,255,255,225,0,1);
dtext("7.无",10,175,255,255,255,0,1);
dtext("8.无",90,175,255,255,255,0,1);
dtext("9.无",170,175,255,255,255,0,1);
dtext("10.无",10,240,225,255,255,0,1);
dtext("11.无",90,240,255,255,255,0,1);
dtext("12.无",170,240,255,255,255,0,1);




int x,y;
if(a<=3)
{x=(a-1)*80;
y=(a-1)/4*65+30;
}

else if(a<=6)
{x=(a-4)*80;
y=(a-3)/4*60+95;
}


else if(a<=9)
{x=(a-7)*80;
y=(a-6)/4*60+160;
}

else 
{x=(a-10)*80;y=(a-9)/4*60+225;}
effsetcon(x,y,80,65,138,138,138);

ref(0,0,240,320);
}

void exitapp()
{
drect(0,120,240,50,255,255,55);
drect(5,122,230,46,0,0,0);
dtext("正在退出…",80,130,255,255,255,0,1);



ref(0,0,240,320);
sleep(1500);
exit();
}


void star()
{
switch(a)
{
case 1:function1();break;
case 2:function2();break;
case 3:function3();break;
case 4:function4();break;
case 5:function5();break;
case 6:function6();break;
case 7:function7();break;
case 8:function8();break;
case 9:function9();break;
case 10:function10();break;
case 11:function11();break;
case 12:function12();break;

}

}


void function1()
{y1();}
void function2()
{
help();sleep(3000);
yl();
}
void function3()
{w();}
void function4()
{w();}
void function5()
{w();}
void function6()
{w();}
void function7()
{w();}
void function8()
{w();}
void function9()
{w();}
void function10()
{w();}
void function11()
{w();}
void function12()
{w();}


void w()
{
drect(0,120,240,50,255,255,255);
drect(2,122,236,46,2,5,5);
dtext("该菜单暂未添加！",10,125,255,255,255,0,1);
ref(0,0,240,320);
sleep(1000);

jm=1;yl();

}

void open()
{
cls(0,0,0);
int i;
for(i=0;i<=140;i+=4)
{
cls(5,10,0);
dtext("忧乐、宫格应用",10,i,255,25,255,0,1);
dtext("无所谓、忧乐 作品",10,320-i,255,255,255,0,1);
ref(0,0,240,320);
}

}
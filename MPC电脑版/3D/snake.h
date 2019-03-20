/*
3D贪吃蛇头文件"3D snake.h"
本源码最终解释权归作者所有，不得修改作者信息，不得修改破坏源码
*/

//作者：风的影子(小厘)

#include <base.h>

#define J 5

uint16 *food; //食物图片
uint16 *body; //蛇身
uint16 *ptr=malloc(SCRW*SCRH*2); //背景
uint16 a=55;


int32 
 zb[9], //(蛇)数组坐标
 g, //控制移动和暂停
 x,
 y,
 z,
 fx, //移动方向
 sw, //食物坐标zb[5]
 fen, //分数
 
 time,
 f;

 char fentxt[20];

int main()
{
int i;

food=malloc(5120);
body=malloc(512);
//ptr=malloc(scrw*scrh*2);
for(i=0;i<=4;i++) //初始化蛇的位置
zb[4-i]=i;

f=open("3d/food.bmp",1);
read(f,food,5120);
close(f);

f=open("3d/body.bmp",1);
read(f,body,512);
close(f);

cls(50,50,50);
toolbar("开始","返回");
cube();
memcpy(ptr,getscrbuf(),scrw*scrh*2);

zb[5]=rand()%999;
time=timercreate();
fx=2;
tmove(0);
rf();
return 0;
}

int gameevent(int type, int p, int p2)
{
if(type==0)
 {
switch(p)
  {
case _UP:
case 2:
upm();
break;
case _DOWN:
case 8:
downm();
break;
case _LEFT:
case 4:
mleft();
 break;
case _RIGHT:
case 6:
rightm();
break;
case _SLEFT:
case _SELECT:
if(g==0){g=2;timerstart(time,500,0,"tmove",1);}
else {g=0;timerstop(time);}
break;
case _SRIGHT:
end();
break;
  }
 }

if(type==1&&p==18)
k=0;



return 0;
}

int helpevent(int type,int p,int p2)
{
if(type==0&&p==_SRIGHT)
init();
if(type==1&&p==_SRIGHT)
k=0;
return 0;
}

int aboutevent(int type,int p,int p2)
{
if(type==0&&p==_SRIGHT)
init();
if(type==1&&p==_SRIGHT)
k=0;
return 0;
}



int end()
{
free(food);free(body);timerdel(time);g=0;init();

return 0;
}

enum
{
ahead, //向前
backward, //向后
up, //向上
down, //向下
left, //向左
right //向右
};

void move(int yd) //移动，每次都把最后一个数丢掉，然后产生新的数补充头部，yd为移动方向，一共六个方向
{
int i;
for(i=3;i>=0;i--)
{zb[i+1]=zb[i];}
if(yd==ahead)zb[0]-=100;
else if(yd==backward)zb[0]+=100;
else if(yd==up)zb[0]-=10;
else if(yd==down)zb[0]+=10;
else if(yd==left)zb[0]--;
else if(yd==right)zb[0]++;
load();
}


void load() //把蛇加载到屏幕上
{
int px[9],min,i,j,temp,m,zj;//排序

bmpshowflip(ptr,0,0,scrw,scrw,200,0,0,0,0);

 //判断是否吃到食物
if(zb[0]==zb[5]){zb[5]=rand()%999;fen+=10;}

for(i=0;i<=5;i++)
px[i]=zb[i];

for(i=0;i<5;i++) //排序，实现立体加载，以此来确定加载顺序
 {
for(j=i+1;j<6;j++)
  {
if(px[i]>px[j]) 
   {
temp=px[i];
px[i]=px[j];
px[j]=temp;
   }
  }
 }
for(i=0;i<6;i++)
 {
m=px[i]%100;
x=m%10;
y=m/10;
z=px[i]/100;
zj=z*J;
if( (zb[0]==px[i]) || (zb[5]==px[i]) ) //蛇头和食物
{bmpshowflip(food,60+x*10-zj,120+y*(-10)+zj,16,16,16,6,0,y*16,2016);}
else 
bmpshowflip(body,60+x*10-zj,120+y*(-10)+zj,16,16,16,6,0,0,2016);
 } sprintf(fentxt,"分数：%d",fen);
dtext(fentxt,20,3,150,150,150,0,2);
dline(115,80,165,30,80,80,80);
dline(15,80,115,80,110,100,100);
dline(115,80,115,180,110,100,100);

ref(0,0,scrw,200);
}



   void tmove(int a)
{
int i,zbz,zby,zbx;
for(i=3;i>=0;i--)//丢掉最后一位数
{zb[i+1]=zb[i];}
zbz=zb[0]/100;
zby=zb[0]%100/10;
zbx=zb[0]%100%10;

if(fx==ahead)
{if(zbz==0)zb[0]+=900;else zb[0]-=100;}
else if(fx==backward)
{if(zbz==9)zb[0]-=900;else zb[0]+=100;}
else if(fx==up)
{if(zby==9)zb[0]-=90; else zb[0]+=10;}
else if(fx==down)
{if(zby==0)zb[0]+=90;else zb[0]-=10;}
else if(fx==left)
{if(zbx==0)zb[0]+=9; else zb[0]--;}
else if(fx==right)
{if(zbx==9)zb[0]-=9; else zb[0]++;}
load();
}


void upm()
{
switch(fx)
 {
case ahead:
case backward:
fx=up;break;
case up:
case down:
fx=ahead;break;
case left:
case right:
fx=up;break;
 }
}

void downm()
{
switch(fx)
 {
case ahead:
case backward:
fx=down;break;
case up:
case down:
fx=backward;break;
case left:
case right:
fx=down;break;
 }
}

void mleft()
{
switch(fx)
 {
case ahead:
case backward:
case up:
case down:
fx=left;break;
case left:
case right:
fx=backward;break;
 }
}

void rightm()
{
switch(fx)
 {
case ahead:
case backward:
case up:
case down:
fx=right;break;
case left:
case right:
fx=ahead;break;
 }
}

void help()
{
cl();
drawtxt2("    方向键：上 下 左 右或2 4 6 8键控制贪吃蛇移动。
这是3D的贪吃蛇哦！所以吃食物也变得没那么简单，告诉你一个密秘，当蛇头的颜色和食物的颜色一样时，就说明它们在同一个平面里。

",10,10,scrw-20,scrh,150,150,150,2);
toolbar(" ","返回");
rf();
}

void about()
{
cl();
drawtxt2("
游戏名：3D贪吃蛇XL
版本：1.0
作者：风的影子(小厘)
声明：不得修改源码，不得修改破坏作者信息

",10,10,scrw-20,scrh,150,150,150,2);
toolbar(" ","返回");
rf();
}

void cube()
{
int cx,cy,cz,i;
for(i=0;i<=10;i++)
 {

//侧面
dline(15+i*5,80-5*i,15+i*5,180-5*i,60,65,65);
dline(15,80+i*10,65,30+i*10,60,65,65);
//后面
dline(65,30+i*10,165,30+i*10,80,80,80);
dline(65+i*10,30,65+i*10,130,80,80,80);

//底面
dline(65-i*5,130+i*5,165-i*5,130+i*5,80,80,80);
dline(65+i*10,130,15+i*10,180,80,80,80);
 }
dline(15,80,15,180,110,100,100);
dline(15,80,65,30,80,80,80);
dline(15,180,115,180,110,100,100);
}


//作者：风的影子
/*
闪侠
by:每天进步一点   小孩图片提供
QQ:643552878；1281132839
2012.5
左软键暂停,右软键退出,
中间键必杀技。#键初始化
*/
#define FONT 1
#define photo "c/闪侠/jpg/F.jpg"

#define photo_F "c/闪侠/jpg/FF.jpg"  
#define photo_D "c/闪侠/jpg/蛋.jpg"

int pw=24,ph=24;
//闪侠图片宽高
int pw2=15,ph2=15;
//蛋图片宽高
#define game_text(txt,x,y,r,g,b) dtext(txt,x,y,r,g,b,0,FONT)

int LV=10;
//初始化中屏幕阻碍物的数量,不能太多了,否则速度会很慢。

#define FV 5
//刷新速度

#define KY_V 1
//按速度

int *fx,*fy;
//阻碍物的坐标

int8 *fv;
//阻碍物的速度

int ffx,ffy;
//飞机的坐标

//int8 ffv;
//飞机的速度,未加入

int8 bxs;
//必杀技

//屏幕分划
int WK=20;
int HK=20;

//每分大小
int FW=SCRW/WK;
int FH=SCRH/HK;


int32 timer=NULL;
int32 timer_an=NULL;
//游戏定时器和按键定时器句柄,

int on=NULL;
//游戏状态
int8 off=NULL;

uint16 *FF,*DD,*scr=getscrbuf();

int game_init()
{
 if(timer)stop();
 if(timer_an)
 timerstop(timer_an);
 on=off=NULL;

 int len=pw*ph*2;
 if(FF!=NULL)free(FF);
 FF=malloc(len);

 int len2=pw2*ph*2;
 if(DD!=NULL)free(DD);
 DD=malloc(len);

 cls(100,200,250);
 img(photo_F,0,0);
 for(int y=0;y<ph;y++)
 memcpy(FF+y*pw,scr+y*SCRW,pw*2);
 cls(100,200,250);
 img(photo_D,0,0);
 for(y=0;y<ph2;y++)
 memcpy(DD+y*pw2,scr+y*SCRW,pw2*2);
  game_inits();
  return 0;
}

//申请内存
void malloc_()
{
 if(fx!=NULL)
 free(fx);
 fx=malloc(LV*4);

 if(fy!=NULL)
 free(fy);
 fy=malloc(LV*4);

 if(fv!=NULL)
 free(fv);
 fv=malloc(LV*4);
}


//初始化
void game_inits()
{ 
 for(int a=SCRW;a>-80;a-=tw/2)
{
 cls(250,250,250);
 img(photo,a,SCRH/2);
 game_text("Loading.......",(SCRW/2)-40,(SCRH/2)+65,255,2*a,50);
 game_text("小孩-每天进步一点",a,50,2*a,a,a);
 REF;

 }
 on=1;

 malloc_();
 memset(fx,NULL,LV*4);
 memset(fy,NULL,LV*4);

for(a=0;a<LV;a++)
{ fx[a]=rand_x();
  fy[a]=0;
  fv[a]=rand_v();
}

ffx=FW*(FW/2);
ffy=FH*(HK-1);
//ffv=3;
bxs=5;


if(timer)timerdel(timer);
timer=timercreate();

if(timer_an)timerdel(timer_an);
timer=timercreate();
timer_an=timercreate();
start();


}

int game_event(int type, int p1, int p2)
{
 if(KY_DOWN==type&&!off)
 switch(p1)
 {
  case _SRIGHT:
  stop();
  over();
  break;

 case _SLEFT:
 if(on)stop();
 else start();
 on=!on;
 break;
 
 case _POUND:
 game_inits();break;

 case _SELECT:
 if(on)BX();break;


case _UP:case _DOWN:
case _LEFT:case _RIGHT:
 case _2:case _8:
 case _4:case _6:
 if(on)
{
 ffxy(p1);
 timerstart(timer_an,KY_V,p1,"ffxy",1);
 }
      break;

 }
else if(type==KY_DOWN&&off)
 switch(p1)
{ 
 case _SLEFT:
 jm=0;kz();
 game_exitapp();break;

 case _SRIGHT:
 off=NULL;
 game_inits();break;
}

else
if(type==KY_UP&&!off)
switch(p1)
{
 case _UP:case _DOWN:
 case _LEFT:case _RIGHT:
 case _2:case _8:
 case _4:case _6:
 if(on)
 timerstop(timer_an);

}
    return 0;
}

int game_pause()
{
 if(on)
 { stop();
   stoptimer(timer_an);
 }
return 0;
}

int game_resume()
{ 
 if(on)start();
 return 0;
}


//退出前的工作
void game_exitapp()
{
//删除定时器
 if(timer)
 timerdel(timer);
 if(timer_an)
 timerdel(timer_an);
 timer=timer_an=NULL;

//释放内存
 if(FF!=NULL)free(FF);
 if(DD!=NULL)free(DD);
 if(fx!=NULL)free(fx);
 if(fy!=NULL)free(fy);
 if(fv!=NULL)free(fv);
 fx=fy=NULL;
 fv=NULL;

}

//开始
void start()
{
timerstart(timer,FV,0,"FFF",1);
}

//暂停
void stop()
{
timerstop(timer);
timerstop(timer_an);
}

//飞机移动
void ffxy(int p1)
{
if(on&&!off)
switch(p1)
{
 case _UP:
 case _2:
 if(ffy>0)ffy-=FH;
 else  ffy=FH*HK;
 break;

 case _DOWN:
 case _8:
 if(ffy<FH*HK)
 ffy+=FH; else ffy=0;
 break;

 case _LEFT:
 case _4:
 if(ffx>0)ffx-=FW;
 else ffx=FW*WK;
 break;

 case _RIGHT:
 case _6:
 if(ffx<FW*WK)
 ffx+=FW;
 else ffx=0;
 break;
}

}


//绘图,并检飞机查是否与阻碍物相撞
void FFF(int data)
{
 if(on&&!off)
{
 cls(100,200,250);
 int w=HK*FH;
 int b;
 for(int a;a<LV;a++)
 {
  if(!on&&off)break;

  fy[a]+=fv[a]*FH;
 if((fy[a]>=w)||(fy[a]<=NULL))
  {
   fx[a]=rand_x();
   fy[a]=0;
   fv[a]=rand_v();
  }

 if((ffx==fx[a])&&(fy[a]>FH/2))
 switch(1)
 {
  case (ffy==fy[a]):
  case (ffy-fy[a]<=FH*fv[a]):
  //case (fy[a]-ffy<=FH/2):
  sss();stop();over();
  b=1;break;
 }
if(b)break;


bmpshowflip(DD,fx[a],fy[a],pw2,pw2,ph2,BM_COPY,0,0,0);
//绘出阻碍物图像
 
}

if(!b)
{
 bmpshowflip(FF,ffx,ffy,pw,pw,ph,BM_COPY,0,0,0);
//绘飞机的图像
 REF;
 }

}

}


//随机产生一个非负数
int rand_()
{
 int a=rand();
 if(a>=0)return a;
 else return (-a);
}

//随机产生阻碍物的x坐标
int rand_x()
{
 for(int a=FW*2;1;a=rand_()%HK)
 if(a<=HK)break;
 return a*FW;
}

//随机产生阻碍物的速度
int rand_v()
{
 for(int a=FH*16;1;a=rand_()%5)
if(a<=9)break;
return a;
}


//必杀技
void BX()
{
if(bxs)
{
 for(int a=HK*FH;a>=0;a-=FH)
{
 drect(0,a,SCRW,a+FH,100,20,50);

 bmpshowflip(FF,ffx,ffy,pw,pw,ph,BM_COPY,0,0,0);
//飞机图像
 REF;
}

 for(a=0;a<LV;a++)
 if(fy[a]<=ffy)
 fy[a]=0;
 REF;

 if(bxs)bxs--;

}

}


//绘死亡图
void sss()
{
 effsetcon(0,SCRH/2,SCRW,30,138,138,138);
 game_text("您已死亡！游戏结束…",0,SCRH/2,255,5,5);
 REF;
 sleep(1000);
 REF;
}

//退出？
void over()
{
 drect(0,0,SCRW,SCRH,100,200,250);

 game_text("是返回主菜单？",SCRW/2-tw*3,SCRH-th*2,255,25,25);

 game_text("是",0,SCRH-th,25,25,25);

 game_text("否",SCRW-tw,SCRH-th,25,25,25);

 REF;

 on=NULL;//暂停开关
 off=!NULL;//结束开关

}
//by:每天进步一点
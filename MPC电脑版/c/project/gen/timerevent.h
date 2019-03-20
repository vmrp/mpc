/*
wildly 按键连发控件
QQ 319965592
使用方法:
int event(int type,int p,int pa)
{TimerEvent(type,p);
if(type == KY_DOWN)
{case _SRIGHT:
TimerEventDel();
exit();
break;
}
  }
TimerEventStop(type);
return 0;
} 连发的事件必须是键盘下时*/

#include <base.h>

int TimeCreate_Buffer[2]={timercreate(),timercreate()};

typedef struct
{
   int timermax;
   int timerlin;
   uint8 off_on;
   int typef;
   int time;
   int type;
   int p;
}Event;

Event Pe;

//启动连发
void TimerEvent(int type,int p)
{if(Pe.off_on)break;
if(Pe.typef)break;
Pe.type=type;
Pe.p=p;
timers(0,Pe.timermax,"TimeRun",1);
}

//不调用
void TimeRun(int pax)
{Stope(0);
timers(1,Pe.timerlin,"TimeTimer",1);
}

//不调用
void TimeTimer(int pax)
{Pe.typef=1;
event(Pe.type,Pe.p,0);
}

//停止连发
void TimerEventStop(int type)
{if(type == KY_UP)
{Pe.typef=0;
Pe.type=-1;
Pe.p=-1;
Stope(1);
} }

//销毁连发，退出时勿必调用
void TimerEventDel()
{
Pe.type=-1;Pe.p=-1;Pe.typef=1;
Stope(0);Stope(1);
}

/*设置连发时间，timermax触发连发时间sm，timerlin连发间隔时间sm*/
int TimerEventSet(int timermax,int timerlin)
{
if(!timermax || !timerlin)return -1;
  Pe.timermax=timermax;
  Pe.timerlin=timerlin;
return 0;}

//连发开关，f=0 ON f=1 OFF
void TimerEventOffOn(uint8 f)
{if(f > 1)break;
  Pe.off_on=f;
}

//初始化连发时间
TimerEventSet(500,100);

//控件独立定时器创建。
void timers(int t,int ms,char *f,int loop)
{
timerstart(TimeCreate_Buffer[t],ms,TRUE,f,loop);
}

//定时器停止。
void Stope(int t)
{
timerstop(TimeCreate_Buffer[t]);
}
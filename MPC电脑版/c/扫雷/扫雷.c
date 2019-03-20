/*扫雷
by:每天进步一点
QQ:643552878
E界ID:47241
*/

#include <base.h>
#include <c/扫雷/day.h>
#include <c/扫雷/格式输出.h>

#define LV 1
//难度1~3

#define W 5
//横格个数

#define H 6
//纵列个数

int Wlen=SCRW/W;
int Hlen=SCRH/H;
//每个格子的宽、高

#define anV 100
//长按间隔


//光标结构体
typedef struct
{
 int timer[2];
/*定时器句柄
0为长按定时器句柄
1为光标闪动定时器
*/
 int x;
 int y;
//光标的xy

 int x2;
 int y2;
//光标移动前的xy
 int s;
//已排雷数
}M;  M *Move;

 int Q[W][H];
/*
 <=0 无雷
  >0 有雷
*/
 int Flag[W][H];
/*标记周围地雷的个数
 -1未点开
 >-1标记，已点开
*/

//初始化
void start()
{
 int len=sizeof(M);
 int len2=sizeof(int)*W*H;
 if((len+len2*2)>getmemremain())
{ cls(255,255,255);
 text("内存不足，自动退出！",0,SCRH/2,0,0,0);
 REF;sleep(1500); exitapp();
}

 cls(0,0,0);
 text("初始化，请稍候…",tw,SCRH/2,255,255,0);
 REF;sleep(500);
 
 if(Move!=NULL)
{ timerdel(Move->timer[1]);
  free(Move);
}

 memset(Q,NULL,len2);
 memset(Flag,-1,len2);

 
 int x,y,S=LV+1;
 for(x=0;x<W;x+=rand()%S+1)
 for(y=0;y<H;y+=rand()%S+1)
{ Q[x][y]=-(rand()%S);
  //printf("%d-",Q[x][y]);
}
 Move=malloc(len);

 Move->x=Move->x2=0;
 Move->y=Move->y2=0;
 Move->s=0;
 Move->timer[0]=timercreate();
 Move->timer[1]=timercreate();
}

 int jm;
/*
 界面控制
 =0关于界面
 =1游戏界面
*/

int init()
{
 kz();
 return 0;
}


int inits()
{
 start();
 daw();
 return 0;
}

void kz()
{
 switch(jm)
{ case 0:about();break;
 case 1:inits();break;
}

}

int event(int type,int p1,int p2)
{
 switch(jm)
{  case 0:
 aboutEvent(type,p1,p2);
 break;
  case 1:
 LeiEvent(type,p1,p2);break;
}

return 0;
}

int aboutEvent(int type,int p1,int p2)
{
 if(type==KY_UP)
 {jm=1;kz();}

 return 0;
}

int LeiEvent(int type, int p1, int p2)
{
 if(KY_DOWN==type)
 {
 switch(p1)
 {
   case _SRIGHT:
  exitapp();break;

 case _0:inits();break;

 case _2:case _UP:
 case _8:case _DOWN:
 case _4:case _LEFT:
 case _6:case _RIGHT:
 an(p1);//短按
 timerstart(Move->timer[0],anV,p1,"an",1);
 //长按
 break;

 case _1:
 FlagLei(Move->x,Move->y);
 break;

  case _SELECT:
  case _5:
  delLei(Move->x,Move->y);break;
   }
  }

else if(type==KY_UP)
{ 
 switch(p1)
 {
 case _2:case _UP:
 case _8:case _DOWN:
 case _4:case _LEFT:
 case _6:case _RIGHT:
 timerstop(Move->timer[0]);break;
 //长按键抬起停止定时器

 }


 }
 return 0;
}


//退出前
void exitapp()
{
 if(Move!=NULL)
{ timerdel(Move->timer[1]);
  free(Move);
}
 dawExit();
 exit();
}
 //长按定时器函数
void an(int p1)
{
 
if(p1)
{
Move->x2=Move->x;
Move->y2=Move->y;
}
//将光标未移动前的xy坐标记录


 switch(p1)
 {
 case _2:case _UP:
 if(Move->y>0)Move->y--;
 else Move->y=H-1;
 break;

 case _8:case _DOWN:
 if(Move->y<H-1)Move->y++;
 else Move->y=0;
 break;

 case _4:case _LEFT:
 if(Move->x>0)Move->x--;
 else Move->x=W-1;
 break;

 case _6:case _RIGHT:
 if(Move->x<W-1)Move->x++;
 else Move->x=0;
 break;
 
}
 int x2=Move->x2,y2=Move->y2;
 
 drect(x2*Wlen+1,y2*Hlen+1,Wlen-1,Hlen-1,128,224,224);
 
 if((Flag[x2][y2]!=-1)&&(Flag[x2][y2]<9))
{ drect(x2*Wlen+1,y2*Hlen+1,Wlen-1,Hlen-1,0,250,0);
 dawLei(x2,y2);
}

if(Flag[x2][y2]==10)
dawFlagLei(x2,y2);

 drect(Move->x*Wlen+1,Move->y*Hlen+1,Wlen-1,Hlen-1,0,0,0);
//绘出光标

 REF;
}

int pause(void)
{return 0;}
int resume(void)
{return 0;}


//绘棋盘图
void daw()
{
 cls(128,224,224);
 for(int n=0;n<=SCRW;n+=Wlen)
{ dline(n,0,n,SCRH,0,0,0);
 REF;
}
 for(n=0;n<=SCRH;n+=Hlen)
{ dline(0,n,SCRW,n,0,0,0);
 REF;
}
 REF;an(0);
 timerstart(Move->timer[1],500,NULL,"dodo",1);
}


int Ms=1;
//光标闪烁
void dodo(int a)
{
 Ms=!Ms;
 int r=128,g=224,b=224;
 if(Ms)
 {r=0;g=0;b=0;}
 drect(Move->x*Wlen+1,Move->y*Hlen+1,Wlen-1,Hlen-1,r,g,b);
REF;
}

//绘出Q[x][y]周围地雷个数
void dawLei(int x,int y)
{
 if(Flag[x][y]==-1)
 Flag[x][y]=Lei(x,y);
 printf_d("%d",Flag[x][y],x*Wlen,y*Hlen,250,0,250);

}

//计算Q[x][y]周围地雷个数
int Lei(int x,int y)
{
 int s=0;

 if(y-1>=0)
{
 if(x-1>=0)
 {if(Q[x-1][y-1])s+=1;}
 if(Q[x][y-1])s++;
 if(x+1<W)
 {if(Q[x+1][y-1])s+=1;}
}
 if(x-1>=0)
 {if(Q[x-1][y])s+=1;}

 if(x+1<W)
 {if(Q[x+1][y])s+=1;}

if(y+1<H)
{ if(x-1>=0)
 {if(Q[x-1][y+1])s+=1;}
 if(Q[x][y+1])s+=1;
 if(x+1<W)
 {if(Q[x+1][y+1])s+=1;}
}

 return s;
}

void dawtext(char *txt)
{
 effsetcon(0,SCRH/2,SCRW,th,128,128,128);
 text(txt,0,SCRH/2,250,250,250);
 
}
void delLei(int x,int y)
{
 if(Move->s>=W*H)
 gameover(1);
 else if(Q[x][y])gameover(-1);
 else
 {
  if(Q[x][y]<=0&&Flag[x][y]==-1)
  {dawLei(x,y);
   Move->s++;
 //Flag[x][y]=0;
  }
 }

}
//标记
void FlagLei(int x,int y)
{
 if(Q[x][y])
 {
  Flag[x][y]=10;
  dawFlagLei(x,y);
 }
 else
 gameover(-2);
}


//绘标记地雷
void dawFlagLei(int x,int y)
{
 drect(x*Wlen+1,y*Hlen+1,Wlen-1,Hlen-1,255,0,0);
}

void gameover(int n)
{
 char *txt;
 if(n==-1)
 txt="挖到地雷！游戏结束！";
 else if(n==-2)
 txt="标记错误！游戏结束";
 else
 txt="雷已排完！扫雷成功！";
 dawtext(txt);
 REF;sleep(1500);
  inits();
}
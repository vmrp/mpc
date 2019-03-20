#include <base.h>
#include <c/天天/射靶/day.h>

int V1=10;
int V2=10;

int W=15,H=SCRH-th*2;

#define Jian "→"


typedef struct
{
 int timer;
 int open;
 int x;
 int y;
 int v;
}M;
 M *Move[2];
 int fen=0;
 int j=0;

int init()
{
 about();
 return 0;
}


void inits()
{
 
 Free();
 int len=sizeof(M);

 Move[0]=malloc(len);
 Move[1]=malloc(len);

 Move[0]->x=0; 
 Move[0]->y=SCRH/2;
 Move[0]->v=V1;

 Move[1]->x=SCRW-tw*2;
 Move[1]->y=th;
 Move[1]->v=V2;
 
 Move[0]->timer=timercreate();
 Move[1]->timer=timercreate();
 Move[0]->open=0;
 Move[1]->open=1;
 

 timerstart(Move[0]->timer,1,1,"dawJian",1);
 timerstart(Move[1]->timer,1,1,"dawBa",1);
 dawMain();
}


int event(int type, int p1, int p2)
{
 if(j<=0){j=1;inits();}

 else if(j&&KY_DOWN==type)
 {
 switch(p1)
  {
  case _SRIGHT:
  exitapp();break;

 case _SLEFT:
 Move[0]->open=1;
  break;

   }
  }
 return 0;
}

int pause()
{return 0;}

int resume()
{return 0;}

void Free()
{
 if(Move[0]!=NULL)
 { if(Move[0]->timer)
  timerstop(Move[0]->timer);
 free(Move[0]);
 }

 if(Move[1]!=NULL)
 {
 if(Move[1]->timer)
 timerstop(Move[1]->timer);
 free(Move[1]);
 }


}

void exitapp()
{

 Free();
 dawExit();
 exit();

}


void dawMain()
{
 int h=SCRH-th;

 cls(100,255,255);
 effsetcon(0,0,SCRW,th,128,128,128);
 effsetcon(0,h,SCRW,th,128,128,128);
 dline(0,th,SCRW,th,250,0,250);
 dline(0,h,SCRW,h,250,0,250);
 dtext("MPC、射靶游戏",SCRW/2-tw*4,0,25,255,0,FALSE,FONT);
 dtext("射靶",0,h,25,255,255,FALSE,FONT);
 dtext("退出",SCRW-tw*2,h,255,25,25,FALSE,FONT);
 dawFen();
 Move[0]->open=1;
 dawJian(0);
 Move[0]->open=0;
 dawBa(0);

 REF;
}
void dawFen()
{
 char *s1="得分:%d",s2[20];
 sprintf(s2,s1,fen);
 drect(0,SCRH-th*2,th*6,th,100,255,255);
 dtext(s2,0,SCRH-th*2,0,0,0,FALSE,FONT); 
 REF;
}

void dawJian(int p1)
{
 if(Move[0]->open)
{
 if(Move[0]->x<=Move[1]->x)
 {if(p1)
  Move[0]->x=Move[0]->x+Move[0]->v;
 }
 else
 {
 
  if((Move[0]->y-Move[1]->y)<=(Move[1]->v))
 {
 if((Move[0]->y-Move[1]->y)>0)
 {fen++;dawFen();}

 }

   Move[0]->x=0;
   Move[0]->open=0;
 }
 int x=Move[0]->x,x2=Move[1]->x,y=Move[0]->y;
  
drect(0,y,x,th,100,255,255);
 dtext(Jian,0,y,125,125,125,FALSE,FONT);
 dtext(Jian,x-tw,y,125,125,125,FALSE,FONT);
  
 REF;
 }

}

void dawBa(int p1)
{
 if(Move[1]->open)
 {
  if(Move[1]->y<(SCRH-th*2))
 {
 if(p1)
 Move[1]->y=Move[1]->y+Move[1]->v;
 }
 else
 {
 Move[1]->v=rand()%5+1;
 if(Move[1]->v<3)
 Move[1]->v+=5;

 Move[1]->y=th;
 }
 drect(Move[1]->x,th,W,H,50,125,125);
 drect(Move[1]->x,Move[1]->y,W,th,255,255,255);
 REF;

 }

}
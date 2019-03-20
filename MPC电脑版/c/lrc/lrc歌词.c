#include <base.h>

#define MusicLrc "c/lrc/1.lrc"
//歌词

typedef struct
{
 int8 f;/*分*/
 int8 s;/*秒*/
 int8 ms;/*毫秒*/
} lrcTime;

int timer=timercreate();
int lrcTimer;

char *lrc;
int time,nowTime;


int lrcY;


int init()
{
 cls(0,0,0);
 lcd(0);ref(0,0,SCRW,SCRH);
 OpenLrc(MusicLrc);
 timerstart(timer,100,500,"timerLrc",1); 
  return 0;
}

int event(int type, int p1, int p2)
{
 if(KY_DOWN==type)
 {
 switch(p1)
  {
  case _SRIGHT:
  exitapp();break;
  case _1:
            break;

   }
  }
 return 0;
}

int pause(void)
{return 0;}
int resume(void)
{return 0;}

void exitapp()
{
 timerdel(timer);
 int len=getlen(MusicLrc);
 len=len-strlen(lrc);
 free(lrc-len);
 lcd(1);exit();

}

void OpenLrc(char *name)
{
 int f=open(name,1);
 int len=getlen(name);
 lrc=malloc(len);
 read(f,lrc,len);
 close(f);
 lrc=strstr(lrc,"[00");


}
void timerLrc(int n)
{
 if(time==0)
 time=getuptime();
 int L=getuptime()-time;
 if(L>0)
 {
 lrcTime T;
 T.f=L/1000/60%60;
 T.s=L/1000%60;
 T.ms=L/1000;
 getlrc(T.f,T.s,T.ms);
 }


}
void getlrc(uint8 f,uint8 s,uint8 ms)
{
 if(strlen(lrc)<=10)exitapp();
 char *lrc2=strstr(lrc,"[");
 lrcTime T;
 T.f=atoi(lrc2+1);
 T.s=atoi(lrc2+4);
 T.ms=atoi(lrc2+7);

/* printf("%d %d %d \n[%2d:%2d,%2d]",f,s,ms,T.f,T.s,T.ms);
*/
 if(f==T.f&&s==T.s/*&&ms==T.ms*/)
 {
 lrc2=strstr(lrc2,"]");
 char *ptr=strstr(lrc2,"[")-2;
 int len=strlen(lrc2)-strlen(ptr);
 char *Txt=malloc(len);
 memset(Txt,NULL,len);
 strncpy(Txt,lrc2+1,len-1);

 printf("[%d:%d.%d]",T.f,T.s,T.ms);
 dawLrc(Txt);
 free(Txt);
 if(Txt!=NULL)
 lrc=lrc2+1;
}


}


void dawLrc(char *txt)
{
 int x,y,w,h,a; 
 textwh(txt,0,1,&w,&h);
 x=(SCRW-w)/2; 
 if(lrcY<SCRH-h*2)
 lrcY+=h*2;
 else
 {
 effsetcon(0,0,SCRW,SCRH,128,128,128);
 ref(0,0,SCRW,SCRH);
 lrcY=0;
 }
 y=lrcY;
 drect(0,y,SCRW,h,0,0,0);
 ref(0,y,SCRW,h); 
dtext(txt,x,y,200,200,100,FALSE,1);
 ref(x,y,w,h);
 dtext(txt,x,y,150,150,250,FALSE,1);
 ref(x,y+h/2,w,h/2);
 printf("%s",txt);
 dtext(txt,x,y,100,100,100,FALSE,1);

 dawCopy(x,y+h,w,h/2);
}

/*将屏幕范围的距形框里反向倒立，行成镜像效果*/
void dawCopy(int x,int y,int w,int h)
{
 uint16 *scr=getscrbuf()-(SCRH-(y+h))*2;
 int nn;
 for(int n=y;n<y+h;n++)
 {nn++;memcpy(scr+n*SCRW+x,scr+(n-nn*2)*SCRW+x,w*2);
 ref(x,y,w,h);
 
}

}
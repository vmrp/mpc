#include <base.h>

#define MusicLrc "c/lrc/2.lrc"
//歌词

int timer=timercreate();

char *lrc;
int time;


int lrcY;


int init()
{
 cls(0,0,0);
 lcd(0);ref(0,0,SCRW,SCRH);
 OpenLrc(MusicLrc);
 timerstart(timer,1000,NULL,"timerLrc",1); 
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
 int f=L/1000/60%60;
 int s=L/1000%60;
 int ms=L/1000;

 getlrc(f,s,ms);
 }


}
void getlrc(int f,int s,int ms)
{

 char *timeTxt=malloc(12);
 memset(timeTxt,NULL,12);
 sprintf(timeTxt,"[%02d:%d.",f,s);
 
// printf("%s\n",timeTxt);
 char *Txt=strstr(lrc,timeTxt);
 free(timeTxt);

 if(Txt!=NULL&&strlen(Txt)>10)
{
 Txt+=10;
 char *ptr=strstr(Txt,"[");
 int len=strlen(Txt)-strlen(ptr);
 char *Ptr;

 
 char *p,*p2;

 while(len<=1)
 {
 p=strstr(Txt,"]");
 p2=strstr(p,"[");
 len=strlen(p)-strlen(p2);
 Txt=p;
 ;
 }

 if(len>1)
 {Ptr=malloc(len);
 memset(Ptr,NULL,len);
 strncpy(Ptr,Txt,len);
 }

 dawLrc(Ptr);
 free(Ptr);
}
 
}


void dawLrc(char *txt)
{
 int x,y,w,h,a; 
 textwh(txt,0,1,&w,&h);
 if(w<SCRW)x=(SCRW*2-w*2)/4; 
 if(lrcY<SCRH-h*2)
 lrcY+=h*2;
 else
 {
 cls(0,0,0);
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
 {
 if(n>SCRH)break;
 nn++;
 memcpy(scr+n*SCRW+x,scr+(n-nn*2)*SCRW+x,w*2);
 ref(x,y,w,h);
 }

}
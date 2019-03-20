#include <base.h>
#include <c/报警器/day.h>

#define jpg img("c/报警器/lock.jpg",0,0)
#define V 200
//闪耀速度

#define bmp1 bmpshowflip(photo[0],0,0,SCRW,SCRW,th,BM_COPY,0,0,0)

#define bmp2 bmpshowflip(photo[1],0,SCRHth,SCRW,SCRW,th,BM_COPY,0,0,0)

uint16 *photo[2],*scr=getscrbuf();

int w=NULL;

int timer=timercreate();

int init()
{
 about();
 return 0;
}

void inits()
{
 daw();
 dawMain();
}

int event(int type, int p1, int p2)
{
 if(KY_DOWN==type)
{
 if(w==NULL)
 {w=1;inits();}
 
 else if(w==1)
 switch(p1)
 {
 case _SRIGHT:
 exitapp();
 break;

  case _SLEFT:
  w=2;start();
  break;

  }
 else if(w==2)
 switch(p1)
 {
 case _SRIGHT:
 w=1;stop();dawMain();
 }


 }
 return 0;
}

int pause()
{return 0;}
int resume()
{return 0;}

void exitapp()
{
 free(photo[0]);
 free(photo[1]);
 del();
 dawExit();
 exit();
}

void daw()
{
  int len=SCRW*th*2;
  photo[0]=malloc(len);
  photo[1]=malloc(len);
 drect(0,0,SCRW,th,255,255,255);
 
 text2("10%");
 
 for(int x;x<=th;x++)
 effsetcon(0,x,SCRW,1,x*5,x*10,x*10);
 

 text("报警器",SCRWz(tw*2),0,255,25,25);
 memcpy(photo[0],scr,len);

 text2("30%");

 drect(0,0,SCRW,th,255,255,255); 
 for(x=0;x<=th;x++)
 effsetcon(0,th-x,SCRW,1,x*5,x*10,x*10);
  
 text2("70%");
 text("报警",0,0,255,25,25);
 text("EXIT",SCRWj(2*tw),00,255,25,25);
 memcpy(photo[1],scr,len);
 

 text2("100%");
 sleep(100);
 text2("正在启动");
 sleep(500);
 cls(0,0,0);
}

void text2(char *txt)
{
 drect(0,SCRH2,SCRW,th,0,0,0);
 int x=SCRW2-tw*2;
 text(txt,x,SCRH2,255,25,25);
 ref(0,SCRH2,SCRW,th);

}

void dawMain()
{
 if(w==1)jpg;

 bmp1;bmp2;
 dawTime(SCRWz(tw),SCRHth,255,25,25);
 REF;
}


void start()
{
 timerstart(timer,V,NULL,"Red",1);
}


void stop()
{timerstop(timer);}


void del()
{timerdel(timer);}

void about()
{
 cls(255,255,255);
 drect(1,1,SCRW-2,SCRH-2,0,0,0);
 
char *txt="关于:
报警器
by:每天进步一点
2012.6.18 22点
说明:任何人不得不经同意，随意修本程序！
按任意键进入！
";
rect_text(txt,0,0,SCRW,SCRH,138,200,250);
rect_text(txt,1,1,SCRW,SCRH,255,0,0);

 REF;
}

void Red(int data)
{
 cls(rand()%255,rand()%255,rand()%255); 
 text("call--->110",tw,SCRH2,0,0,0);
 dawMain();
 shake(100); 

}


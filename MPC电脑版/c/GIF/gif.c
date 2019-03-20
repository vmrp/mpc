#include <base.h>
#define FONT 1
#define GifName "c/GIF/1.gif"


/*头文件结构*/
typedef struct
{
 char bySignature[3];
 char byVersion[3];
}GIFHEADER;


/*逻辑屏幕描述块*/
typedef struct
{
 char *wWidth;
 char *wDepth;

 struct globalflag
 {
  char PalBitp[3];
  char SortFlag;
 char GlobalPal;
}GlobalFlag;

char byBackground;
}GIFSCRDESC;


int init()
{
 cls(250,250,250);
 img(GifName,0,0);
 ref(0,0,SCRW,SCRH);

 OpenGifHeader(GifName);
 return 0;

}

int event(int type, int p1, int p2)
{
 if(KY_DOWN==type)
 {
 switch(p1)
  {
  case _SRIGHT:
  exit();break;
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

void OpenGifHeader(char *name)
{
 int f=open(name,1|8);
 int len=getlen(name);
 GIFHEADER gif;

 read(f,&gif,6);
 close(f);

 dawgif(gif.bySignature,SCRW/2,0,3);
 dawgif(gif.byVersion,SCRW/2,1,3);

}

void dawgif(char *txt,int x,int n,int len)
{
 int y,w,h;
 textwh(txt,0,1,&w,&h);
 //x=(SCRW-w)/2;
 y=h*n;
 //int len=strlen(txt); 
 char *s=malloc(len);
 memset(s,NULL,len);
 strncpy(s,txt,len);

 dtext(s,x,y,0,0,0,FALSE,FONT);
 ref(x,y,w,h);
 dtext(s,x,y,100,128,25,FALSE,FONT);
 ref(x,y+h/2,w,h);
 free(s);
}
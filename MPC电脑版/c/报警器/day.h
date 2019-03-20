#ifndef MPC_DAY_H
#define MPC_DAY_H

#define FONT 1
//字体大小

#define REF ref(0,0,SCRW,SCRH)
//刷新宏

#define text(txt,x,y,r,g,b) dtext(txt,x,y,r,g,b,FALSE,FONT)
//绘字宏

int tw,th;
textwh("志",FALSE,FONT,&tw,&th);
//获得当前字体下的文字宽高

int SCRW2=SCRW/2;
int SCRH2=SCRH/2;
int SCRHth=SCRH-th;

#define SCRWj(x) SCRW-x
#define SCRWz(x) (SCRW-x)/2

//矩形框内绘字
void rect_text(char *txt,int x,int y,int w,int h,int r,int g,int b)
{
 rectst rect; 
 colorst color;
 rect.x=x;
 rect.y=y;
 rect.w=w;
 rect.h=h;

 color.r=r;
 color.g=g;
 color.b=b;

dtextex(txt,rect.x,rect.y,&rect,&color,2,FONT);
}


typedef struct
//时间结构
{
uint16 year;
uint8  month;
uint8  day;
uint8  hour;
uint8  minute;
uint8  second;
}datetime;


//获取时间
void dawTime(int x,int y,int r,int g,int b)
{
datetime date;
getdatetime(&date);
char *da=malloc(5);
sprintf(da,"%d:%d",date.hour,date.minute);

text(da,x,y,r,g,b);
free(da);

}


void dawExit()
{
 drect(0,SCRH2,SCRW,th,255,255,250);
 drect(1,SCRH2+1,SCRW-2,th-2,0,0,0);
 text("by:每天进步一点",tw,SCRH2);
 for(int x;x<=SCRW;x+=10)
 ref(0,SCRH2,x,th);

}

#endif
#include <base.h>

#define FONT 2




//外圆角矩形框
void nrect(int x,int y,int w,int h,int r,int g,int b)
{
drect(x+1,y,w-2,2,r,g,b);
drect(x+1,y+h-2,w-2,2,r,g,b);
drect(x,y+1,2,h-2,r,g,b);
drect(x+w-2,y+1,2,h-2,r,g,b);
}




 void drawtxt2(char *str,int x,int y,int w,int h,int r,int g,int b,int font)
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
dtextex(str,x,y,&rect,&color,2,font);
}

int printscr(char *file)
{
remove(file);
int qp=SCRW*SCRH*2;
uint16 *pic=malloc(qp);
memcpy(pic,getscrbuf(),qp);
bmpshowflip(pic,0,0,SCRW,SCRW,SCRH,0x600,0,0,0);
memcpy(pic,getscrbuf(),qp);

int f=open(file,8);
int16 s=19778;
int h[18]={SCRW*SCRH*2+70,0,70,56,SCRW,SCRH,1048577,3,51200,4000,4000,0,0,63488,2016,31,0,0};
write(f,&s,2);
write(f,h,68);
write(f,pic,qp);
close(f);
bmpshowflip(pic,0,0,SCRW,SCRW,SCRH,0x600,0,0,0);
free(pic);
    return 0;
}

void srect(int16 xx,int16 yy,int16 ww,int16 hh,uint8 rr,uint8 gg,uint8 bb,uint8 r1,uint8 g1,uint8 b1,uint8 nn)
{
int p=rr*0x10000+gg*0x100+bb,q=r1*0x10000+g1*0x100+b1;
shaderect(xx,yy,ww,hh,p,q,nn);}






void jz(char *txt,int x,int y,int w,int h)//文字居中显示
{
int fontw,fonth;
textwh(txt,0,1,&fontw,&fonth);
dtext(txt,x+(w-fontw)/2,y+(h-fonth)/2,170,170,170,0,FONT);
}



void wz(char *txt, int x, int y, int h)//纵向居中
{
int fontw,fonth;
textwh(txt,0,1,&fontw,&fonth);
dtext(txt,x,y+(h-fonth)/2,50,150,220,0,FONT);
}



void wzright(char *txt, int x, int y, int h)//右对齐纵向居中
{
int fontw,fonth;
textwh(txt,0,FONT,&fontw,&fonth);
dtext(txt,x-fontw,y+(h-fonth)/2,50,150,220,0,FONT);
}


void toolbar(char *text,char *text2)//工具条
{
int fontw,fonth;
textwh("字",0,FONT,&fontw,&fonth);
wz(text,2,scrh-fonth-4,fonth+4);
wzright(text2,scrw-2,scrh-fonth-4,fonth+4);
}


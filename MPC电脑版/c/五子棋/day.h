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

//退出画面
void dawExit()
{
 drect(0,SCRH/2,SCRW,th,255,255,250);
 drect(1,SCRH/2+1,SCRW-2,th-2,0,0,0);
 text("by:每天进步一点",tw,SCRH/2,255,255,255);
 for(int x;x<=SCRW;x+=2)
 ref(0,SCRH/2,x,th);
 

}

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

#endif

//by:每天进步一点
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


void about()
{
cls(255,255,255);
drect(1,1,SCRW-2,SCRH-2,0,0,0);
char *txt="MPC-扫雷
by:每天进步一点
5、中心键扫雷，1键标记。
0键初始化，支持长按。
说明:任何人不得不经同意，随意修本程序！编写中错误再所难免！请见凉！
按任意键进入！
";
rect_text(txt,0,0,SCRW,SCRH,255,100,255);
rect_text(txt,1,1,SCRW,SCRH,25,255,100);

 for(int h=10;h<=SCRH;h+=5+h/5)
 ref(0,0,SCRW,h);
 REF;
}
#endif

//by:每天进步一点
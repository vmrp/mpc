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


 for(int x=0;x<=SCRW;x+=2)
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
cls(255,250,255);
drect(1,1,SCRW-2,SCRH-2,0,0,0);
char *txt="MPC、猜拳、
石头、剪刀、布、
by:每天进步一点
按键:0键初始化、1键电脑出拳、其它的键不用解释。
说明:任何人不得不经同意，随意修本程序！编写中错误再所难免！请见凉！
按任意键进入！
";
rect_text(txt,0,0,SCRW,SCRH,125,255,255);
rect_text(txt,1,1,SCRW,SCRH,100,50,100);

  
 for(int x=0;x<=SCRW;x+=2)
 {
 ref(x,0,1,SCRH);
  ref(SCRW-x-1,0,1,SCRH);
 }
 REF;
}



/*
x,y为在屏幕输出位置。,r,g,b为文字颜色，下同
返回:
成功 1
示败 0
*/

int printf_d(char *s1,int f,int x,int y,int8 r,int8 g,int8 b)
{

char *s2=malloc(strlen(s1)+10);

sprintf(s2,s1,f);
if(strlen(s2)<=0)
{ free(s2);
 return 0;
}

dtext(s2,x,y,r,g,b,FALSE,1);
 free(s2);
 return 1;
}


#endif

//by:每天进步一点
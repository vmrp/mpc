//龙魂噬心(6369)
//create date 2012_7_18

#include "mrc_base.h"
#include "type.h"
#include "textfun.h"



/*创建文本框*/

PTextfun textFunCreate(int x,int y,int w,int h,int32 bgclr,int32 chclr,char* text)
{
int32 zw,zh;
PTextfun a;

mrc_textWidthHeight("鼎",0,FONT,&zw,&zh);
zh+=2;
a=(PTextfun)mrc_malloc(sizeof(Textfun));
if(a==NULL) return 0;

a->text=(char*)mrc_malloc(mrc_strlen(text)+2);
if(a->text==NULL)
  {mrc_free(a);a=0;return 0;}
mrc_memset(a->text,'\0',sizeof(a->text));
mrc_strcpy(a->text,text);

a->th=zh;
a->pageline=h/zh;
a->scroll=scrollBarCreate(x+w-4,y,4,h,a->pageline,0x000000,0x0055ff);
if(a->scroll==0)
  {mrc_free(a->text);free(a);a=0;return 0;}

a->allline=mrc_unicodeTextRow((uint16*)gb_uni(text,1),FONT,w-4);
setScrollBarAllId(a->scroll,a->allline);//-a->pageline+1);

a->nowline=0;
setScrollBarNowId(a->scroll,a->nowline);

a->x=x;
a->y=y;
a->w=w;
a->h=h;
a->bgclr=bgclr;
a->chclr=chclr;
return a;
}

/*删除文本框*/
void textFunDel(PTextfun a)
{
mrc_free(a->text);
scrollBarDel(a->scroll);
mrc_free(a);
a=0;
}


/*文本框文字翻行*/

void textFunLineNext(PTextfun a,int is_next)
{
if(is_next)
{//a->nowline++;
if(a->nowline</*a->pageline-1*/  a->allline)
a->nowline++;//=a->allline-a->pageline;
}
else
{

a->nowline--;
if(a->nowline<0)
  a->nowline=0;
}
setScrollBarNowId(a->scroll,a->nowline);
drawTextFun(a);

}


/*绘制文本框*/

void drawTextFun(PTextfun a)
{
int r,g,b;
mr_screenRectSt rect;
mr_colourSt clr;
rect.x=a->x;
rect.y=a->y;
rect.w=a->w-4;
rect.h=a->h;
make(a->bgclr,&r,&g,&b);
mrc_drawRect(a->x,a->y,a->w,a->h,r,g,b);
make(a->chclr,&r,&g,&b);
clr.r=r;
clr.g=g;
clr.b=b;
mrc_drawTextEx(a->text,a->x,a->y-(a->nowline*a->th),rect,clr,2,FONT);
drawScrollBar(a->scroll);
}
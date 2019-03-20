#ifndef TEXTFUN_H
#define TEXTFUN_H

//龙魂噬心(6369)
//create date 2012_7_18
#include "scrollbar.h"

typedef struct
{
int x;int y;int w;int h;
int32 bgclr;
int32 chclr;
char* text;
int nowline;//第几行
int allline;//一共几行会
int pageline;//每页几行
int th;//每行高
PScroll scroll;
}Textfun,*PTextfun;


/*创建文本框*/

PTextfun textFunCreate(int x,int y,int w,int h,int32 bgclr,int32 chclr,char* text);



/*删除文本框*/
void textFunDel(PTextfun a);




/*文本框文字翻行*/

void textFunLineNext(PTextfun a,int is_next);


/*绘制文本框*/

void drawTextFun(PTextfun a);


#endif

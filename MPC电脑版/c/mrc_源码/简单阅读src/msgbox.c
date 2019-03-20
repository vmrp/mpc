//龙魂噬心(6369)
//create date 2012-7-13


#include "type.h"
#include "msgbox.h"


/*显示提示框(800)ms后消失

title 显示内容
*/

void showMsgBox(char*title)
{
int32 zw,zh,x,y;
int r,g,b;
make(MSGBOXBK,&r,&g,&b);
mrc_textWidthHeight(title,0,FONT,&zw,&zh);
zw+=10;zh+=5;
x=(SCRW-zw)/2;
y=(SCRH-zh)/2;
mrc_drawLine(x+1,y,x+zw-1,y,r,g,b);
mrc_drawLine(x+1,y+zh,x+zw-1,y+zh,r,g,b);
mrc_drawLine(x,y+1,x,y+zh-1,r,g,b);
mrc_drawLine(x+zw,y+1,x+zw,y+zh-1,r,g,b);
make(MSGBOXBG,&r,&g,&b);
mrc_drawRect(x+1,y+1,zw-1,zh-1,r,g,b);
make(MSGBOXCH,&r,&g,&b);
mrc_drawText(title,x+5,y+3,r,g,b,0,FONT);
mrc_refreshScreen(x,y,zw+1,zh+1);
//mrc_sleep(800);
}
//注：以下的函数为早期简单阅读mpc版使用，为已废弃，请勿使用。请勿删除，否则简单阅读mpc版无法运行

/*显示标题栏
x y w h 标题栏的位置区域
uclr，dclr，背景上部颜色，下部颜色 888格式(0x00005500)(绿色)
tclr 文字颜色 同上
title 标题
*/


void showTitleBar(int x,int y,int w,int h,int32 uclr,int32 dclr,int32 tclr,char* title)
{
int32 zw,zh;
mrc_textWidthHeight(title,0,FONT,&zw,&zh);
shaderect(x,y,w,h,uclr,dclr,0);
mrc_drawText(title,x+2,y+(h-zh)/2,MAKER(tclr),MAKEG(tclr),MAKEB(tclr),0,FONT);

}

/*显示工具栏
x y w h 位置信息
uclr dclr tclr 背景上、下颜色，文字颜色
left 左边文字
zhon 中间
right 右边
*/
void showToolBar(int x,int y,int w,int h,int32 uclr,int32 dclr,int32 tclr,char* left,char* zhon,char* right)
{
int32 zw,zh;
int r,g,b;
mrc_textWidthHeight(left,0,FONT,&zw,&zh);
shaderect(x,y,w,h,uclr,dclr,0);
make(tclr,&r,&g,&b);
mrc_drawText(left,x+2,y+(h-zh)/2,r,g,b,0,FONT);
mrc_textWidthHeight(zhon,0,FONT,&zw,&zh);
mrc_drawText(zhon,x+(w-zw)/2,y+(h-zh)/2,r,g,b,0,FONT);
mrc_textWidthHeight(right,0,FONT,&zw,&zh);
mrc_drawText(right,x+w-zw-2,y+(h-zh)/2,r,g,b,0,FONT);
}
#ifndef _MSGBOX_H
#define _MSGBOX_H


//龙魂噬心(6369)
//create date 2012-7-13
#include "mrc_base.h"

#define MSGBOXBK 0x00ff0000
#define MSGBOXBG 0x000011ff
#define MSGBOXCH 0x00ffffff


/*显示提示框(800)ms后消失

title 显示内容
*/

void showMsgBox(char*title);

//注：以下的函数为早期简单阅读mpc版使用，为已废弃，请勿使用。请勿删除，否则简单阅读mpc版无法运行

/*显示标题栏
x y w h 标题栏的位置区域
uclr，dclr，背景上部颜色，下部颜色 888格式(0x00005500)(绿色)
tclr 文字颜色 同上
title 标题
*/
void showTitleBar (int x,int y,int w,int h,int32 uclr,int32 dclr,int32 tclr,char* title);


/*显示工具栏
x y w h 位置信息
uclr dclr tclr 背景上、下颜色，文字颜色
left 左边文字
zhon 中间
right 右边
*/
void showToolBar(int x,int y,int w,int h,int32 uclr,int32 dclr,int32 tclr,char* left,char* zhon,char* right);

#endif
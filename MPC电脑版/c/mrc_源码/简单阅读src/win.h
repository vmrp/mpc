//龙魂噬心(6369)
#ifndef _WIN_H
#define _WIN_H

typedef enum
{
WIN_FILEFIND,//文件寻找
WIN_READ,//阅读界面
WIN_MENU,//菜单
WIN_INJUMP,//跳转输入
WIN_INBK,//边框输入
WIN_INLINESIZE//行距输入
}WIN;

extern WIN win;

#endif
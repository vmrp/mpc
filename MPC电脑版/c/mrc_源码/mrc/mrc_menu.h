#ifndef __MRC_MENU_H__
#define __MRC_MENU_H__

/*
本文件以及对应的C代码文件，对mythroad提供的基本函数
进行了封装，使用mrc_win提供的基本窗体架构，提供对
菜单窗体的封装。
*/


/*
菜单项被选择时的回调函数
输入:
data        菜单项数据
*/
typedef void (*PtrHilight)(int32 data);

typedef struct
{
	PtrHilight hCb;
	int32 data;              
}T_MENU_ITEM;

typedef struct
{
	int32 handle;  
	int32 itemNum;  
	T_MENU_ITEM EscMenu[1];//用户不作任何选择，直接选择“返回”键时的回调函数的函数指针和参数指针。	
	T_MENU_ITEM subMenu[1];//其他菜单项的回调函数指针和参数指针
}T_MENU;

/*
新建一个菜单

输入:
title:       菜单标题
num:         菜单项的数目

返回:
      其他              窗体句柄
      MR_FAILED         失败
*/
int32 mrc_platMenuNew(char* title, int32 num);

/*
	在菜单里设置一个菜单项，参数index是该菜单项的
	序号。

输入:
menu   菜单的句柄
text      菜单项的名字，使用unicode编码，网络字节序。当等于num时，此项无意义。
index    菜单项的index号。index取值范围为[0，num](num是用mrc_platMenuNew创建
菜单时指定的菜单条数目)。当等于num时，是按“返回”键的回调函数。
SDK在处理用户点击"返回"键时，默认不作任何操作，直接关闭窗口；
当用户自定义了回调函数时，需要用户在回调函数中负责关闭窗口。


cb         当用户选择该菜单项时的回调
data      菜单项数据

返回:
      MR_SUCCESS  成功
      MR_FAILED   失败
*/
int32 mrc_platMenuSetItem(int32 menu, const char *text, int32 index, PtrHilight cb, int32 data);

/*
显示菜单。

输入:
menu 菜单的句柄

返回:
      MR_SUCCESS  成功
      MR_FAILED   失败
*/
int32 mrc_platMenuShow(int32 menu);

/*
刷新菜单显示。

输入:
menu 菜单的句柄

返回:
      MR_SUCCESS  成功
      MR_FAILED   失败
*/
int32 mrc_platMenuRefresh(int32 menu);

#endif


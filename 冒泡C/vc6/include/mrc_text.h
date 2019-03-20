#ifndef __MRC_TEXT_H__
#define __MRC_TEXT_H__

/*
本文件以及对应的C代码文件，对mythroad提供的基本函数
进行了封装，使用mrc_win提供的基本窗体架构，提供对
文本框、对话框以及可扩展窗体的封装。
*/

/*
文本框、对话框窗体的按键回调函数
输入:
data        窗体数据
*/
typedef void (*PtrHilight)(int32 data);

typedef struct
{
	int32 handle;
	int32 data;              
	PtrHilight hOk;
	PtrHilight hCancel;
}T_PLAT_TEXT;

typedef struct
{
	int32 handle; 
	int32 data;              
	int32 type;
	PtrWinEvent winEvent;
     PtrKeyEvent keyEvent;
}T_PLAT_WIN;



/*
新建一个文本框

输入:
title:          文本框标题
texttext:    文本框内容
type:         文本框类型
      MR_DIALOG_OK：文本框有"确定"键。
      MR_DIALOG_OK_CANCEL：文本框有"确定"和"取消"键。
      MR_DIALOG_CANCEL：文本框有 "取消/返回"键。
textdata    窗体数据
hOk          用户选择"确定"时的回调
hCancel    用户选择"取消/返回"时的回调

返回:
      其他              窗体句柄
      MR_FAILED         失败
*/
int32 mrc_platTextNew(char* title, char* texttext, int32 type, int32 textdata, PtrHilight hOk, PtrHilight hCancel);

/*
刷新文本框

输入:
title:          文本框标题
texttext:    文本框内容
text:         文本框句柄

返回:
      MR_SUCCESS  成功
      MR_FAILED   失败
*/
int32 mrc_platTextRefresh(int32 text, char * title, char * texttext);

/*
新建一个可扩展窗体

输入:
data            窗体数据
winEvent      窗体事件处理函数
keyEvent     按键事件处理函数

返回:
      其他              窗体句柄
      MR_FAILED         失败
*/
int32 mrc_platWinNew(int32 data, PtrWinEvent winEvent, PtrKeyEvent keyEvent);

/*
虚拟新建一个基本窗体

输入:
data            窗体数据
winEvent      窗体事件处理函数
keyEvent     按键事件处理函数

返回:
      其他              窗体句柄
      MR_FAILED         失败
*/
int32 mrc_platBaseWinNew(int32 data, PtrWinEvent winEvent, PtrKeyEvent keyEvent);

/*
新建一个对话框

输入:
title:          对话框标题
texttext:    对话框内容
type:         对话框类型
      MR_DIALOG_OK：对话框有"确定"键。
      MR_DIALOG_OK_CANCEL：对话框有"确定"和"取消"键。
      MR_DIALOG_CANCEL：对话框有 "取消/返回"键。
textdata    窗体数据
hOk          用户选择"确定"时的回调
hCancel    用户选择"取消/返回"时的回调

返回:
      其他              窗体句柄
      MR_FAILED         失败
*/
int32 mrc_platDialogNew(char* title, char* texttext, int32 type, int32 textdata, PtrHilight hOk, PtrHilight hCancel);

/*
刷新对话框

输入:
title:          对话框标题
texttext:    对话框内容
text:         对话框句柄
type:         对话框类型
      MR_DIALOG_OK：对话框有"确定"键。
      MR_DIALOG_OK_CANCEL：对话框有"确定"和"取消"键。
      MR_DIALOG_CANCEL：对话框有 "取消/返回"键。

返回:
      MR_SUCCESS  成功
      MR_FAILED   失败
*/
int32 mrc_platDialogRefresh(int32 text, char * title, char * texttext, int32 type);

#endif


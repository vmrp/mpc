#ifndef _OPENDLG_H
#define _OPENDLG_H

#include "window.h"

typedef enum {
	DIALOG_SELECTFILE, //选择文件
	DIALOG_SELECTDIR   //选择文件夹
}SMP_DIALOG_TYPE;

//以下供外部使用,但调用对话框后内容会被改变
extern char SMP_DIALOG_CUR_PATH[128];  //当前搜索路径(如果要设置初始目录,则目录后应该有"/"例如"moc/"
extern char SMP_DIALOG_PATH[256];      //对话框消毁后返回路径(文件或文件夹)
extern char SMP_DIALOG_TEMP_PATH[256]; //临时使用
extern char SMP_DIALOG_TEMP[128];      //路径返回时使用
extern uint8 SMP_DIALOG_ID;

#define DIALOG_GET   WM_USER+1


VOID ShowOpenDlg(uint8 Id,WID wndid, SMP_DIALOG_TYPE type);

//注册窗口函数
LRESULT OpenDlg_WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

#endif

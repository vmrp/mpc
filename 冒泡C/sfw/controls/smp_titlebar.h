#ifndef _SMP_TITLEBAR_H
#define _SMP_TITLEBAR_H

#include "window.h"

//标题栏唯一样式
#define SMP_TITLEBAR_STATIC		0x0002L

//显示本地时间
VOID ShowLocalTime(uint32 color);

//设置标题栏右边的字符串(时间)
VOID SMP_Titlebar_SetContentR(HWND hWnd, PCWSTR content, BOOL redraw);

//设置标题栏内容
VOID SMP_Titlebar_SetContent(HWND hWnd, DWORD bmpID, PCWSTR content, BOOL redraw);

//标题栏窗口事件
LRESULT SMP_Titlebar_WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
	
#endif

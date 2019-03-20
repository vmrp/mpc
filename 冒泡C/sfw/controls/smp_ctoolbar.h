#ifndef _SMP_TOOLBAR_H
#define _SMP_TOOLBAR_H

#include "window.h"

/**
  \工具栏

  工具栏的通知消息将转移到主要事件：
   - 左 - 左软键注册
   - 中 - 中软键(选择)
   - 右 - 右软键注册

  若要使用工具栏：
   - 创建工具栏
   - 设置工具栏的字符串
   - 对以左软键/选择/右

  \code
  	//创建工具栏

  	hToolbar = SGL_CreateWindow(SMP_Toolbar_WndProc, ...);
  	SMP_Toolbar_SetStrings(hToolbar, ...);
  	SGL_AddChildWindow(hWnd, hToolbar);

  	//响应
  	case WM_KEYUP:
  		if(MR_KEY_SOFTLEFT == wParam)
  		{
  			//左
  		}else if(MR_KEY_SELECT == wParam){
  			//中
  		}else if(MR_KEY_SOFTRIGHT == wParam){
  			//右
  		}
  \endcode

 * \最大支持工具栏项
 */
#define SMP_TOOLBAR_ITEMCOUNT		3

/**
 * \工具栏左边的位置
 */
#define SMP_TOOLBARLOC_LEFT			0

/**
 * \工具栏中间的位置
 */
#define SMP_TOOLBARLOC_MID			1

/**
 * \工具栏右边的位置
 */
#define SMP_TOOLBARLOC_RIGHT		2

/**
窗口成员函数

 * \设置工具栏的字符串
 *
 * \参数hWnd		工具栏句柄
 * \参数left		左边的字符串ID
 * \参数mid			中间的字符串ID
 * \参数right		右边的字符串ID
 * \参数redraw		重绘工具栏
 */
VOID SMP_Toolbar_SetStrings(HWND hWnd, DWORD left, DWORD mid, DWORD right, BOOL redraw);

/**
窗口成员函数

 * \设置工具栏的字符串
 *
 * \参数hWnd		工具栏句柄
 * \参数left		左边的字符串
 * \参数mid			中间的字符串
 * \参数right		右边的字符串
 * \参数redraw		重绘工具栏
 */
VOID SMP_Toolbar_SetStrings2(HWND hWnd, PCWSTR left, PCWSTR mid, PCWSTR right, BOOL redraw);

/**
 * \获取工具栏字符串的类型
 * 
 * \参数hWnd			工具栏的窗口句柄
 * \字符串ID形式返回TRUE，字符串指针形式返回FALSE
 */
BOOL SMP_Toolbar_StrIsID(HWND hWnd);

/**
 * \获取工具栏项目的字符串ID
 * 
 * \参数hWnd			工具栏的窗口句柄
 * \参数location		项目位置
 * \返回项目ID
 */
DWORD SMP_Toolbar_GetString(HWND hWnd, int location);

/**
 * \获取工具栏项目的字符串
 * 
 * \参数hWnd			工具栏的窗口句柄
 * \参数location		项目位置
 * \返回项目字符串
 */
PCWSTR SMP_Toolbar_GetString2(HWND hWnd, int location);

/** 
窗口程序
 * \工具栏的窗口过程
 *
 * \参数hWnd		窗口句柄
 * \参数Msg			窗口消息
 * \参数wParam		第一个参数
 * \参数lParam		第二个参数
 * \返回消息的处理结果
 */
LRESULT SMP_Toolbar_WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

#endif /* _SMP_TOOLBAR_H */




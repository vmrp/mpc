#ifndef _SMP_SCROLLBAR_H
#define _SMP_SCROLLBAR_H

#include "window.h"

/*滚动条

若要用简单的滚动条：
   - 创建滚动窗口
   - 设置滚动窗口的信息
   - 添加到父窗口
   - 响应消息的通知

  \code
  	//创建滚动条
  	hScrbar = SGL_CreateWindow(SMP_Scrollbar_WndProc, ...);
  	SMP_Scrollbar_SetInfo(hScrbar);
  	SGL_AddChildWindow(hWnd, hScrbar);

  	//响应消息的通知
	case WM_COMMAND:
		WID id = LOWORD(wParam);
		WORD code = HIWORD(wParam);
		if(id == "the scrollbar id" && code == SMP_SCRBARN_VALUECHANGED)
		{
			int index = (int)lParam; //当前滚动条值
		}	
  \endcode


窗口消息
	
 * \滚动窗口值更改通知消息
 *
 * \code
 *	case WM_COMMAND:
 *		WID id = LOWORD(wParam);
 *		WORD code = HIWORD(wParam);
 *
		if(id == "the scrollbar id" && code == SMP_SCRBARN_VALUECHANGED)
		{
			int index = (int)lParam; //当前滚动条值
		}	
 * \endcode
 *
 * \参数index   滚动条的当前值
 */
#define SMP_SCRBARN_VALUECHANGED		0x0001


//滚动条的当前值设置到了最后
#define SMP_SCRBARN_LAST  2
/** 
窗口的成员函数

 * \设置滚动条的信息
 *
 * \参数hWnd        窗口句柄
 * \参数min         最低值
 * \参数max         最大值
 * \参数pagesize    响应鼠标事件时每页大小
 * \成功返回TRUE，否则返回FALSE
 */
BOOL SMP_Scrollbar_SetInfo(HWND hWnd, int min, int max, int pagesize);

/**
 * \获得滚动条信息
 *
 * \参数hWnd       窗口句柄
 * \参数min        最低值
 * \参数max        最大值
 * \参数pagesize   每页大小
 * \成功返回TRUE，否则返回FALSE
 */
BOOL SMP_Scrollbar_GetInfo(HWND hWnd, int* min, int* max, int* pagesize);

/**
 * \设置滚动条的步长
 *
 * \参数hWnd    窗口句柄
 * \参数step    单步值
 */
VOID SMP_Scrollbar_SetStep(HWND hWnd, int step);

/**
 * \设置滚动条的新位置
 *
 * \参数hWnd        窗口句柄
 * \参数value       新值
 * \参数redraw      是否重绘窗口
 * \成功返回TRUE，否则返回FALSE
 */
BOOL SMP_Scrollbar_SetValue(HWND hWnd, int value, BOOL redraw);

/**
 * \得到滚动条的当前位置
 *
 * \参数hWnd        窗口句柄
 * \返回当前位置值
 */
int SMP_Scrollbar_GetValue(HWND hWnd);

/**
 * \向上滚动
 *
 * \参数hWnd        窗口句柄
 * \成功返回TRUE，否则返回FALSE
 */
BOOL SMP_Scrollbar_PageUp(HWND hWnd);

/**
 * \向下滚动
 *
 * \参数hWnd        窗口句柄
 * \成功返回TRUE，否则返回FALSE
 */
BOOL SMP_Scrollbar_PageDown(HWND hWnd);

/**
 * \按步长向上滚动
 *
 * \参数hWnd     窗口句柄
 * \成功返回TRUE，否则返回FALSE
 */
BOOL SMP_Scrollbar_LineUp(HWND hWnd);

/**
 * \按步长向下滚动
 *
 * \参数hWnd     窗口句柄
 * \成功返回TRUE，否则返回FALSE
 */
BOOL SMP_Scrollbar_LineDown(HWND hWnd);

/** 
窗口程序

 * \滚动条的窗口过程
 *
 * \参数hWnd        窗口句柄
 * \参数Msg         窗口消息
 * \参数wParam      第一个参数
 * \参数lParam      第二个参数
 * \返回消息的处理结果
 */
LRESULT SMP_Scrollbar_WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);


#endif /* _SMP_SCROLLBAR_H */


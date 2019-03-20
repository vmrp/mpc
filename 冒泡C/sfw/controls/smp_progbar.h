#ifndef _SMP_PROGBAR_H
#define _SMP_PROGBAR_H

#include "window.h"


/**
  \进度条

  若要使用进度条：

  - 创建进度条
  - 设置进度条范围
  - 添加到父窗口
  - 响应消息的通知

  \code
  	//创建滚动条
  	hPorgBar = SGL_CreateWindow(SMP_ProgBar_WndProc, ...);
  	SMP_ProgBar_SetRange(hPorgBar, 1, 30);  //默认范围是[0 - 99]
  	SGL_AddChildWindow(hWnd, hPorgBar);

  	//响应消息的通知
	case WM_COMMAND:
		WID id = LOWORD(wParam);
		WORD code = HIWORD(wParam);

		if(id == "the progbar id" && code == SMP_PROGBARN_VALUECHANGED)
		{
			int value = (int)lParam; //当前进度条的值
		}	
  \endcode
  


窗口通知消息

 * \进度条的窗口值更改消息
 *
 * \code
 *	case WM_COMMAND:
 *		WID id = LOWORD(wParam);
 *		WORD code = HIWORD(wParam);
 *
		if(id == "the progbar id" && code == SMP_PROGBARN_VALUECHANGED)
		{
			int value = (int)lParam; //当前progbar价值
		}	
 * \endcode

 * \通用值
 */
#define SMP_PROGBARN_VALUECHANGED		0x0001

/** 
窗口的成员函数

 * \设置进度条范围。该窗口的默认范围是[0-99]
 *
 * \参数的hWnd		进度条的窗口句柄
 * \参数min			进度条的最小值
 * \参数max			进度条的最大值
 */
VOID SMP_ProgBar_SetRange(HWND hWnd, Sint32 min, Sint32 max);

/**
 * \设置进度条的值
 *
 * \参数hWnd	处理的进度条
 * \参数value	新值
 * \参数redraw	是否重绘进度
 * \参数notify	如果值变化发送通知消息
 */
VOID SMP_ProgBar_SetValue(HWND hWnd, Sint32 value, BOOL redraw, BOOL notify);

/** 
窗口程序

 * \进度条程序
 *
 * \参数hWnd		窗口句柄
 * \参数Msg			窗口消息
 * \参数wParam		第一个参数
 * \参数lParam		第二个参数
 * \返回消息的处理结果
 */
LRESULT SMP_ProgBar_WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);



#endif

#ifndef _SMP_SPIN_H
#define _SMP_SPIN_H

#include "window.h"

/**
  \旋盒

  若要使用简单的数字显示框：
 - 创建自旋窗口
 - 设置旋转范围
 - 添加到父窗口
 - 响应消息的自旋通知

  
  \code
	//创建一个旋转框
	hControl = SGL_CreateWindow(SMP_Spin_WndProc, ...);
	SMP_Spin_SetRange(hControl,  ...);
	SGL_AddChildWindow(hWnd, hControl);

	//响应消息通知旋转
	case WM_COMMAND:
		WID id = LOWORD(wParam);
		WORD code = HIWORD(wParam);

		if(id == "the spin id")
		{
			switch(code)
			{
			case SMP_SPINN_VALUECHANGED:
				Sint32 value = (Sint32)lParam; //自旋值

				//处理通知事件
				break;
			}
		}	
  \endcode
  



窗口样式


 * \表示按下左键
 */
#define SMP_SPINS_LEFTARROW			0x0001L

/**
 * \表示按下右键
 */
#define SMP_SPINS_RIGHTARROW		0x0002L

/**  
窗口通知消息

 * \旋转时发送窗口值改变消息
 *
 * \code
 *	case WM_COMMAND:
 *		WID id = LOWORD(wParam);
 *		WORD code = HIWORD(wParam);
 *
 *		if(id == "the spin id" && code == SMP_SPINN_VALUECHANGED)
 *		{
			Sint32 value = (Sint32)lParam; //自旋值

 * 			//处理自旋的通知消息
 *		}
 * \endcode
 *
 * \参数hControl	旋转框处理发送此消息
 */
#define SMP_SPINN_VALUECHANGED		0x0001

/** 
窗口成员函数

 * 设置窗口范围内的自旋
 *
 * \参数hWnd	自旋窗口句柄
 * \参数min		最小值
 * \参数max		最大值
 * \参数step	单步
 */
VOID SMP_Spin_SetRange(HWND hWnd, Sint32 min, Sint32 max, Sint32 step);

/**
 * \设置当前值
 *
 * \参数hWnd		自旋窗口句柄
 * \参数param		当前值
 * \参数notify		发送通知
 * \参数update		更新窗口
 */
VOID SMP_Spin_SetValue(HWND hWnd, Sint32 value, BOOL notify, BOOL update);

/**
 * \获取自旋窗口的当前值
 *
 * 参数hWnd	自旋窗口句柄
 * \返回当前值
 */
Sint32 SMP_Spin_GetValue(HWND hWnd);

/** 
 * \自旋窗口程序
 *
 * \参数hWnd		窗口句柄
 * \参数Msg			窗口消息
 * \参数wParam		第一个参数
 * \参数wParam		第一个参数
 * \返回消息的处理结果
 */
LRESULT SMP_Spin_WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
	

#endif

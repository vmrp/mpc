#ifndef _SMP_BUTTON_H
#define _SMP_BUTTON_H

#include "window.h"

/**
	按钮

  若要使用按钮：
 - 创建按钮的窗口
 - 设置按钮的标题或其他必要的信息
 - 添加到父窗口
 - 响应按钮的消息通知
  \code
	//创建一个按钮
	hBtn = SGL_CreateWindow(SMP_Button_WndProc, ...);
	SMP_Button_SetTitle(hBtn, SGL_LoadString(STR_OK));
	SGL_AddChildWindow(hWnd, hBtn);

	//响应按钮的消息通知
	case WM_COMMAND:
		WID id = LOWORD(wParam);
		WORD code = HIWORD(wParam);

		if(id == "the button id")
		{
			HWND hBtn = (HWND)lParam;
			switch(code)
			{
			case SMP_BUTTONN_CLICKED:
				//处理通知事件。
				break;
			}
		}	
  \endcode


窗口样式
简要说明按钮是一个复选框按钮。
 */
#define SMP_BUTTONS_CHECKBOX		0x0001L

/**
 * 简要说明的按钮是一个radiobox按钮。
 */
#define SMP_BUTTONS_RADIOBOX		0x0002L

/**
 * 的标题是右对齐的样式。
 */
#define SMP_BUTTONS_HRIGHT			0x0004L

/**
 *的标题是中心对齐的样式。
 */
#define SMP_BUTTONS_HCENTER			0x0008L

/**
 * 的标题是在垂直中心对齐。
 */
#define SMP_BUTTONS_VCENTER			0x0010L

/**
 * 的标题是在垂直对齐底部。
 */
#define SMP_BUTTONS_VBOTTOM			0x0020L

/**
 * 指示按钮被选中。
 */
#define SMP_BUTTONS_CHECKED			0x0040L	

/*
窗口NOTIFY消息
点击发送按钮时
 * \code
 *	case WM_COMMAND:
 *		WID id = LOWORD(wParam);
 *		WORD code = HIWORD(wParam);
 *
 *		if(id == "the button id" && code == SMP_BUTTONN_CLICKED)
 *		{
 *			HWND hBtn = (HWND)lParam;
 * 			//处理按钮点击通知消息
 *		}
 * \endcode
 *
 * 参数hBtn按钮发送通知消息
*/
#define SMP_BUTTONN_CLICKED			0x0001

/**
窗口成员函数
 * \设置按钮窗口的标题。
 * \参数hBtn	按钮窗口句柄
 * \参数title	新标题
 */
VOID SMP_Button_SetTitle(HWND hBtn, PCWSTR title);

/**
 * \获取按钮的标题。
 * \参数hBtn	按钮窗口句柄
 * \返回按钮窗口的标题
 */
PCWSTR SMP_Button_GetTitle(HWND hBtn);

/**
 * \设置按钮的背景位图。
 * \参数hBtn	按钮窗口句柄
 * \参数bmp		正常状态的背景位图的ID
 * \参数bmp_p	按下状态的背景位图的ID
 */
VOID SMP_Button_SetBitmapGroup(HWND hBtn, DWORD bmp, DWORD bmp_p);

/**
 * \选中按钮。
 * \参数hBtn	按钮窗口句柄
 * \参数checked
 *	- TRUE		选中按钮
 *	- FALSE		取消选中
 */
VOID SMP_Button_SetChecked(HWND hBtn, BOOL checked);

/**
窗口程序
 * \按钮的窗口过程。
 * \参数hBtn		按钮窗口句柄
 * \参数Msg			窗口消息
 * \参数wParam		第一个参数
 * \参数lParam		第二个参数
 * \返回消息的处理结果
 */
LRESULT SMP_Button_WndProc(HWND hBtn, UINT Msg, WPARAM wParam, LPARAM lParam);

/*结束smp_button*/
#endif /* _SMP_BUTTON_H */


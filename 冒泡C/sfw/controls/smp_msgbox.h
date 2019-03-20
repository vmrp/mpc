#ifndef _SMP_MSGBOX_H
#define _SMP_MSGBOX_H

#include "window.h"

/**
  简单MsgBox

  若要使用简单msgbox
  - 显示MsgBox
  - 响应消息

  \code
  	//只需要调用这个函数来显示在MsgBox
  	SMP_MsgBox(...);

  	//响应消息的通知
	case WM_COMMAND:
		WID id = LOWORD(wParam);
		WORD code = HIWORD(wParam);

		if(id == "the msgbox id")
		{
			HWND hMsgbox = (HWND)lParam;
			if(code == ID_OK)
			{
				//按下确定

			}else{
				//按取消
			}
		}	
  \endcode


窗口样式

 * \MsgBox没有按钮
 */
#define ID_NON		0 //由窗口manangement系统模式窗口关闭


/**
 * \MsgBox有确定按钮
 */
#define ID_OK		0x0001L

/**
 * \MsgBox有取消按钮
 */
#define ID_CANCEL	0x0002L

/**
 * \MsgBox按钮有“是/否”按钮
 */
#define ID_YESNO	0x0004L

/**
 * \MsgBox可以在一定时间后关闭
 * 此样式只在使用了不到一个按钮时有效。
 */
#define SMP_MSGBOXS_AUTOCLOSE		0x0008L

/**
 * \MsgBox自动关闭时长
 *
 * 当在MsgBox只有一个按钮时
 * MsgBox将在SMP_MSGBOX_AUTOCLOSE_INTERVAL毫秒后自动关闭
 */
#define SMP_MSGBOX_AUTOCLOSE_INTERVAL	3000 //ms

/**
窗口成员函数

 * \显示一个消息框
 *
 * \参数id			信息框的编号
 * \参数hParent		顶层窗口句柄
 * \参数hBmp		图标
 * \参数title		标题字符串
 * \参数content		内容字符串
 * \参数style		窗口的按钮样式可能是ID_OK，ID_CANCEL，ID_YESNO
 * \参数listener	监听接收通知消息，当它为NULL时父窗口将得到消息
 */
HWND SMP_MsgBox(WID id, HWND hParent, HBITMAP hBmp, PCWSTR title, PCWSTR content, DWORD style, HWND listener);

VOID SMP_Close_MsgBox(HWND hMsgBox);

/**
窗口程序

 * \MsgBox窗口过程
 *
 * \参数hDlg	窗口句柄
 * \参数Msg		窗口消息
 * \参数wParam	第一个参数
 * \参数lParam	第二个参数
 * \返回消息的处理结果
 */
LRESULT SMP_MsgBox_WndProc(HWND hDlg, UINT Msg, WPARAM wParam, LPARAM lParam);
	

#endif /* _SMP_MSGBOX_H */


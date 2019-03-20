#ifndef _PLT_MSGBOX_H
#define _PLT_MSGBOX_H

#include "window.h"

/*
  平台MsgBox窗口
  若要使用平台的信息框：
   - 调用PLT_MsgBox来显示信息框
   - 响应WM_COMMAND消息

   \code
   //显示一个平台信息框
  PLT_MsgBox(0, hParent, SGL_LoadString(STR_MESSENGER), SGL_LoadString(STR_EXITCONFIRM), MR_DIALOG_OK_CANCEL, NULL);
   //响应WM_COMMAND
  case WM_COMMAND:
  	WID id = LOWORD(wParam);
  	WORD code = HIWORD(wParam);
  	if(id == "平台信息框的ID")
  	{
  		if(code == MR_DIALOG_KEY_OK)
  		{
  			//你的代码
  			......
  		}else if(code == MR_DIALOG_KEY_CANCEL){
			//你的代码
			......
  		}
  	}
	\endcode

 * \显示一个信息框。
 * \参数id					窗口ID
 * \参数hParent				顶层窗口句柄
 * \参数title				标题字符串
 * \参数content				内容字符串
 * \参数style
 * - MR_DIALOG_OK			//只有一个确定按钮
 * - MR_DIALOG_OK_CANCEL 	//有确定和取消按钮
 * - MR_DIALOG_CANCEL		//只有一个取消按钮
 * \参数listener			监听通知消息，当它为空时父窗口将得到消息
 * \返回MsgBox的句柄
 **/
HWND PLT_MsgBox(WID id, HWND hParent, PCWSTR title, PCWSTR content, DWORD style, HWND listener);

/*
窗口程序
 * \信息框窗口，该平台的程序。
 * \参数hDlg		窗口句柄
 * \参数Msg			窗口消息
 * \参数wParam		第一个参数
 * \参数lParam		第二个参数
 * \返回消息的处理结果
 */

LRESULT PLT_MsgBox_WndProc(HWND hDlg, UINT Msg, WPARAM wParam, LPARAM lParam);








/** 结束pltmsgbox */
#endif


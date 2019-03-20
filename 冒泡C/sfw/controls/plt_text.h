#ifndef _PLT_TEXT_H
#define _PLT_TEXT_H

#include "window.h"

/*
  文本窗口

  若要使用文字视窗平台：
   - 创建文本窗口
   - 设置文字信息
   - 添加到桌面

  \code
  //创建窗口
  hWnd = SGL_CreateWindow(PLT_Text_WndProc, ..., MR_DIALOG_OK_CANCEL, ...);
  PLT_Text_SetInfo(hWnd, SGL_LoadString(STR_TITLE), SGL_LoadString(STR_CONTENT));

  _LISTENER(hWnd) = hYouWnd; //设置窗口，得到消息的命令
  SGL_AddChildWindow(HWND_DESKTOP, hWnd);

  //响应了WM_COMMAND
  case WM_COMMAND:
  	WID id = LOWORD(wParam);
  	WORD code = HIWORD(wParam);
  	if(id == "the platform text id")
  	{
  		if(code == MR_DIALOG_KEY_OK)
  		{
  			//your code
  			......
  		}else if(code == MR_DIALOG_KEY_CANCEL){
			//your code
			......
  		}
  	}
  \endcode
 窗口成员函数
 * \设置文本窗口的信息
 *
 * \参数hWnd	窗口句柄
 * \参数title	标题字符串
 * \参数text	内容
 */
VOID PLT_Text_SetInfo(HWND hWnd, PCWSTR title, PCWSTR text);
/*
 * \文字框，该平台的窗口过程。
 *
 * 该平台的文字可以是这些窗口样式之一：
 * - MR_DIALOG_OK
 * - MR_DIALOG_OK_CANCEL
 * - MR_DIALOG_CANCEL
 *
 * \参数hWnd		窗口句柄
 * \参数Msg			窗口消息
 * \参数wParam		第一个参数
 * \参数lParam		第二个参数
 * \返回消息的处理结果
 */
LRESULT PLT_Text_WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

#endif

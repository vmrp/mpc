#ifndef _SMP_MENUWND_H
#define _SMP_MENUWND_H

#include "window.h"

/**
  菜单为基础的视窗

  这不是一个窗口或控件在父窗口中。
  这仅仅是一个窗口过程中添加功能，在这种情况下，父窗口：
  当一个顶层窗口菜单上的显示，工具栏上应改变
  它的左/右软键“确定/取消”。只需调用默认的窗口过程作为这个功能
  您的顶层窗口，有弹出菜单。
  
  \code
	LRESULT  YOUR_WINDOW(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
	{
		switch(Msg)
		{
			//处理你的信息
		}
		//调用这个函数作为默认
		return SMP_MenuWnd_WndProc(hWnd, Msg, wParam, lParam);
	}  
  \endcode

窗口程序

 * \菜单的窗口过程。
 * \参数hToolbar 工具条窗口句柄
 * \参数hWnd	窗口句柄
 * \参数Msg		窗口消息
 * \参数wParam	第一个参数
 * \参数lParam	第二个参数
 * \返回该进程的结果
 */
LRESULT SMP_MenuWnd_WndProc(HWND hToolbar, HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);



#endif


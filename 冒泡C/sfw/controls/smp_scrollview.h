#ifndef _SMP_SCROLLVIEW_H
#define _SMP_SCROLLVIEW_H

#include "window.h"

/**
  \滚动

  若要用简单的滚动：
 - 创建滚动
 - 获取的滚动内容视图
 - 加入你的窗口的内容查看
 - 设置您的窗口，您的顶级C＃高精度秒表的聆听
  

  \code
	//创建一个滚动

	hScrollView = SGL_CreateWindow(SMP_ScrollView_WndProc, ...);
	hContent = SMP_ScrollView_GetContentView(hScrollView);

	hControl = SGL_CreateWindow("your window", ...);
	_LISTENER(hControl) = hWnd; //应该设置你的监听器和通知消息将发送到的hWnd

	SGL_AddChildWindow(hContent, hControl);

	......
	SGL_AddChildWindow(hWnd, hScrollView);
  \endcode
 

窗口成员函数

 * \brief Get the scrollview content view获取滚动内容的看法
 *
 * For we must add your window to a scrollview's content view not the scrolview itself.
 因为我们必须添加一个滚动窗口的内容的看法不是scrolview本身
 *
 * \param hWnd the scrollview window handle \参数hWnd   窗口句柄
 * \return the content view handle\返回的内容视图处理
 */
HWND SMP_ScrollView_GetContentView(HWND hWnd);

/**
窗口程序

 * \滚动窗口过程
 *
 * \参数hWnd		窗口句柄
 * \参数			窗口消息
 * \参数wParam		第一个参数
 * \参数lParam		第二个参数
 * \返回消息的处理结果
 */
LRESULT SMP_ScrollView_WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);


#endif

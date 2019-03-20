#ifndef _SMP_TABWINDOW_H
#define _SMP_TABWINDOW_H

#include "window.h"
#include "smp.h"


/**
  \defgroup smp_tabwindow Simple tab window

  若要使用简单的tab窗口：
 - 创建选项卡窗口
 - 创建标签并添加到选项卡窗口
 - 添加标签窗口的父窗口
 - 响应消息的通知
  

  \code
	//create a button创建一个按钮
	HWND hTabWnd = SGL_CreateWindow(SMP_TabWindow_WndProc, ...);
	SGL_AddChildWindow(hWnd, hTabWnd);

	hControl = SGL_CreateWindow(TAB1_WndProc, ...);
	SMP_TabWindow_AddTab(hTabWnd, BMP_BUSY, SGL_LoadString(STR_OK), hControl);		

	//response to tab window notify messages响应消息选项卡窗口通知
	case WM_COMMAND:
		WID id = LOWORD(wParam);
		WORD code = HIWORD(wParam);

		if(id == "the tab window id")
		{
			Sint32 index = (Sint32)lParam; // the tab index/标签指数

			switch(code)
			{
			case SMP_TABWNDN_TABSWITCHED:
				//handle the notify event./处理通知事件
				break;
			}
		}	
  \endcode


窗口样式

 * \brief Indicates that right arrow pressed简短表示按下右箭头
 */
#define SMP_TABWNDS_RIGHTARROW			0x0001L

/**
 * \brief Indicates that left arrow pressed\简短表示左键按下
 */
#define SMP_TABWNDS_LEFTARROW			0x0002L

/** 

窗口NOTIFY消息

 * \brief Sent when tab window switched \简短的发送选项卡窗口切换时
 *
 * \code
 *	case WM_COMMAND:
 *		WID id = LOWORD(wParam);
 *		WORD code = HIWORD(wParam);
 * 
 *		if(id == "the tab window id")
 *		{
 *			Sint32 index = (Sint32)lParam; // the tab index/标签指数
 *			switch(code)
 *			{
 *			case SMP_TABWNDN_TABSWITCHED:
 *				//handle the notify event./处理通知事件
 *				break;
 *			}
 *		}	
 * \endcode
 *
 * \param index the high light tab index 参数指标的高光选项卡索引
 */
#define SMP_TABWNDN_TABSWITCHED		0x0001

/*
 * \brief The tab window bar height\简短的标签窗口栏的高度
 */
#define SMP_TABWND_TABBAR_HEIGHT		SMP_ITEM_HEIGHT

/**
 * \brief The max tab the tab window support\简短的最大标签上的标签窗口支持
 */
#define SMP_TABWND_TABCOUNT			12

/**
窗口成员函数

 * \brief Add a tab window简短添加选项卡窗口
 *
 * \param hWnd the tab window handle参数的hWnd窗口句柄的标签
 * \param bmp the icon id参数骨形态发生蛋白的图标的ID
 * \param title the child tab window title\参数标题子窗口的标题标签
 * \param hTab the  child window handle\参数hTab子窗口句柄
 */
VOID SMP_TabWindow_AddTab(HWND hWnd, DWORD bmp, PCWSTR title, HWND hTab);

/**
 * \brief Remove a tab\简短删除标签
 *
 * \param hWnd the tab window handle\参数的hWnd窗口句柄的标签
 * \param id the child tab window id\参数标识标签的子窗口ID
 */
VOID SMP_TabWindow_RemoveTab(HWND hWnd, WID id);

/**
 * \brief Remove all the tabs\简短删除所有的标签
 *
 * \param hWnd the tab window handle\参数的hWnd窗口句柄的标签
 */
VOID SMP_TabWindow_RemoveAllTabs(HWND hWnd);

/**
 * \brief Get the tab child window by index\简短找子窗口的选项卡索引
 *
 * \param hWnd the tab window handle \参数的hWnd窗口句柄的标签
 * \param index the child index\参数指标指数的孩子
 * \return the child tab window handle\返回子标签窗口句柄
 */
HWND SMP_TabWindow_GetTabByIndex(HWND hWnd, Sint32 index);

/**
 * \brief Get the tab child window by id\简短找子窗口ID的标签
 *
 * \param hWnd the tab window handle\参数的hWnd窗口句柄的标签
 * \param id the child tab window id\参数标识标签的子窗口ID
 * \return the child tab window handle \返回子标签窗口句柄
 */
HWND SMP_TabWindow_GetTabByID(HWND hWnd, WID id);

/**
 * \brief Get the active tab index\简短获取当前标签指数
 *
 * \param hWnd the tab window handle\参数的hWnd窗口句柄的标签
 * \return the active tab index \返回当前标签指数
 */
Sint32 SMP_TabWindow_GetActiveTabIndex(HWND hWnd);

/**
 * \brief Set the tab with id as the active tab.\简短的设置作为活动标签的ID标签
 *
 * \param hWnd the tab window handle\参数的hWnd窗口句柄的标签
 * \param id the child window handle\参数标识子窗口句柄
 */
VOID SMP_TabWindow_SetActiveTabByID(HWND hWnd, WID id);

/**
 * \brief Set the tab of index as the active tab\简短设置指数作为当前标签标签
 *
 * \param hWnd the tab window handle\参数的hWnd窗口句柄的标签
 * \param index the child window index\参数指标指数的子窗口
 */
VOID SMP_TabWindow_SetActiveTabByIndex(HWND hWnd, Sint32 index);

/**

窗口程序

 * \brief The simple tab window procedure.	\简短的简单标签的窗口过程
 *
 * \param hWnd the tab window handle\参数的hWnd窗口句柄的标
 * \param Msg the window message\参数消息的窗口消息
 * \param wParam the first parameter\参数wParam的第一个参数
 * \param lParam the second parameter * \参数lParam的第二个参数
 * \return the result of message process  * \返回消息的处理结果
 */
LRESULT SMP_TabWindow_WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
	

#endif

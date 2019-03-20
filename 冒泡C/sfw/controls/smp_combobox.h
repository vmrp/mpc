#ifndef _SMP_COMBOBOX_H
#define _SMP_COMBOBOX_H

#include "window.h"

/**
  简单组合框

  若要使用组合框：
   - 创建组合框窗口
   - 设置组合框项目和选定项目
   - 添加到父窗口
   - 响应消息的通知

  \code
  	//创建组合框
  	hCombo = SGL_CreateWindow(SMP_ComboBox_WndProc, ...);
  	SMP_ComboBox_SetItems(hCombo, ...);
  	SMP_ComboBox_SetSelectedItem(hCombo, ...);
  	SGL_AddChildWindow(hWnd, hCombo);

  	//响应消息
	case WM_COMMAND:
		WID id = LOWORD(wParam);
		WORD code = HIWORD(wParam);

		if(id == "the combo id" && code == SMP_COMBON_VALUECHANGED)
		{
			int index = (int)lParam; //当前选定的索引
			...
		}	
  \endcode


窗口消息
 * 的组合框的值更改通知消息。
 *
 * \code
 *	case WM_COMMAND:
 *		WID id = LOWORD(wParam);
 *		WORD code = HIWORD(wParam);
 *
 *		if(id == "the combo id" && code == SMP_COMBON_VALUECHANGED)
 *		{
 *			int index = (int)lParam; //当前选定的索引
 *			...
 *		}
 * \endcode
 *
 * 参数index	当前选定的索引
 */

#define SMP_COMBON_VALUECHANGED		0x0001

/** 
 * \设置组合框列表的大小
 *
 * \参数hWnd	组合框的窗口句柄
 * \参数size	列表的大小
 * \成功返回TRUE，否则返回FALSE
 */
BOOL SMP_ComboBox_SetSize(HWND hWnd, Sint32 size);

/**
 * \获取组合框列表的大小.
 * \参数hWnd	组合框的窗口句柄
 * \返回组合框列表的大小
 */
Sint32 SMP_ComboBox_GetSize(HWND hWnd);

/**
 * \设置组合框列表项的信息
 * \参数hWnd		组合框的窗口句柄
 * \参数index		列表索引
 * \参数str			显示的字符串
 * \参数userdata	用户特定的数据
 */
VOID SMP_ComboBox_SetItem(HWND hWnd, Sint32 index, PCWSTR str, DWORD userdata);

/**
 * \获取组合框列表项的信息
 * \参数hWnd			组合框的窗口句柄
 * \参数index			索引项
 * \参数[出]str			显示的字符串
 * \参数[出]userdata	用户特定的数据
 */
VOID SMP_ComboBox_GetItem(HWND hWnd, Sint32 index, PCWSTR * str, DWORD* userdata);

/**
 的设置组合框列表项。
 这是一个SMP_ComboBox_SetSize / SMP_ComboBox_SetItem wraper功能
 所以，如果SMP_ComboBox_SetItems使用请不要叫SMP_ComboBox_SetSize / SMP_ComboBox_SetItem了
 参数的hWnd窗口句柄的组合框
 参数项的组合框列表项的数组
 参数数组的大小尺寸的物品
 成功返回true，否则返回FALSE
 * \对应 SMP_ComboBox_SetSize, SMP_ComboBox_SetItem
 */
BOOL SMP_ComboBox_SetItems(HWND hWnd, const DWORD* items, Sint32 size);

/**
 * \选择它的索引项。
 * \参数hWnd		组合框的窗口句柄
 * \参数index		列表项的索引
 * \参数redraw		是否重绘组合框
 * \参数notify		是否发送通知消息
 */
VOID SMP_ComboBox_SetSelectedItem(HWND hWnd, Sint32 index, BOOL redraw, BOOL notify);

/**
 * \获取选中项的索引
 * \参数hWnd		组合框的窗口句柄
 * \返回所选项目的索引
 */
Sint32 SMP_ComboBox_GetSelectedItem(HWND hWnd);

/** 
窗口程序
 * \组合框程序
 * \参数hWnd		窗口句柄
 * \参数Msg			窗口消息
 * \参数wParam		第一个参数
 * \参数lParam		第二个参数
 * \返回消息的处理结果
 */
LRESULT SMP_ComboBox_WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);


#endif

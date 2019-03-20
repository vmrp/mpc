#ifndef _SMP_DATEPICKER_H
#define _SMP_DATEPICKER_H

#include "window.h"

/**
defgroup smp_datepickerDatePicker
   若要使用日期选择器：
 - 创建日期选择器窗口
 - 设置当前日期
 - 添加到父窗口
 - 响应消息的通知
 
  \code
	//创建一个日期选择器
	hControl = SGL_CreateWindow(SMP_DatePicker_WndProc, ...);
	SMP_DatePicker_Set2CurrentDate(hControl, FALSE, FALSE);
	SGL_AddChildWindow(hWnd, hControl);

	//响应消息的通知
	case WM_COMMAND:
		WID id = LOWORD(wParam);
		WORD code = HIWORD(wParam);

		if(id == "the datepicker id")
		{
			switch(code)
			{
			case SMP_DATEPICKERN_VALUECHANGED:
				HWND hControl = (HWND)lParam; // DatePicker的窗口句柄
				//处理通知事件。
				break;
			}
		}	
  \endcode
*/

/*
窗口NOTIFY消息 
 *\DatePicker的窗口时发送的值改变。
 *
 * \code
 *	case WM_COMMAND:
 *		WID id = LOWORD(wParam);
 *		WORD code = HIWORD(wParam);
 *
 *		if(id == "the calendar id" && code == SMP_DATEPICKERN_VALUECHANGED)
 *		{
			HWND hControl = (HWND)lParam; //DatePicker的窗口句柄
 * 			//处理的通知消息
 *		}
 * \endcode
 *
 * 参数hControl日历发送此消息处理
*/
#define SMP_DATEPICKERN_VALUECHANGED		0x0001

/** 
窗口成员函数
 * \brief Set the new date 设定新的日期
 *
 * \param hWnd the date picker window handle日期选择器参数的hWnd窗口句柄
 * \param year the year of the date参数一年一年的日期
 * \param month the month of the date参数月份的日期的月份
 * \param day the day of the date参数一天一天的日期
 * \param notify if send notify message参数如果发送通知消息通知
 * \param update if redraw the window参数更新，如果重绘窗口
 */
VOID SMP_DatePicker_SetDate(HWND hWnd, int year, int month, int day, BOOL notify, BOOL update);

/**
 * \brief Get the date from the control获取日期从控制
 *
 * \param hWnd the Date picker window handle参数的hWnd窗口句柄的日期选择器
 * \param[out] year the year of the date参数[输出]年全年的日期
 * \param[out] month the month of the date参数[出]月份的日期的月份
 * \param[out] day the day of the date参数[出]一天一天的日期
 */
VOID SMP_DatePicker_GetDate(HWND hWnd, int* year, int* month, int* day);

/**
 * \brief Set the Date picker to current date的日期选择器设置为当前日期
 *
 * \param hWnd the date picker window handle参数的hWnd窗口句柄的日期选择器
 * \param notify if send notify message参数如果发送通知消息通知
 * \param update if redraw the window参数更新，如果重绘窗口
 */
VOID SMP_DatePicker_Set2CurrentDate(HWND hWnd, BOOL notify, BOOL update);

/** 
窗口程序
 * \brief The simple Date picker window procedure.日期选择器窗口过程
 *
 * \param hWnd the window handle参数的hWnd窗口句柄
 * \param Msg the window message\参数消息的窗口消息
 * \param wParam the first parameter\参数wParam的第一个参数
 * \param lParam the second parameter\参数lParam的第二个参数
 * \return the result of message process \返回消息的处理结果
 */
LRESULT SMP_DatePicker_WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);


#endif

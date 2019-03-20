#ifndef _SMP_CALENDAR_H
#define _SMP_CALENDAR_H

#include "window.h"

/**
  defgroup smp_calendar简单的日历

  若要使用简单的日历：
 - 创建日历窗口
 - 设置当前日期
 - 添加到父窗口
 - 响应消息的日历通知

  \code
	//创建一个日历窗口
	hControl = SGL_CreateWindow(SMP_Calendar_WndProc, ...);
	SMP_Calendar_SetDate(hControl,  ...);
	SGL_AddChildWindow(hWnd, hControl);

	//响应消息通知到日历
	case WM_COMMAND:
		WID id = LOWORD(wParam);
		WORD code = HIWORD(wParam);

		if(id == "the calendar id")
		{
			hControl = (HWND)lParam;
			switch(code)
			{
			case SMP_CALENDARN_SELECTED:
				//处理通知事件。
				break;
			}
		}	
  \endcode
*/ 

/**
 *窗口NOTIFY消息
 *简短的发送，当用户选择一个日期
 * \code
 *	case WM_COMMAND:
 *		WID id = LOWORD(wParam);
 *		WORD code = HIWORD(wParam);
 *
 *		if(id == "the calendar id" && code == SMP_CALENDARN_SELECTED)
 *		{
 *			HWND hControl = (HWND)lParam; //日历处理
 * 			//处理通知消息的日历
 *		}
 * \endcode
 *参数hControl日历窗口句柄
 */
#define SMP_CALENDARN_SELECTED	0x0001

/**
窗口成员函数
 * \brief Set the calendar date简短设置日历日期
 * \param hWnd the calendar window handle参数的hWnd窗口句柄的日历
 * \param year the new value of year参数年一年中新的价值
 * \param month the new value of month参数月份一个月的新值
 * \param day the new value of day参数一天一天的新值
 */
VOID SMP_Calendar_SetDate(HWND hWnd, int year, int month, int day);

/**
 * \brief Get the current date简短获取当前日期
 * \param hWnd the calendar window handle参数的hWnd窗口句柄的日历
 * \param[out] year return the value of the year参数[输出]年返回年份值
 * \param[out] month return the value of the month参数[输出]月返回月份值
 * \param[out] day return the value of the day参数[输出]日返回值的一天
 */
VOID SMP_Calendar_GetDate(HWND hWnd, int* year, int* month, int* day);
/*
窗口程序
 * \brief The simple calendar window procedure. \简报简单的日历窗口过程
 *
 * \param hWnd the window handle\参数的hWnd窗口句柄
 * \param Msg the window message\参数消息的窗口消息
 * \param wParam the first parameter\参数wParam的第一个参数
 * \param lParam the second parameter\参数lParam的第二个参数
 * \return the result of message process 返回消息的处理结果
 */
LRESULT SMP_Calendar_WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);


#endif

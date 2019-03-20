#include "smp_datepicker.h"
#include "smp_calendar.h"
#include "bmp.h"
#include "smp.h"
#include "smp_msgbox.h"
#include "smp_toolbar.h"
#include "i18n.h"


#define STR_DATE_FORMAT	"%d-%d-%d" //display format显示格式

#define STR_DATE_LENGTH	10 //max chars count最大字符数
#define SMP_CALENDAR_HEIGHT	190 //the calendar control height日历控件的高度
#define SMP_DATEPICKER_CALENDAR_HEIGHT	(SMP_CALENDAR_HEIGHT + SMP_TOOLBAR_HEIGHT) // the msgbox height在MsgBox高度


#define SMP_DATEPICKER_MSGBOX		1 //the msgbox id在MsgBox身份证

#define SMP_DATEPICKER_TOOLBAR	2 //the toolbar id工具栏的ID

#define SMP_DATEPICKER_CALENDAR 	3 //the calendar id 的日历ID


//date picker data日期选择器数据
typedef struct SMP_DatePickerData
{
	int year; //current year当前年
	int month; //current month当前月

	int day; //current day当前日
}DPDATA, *PDPDATA;

//////////////////////////////////////////////////////////////////////


LRESULT SMP_DatePicker_Calendar_WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch(Msg)
	{
	case WM_CREATE:
	{
		HWND hControl;
		hControl = SGL_CreateWindow(SMP_Calendar_WndProc,
				0, 0, _WIDTH(hWnd), SMP_CALENDAR_HEIGHT,
				SMP_DATEPICKER_CALENDAR, WS_TABSTOP | WS_BORDER, 0);
		SGL_AddChildWindow(hWnd, hControl);
		
		hControl = SGL_CreateWindow(SMP_Toolbar_WndProc, 
				0, _HEIGHT(hWnd) - SMP_TOOLBAR_HEIGHT, _WIDTH(hWnd), SMP_TOOLBAR_HEIGHT, 
				SMP_DATEPICKER_TOOLBAR, 0, 0);
 		SMP_Toolbar_SetStrings(hControl, STR_SELECT, RESID_INVALID, STR_CANCEL, FALSE);
		SGL_AddChildWindow(hWnd, hControl);
		break;
	}

	case WM_INITFOCUS:
	{
		SGL_FocusNext(hWnd, TRUE);
		break;
	}

	case WM_KEYUP:
	{
		if(wParam == MR_KEY_SOFTRIGHT)
		{
			SGL_ExitModal(hWnd, (WORD)ID_CANCEL);
			return 1;
		}else if((wParam == MR_KEY_SELECT || wParam == MR_KEY_SOFTLEFT)){
			SGL_ExitModal(hWnd, (WORD)ID_OK);			
			return 1;
		}
		break;
	}

	case WM_COMMAND:
	{
		WID id = LOWORD(wParam);
		WORD code = HIWORD(wParam);
		if(id == SMP_DATEPICKER_CALENDAR && code == SMP_CALENDARN_SELECTED)
			SGL_ExitModal(hWnd, (WORD)ID_OK);			
		break;
	}

	}

	return 0;
}


static VOID SMP_DatePicker_ShowCalendar(HWND hWnd)
{
	PDPDATA pData = _GET_WINDATA(hWnd, PDPDATA);	
	HWND hTopWnd = SGL_GetTopWindow(hWnd);
	HWND hModal = SGL_CreateWindow(SMP_DatePicker_Calendar_WndProc,
			0, _HEIGHT(hTopWnd) - SMP_DATEPICKER_CALENDAR_HEIGHT, _WIDTH(hTopWnd), SMP_DATEPICKER_CALENDAR_HEIGHT,
			SMP_DATEPICKER_MSGBOX, WS_TRANSPARENT, 0);	
	HWND hControl = SGL_FindChildWindow(hModal, SMP_DATEPICKER_CALENDAR);
	SMP_Calendar_SetDate(hControl, pData->year, pData->month, pData->day);
	_LISTENER(hModal) = hWnd;
	SGL_DoModal(hModal, hTopWnd);
}


VOID SMP_DatePicker_SetDate(HWND hWnd, int year, int month, int day, BOOL notify, BOOL update)
{
	PDPDATA pData = _GET_WINDATA(hWnd, PDPDATA);

	if(pData->year == year && pData->month == month && pData->day == day)
		return;

	pData->year = year;
	pData->month = month;
	pData->day = day;

	if(update)
		SGL_UpdateWindow(hWnd);

	if(notify)
		SGL_NotifyParent(hWnd, SMP_DATEPICKERN_VALUECHANGED, hWnd);
}


VOID SMP_DatePicker_GetDate(HWND hWnd, int* year, int* month, int* day)
{
	PDPDATA pData = _GET_WINDATA(hWnd, PDPDATA);

	if(year) *year = pData->year;
	if(month) *month = pData->month;
	if(day) *day = pData->day;
}


VOID SMP_DatePicker_Set2CurrentDate(HWND hWnd, BOOL notify, BOOL update)
{
	mr_datetime date;
	
	mrc_getDatetime(&date);
	
	SMP_DatePicker_SetDate(hWnd, date.year, date.month, date.day, notify, update);
}


LRESULT SMP_DatePicker_WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	PDPDATA pData = _GET_WINDATA(hWnd, PDPDATA);
	
	switch(Msg)
	{
	
	case WM_CREATE:
	{
		pData = (PDPDATA)SGL_MALLOC(sizeof(DPDATA));
		if(NULL == pData){
			SGL_TRACE("%s, %d: memory out\n", __FILE__, __LINE__);
			return 1;
		}
		
		SGL_MEMSET(pData, 0, sizeof(DPDATA));
		_SET_WINDATA(hWnd, pData);
		break;
	}

	case WM_DESTROY:
	{
		if(pData) SGL_FREE(pData);
		break;
	}
	
	case WM_SHOW:
	{
		_CLR_STYLE(hWnd, WS_PRESSED);
		if(pData->year == 0 || pData->month == 0 || pData->day == 0)
			SMP_DatePicker_Set2CurrentDate(hWnd, FALSE, FALSE);
		break;
	}
	
	case WM_PAINT:
	{
		int x = 0, y = 0, w, h;
		CHAR date[STR_DATE_LENGTH+1];
		int32 wt, ht;
		mr_screenRectSt r;
		mr_colourSt color = {0, 0, 0};
		HFONT font = SGL_GetSystemFont();
		HBITMAP hBmp = SGL_LoadBitmap(_IS_SET_ANY(hWnd, WS_PRESSED)? BMP_ARRDOWN_PRESS : BMP_ARRDOWN, &w, &h);
		
		SGL_WindowToScreen(hWnd, &x, &y);

		//draw the border绘制边界
		GAL_Rectangle(PHYSICALGC, x, y, _WIDTH(hWnd), _HEIGHT(hWnd), _IS_SET_ANY(hWnd, WS_FOCUSED)? COLOR_focus : COLOR_black);

		//draw the content绘制的内容
		SGL_SPRINTF(date, STR_DATE_FORMAT, pData->year, pData->month, pData->day);
		mrc_textWidthHeight(date, FALSE, (uint16)font, &wt, &ht);
		r.x = (uint16)(x+SMP_ITEM_CONTENT_MARGIN); r.y = (uint16)y; r.h = (uint16)_HEIGHT(hWnd); r.w = (uint16)(_WIDTH(hWnd) - w - 2*SMP_ITEM_CONTENT_MARGIN);
		mrc_drawTextEx((PSTR)date, (int16)(r.x) , (int16)(r.y + DIV(r.h - ht, 2)), r, color, 0, (uint16)font);

		//draw the bitmap绘制位图

		mrc_bitmapShowEx((uint16 *)hBmp, (int16)(x + _WIDTH(hWnd) - w - 2), (int16)(y + DIV(_HEIGHT(hWnd) - h, 2)), (int16)w, (int16)w, (int16)h, BM_COPY, 0, 0);
		break;
	}

	case WM_MOUSEDOWN:
	{
		_SET_STYLE(hWnd, WS_PRESSED);
		SGL_SetMouseTracker(hWnd);
		SGL_UpdateWindow(hWnd);
		break;
	}

	case WM_MOUSEUP:
	{
		int x = (int)wParam, y = (int)lParam;
		if(!_IS_SET_ANY(hWnd, WS_PRESSED) )
			break;

		_CLR_STYLE(hWnd, WS_PRESSED);
		SGL_UpdateWindow(hWnd);

		if(x < 0 || y < 0 || x > _WIDTH(hWnd) || y > _HEIGHT(hWnd))
			break;

		SMP_DatePicker_ShowCalendar(hWnd);
		break;
	}

	case WM_KEYUP:
	{
		if(MR_KEY_SELECT == wParam)
		{
			SMP_DatePicker_ShowCalendar(hWnd);
			return 1;
		}
		break;
	}

	case WM_SETFOCUS:
	case WM_KILLFOCUS:
	{
		SGL_UpdateWindow(hWnd);
		break;
	}

	case WM_COMMAND:
	{
		WID id = LOWORD(wParam);
		WORD code = HIWORD(wParam);
		if(id == SMP_DATEPICKER_MSGBOX)
		{
			if(code == ID_OK)
			{
				int year, month, day;
				HWND hControl = SGL_FindChildWindow((HWND)lParam, SMP_DATEPICKER_CALENDAR);
				SMP_Calendar_GetDate(hControl, &year, &month, &day);
				SMP_DatePicker_SetDate(hWnd, year, month, day, TRUE, TRUE);
			}
			SGL_DestroyWindow((HWND)lParam);
		}
		break;
	}
	
	}
	
	return 0;
}


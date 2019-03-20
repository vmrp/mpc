#include "smp_calendar.h"
#include "smp_spin.h"
#include "smp.h"
#include "i18n.h"

//supported year支持一年
#define SMP_CALENDAR_YEAR_MIN			1900
#define SMP_CALENDAR_YEAR_MAX		2100

//supported month支持月

#define SMP_CALENDAR_MONTH_MIN		1
#define SMP_CALENDAR_MONTH_MAX		12

#define SMP_CALENDAR_SPIN_YEAR			1 //year spin wid年自旋妇女参与发展
#define SMP_CALENDAR_SPIN_YEAR_X			SMP_ITEM_SPACE //year spin left年左旋转
#define SMP_CALENDAR_SPIN_YEAR_WIDTH	90 //year spin width年自旋宽度
#define SMP_CALENDAR_STR_YEAR_X			(SMP_CALENDAR_SPIN_YEAR_X + SMP_CALENDAR_SPIN_YEAR_WIDTH + SMP_ITEM_SPACE) //year string left

#define SMP_CALENDAR_SPIN_MONTH			2 //month spin wid月自旋妇女参与发展
#define SMP_CALENDAR_SPIN_MONTH_X		(SMP_CALENDAR_SPIN_YEAR_WIDTH + SMP_ITEM_HEIGHT + 3*SMP_ITEM_SPACE) //month spin left
#define SMP_CALENDAR_SPIN_MONTH_WIDTH	70 //month spin width月自旋宽度

#define SMP_CALENDAR_STR_MONTH_X			(SMP_CALENDAR_SPIN_MONTH_X + SMP_CALENDAR_SPIN_MONTH_WIDTH + SMP_ITEM_SPACE) //month string left 
#define SMP_CALENDAR_WEEKDAY_TITLE_X	SMP_ITEM_SPACE //weekday bar left space平日栏左侧空间
 
//calendar data日历数据

typedef struct SMP_CalendarData
{
	int year; //current year当前年
	int month; //current month当前月
	int day; //current day当前日
	int32 dayw, dayh; //temp information for the control为控制温度信息
}CALDATA, *PCALDATA;

////////////////////////////////////////////////////////////////
const char sStrDays[32][3] =
{
	" 0", " 1", " 2", " 3", " 4", " 5", " 6", " 7", " 8", " 9",
	"10", "11", "12", "13", "14", "15", "16", "17", "18", "19",
	"20", "21", "22", "23", "24", "25", "26", "27", "28", "29",
	"30", "31"
};

//根据日期算出是星期几
static int CaculateWeekDay(int year, int month, int day)
{
	int weekday;

	//一月和二月被当作前一年的
	if((month==1) || (month==2))
	{
		month += 12;
		year--;
	}
	
	weekday = (day + 2*month + 3*(month+1)/5 + year + year/4 - year/100 + year/400)%7 + 1;
	if(weekday==7) weekday=0;
	return weekday;
}


//根据日期，算出某个月有多少天
static int DaysOfMonth(int year, int month)
{
	static const char sDays[2][12] = {
		31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31, 
		31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
	};

	if(month < 1){
		month += 12;
		year--;
	}
	else if(month > 12){
		month -= 12;
		year++;
	}
	
	return sDays[(year%100!=0 && year%4==0) || year%400==0][month-1];
}


static VOID SMP_Calendar_HighLightDate(HWND hWnd, int DayOld, int DayNew)
{
	PCALDATA pData = _GET_WINDATA(hWnd, PCALDATA);
	int weekday = CaculateWeekDay(pData->year, pData->month, 1);
	int step, x = SMP_CALENDAR_WEEKDAY_TITLE_X, y = 2*SMP_ITEM_HEIGHT + 2;

	SGL_WindowToScreen(hWnd, &x, &y);

	step = (_WIDTH(hWnd) - 2*SMP_CALENDAR_WEEKDAY_TITLE_X) /7;
	DayOld += weekday - 1;
	GAL_Rectangle(PHYSICALGC, x + step * MOD(DayOld, 7), y + pData->dayh * DIV(DayOld, 7), step, pData->dayh, COLOR_lightwhite);	
	GAL_FlushRegion(PHYSICALGC, x + step * MOD(DayOld, 7), y + pData->dayh * DIV(DayOld, 7), step, pData->dayh);

	DayNew += weekday - 1;
	GAL_Rectangle(PHYSICALGC, x + step * MOD(DayNew, 7), y + pData->dayh * DIV(DayNew, 7), step, pData->dayh, COLOR_red);	
	GAL_FlushRegion(PHYSICALGC, x + step * MOD(DayNew, 7), y + pData->dayh * DIV(DayNew, 7), step, pData->dayh);
}


static VOID SMP_Calendar_UpdateValuesToSpins(HWND hWnd)
{
	HWND hControl;
	PCALDATA pData = _GET_WINDATA(hWnd, PCALDATA);
	
	hControl = SGL_FindChildWindow(hWnd, SMP_CALENDAR_SPIN_YEAR);	
	SMP_Spin_SetValue(hControl, pData->year, FALSE, FALSE);
	
	hControl = SGL_FindChildWindow(hWnd, SMP_CALENDAR_SPIN_MONTH);
	SMP_Spin_SetValue(hControl, pData->month, FALSE, FALSE);	
}


static VOID SMP_Calendar_MoveByStep(HWND hWnd, int step)
{
	PCALDATA pData = _GET_WINDATA(hWnd, PCALDATA);
	int year = pData->year;
	int month = pData->month;
	int day = pData->day;
	
	day += step;
	if(day < 1)
	{
		--month;
		day = DaysOfMonth(pData->year, month);
	}else if(day > DaysOfMonth(pData->year, pData->month)){
		++month;
		day = 1;
	}

	if(month < 1)
	{
		month = 12;
		--year;
	}else if(month > 12){
		month = 1;
		++year;
	}

	if(year < SMP_CALENDAR_YEAR_MIN || year > SMP_CALENDAR_YEAR_MAX)
		return;

	if(year == pData->year && month == pData->month)
	{
		SMP_Calendar_HighLightDate(hWnd, pData->day, day);
		pData->day = day;
	}else{
		pData->year = year;
		pData->month = month;
		pData->day = day;
		SMP_Calendar_UpdateValuesToSpins(hWnd);
		SGL_UpdateWindow(hWnd);		
	}
}


VOID SMP_Calendar_GetDate(HWND hWnd, int * year, int * month, int * day)
{
	PCALDATA pData = _GET_WINDATA(hWnd, PCALDATA);

	if(year) *year = pData->year;
	if(month) *month = pData->month;
	if(day) *day = pData->day;
}


VOID SMP_Calendar_SetDate(HWND hWnd, int year, int month, int day)
{
	PCALDATA pData = _GET_WINDATA(hWnd, PCALDATA);

	if(year < SMP_CALENDAR_YEAR_MIN || year > SMP_CALENDAR_YEAR_MAX
		|| month < SMP_CALENDAR_MONTH_MIN || month > SMP_CALENDAR_MONTH_MAX
		|| day < 1 || day > DaysOfMonth(year, month))
		return;

	pData->year = year;
	pData->month = month;
	pData->day = day;
}


LRESULT SMP_Calendar_WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	PCALDATA pData = _GET_WINDATA(hWnd, PCALDATA);
	
	switch(Msg)
	{
	
	case WM_CREATE:
	{
		HWND hControl;

		//malloc the data
		pData = (PCALDATA)SGL_MALLOC(sizeof(CALDATA));
		if(NULL == pData){
			SGL_TRACE("%s, %d: memory out\n", __FILE__, __LINE__);
			return 1;
		}
		
		SGL_MEMSET(pData, 0, sizeof(CALDATA));
		_SET_WINDATA(hWnd, pData);

		//create the children windows创建窗口的儿童
		//spin of year自旋年
		hControl = SGL_CreateWindow(SMP_Spin_WndProc,
			SMP_CALENDAR_SPIN_YEAR_X, 1, SMP_CALENDAR_SPIN_YEAR_WIDTH, SMP_ITEM_HEIGHT - 1,
			SMP_CALENDAR_SPIN_YEAR, 0, 0);
		SMP_Spin_SetRange(hControl, SMP_CALENDAR_YEAR_MIN, SMP_CALENDAR_YEAR_MAX, 1);
		SGL_AddChildWindow(hWnd, hControl);
		_BGCOLOR(hControl) = COLOR_controlbg;
		
		//spin of month自旋月
		hControl = SGL_CreateWindow(SMP_Spin_WndProc,
			SMP_CALENDAR_SPIN_MONTH_X, 1, SMP_CALENDAR_SPIN_MONTH_WIDTH, SMP_ITEM_HEIGHT - 1,
			SMP_CALENDAR_SPIN_MONTH, 0, 0);
		SMP_Spin_SetRange(hControl, SMP_CALENDAR_MONTH_MIN, SMP_CALENDAR_MONTH_MAX, 1);
		SGL_AddChildWindow(hWnd, hControl);
		_BGCOLOR(hControl) = COLOR_controlbg;
		break;
	}
		
	case WM_DESTROY:
	{
		if(pData) SGL_FREE(pData);
		break;
	}
		
	case WM_SHOW:
	{
		if(pData->year == 0 ||pData->month == 0 || pData->day == 0 )
		{
			mr_datetime date;
			mrc_getDatetime(&date);
			pData->year = date.year;
			pData->month = date.month;
			pData->day = date.day;
		}

		SMP_Calendar_UpdateValuesToSpins(hWnd);
		mrc_textWidthHeight((PSTR)sStrDays[31], FALSE, (uint16)SGL_GetSystemFont(), &pData->dayw, &pData->dayh);
		pData->dayh += 2;
		break;
	}
	
	case WM_PAINT:
	{
		int i, j, step, x = 0, y = 0; 
		int days, weekday;
		int32 wt, ht;
		HFONT font = SGL_GetSystemFont();
		
		SGL_WindowToScreen(hWnd, &x, &y);

		//draw the title bar绘制标题栏
		mrc_textWidthHeight((PSTR)SGL_LoadString(STR_YEAR), TRUE, (uint16)font, &wt, &ht);
		if(_IS_SET_ANY(hWnd, WS_BORDER))
		{
			GAL_Rectangle(PHYSICALGC, x, y, _WIDTH(hWnd), _HEIGHT(hWnd), COLOR_lightgray);
			GAL_FillBox(PHYSICALGC, x + 1, y + 1, _WIDTH(hWnd) - 2, SMP_ITEM_HEIGHT - 1, COLOR_controlbg);
		}else{
			GAL_FillBox(PHYSICALGC, x, y, _WIDTH(hWnd), SMP_ITEM_HEIGHT, COLOR_controlbg);
		}
		mrc_drawText((PSTR)SGL_LoadString(STR_YEAR), (int16)SMP_CALENDAR_STR_YEAR_X, (int16)(y + DIV(SMP_ITEM_HEIGHT-ht, 2)), 0, 0, 0, TRUE, (uint16)font);
		mrc_drawText((PSTR)SGL_LoadString(STR_MONTH), (int16)SMP_CALENDAR_STR_MONTH_X, (int16)(y + DIV(SMP_ITEM_HEIGHT-ht, 2)), 0, 0, 0, TRUE, (uint16)font);

		//draw the weekday title bar绘制标题栏平日
		step = (_WIDTH(hWnd) - 2*SMP_CALENDAR_WEEKDAY_TITLE_X) /7;
		y += SMP_ITEM_HEIGHT;
		for(i = 0; i < 7; i++)
			mrc_drawText((PSTR)SGL_LoadString(STR_SUNDAY + i), (int16)(x+SMP_CALENDAR_WEEKDAY_TITLE_X + i*step), (int16)(y + DIV(SMP_ITEM_HEIGHT - ht, 2)), 0, 0, 0, TRUE, (uint16)font);
		
		y+= SMP_ITEM_HEIGHT;
		GAL_Rectangle(PHYSICALGC, x+1, y, _WIDTH(hWnd)-2, 6*pData->dayh + 4, COLOR_lightgray);
		weekday = CaculateWeekDay(pData->year, pData->month, 1);
		days = DaysOfMonth(pData->year, pData->month);

		y += 2; x += SMP_CALENDAR_WEEKDAY_TITLE_X;
		for(i = 1; i <= days; i++)
		{
			j = MOD(weekday + i - 1, 7);
			mrc_drawText((PSTR)sStrDays[i], (int16)(x + j*step), (int16)y, 0, 0, 0, FALSE, (uint16)font);
			if(i == pData->day)
				GAL_Rectangle(PHYSICALGC, x + j*step, y, step, pData->dayh, COLOR_red);
			if(j == 6) y+=pData->dayh;
		}
		
		break;
	}
	
	case WM_KEYDOWN:
	case WM_KEYDOWNREPEAT:
	{
		switch(wParam)
		{
		case MR_KEY_UP:
			SMP_Calendar_MoveByStep(hWnd, -7);
			return 1;
		case MR_KEY_DOWN:
			SMP_Calendar_MoveByStep(hWnd, 7);
			return 1;
		case MR_KEY_RIGHT:
			SMP_Calendar_MoveByStep(hWnd, 1);
			return 1;
		case MR_KEY_LEFT:
			SMP_Calendar_MoveByStep(hWnd, -1);
			return 1;
		}
		break;
	}
		
	case WM_KEYUP:
	{
		switch(wParam)
		{
		case MR_KEY_SELECT:
			SGL_NotifyParent(hWnd, SMP_CALENDARN_SELECTED, hWnd);
			return 1;
		}
		break;
	}

	case WM_MOUSEDOWN:
	case WM_MOUSEUP:
	{
		if((int)wParam >= SMP_CALENDAR_WEEKDAY_TITLE_X && (int)wParam < _WIDTH(hWnd) - SMP_CALENDAR_WEEKDAY_TITLE_X
			&& (int)lParam >= 2*SMP_ITEM_HEIGHT + 2 && (int)lParam < _HEIGHT(hWnd) - 2)
		{
			int step = (_WIDTH(hWnd) - 2*SMP_CALENDAR_WEEKDAY_TITLE_X) /7;
			int day = (wParam - SMP_CALENDAR_WEEKDAY_TITLE_X)/step +  (lParam -2*SMP_ITEM_HEIGHT - 2) / pData->dayh * 7; 
			day -= CaculateWeekDay(pData->year, pData->month, 1) - 1;
			if(Msg == WM_MOUSEDOWN){	
				if(day != pData->day && day > 0 && day <= DaysOfMonth(pData->year, pData->month)){
					SMP_Calendar_HighLightDate(hWnd, pData->day, day);
					pData->day = day;
				}
			}
			else{
				if(day == pData->day)
					SGL_NotifyParent(hWnd, SMP_CALENDARN_SELECTED, hWnd);
			}
		}
		break;
	}

	case WM_COMMAND:
	{
		WID id = LOWORD(wParam);
		//WORD code = HIWORD(wParam); //just one kind notify eventWord代码= HIWORD（wParam参数）; / /只是一种通知事件

		RECT clip;
		
		if(id == SMP_CALENDAR_SPIN_YEAR)
			pData->year = (Sint32)lParam;
		else
			pData->month = (Sint32)lParam;

		if(pData->day > DaysOfMonth(pData->year, pData->month))
			pData->day = DaysOfMonth(pData->year, pData->month);

		clip.left = 0; clip.top = 2*SMP_ITEM_HEIGHT; clip.width = _WIDTH(hWnd); clip.height = _HEIGHT(hWnd) - clip.top;
		SGL_UpdateWindowEx(hWnd, &clip);
		break;
	}
	
	}
	
	return 0;
}


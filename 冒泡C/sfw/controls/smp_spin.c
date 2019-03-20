#include "smp_spin.h"
#include "string.h"
#include "bmp.h"

//left&right bitmap width
#define SMP_SPIN_BMP_WIDTH		16

//spin box data
typedef struct SMP_SpinData
{
	int min; //min value of the spin box
	int max; //max value of the spin box
	int cur; //current value of the spin box
	int step; //one step to inc/dec the value
}SPINDATA, *PSPINDATA;

/////////////////////////////////////////////////////////////////////////////////////


VOID SMP_Spin_SetRange(HWND hWnd, Sint32 min, Sint32 max, Sint32 step)
{
	PSPINDATA pData = _GET_WINDATA(hWnd, PSPINDATA);

	pData->min = min;
	pData->max = max;
	pData->step = step;
	pData->cur = min;
}


VOID SMP_Spin_SetValue(HWND hWnd, Sint32 value, BOOL notify, BOOL update)
{
	PSPINDATA pData = _GET_WINDATA(hWnd, PSPINDATA);

	if(value == pData->cur || value < pData->min || value > pData->max)
		return;

	pData->cur = value;

	if(update)
		SGL_UpdateWindow(hWnd);
	
	if(notify)
		SGL_NotifyParent(hWnd, SMP_SPINN_VALUECHANGED, value);
}


Sint32 SMP_Spin_GetValue(HWND hWnd)
{
	PSPINDATA pData = _GET_WINDATA(hWnd, PSPINDATA);
	return pData->cur;
}


LRESULT SMP_Spin_WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	PSPINDATA pData = _GET_WINDATA(hWnd, PSPINDATA);

	switch(Msg)
	{

	case WM_CREATE:
	{
		pData = (PSPINDATA)SGL_MALLOC(sizeof(SPINDATA));
		if(!pData){
			SGL_TRACE("%s, %d: memory out\n", __FILE__, __LINE__);
			return 1;
		}

		SGL_MEMSET(pData, 0, sizeof(SPINDATA));
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
		_CLR_STYLE(hWnd, WS_PRESSED | SMP_SPINS_LEFTARROW | SMP_SPINS_RIGHTARROW);
		break;
	}
	
	case WM_PAINT:
	{
		int x = 0, y = 0, w, h;
		int32 wt, ht;
		HBITMAP hBmp;
		CHAR utf8[12];
		mr_screenRectSt rect;
		mr_colourSt color = {0, 0, 0};
		HFONT font = SGL_GetSystemFont();
		
		SGL_WindowToScreen(hWnd, &x, &y);
		if(_IS_SET_ANY(hWnd, WS_BORDER))
			GAL_Rectangle(PHYSICALGC, x, y, _WIDTH(hWnd), _HEIGHT(hWnd), _FGCOLOR(hWnd));

		hBmp = SGL_LoadBitmap(BMP_ARRLEFT, &w, &h);
		mrc_bitmapShowEx((uint16 *)hBmp, (int16)(x+2), (int16)(y + DIV(_HEIGHT(hWnd) - h, 2)), (int16)w, (int16)w, (int16)h, BM_COPY, 0, 0);

		hBmp = SGL_LoadBitmap(BMP_ARRRIGHT, &w, &h);
		mrc_bitmapShowEx((uint16 *)hBmp, (int16)(x + _WIDTH(hWnd) - w -2), (int16)(y + DIV(_HEIGHT(hWnd) - h, 2)), (int16)w, (int16)w, (int16)h, BM_COPY, 0, 0);

		SGL_SPRINTF(utf8, "%d", pData->cur);
		mrc_textWidthHeight((PSTR)utf8, FALSE, (uint16)font, &wt, &ht);
		rect.x =(uint16)(x + DIV(_WIDTH(hWnd) - wt, 2)); rect.y = (int16)(y); rect.w = (uint16)(_WIDTH(hWnd) - 2*(w+4)); rect.h = (uint16)_HEIGHT(hWnd); 
		mrc_drawTextEx((PSTR)utf8, (int16)rect.x, (int16)(rect.y + DIV(_HEIGHT(hWnd)-ht, 2)), rect, color, 0, (uint16)font);
		break;
	}

	case WM_KEYDOWN:
	case WM_KEYDOWNREPEAT:
	{
		switch(wParam)
		{
		case MR_KEY_LEFT:
			SMP_Spin_SetValue(hWnd, pData->cur - pData->step, TRUE, TRUE);
			return 1;
		case MR_KEY_RIGHT:
			SMP_Spin_SetValue(hWnd, pData->cur + pData->step, TRUE, TRUE);
			return 1;
		}
		break;
	}

	case WM_MOUSEDOWN:
	{
		int w, h, x = 0, y = 0;
		HBITMAP hBmp;
		
		if((int)wParam < 2 + SMP_SPIN_BMP_WIDTH && pData->cur - pData->step >= pData->min)
		{
			_SET_STYLE(hWnd, WS_PRESSED | SMP_SPINS_LEFTARROW);
			hBmp = SGL_LoadBitmap(BMP_ARRLEFT_PRESS, &w, &h);
			SGL_WindowToScreen(hWnd, &x, &y);
			mrc_bitmapShowEx((uint16 *)hBmp, (int16)(x+2), (int16)(y + DIV(_HEIGHT(hWnd) - h, 2)), (int16)w, (int16)w, (int16)h, BM_COPY, 0, 0);
			GAL_FlushRegion(PHYSICALGC, x+2, y, w, _HEIGHT(hWnd));
		}else if((int)wParam > _WIDTH(hWnd) - SMP_SPIN_BMP_WIDTH -2 && pData->cur + pData->step <= pData->max){
			_SET_STYLE(hWnd, WS_PRESSED | SMP_SPINS_RIGHTARROW);
			hBmp = SGL_LoadBitmap(BMP_ARRRIGHT_PRESS, &w, &h);
			SGL_WindowToScreen(hWnd, &x, &y);
			mrc_bitmapShowEx((uint16 *)hBmp, (int16)(x + _WIDTH(hWnd) - w -2), (int16)(y + DIV(_HEIGHT(hWnd) - h, 2)), (int16)w, (int16)w, (int16)h, BM_COPY, 0, 0);
			GAL_FlushRegion(PHYSICALGC, x + _WIDTH(hWnd) - w -2, y, w, _HEIGHT(hWnd));
		}
		
		if(_IS_SET_ANY(hWnd, WS_PRESSED))
			SGL_SetMouseTracker(hWnd);
		break;
	}
	
	case WM_MOUSEUP:
	{
		if(!_IS_SET_ANY(hWnd, WS_PRESSED))
			return 0;
		
		if( _IS_SET_ANY(hWnd, SMP_SPINS_LEFTARROW))
		{
			_CLR_STYLE(hWnd, WS_PRESSED | SMP_SPINS_LEFTARROW);
			SMP_Spin_SetValue(hWnd, pData->cur - pData->step, TRUE, TRUE);
		}else if(_IS_SET_ANY(hWnd, SMP_SPINS_RIGHTARROW)){
			_CLR_STYLE(hWnd, WS_PRESSED | SMP_SPINS_RIGHTARROW);
			SMP_Spin_SetValue(hWnd, pData->cur + pData->step, TRUE, TRUE);
		}
		break;
	}
	
	}

	return 0;
}



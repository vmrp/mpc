#include "smp.h"
#include "smp_button.h"
#include "bmp.h"

typedef struct SMP_ButtonData
{
	PCWSTR title;	//按钮的标题字符串
	DWORD bmp;		//正常状态的位图背景
	DWORD bmp_p;	//按下状态的位图背景
}BTNDATA, *PBTNDATA;

////////////////////////////////////////////////////////////////////////////////////////
VOID SMP_Button_SetTitle(HWND hBtn, PCWSTR title)
{
	PBTNDATA pData = _GET_WINDATA(hBtn, PBTNDATA);
	pData->title = title;
}

PCWSTR SMP_Button_GetTitle(HWND hBtn)
{
	PBTNDATA pData = _GET_WINDATA(hBtn, PBTNDATA);
	return pData->title;
}

VOID SMP_Button_SetBitmapGroup(HWND hBtn, DWORD bmp, DWORD bmp_p)
{
	PBTNDATA pData = _GET_WINDATA(hBtn, PBTNDATA);

	pData->bmp = bmp;
	pData->bmp_p = bmp_p;
}

VOID SMP_Button_SetChecked(HWND hBtn, BOOL checked)
{
	if(checked)
		_SET_STYLE(hBtn, SMP_BUTTONS_CHECKED);
	else
		_CLR_STYLE(hBtn, SMP_BUTTONS_CHECKED);
}

LRESULT SMP_Button_WndProc(HWND hBtn, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	PBTNDATA pData = _GET_WINDATA(hBtn, PBTNDATA);

	switch(Msg)
	{

	case WM_CREATE:
	{
		pData = (PBTNDATA)SGL_MALLOC(sizeof(BTNDATA));
		if(NULL == pData){
		//	SGL_TRACE("%s, %d: memory out\n", __FILE__, __LINE__);
			return 1;
		}

		pData->bmp = pData->bmp_p = RESID_INVALID;
		_SET_WINDATA(hBtn, pData);
		break;
	}
	
	case WM_DESTROY:
	{
		if(pData) SGL_FREE(pData);
		break;
	}
	
	case WM_SHOW:
	{
		_CLR_STYLE(hBtn, WS_PRESSED);
		break;
	}
	
	case WM_PAINT:
	{
		Uint32 fgcolor = _FGCOLOR(hBtn);
		int x = _LEFT(hBtn);
		int y = _TOP(hBtn);
		mr_screenRectSt rect;
		mr_colourSt color;
		HBITMAP hBmp;
		int w, h;
		rect.y = (Uint16)y;

		SGL_WindowToScreen(_PARENT(hBtn), &x, &y);
		
		if(_IS_SET_ANY(hBtn, WS_PRESSED))
			fgcolor = COLOR_focus;

		hBmp = SGL_LoadBitmap(_IS_SET_ANY(hBtn, WS_PRESSED)? pData->bmp_p : pData->bmp, &w, &h);
		if(hBmp) mrc_bitmapShowEx((uint16*)hBmp, (int16)x, (int16)y, (int16)w, (int16)w, (int16)h, BM_COPY, 0, 0);			

#if 0
		//绘制边界

		if((_STYLE(hBtn) & (WS_BORDER | WS_TRANSPARENT)) == WS_BORDER)
			GAL_Rectangle(PHYSICALGC, x, y, _WIDTH(hBtn), _HEIGHT(hBtn), COLOR_border);
#endif

		//绘制状态

		if(_IS_SET_ANY(hBtn, SMP_BUTTONS_CHECKBOX | SMP_BUTTONS_RADIOBOX))
			SMP_DrawCheckBox(x + SMP_ITEM_CONTENT_MARGIN, y + DIV(_HEIGHT(hBtn) - SMP_CHECKBOX_SIZE, 2), _IS_SET_ANY(hBtn, SMP_BUTTONS_CHECKED));

		//绘制图标 标题
		if(SMP_Button_GetTitle(hBtn))
		{
			int32 w, width, height;
			HFONT font = SGL_GetSystemFont();
			
			w = _WIDTH(hBtn);
			if(_IS_SET_ANY(hBtn, SMP_BUTTONS_CHECKBOX | SMP_BUTTONS_RADIOBOX))
			{
				x += 2*SMP_ITEM_CONTENT_MARGIN + SMP_CHECKBOX_SIZE;
				w -= 2*SMP_ITEM_CONTENT_MARGIN + SMP_CHECKBOX_SIZE;
			}

			if(_IS_SET_ANY(hBtn, WS_FOCUSED))
				GAL_Rectangle(x, y, w, _HEIGHT(hBtn), COLOR_focus);
			
            if(enable_smallfont)
                skyfontWidthHeight((PSTR)SMP_Button_GetTitle(hBtn),&width, &height);
            else
                mrc_textWidthHeight((PSTR)SMP_Button_GetTitle(hBtn), TRUE, (Uint16)font, &width, &height);
			if(_IS_SET_ANY(hBtn, SMP_BUTTONS_VCENTER))
				y += DIV(_HEIGHT(hBtn) - height, 2);
			else if(_IS_SET_ANY(hBtn, SMP_BUTTONS_VBOTTOM))
				y = y + _HEIGHT(hBtn) - height - 1;

			if(width > w - 2*SMP_ITEM_CONTENT_MARGIN)
			{
				rect.x = (Uint16)(x + SMP_ITEM_CONTENT_MARGIN), rect.w = (Uint16)(w - 2*SMP_ITEM_CONTENT_MARGIN), rect.h = (Uint16)_HEIGHT(hBtn);
				color.r = PIXEL888RED(fgcolor), color.g = PIXEL888GREEN(fgcolor), color.b = PIXEL888BLUE(fgcolor);
				mrc_drawTextEx((PSTR)SMP_Button_GetTitle(hBtn), (int16)rect.x, (int16)y, rect, color, 1, (Uint16)font);				
			}else{
				if(_IS_SET_ANY(hBtn, SMP_BUTTONS_HCENTER))
					x += DIV(w - width, 2);
				else if(_IS_SET_ANY(hBtn, SMP_BUTTONS_HRIGHT))
					x = x + w - width - SMP_ITEM_CONTENT_MARGIN;
				else
					x += SMP_ITEM_CONTENT_MARGIN;

				mrc_drawText((PSTR)SMP_Button_GetTitle(hBtn), (int16)x, (int16)y, 
					PIXEL888RED(fgcolor), PIXEL888GREEN(fgcolor), PIXEL888BLUE(fgcolor), TRUE, (Uint16)font);
			}
		}
		return 0;		
	}

	case WM_MOUSEDOWN:
	{
		_SET_STYLE(hBtn, WS_PRESSED);
		SGL_UpdateWindow(hBtn);
		SGL_SetMouseTracker(hBtn);
		return 0;
	}

	case WM_MOUSEUP:
	{
		if(!_IS_SET_ANY(hBtn, WS_PRESSED))
			return 0;
		_CLR_STYLE(hBtn, WS_PRESSED);
		if(_IS_SET_ANY(hBtn, SMP_BUTTONS_CHECKBOX) 
			|| (_IS_SET_ANY(hBtn, SMP_BUTTONS_RADIOBOX) && !_IS_SET_ANY(hBtn, SMP_BUTTONS_CHECKED)))
			_REV_STYLE(hBtn, SMP_BUTTONS_CHECKED);
		SGL_UpdateWindow(hBtn);
		SGL_NotifyParent(hBtn, SMP_BUTTONN_CLICKED, (DWORD)hBtn);
		return 0;
	}

	case WM_KEYDOWN:
	{
		_SET_STYLE(hBtn, WS_KEYDOWN);
		if(wParam == MR_KEY_SELECT && (_IS_SET_ANY(hBtn, SMP_BUTTONS_CHECKBOX) 
			|| (_IS_SET_ANY(hBtn, SMP_BUTTONS_RADIOBOX) && !_IS_SET_ANY(hBtn, SMP_BUTTONS_CHECKED))))
		{
			_REV_STYLE(hBtn, SMP_BUTTONS_CHECKED);
			SGL_UpdateWindow(hBtn);
			return 1;
		}
		return 0;
	}

	case WM_KEYUP:
	{
		if(!_IS_SET_ANY(hBtn, WS_KEYDOWN))
			return 0;
		_CLR_STYLE(hBtn, WS_KEYDOWN);		
		if(wParam == MR_KEY_SELECT)
		{
			SGL_NotifyParent(hBtn, SMP_BUTTONN_CLICKED, (DWORD)hBtn);
			return 1;
		}
		return 0;
	}

	case WM_KILLFOCUS:
	case WM_SETFOCUS:
		SGL_UpdateWindow(hBtn);
		return 0;

	}

	return 0;
}


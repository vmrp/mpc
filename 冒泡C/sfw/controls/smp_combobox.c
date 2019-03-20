#include "bmp.h"
#include "i18n.h"
#include "smp_combobox.h"
#include "smp_menu.h"
#include "smp.h"

//组合项目
typedef struct SMP_ComboItem
{
	DWORD data; // 用户数据项
	PCWSTR str; // 字符串项

}COMBOITEM, *PCOMBOITEM;

//组合数据
typedef struct SMP_ComboData
{
	PCOMBOITEM items;	// 数组项
	Sint32 size;		// 数组项的大小
	Sint32 hili;		// 数组项高光索引
}COMBODATA, *PCOMBODATA;

//////////////////////////////////////////////////////////////////////////

static VOID SMP_ComboBox_ShowContentMenu(HWND hWnd)
{
	int x = 0, y = 0;
	int32 i, w, h;
	HWND hTopWnd = NULL;
	PCOMBODATA pData = _GET_WINDATA(hWnd, PCOMBODATA);

	//清除菜单项
	SMP_Menu_ClearMenuItems();

	//设置菜单项
	for(i = 0; i < pData->size - 1; i++)
		SMP_Menu_SetMenuItem(i, (WID)i, pData->items[i].str, 0,  i+1);
	SMP_Menu_SetMenuItem(i, (WID)i, pData->items[i].str, 0,  -1);

	/*
	 * 为获得菜单的高度，在这里必须获得菜单项目的高度和其空间与边界的高度
	 * 但这样要重新设计菜单，所以...
	 */
	mrc_textWidthHeight("\x78\x6E\x5B\x9A\x00\x00", TRUE, (uint16)SGL_GetSystemFont(), &w, &h);
	h = (h + 2) * pData->size + 2; // 项目的空间是2，边界是1
	
	SGL_WindowToScreen(hWnd, &x, &y);
	hTopWnd = SGL_GetTopWindow(hWnd);
	
	if(y + _HEIGHT(hWnd) + h < SCREEN_HEIGHT - SMP_MENU_BOTTOM_HEIGHT || y < h)
	{
		SGL_ScreenToWindow(hTopWnd, &x, &y);
		SMP_Menu_PopupFlat(0, WS_NOREDRAW, hTopWnd, x, y + _HEIGHT(hWnd), _WIDTH(hWnd), hWnd);
	}else{
		SGL_ScreenToWindow(hTopWnd, &x, &y);
		SMP_Menu_PopupFlat(0, WS_NOREDRAW | SMP_MENUS_BOTTOMLEFT, hTopWnd, x, y, _WIDTH(hWnd), hWnd);
	}
}


BOOL SMP_ComboBox_SetSize(HWND hWnd, Sint32 size)
{
	PCOMBODATA pData = _GET_WINDATA(hWnd, PCOMBODATA);

	if(size <= 0) 
		return FALSE;

	if(pData->items && pData->size != size)
	{
		SGL_FREE(pData->items);
		pData->items = NULL;
	}

	if(!pData->items)
		pData->items = (PCOMBOITEM)SGL_MALLOC(size * sizeof(COMBOITEM));

	if(!pData->items)
		return FALSE;

	SGL_MEMSET(pData->items, 0, size * sizeof(COMBOITEM));
	pData->size = size;

	return TRUE;
}


Sint32 SMP_ComboBox_GetSize(HWND hWnd)
{
	PCOMBODATA pData = _GET_WINDATA(hWnd, PCOMBODATA);
	return pData->size;
}


VOID SMP_ComboBox_SetItem(HWND hWnd, Sint32 index, PCWSTR str, DWORD userdata)
{
	PCOMBODATA pData = _GET_WINDATA(hWnd, PCOMBODATA);
	pData->items[index].str = str;
	pData->items[index].data = userdata;
}


VOID SMP_ComboBox_GetItem(HWND hWnd, Sint32 index, PCWSTR * str, DWORD* userdata)
{
	PCOMBODATA pData = _GET_WINDATA(hWnd, PCOMBODATA);

	if(str) *str = pData->items[index].str;
	if(userdata) *userdata = pData->items[index].data;
}


BOOL SMP_ComboBox_SetItems(HWND hWnd, const DWORD* items, Sint32 size)
{
	int i;
	PCOMBODATA pData = _GET_WINDATA(hWnd, PCOMBODATA);

	if(!SMP_ComboBox_SetSize(hWnd, size))
		return FALSE;
	
	for(i = 0; i < size; i++)
	{
		pData->items[i].str = SGL_LoadString(items[i]);
		pData->items[i].data = 0;
	}

	return TRUE;
}


VOID SMP_ComboBox_SetSelectedItem(HWND hWnd, Sint32 index, BOOL redraw, BOOL notify)
{
	PCOMBODATA pData = _GET_WINDATA(hWnd, PCOMBODATA);

	if(index < 0 || index >= pData->size || index == pData->hili)
		return;

	pData->hili = index;

	if(redraw)
		SGL_UpdateWindow(hWnd);

	if(notify)
		SGL_NotifyParent(hWnd, SMP_COMBON_VALUECHANGED, index);
}


Sint32 SMP_ComboBox_GetSelectedItem(HWND hWnd)
{
	PCOMBODATA pData = _GET_WINDATA(hWnd, PCOMBODATA);

	return pData->hili;
}


LRESULT SMP_ComboBox_WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	PCOMBODATA pData = _GET_WINDATA(hWnd, PCOMBODATA);
	
	switch(Msg)
	{
	
	case WM_CREATE:
	{
		pData = (PCOMBODATA)SGL_MALLOC(sizeof(COMBODATA));
		if(NULL == pData){
			SGL_TRACE("%s, %d: memory out\n", __FILE__, __LINE__);
			return 1;
		}
		
		SGL_MEMSET(pData, 0, sizeof(COMBODATA));
		_SET_WINDATA(hWnd, pData);
		break;
	}

	case WM_DESTROY:
	{
		if(pData)
		{
			if(pData->items)
				SGL_FREE(pData->items);
			SGL_FREE(pData);
		}
		break;
	}
	
	case WM_SHOW:
	{
		_CLR_STYLE(hWnd, WS_PRESSED);
		break;
	}
	
	case WM_PAINT:
	{
		int x = 0, y = 0, w, h;
		HFONT font = SGL_GetSystemFont();
		HBITMAP hBmp = SGL_LoadBitmap(_IS_SET_ANY(hWnd, WS_PRESSED)? BMP_ARRDOWN_PRESS : BMP_ARRDOWN, &w, &h);
		SGL_WindowToScreen(hWnd, &x, &y);

		GAL_Rectangle(PHYSICALGC, x, y, _WIDTH(hWnd), _HEIGHT(hWnd), _IS_SET_ANY(hWnd, WS_FOCUSED)? COLOR_focus : COLOR_black);
		if(pData->hili >= 0 && pData->hili < pData->size)
		{
			int32 width, height;
			mr_screenRectSt rect;
			mr_colourSt color;

			rect.x = (int16)(x+SMP_ITEM_CONTENT_MARGIN);
			rect.y = (int16)(y);
			rect.w = _WIDTH(hWnd) - w - 2*SMP_ITEM_CONTENT_MARGIN;
			rect.h = _HEIGHT(hWnd);
			mrc_textWidthHeight((PSTR)pData->items[pData->hili].str, TRUE, (Uint16)font, &width, &height);

			color.r = PIXEL888RED(_FGCOLOR(hWnd));
			color.g = PIXEL888GREEN(_FGCOLOR(hWnd));
			color.b = PIXEL888BLUE(_FGCOLOR(hWnd));
			mrc_drawTextEx((PSTR)pData->items[pData->hili].str, rect.x, (int16)(rect.y + DIV(_HEIGHT(hWnd)-height, 2)), rect, color, TRUE, (uint16)font);
		}
		
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

		SMP_ComboBox_ShowContentMenu(hWnd);
		break;
	}

	case WM_KEYUP:
	{
		if(MR_KEY_SELECT == wParam)
		{
			SMP_ComboBox_ShowContentMenu(hWnd);
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
		WORD index = HIWORD(wParam); 

		if((int)index != pData->hili) 
			pData->hili = index;

		SGL_UpdateWindow(SGL_GetNextChild(HWND_DESKTOP, NULL));
		SGL_NotifyParent(hWnd, SMP_COMBON_VALUECHANGED, pData->hili);
		break;
	}
	
	}
	
	return 0;
}

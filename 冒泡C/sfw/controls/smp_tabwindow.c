#include "smp_tabwindow.h"
#include "bmp.h"

//标签栏左侧的图标，以宽的BMP /右箭头宽度
#define SMP_TABWND_ARRBAR_WIDTH		16

//一个标签的数据类型
typedef struct SMP_Tab {
	DWORD bmp;		//图标的ID
	PCWSTR title;	//选项卡的标题
	HWND hWnd;		//标签子窗口句柄
	int32 width;	//BMP和标题宽度
}TAB, *PTAB;

//选项卡窗口的数据类型

typedef struct SMP_TabWndData
{
	Sint32  hilight;	//高亮标签的索引
	Sint32 count;		//当前标签计数
	TAB tabs[SMP_TABWND_TABCOUNT];				//窗口的标签
	int32 titlewidth[SMP_TABWND_TABCOUNT];		//标题宽度
	int32 height;		//选项卡标题内容的高度
	Sint32 first;		//第一个选项卡可以显示
	BOOL hasArrBar;		//有箭头
}TABWNDDATA, *PTABWNDDATA;

//////////////////////////////////////////////////////////////////////////

VOID SMP_TabWindow_AddTab(HWND hWnd, DWORD bmp, PCWSTR title, HWND hTab)
{
	PTABWNDDATA pData = _GET_WINDATA(hWnd, PTABWNDDATA);

	if((title == NULL && bmp == RESID_INVALID) || !IS_NORMAL_WINDOW(hTab)
		|| pData->count >= SMP_TABWND_TABCOUNT)
		return;

	pData->tabs[pData->count].bmp = bmp;
	pData->tabs[pData->count].title = title;
	pData->tabs[pData->count].hWnd = hTab;
	pData->tabs[pData->count].width = 0;
	if(bmp != RESID_INVALID)
	{
		int w;
		SGL_LoadBitmap(bmp, &w, NULL);
		pData->tabs[pData->count].width += w; 
	}
	
	if(title)
	{
		int32 w, h;
		mrc_textWidthHeight((PSTR)title, TRUE, (uint16)SGL_GetSystemFont(), &w, &h);
		pData->tabs[pData->count].width += w;
		if(h > pData->height) pData->height = h;
	}
	
	pData->tabs[pData->count].width += 2* SMP_ITEM_CONTENT_MARGIN;
	if(pData->count++ == 0)
		SMP_TabWindow_SetActiveTabByIndex(hWnd, 0);
}


VOID SMP_TabWindow_RemoveTab(HWND hWnd, WID id)
{
	int i;
	PTABWNDDATA pData = _GET_WINDATA(hWnd, PTABWNDDATA);

	for(i = 0; i < pData->count; i++)
	{
		if(_WID(pData->tabs[i].hWnd) == id)
		{
			SGL_DestroyWindow(pData->tabs[i].hWnd);
			SGL_MEMMOVE(pData->tabs + i, pData->tabs + i + 1, sizeof(TAB)*(pData->count - i -1));
			pData->count--;

			if(i == pData->hilight)
			{
				pData->hilight = pData->first = 0;
			}else{
				if( i < pData->hilight)
					pData->hilight --;
				if( i < pData->first)
					pData->first --;
			}
			break;
		}
	}
}


VOID SMP_TabWindow_RemoveAllTabs(HWND hWnd)
{
	int i;
	PTABWNDDATA pData = _GET_WINDATA(hWnd, PTABWNDDATA);

	for(i = 0; i < pData->count; i++)
		SGL_DestroyWindow(pData->tabs[i].hWnd);

	SGL_MEMSET(pData, 0, sizeof(TABWNDDATA));
}


HWND SMP_TabWindow_GetTabByIndex(HWND hWnd, Sint32 index)
{
	PTABWNDDATA pData = _GET_WINDATA(hWnd, PTABWNDDATA);

	if(index >= 0 && index < pData->count)
		return pData->tabs[index].hWnd;

	return NULL;
}


HWND SMP_TabWindow_GetTabByID(HWND hWnd, WID id)
{
	int i;
	PTABWNDDATA pData = _GET_WINDATA(hWnd, PTABWNDDATA);

	for(i = 0; i < pData->count; i++)
	{
		if(_WID(pData->tabs[i].hWnd) == id)
			return pData->tabs[i].hWnd;
	}

	return NULL;
}


VOID SMP_TabWindow_SetActiveTabByID(HWND hWnd, WID id)
{
	int i;
	PTABWNDDATA pData = _GET_WINDATA(hWnd, PTABWNDDATA);

	for(i = 0; i < pData->count; i++)
	{
		if(_WID(pData->tabs[i].hWnd) == id)
		{
			SMP_TabWindow_SetActiveTabByIndex(hWnd, id);
			break;
		}
	}
}


Sint32 SMP_TabWindow_GetActiveTabIndex(HWND hWnd)
{
	PTABWNDDATA pData = _GET_WINDATA(hWnd, PTABWNDDATA);

	if(_CHILD(hWnd))
		return pData->hilight;

	return -1;
}


static BOOL SMP_TabWindow_HasFoucsed(HWND hWnd)
{
	HWND hTopWnd, hFocus;

	if(!SGL_IsWindowVisible(hWnd))
		return FALSE;

	hTopWnd = SGL_GetTopWindow(hWnd);
	for(hFocus = _FOCUS(hTopWnd); hFocus && hFocus != hTopWnd; hFocus = _PARENT(hFocus))
	{
		if(hWnd == hFocus)
			return TRUE;
	}

	return FALSE;
}


VOID SMP_TabWindow_SetActiveTabByIndex(HWND hWnd, Sint32 index)
{
	BOOL focused = FALSE;
	HWND hFocus;
	PTABWNDDATA pData = _GET_WINDATA(hWnd, PTABWNDDATA);
	HWND hTab = pData->tabs[index].hWnd; 
	int i, w;

	if(_CHILD(hWnd) && _CHILD(hWnd) == hTab)
		return;

	//暂停绘图
	SGL_SuspendDrawing();

	//删除旧标签
	if(_CHILD(hWnd))
	{
		focused = SMP_TabWindow_HasFoucsed(hWnd);
		SGL_RemoveChildWindow(_CHILD(hWnd));
	}

	//设置新的标签的位置，并显示它
	_LEFT(hTab) = 1; _TOP(hTab) = SMP_TABWND_TABBAR_HEIGHT;
	_WIDTH(hTab) = _WIDTH(hWnd) - 2; _HEIGHT(hTab) = _HEIGHT(hWnd) - SMP_TABWND_TABBAR_HEIGHT - 1;
	SGL_AddChildWindow(hWnd, hTab);
	pData->hilight = index;

	//初始化焦点的窗口
	if(focused && (hFocus = SGL_FindFocusNext(_PREV(_CHILD(hWnd)), _PREV(_CHILD(hWnd)), TRUE)) != NULL)
		SGL_SetFocusWindow(SGL_GetTopWindow(hWnd), hFocus);

	//可能要调整第一个标签
	if(pData->hilight < pData->first)
	{
		pData->first = pData->hilight;
	}else{
		for(w=0, i = pData->hilight; i >= pData->first; i--)
		{
			if((w+=pData->tabs[i].width) > _WIDTH(hWnd))
			{
				pData->first = i + 1;
				break;
			}
		}
	}
	
	//更新选项卡窗口
	SGL_UnsuspendDrawing();
	SGL_UpdateWindow(hWnd);
	SGL_NotifyParent(hWnd, SMP_TABWNDN_TABSWITCHED, index);
}


LRESULT SMP_TabWindow_WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	int i;
	PTABWNDDATA pData = _GET_WINDATA(hWnd, PTABWNDDATA);

	switch(Msg)
	{
	
	case WM_CREATE:
	{
		pData = (PTABWNDDATA)SGL_MALLOC(sizeof(TABWNDDATA));
		if(NULL == pData){
			//SGL_TRACE("%s, %d: memory out\n", __FILE__, __LINE__);
			return 1;
		}
		
		SGL_MEMSET(pData, 0, sizeof(TABWNDDATA));
		_SET_STYLE(hWnd, WS_TRANSPARENT | WS_TABSTOP | WS_FOCUSCHILD);
		_SET_WINDATA(hWnd, pData);
		break;
	}
	
	case WM_DESTROY:
	{
		if(pData)
		{
			for(i = 0; i < pData->count; i++)
				SGL_DestroyWindow(pData->tabs[i].hWnd);
			SGL_FREE(pData);
		}
		break;
	}

	case WM_SHOW:
	{
		int i, w;
		_CLR_STYLE(hWnd, WS_PRESSED | SMP_TABWNDS_LEFTARROW | SMP_TABWNDS_RIGHTARROW);
		for(i = w = 0; i < pData->count; i++)
			w += pData->tabs[i++].width;
		pData->hasArrBar = w > _WIDTH(hWnd);			
		break;
	}
	
	case WM_PAINT:
	{
		int x = 0, y = 0, dx = 0;
		int32 w, h, wtab;
		HFONT font = SGL_GetSystemFont();
		mr_screenRectSt r;
		mr_colourSt color = {0,};
		HBITMAP hBmp;

		SGL_WindowToScreen(hWnd, &x, &y);
		GAL_FillBox(PHYSICALGC, x, y, _WIDTH(hWnd), SMP_TABWND_TABBAR_HEIGHT-1, _BGCOLOR(hWnd));
		GAL_Rectangle(PHYSICALGC, x, y + SMP_TABWND_TABBAR_HEIGHT-1, _WIDTH(hWnd), _HEIGHT(hWnd) - SMP_TABWND_TABBAR_HEIGHT + 1, _FGCOLOR(hWnd));
		for(i = pData->first; i < pData->count && dx < _WIDTH(hWnd) - SMP_TABWND_ARRBAR_WIDTH; dx += pData->tabs[i++].width)
		{
			wtab = dx + pData->tabs[i].width > _WIDTH(hWnd) ? _WIDTH(hWnd) - dx : pData->tabs[i].width;
			GAL_Rectangle(PHYSICALGC, x + dx, y, wtab - 1, SMP_TABWND_TABBAR_HEIGHT, _FGCOLOR(hWnd));
			if(i == pData->hilight)
				GAL_DrawHLine(PHYSICALGC, x+dx+1, y+SMP_TABWND_TABBAR_HEIGHT-1, wtab-3 , _BGCOLOR(hWnd));

			wtab -= SMP_ITEM_CONTENT_MARGIN + 2;
			if(pData->tabs[i].bmp != RESID_INVALID)
			{
				hBmp = SGL_LoadBitmap(pData->tabs[i].bmp, (int*)&w, (int*)&h);
				mrc_bitmapShowEx((uint16*)hBmp, (int16)(x + dx + SMP_ITEM_CONTENT_MARGIN), (int16)(y + DIV(SMP_TABWND_TABBAR_HEIGHT-h, 2)), (int16)w, (int16)MIN(w, wtab), (int16)h, BM_TRANSPARENT, 0, 0);
				wtab -= w;
			}
			
			if(pData->tabs[i].title && wtab > 0)
			{
				r.x = (uint16)(x + dx + SMP_ITEM_CONTENT_MARGIN); r.y =(uint16)(y + DIV(SMP_TABWND_TABBAR_HEIGHT-pData->height, 2)); r.w = (uint16)wtab; r.h = SMP_TABWND_TABBAR_HEIGHT;
				if(pData->tabs[i].bmp != RESID_INVALID) r.x += (uint16)w;
				mrc_drawTextEx((PSTR)pData->tabs[i].title, r.x, r.y, r, color, 1, (uint16)font);
			}
		}

		if(pData->hasArrBar)
		{
			pData->hasArrBar = TRUE;
			hBmp = SGL_LoadBitmap(BMP_SPIN, NULL, NULL);
			mrc_bitmapShowEx((uint16 *)hBmp, (int16)(x + _WIDTH(hWnd) - SMP_TABWND_ARRBAR_WIDTH), 	(int16)y, 
							SMP_TABWND_ARRBAR_WIDTH, SMP_TABWND_ARRBAR_WIDTH, SMP_TABWND_TABBAR_HEIGHT, BM_COPY, 0, 0);
		}
		
		break;
	}

	case WM_KEYUP:
	{
		switch(wParam)
		{
		case MR_KEY_RIGHT:
			if(pData->hilight < pData->count - 1)
			{
				SMP_TabWindow_SetActiveTabByIndex(hWnd, pData->hilight + 1);
				return 1;
			}
			break;
		case MR_KEY_LEFT:
			if(pData->hilight > 0)
			{
				SMP_TabWindow_SetActiveTabByIndex(hWnd, pData->hilight - 1);
				return 1;
			}
			break;
		}
		break;
	}

	case WM_MOUSEDOWN:
	{
		int y = 0, x = _WIDTH(hWnd) - SMP_TABWND_ARRBAR_WIDTH;

		//check if click on the arrow bar/检查如果点击栏上的箭头
		if(pData->hasArrBar
			&& (int)wParam >= x && (int)wParam < _WIDTH(hWnd)
			&& (int)lParam >= 0 && (int)lParam < SMP_TABWND_TABBAR_HEIGHT)
		{
			HBITMAP hBmp = SGL_LoadBitmap(BMP_SPIN_PRESS, NULL, NULL);
			if((int)wParam > x + DIV(SMP_TABWND_ARRBAR_WIDTH, 2))
			{
				_SET_STYLE(hWnd, WS_PRESSED | SMP_TABWNDS_RIGHTARROW);
				SGL_WindowToScreen(hWnd, &x, &y);
				mrc_bitmapShowEx((uint16 *)hBmp, (int16)(x + SMP_TABWND_ARRBAR_WIDTH/2), (int16)y, 
							SMP_TABWND_ARRBAR_WIDTH, SMP_TABWND_ARRBAR_WIDTH/2, SMP_TABWND_TABBAR_HEIGHT, BM_COPY, SMP_TABWND_ARRBAR_WIDTH/2, 0);
			}else{
				_SET_STYLE(hWnd, WS_PRESSED | SMP_TABWNDS_LEFTARROW);
				SGL_WindowToScreen(hWnd, &x, &y);
				mrc_bitmapShowEx((uint16 *)hBmp, (int16)x, (int16)y, 
							SMP_TABWND_ARRBAR_WIDTH, SMP_TABWND_ARRBAR_WIDTH/2, SMP_TABWND_TABBAR_HEIGHT, BM_COPY, 0, 0);
			}

			GAL_FlushRegion(PHYSICALGC, x, y, SMP_TABWND_ARRBAR_WIDTH, SMP_TABWND_TABBAR_HEIGHT);
			SGL_SetMouseTracker(hWnd);
		}
		break;
	}

	case WM_MOUSEUP:
	{
		RECT rclip = {0, 0, 0, SMP_TABWND_TABBAR_HEIGHT};		
		int i, x = _WIDTH(hWnd) - SMP_TABWND_ARRBAR_WIDTH;

		//check if click on the arrow bar	 /检查如果点击栏上的箭头
		if(pData->hasArrBar 
			&& (int)lParam >= 0 && (int)lParam < SMP_TABWND_TABBAR_HEIGHT
			&& (int)wParam >= x && (int)wParam < _WIDTH(hWnd))
		{
			if(_IS_SET_ANY(hWnd, WS_PRESSED))
			{
				if((int)wParam < x + SMP_TABWND_ARRBAR_WIDTH/2 && _IS_SET_ANY(hWnd, SMP_TABWNDS_LEFTARROW))
				{
					if(pData->first > 0) pData->first--;
				}else if((int)wParam >= x + SMP_TABWND_ARRBAR_WIDTH/2 && _IS_SET_ANY(hWnd, SMP_TABWNDS_RIGHTARROW)){
					int w = _WIDTH(hWnd);
					for(i = pData->first; i < pData->count; w -= pData->tabs[i++].width);
					if(pData->first < pData->count -1 && w < 0)
						pData->first ++;
				}

				_CLR_STYLE(hWnd, WS_PRESSED | SMP_TABWNDS_LEFTARROW | SMP_TABWNDS_RIGHTARROW);
				rclip.width = _WIDTH(hWnd);
				SGL_UpdateWindowEx(hWnd, &rclip);
			}
			break;			
		}

		//check the tab clicked /检查标签点击

		if((int)lParam >= 0 && (int)lParam < SMP_TABWND_TABBAR_HEIGHT)
		{
			for(i = pData->first; i < pData->count; wParam -= pData->tabs[i++].width)
			{	
				if((int)wParam > pData->tabs[i].width)
					continue;
				if(i != pData->hilight)
					SMP_TabWindow_SetActiveTabByIndex(hWnd, i);
				break;
			}
		}
		break;
	}

	}
	
	return 0;
}


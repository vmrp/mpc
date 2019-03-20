#include "smp_scrollview.h"
#include "smp_scrollbar.h"
#include "smp.h"


//子窗口ID
enum{
	SMP_SCROLLVIEW_SCROLLBAR = 1,
	SMP_SCROLLVIEW_CONTENTVIEW
};

//////////////////////////////////////////////////////////////////////////////////////

HWND SMP_ScrollView_GetContentView(HWND hWnd)
{
	return SGL_FindChildWindow(hWnd, SMP_SCROLLVIEW_CONTENTVIEW);
}


VOID SMP_ScrollView_UpdateContents(HWND hWnd)
{
	HWND hChild;
	HWND hContent = SGL_FindChildWindow(hWnd, SMP_SCROLLVIEW_CONTENTVIEW);
	int height = _HEIGHT(hWnd);

	_LEFT(hContent) = _TOP(hContent) = 0;

	if(_CHILD(hContent))
	{
		hChild = _CHILD(hContent);
		do{
			if(_TOP(hChild) + _HEIGHT(hChild) > height) 
				height = _TOP(hChild) + _HEIGHT(hChild);
		}while((hChild = _NEXT(hChild)) != _CHILD(hContent));
	}

	if(height > _HEIGHT(hWnd)) 
		height += SMP_ITEM_SPACE;
	_HEIGHT(hContent) = height;
	hChild = SGL_FindChildWindow(hWnd, SMP_SCROLLVIEW_SCROLLBAR);
	SMP_Scrollbar_SetInfo(hChild, 0, height-1, _HEIGHT(hWnd));
	SMP_Scrollbar_SetValue(hChild, 0, FALSE);
}


LRESULT SMP_ScrollView_WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch(Msg)
	{

	case WM_CREATE:
	{
		HWND hControl = SGL_CreateWindow(SGL_Window_WndProc,
			0, 0, _WIDTH(hWnd) - SMP_SCRBAR_WIDTH, _HEIGHT(hWnd),
			SMP_SCROLLVIEW_CONTENTVIEW, WS_TABSTOP | WS_FOCUSCHILD, 0);
		SGL_AddChildWindow(hWnd, hControl);

		hControl = SGL_CreateWindow(SMP_Scrollbar_WndProc,
			_WIDTH(hWnd) - SMP_SCRBAR_WIDTH, 0, SMP_SCRBAR_WIDTH, _HEIGHT(hWnd),
			SMP_SCROLLVIEW_SCROLLBAR, 0, 0);
		SMP_Scrollbar_SetStep(hControl, SMP_ITEM_HEIGHT);
		SGL_AddChildWindow(hWnd, hControl);

		_SET_STYLE(hWnd, WS_TRANSPARENT | WS_TABSTOP | WS_FOCUSCHILD);
		break;
	}

	case WM_SHOW:
	{
		SMP_ScrollView_UpdateContents(hWnd);
		break;
	}

	case WM_PAINT:
	{
		/*
        *在这里，你不能调用SGL_PaintWindowEx（hWnd，...),
        *这将导致发送WM_PAINT消息给hWnd
        */
		RECT r;
		HWND hControl;
		hControl = SGL_FindChildWindow(hWnd, SMP_SCROLLVIEW_SCROLLBAR);
		SGL_PaintWindow(hControl);
		
		hControl = SGL_FindChildWindow(hWnd, SMP_SCROLLVIEW_CONTENTVIEW);
		r.width = _WIDTH(hControl); r.height = _HEIGHT(hWnd); r.left = 0; r.top = -_TOP(hControl);
		SGL_PaintWindowEx(hControl, &r);
		return 1;
	}

	case WM_COMMAND:
	{
		WID id = LOWORD(wParam);
		WORD code = HIWORD(wParam);

		if(id == SMP_SCROLLVIEW_SCROLLBAR && code == SMP_SCRBARN_VALUECHANGED)
		{
			HWND hContent = SGL_FindChildWindow(hWnd, SMP_SCROLLVIEW_CONTENTVIEW);
			_TOP(hContent)  = -(int)lParam;
			SGL_UpdateWindow(hContent);
		}
		break;
	}	

	case WM_KEYDOWN:
	case WM_KEYDOWNREPEAT:
	{
		switch(wParam)
		{
		case MR_KEY_DOWN:
		case MR_KEY_UP:			
		{
			HWND hTopWnd = SGL_GetTopWindow(hWnd);
			HWND hScrBar = SGL_FindChildWindow(hWnd, SMP_SCROLLVIEW_SCROLLBAR);
			HWND hFocus = SGL_FindFocus4TopWnd(hTopWnd, wParam == MR_KEY_DOWN);
			int pagestart = SMP_Scrollbar_GetValue(hScrBar);
			
			if(_TOP(hFocus) >= pagestart 
				&& _TOP(hFocus) + _HEIGHT(hFocus) <= pagestart + _HEIGHT(hWnd))
			{
				SGL_SetFocusWindow(hTopWnd, hFocus);
			}else{
				HWND hContent = SGL_FindChildWindow(hWnd, SMP_SCROLLVIEW_CONTENTVIEW);
				SGL_SuspendDrawing();
				if(_TOP(hFocus) < pagestart)
					SMP_Scrollbar_SetValue(hScrBar, MAX(_TOP(hFocus) - SMP_ITEM_SPACE, 0), FALSE);
				else
					SMP_Scrollbar_SetValue(hScrBar, _TOP(hFocus) + _HEIGHT(hFocus) - _HEIGHT(hWnd) + SMP_ITEM_SPACE, FALSE);
				_TOP(hContent)  = -SMP_Scrollbar_GetValue(hScrBar);
				SGL_SetFocusWindow(hTopWnd, hFocus);
				SGL_UnsuspendDrawing();			
				SGL_UpdateWindow(hWnd);
			}
			return 1;
		}

		}
		break;
	}
	
	}
	
	return 0;
}



#include "i18n.h"
#include "smp.h"
#include "smp_toolbar.h"
#include "smp_msgbox.h"


#define MSGBOX_TOOLBAR_ID		1 //只在MsgBox


//局部变量
static HWND hMsgBox;
static int32 sAutoCloseTimer;

//////////////////////////////////////////////////////////////////////////////////////////

static VOID SMP_MsgBox_AutoCloseCallback(int32 data)
{
	HWND hDlg = (HWND)data;

	if(!SGL_IsWindowVisible(hDlg))
		return;

	if(_IS_SET_ANY(hDlg, ID_OK))
		SGL_ExitModal(hDlg, (WORD)ID_OK, (LPARAM)hDlg);	
	else if(_IS_SET_ANY(hDlg, ID_CANCEL))
		SGL_ExitModal(hDlg, (WORD)ID_CANCEL, (LPARAM)hDlg);
	else
		SGL_ExitModal(hDlg, (WORD)ID_NON, (LPARAM)hDlg);
}


static VOID SMP_MsgBox_StartAutoClose(HWND hDlg)
{
	if(!sAutoCloseTimer)
		sAutoCloseTimer = mrc_timerCreate();

	mrc_timerStart(sAutoCloseTimer, SMP_MSGBOX_AUTOCLOSE_INTERVAL, (int32)hDlg, SMP_MsgBox_AutoCloseCallback, FALSE);
}


static VOID SMP_MsgBox_StopAutoClose(HWND hDlg)
{
	if(sAutoCloseTimer)
		mrc_timerStop(sAutoCloseTimer);
}


HWND SMP_MsgBox(WID id, HWND hParent, HBITMAP hBmp, PCWSTR title, PCWSTR content, DWORD style, HWND listener)
{
	if(SGL_IsWindowVisible(hMsgBox))
		SGL_ExitModal(hMsgBox, ID_NON, (LPARAM)hMsgBox);
	
	if(!hMsgBox)
	{
		hMsgBox = SGL_CreateWindow(SMP_MsgBox_WndProc, 0, 1, 1, 1, id, style, (DWORD)title);
	}else{
		_WID(hMsgBox) = id;
		_STYLE(hMsgBox) = style;
		_SET_USERDATA(hMsgBox, title);		
	}

	_LISTENER(hMsgBox) = listener;
	_SET_WINDATA(hMsgBox, content);
	SGL_DoModal(hMsgBox, hParent);
	
	if(_IS_SET_ANY(hMsgBox, SMP_MSGBOXS_AUTOCLOSE) && !_IS_SET_ALL(hMsgBox, ID_OK | ID_CANCEL))
		SMP_MsgBox_StartAutoClose(hMsgBox);
	
	return hMsgBox;
}


LRESULT SMP_MsgBox_WndProc(HWND hDlg, UINT Msg, WPARAM wParam, LPARAM lParam)
{	
	switch(Msg) 
	{
	
	case WM_CREATE:
	{
		HWND hControl;
        
        hControl = SGL_CreateWindow(SMP_Toolbar_WndProc, 
			0, 1, 1, SMP_ITEM_HEIGHT, 
			MSGBOX_TOOLBAR_ID, 0, 0);
		if(!hControl)
			return 1;
		SGL_AddChildWindow(hDlg, hControl);
        _WIDTH(hControl) = _WIDTH(hDlg) = SCREEN_WIDTH;
        break;
    }
	case WM_SHOW:
	{
		int32 w, h, lines, i, lw, lh;//lh保存最大字符高度
		UCHAR tmp[4] = {0,};
		HFONT font = SGL_GetSystemFont();
		PCWSTR content = _GET_WINDATA(hDlg, PCWSTR);
		HWND hToolbar = _CHILD(hDlg);

		lines = lw = lh = 0;
		for(i = 0; content[i] || content[i+1]; i+=2)
		{
			tmp[0] = content[i];
			tmp[1] = content[i+1];
            if(enable_smallfont)
                skyfontWidthHeight((PSTR)tmp,&w, &h);
            else
			    mrc_textWidthHeight((PSTR)tmp, TRUE, (uint16)font, &w, &h);
			if(h > lh ) lh = h;
			if(lw + w > SCREEN_WIDTH - 2*SMP_ITEM_MARGIN || tmp[0] == '\n' || tmp[1] == '\n')
			{
				lines++;
				lw = w;
			}
            else lw += w;
		}

		lh += 2;
		lines++;
		_HEIGHT(hDlg) = 2 * SMP_ITEM_MARGIN + lines*lh + 2*SMP_ITEM_HEIGHT;
		_TOP(hDlg) = SCREEN_HEIGHT - _HEIGHT(hDlg);
		_TOP(hToolbar) = _HEIGHT(hDlg) - SMP_ITEM_HEIGHT;

		SMP_Toolbar_SetStrings(hToolbar,
			_IS_SET_ANY(hDlg, ID_OK)? (_IS_SET_ANY(hDlg, ID_YESNO)? STR_YES : STR_OK) : RESID_INVALID,
			RESID_INVALID,
			_IS_SET_ANY(hDlg, ID_CANCEL)? (_IS_SET_ANY(hDlg, ID_YESNO)? STR_NO : STR_CANCEL) : RESID_INVALID,
			FALSE);

		return 0;
	}

	case WM_HIDE:
	{
		if(_IS_SET_ANY(hMsgBox, SMP_MSGBOXS_AUTOCLOSE) && !_IS_SET_ALL(hMsgBox, ID_OK | ID_CANCEL))
			SMP_MsgBox_StopAutoClose(hMsgBox);
		break;
	}

	case WM_PAINT:
	{
		int32 w, h;
		int x=0, y=0;
		mr_screenRectSt rect;
		mr_colourSt color={0xff, 0xff, 0xff};
		HFONT font = SGL_GetSystemFont();
		PCWSTR title = _GET_USERDATA(hDlg, PCWSTR);
		PCWSTR content = _GET_WINDATA(hDlg, PCWSTR);

		SGL_WindowToScreen(hDlg, &x, &y);
		SMP_DrawMsgBoxFrame(x, y, _WIDTH(hDlg), _HEIGHT(hDlg) - SMP_ITEM_HEIGHT);
		rect.x = SMP_ITEM_MARGIN;
        rect.w = _WIDTH(hDlg)-2*SMP_ITEM_MARGIN;

		if(title)
        {
            if(enable_smallfont)
                skyfontWidthHeight((PSTR)title,&w, &h);
            else
                mrc_textWidthHeight((PSTR)title, TRUE, (uint16)font, &w, &h);
			rect.y = (uint16)(y + DIV(SMP_ITEM_HEIGHT-h, 2));
            rect.h = SMP_ITEM_HEIGHT;
            if(enable_smallfont)
                skyfontdraw((PSTR)title, rect.x, rect.y, rect, color, 0);
            else
                mrc_drawTextEx((PSTR)title, rect.x, rect.y, rect, color, 1, (uint16)font);
		}
		if(content)
        {
            memset(&color,0,sizeof(color));
            rect.y = y + SMP_ITEM_HEIGHT + SMP_ITEM_MARGIN;
            rect.h = _HEIGHT(hDlg) - 2 * SMP_ITEM_MARGIN - SMP_ITEM_HEIGHT;
            if(enable_smallfont)
                skyfontdraw((PSTR)content, rect.x, rect.y, rect, color, 7);
            else
                mrc_drawTextEx((PSTR)content, rect.x, rect.y, rect, color, 3, (uint16)font);
        }
        return 0;
	}

	case WM_KEYUP:
	{
		if(wParam == MR_KEY_SOFTRIGHT && _IS_SET_ANY(hDlg, ID_CANCEL))
		{
			SGL_ExitModal(hDlg, (WORD)ID_CANCEL, (LPARAM)hDlg);
			return 1;
		}else if((wParam == MR_KEY_SELECT || wParam == MR_KEY_SOFTLEFT) && _IS_SET_ANY(hDlg, ID_OK))
		{
			SGL_ExitModal(hDlg, (WORD)ID_OK, (LPARAM)hDlg);			
			return 1;
		}
		break;
	}

	}
	
	return 0;
}

VOID SMP_Close_MsgBox(HWND hMsgBox)
{
	SGL_ExitModal(hMsgBox, ID_NON, (LPARAM)hMsgBox);
}
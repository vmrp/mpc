#include "smp.h"
#include "i18n.h"
#include "string.h"
#include "smp_inputbox.h"
#include "smp_toolbar.h"
#include "smp_edit.h"

#define INPUTBOX_TOOLBAR_ID		1 //Ö»ÔÚInputbox
#define INPUTBOX_EDIT_ID        2 

static HWND hInputbox;

//////////////////////////////////////////////////////////////////////////////////////////

HWND SMP_InputBox(WID id, HWND hParent, PCWSTR title, PCWSTR content, Uint32 maxsize, DWORD style, HWND listener)
{
	SMP_PInputBoxData pData;

    if(!hInputbox)
	    hInputbox = SGL_CreateWindow(SMP_InputBox_WndProc, 1, 1, 1, 1, id, 0, 0);


    pData = _GET_WINDATA(hInputbox, SMP_PInputBoxData);
	_WID(hInputbox) = id;
	_LISTENER(hInputbox) = listener;
	_SET_STYLE(pData->hEdit, style);

    pData->title = title;
	wstrcpy((PWSTR)pData->buffer,content);
    pData->size = maxsize;

	SGL_DoModal(hInputbox, hParent);
	return hInputbox;
}


LRESULT SMP_InputBox_WndProc(HWND hDlg, UINT Msg, WPARAM wParam, LPARAM lParam)
{	
	SMP_PInputBoxData pData = _GET_WINDATA(hDlg, SMP_PInputBoxData);

	switch(Msg) 
	{
	
	case WM_CREATE:
	{	
        HWND hToolbar;

		pData = (SMP_PInputBoxData)SGL_MALLOC(sizeof(SMP_InputBoxData));
		if(!pData){
		//	SGL_TRACE("%s, %d: memory out\n", __FILE__, __LINE__);
			return 1;
		}
		SGL_MEMSET(pData, 0, sizeof(SMP_InputBoxData));
		_SET_WINDATA(hDlg, pData);

        hToolbar = SGL_CreateWindow(SMP_Toolbar_WndProc, 
			1, 1, 1, 1, 
			INPUTBOX_TOOLBAR_ID, 0, 0);
		SMP_Toolbar_SetStrings(hToolbar, STR_OK, RESID_INVALID, STR_CANCEL, FALSE);
		SGL_AddChildWindow(hDlg, hToolbar);

        pData->hEdit = SGL_CreateWindow(SMP_Edit_WndProc, 
			1, 1, 1, 1, 
			INPUTBOX_EDIT_ID, WS_TABSTOP, 0);
		SGL_AddChildWindow(hDlg, pData->hEdit);

        //¼ÆËãÆÁÄ»
		_LEFT(hToolbar) = _LEFT(hDlg) = 0;
        _LEFT(pData->hEdit) = SMP_ITEM_MARGIN;

		_HEIGHT(hDlg) = 2 * SMP_ITEM_MARGIN + 3*SMP_ITEM_HEIGHT;
		_HEIGHT(pData->hEdit) = _HEIGHT(hToolbar) = SMP_ITEM_HEIGHT;
		_TOP(hDlg) = SCREEN_HEIGHT - _HEIGHT(hDlg);
		_TOP(hToolbar) = _HEIGHT(hDlg) - SMP_ITEM_HEIGHT;
        _TOP(pData->hEdit) = SMP_ITEM_MARGIN + SMP_ITEM_HEIGHT;
        _WIDTH(hToolbar) = _WIDTH(hDlg) = SCREEN_WIDTH;
        _WIDTH(pData->hEdit) = SCREEN_WIDTH - 2*SMP_ITEM_MARGIN;

		return 0;
	}
	case WM_DESTROY:
	{
		if(pData)
            SGL_FREE(pData);
        hInputbox = NULL;
		return 0;
	}
	case WM_SHOW:
	{
        SMP_Edit_SetInfo(pData->hEdit, pData->title, (PWSTR)pData->buffer, pData->size);
		return 0;
	}
    case WM_INITFOCUS:
    {
        SGL_SetFocusWindow(hDlg, pData->hEdit);
		break;
    }
	case WM_HIDE:
	{
		break;
	}

	case WM_PAINT:
	{
		int32 w, h;
		int x=0, y=0;
		mr_screenRectSt rect;
		mr_colourSt color={0xff, 0xff, 0xff};
		HFONT font = SGL_GetSystemFont();
		PCWSTR title = pData->title;

		SGL_WindowToScreen(hDlg, &x, &y);
		SMP_DrawMsgBoxFrame(x, y, _WIDTH(hDlg), _HEIGHT(hDlg) - SMP_ITEM_HEIGHT);
		rect.x = SMP_ITEM_MARGIN; 
        rect.w = _WIDTH(hDlg) - 2*SMP_ITEM_MARGIN;

		if(title)
		{
            if(enable_smallfont)
                skyfontWidthHeight((PSTR)title,&w, &h);
            else
			    mrc_textWidthHeight((PSTR)title, TRUE, (uint16)font, &w, &h);
			rect.y = (uint16)(y + DIV(SMP_ITEM_HEIGHT-h, 2));
            rect.h = SMP_ITEM_HEIGHT;
            if(enable_smallfont)
                skyfontdraw((PSTR)title, rect.x, rect.y,rect, color,0);
            else
			    mrc_drawTextEx((PSTR)title, rect.x, rect.y, rect, color, 1, (uint16)font);
		}
		return 0;
	}

	case WM_KEYUP:
	{
		if(wParam == MR_KEY_SOFTRIGHT)
		{
			SGL_ExitModal(hDlg, INPUT_CANCEL, 0);
			return 1;
		}else if((wParam == MR_KEY_SOFTLEFT))
		{
			SGL_ExitModal(hDlg, INPUT_OK, (LPARAM)pData->buffer);			
			return 1;
		}
		break;
	}

	}
	
	return 0;
}


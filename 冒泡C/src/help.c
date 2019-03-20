#include "help.h"
#include "mainwnd.h"

#include "i18n.h"
#include "smp.h"
#include "smp_toolbar.h"
#include "smp_textinfo.h"
#include "smp_menuwnd.h"
#include "smp_edit.h"
#include "smp_button.h"
#include "smp_msgbox.h"
#include "bmp.h"
#include "smp_label.h"
#include "smp_progbar.h"
#include "smp_combobox.h"
#include "smp_list.h"
#include "topwnd.h"
#include "string.h"
#include "smp_titlebar.h"

//子窗口ID
enum
{
    HELP_TITLEBAR = 1,
	HELP_TOOLBAR,
    HELP_TEXTINFO
};

/////////////////////////////////////////////////////////

VOID ShowHELP(WID wndid)
{
	HideTopWindow(wndid, FALSE, FALSE);
	ShowTopWindow(TOPWND_HELP, NULL, (DWORD)wndid);
}

static VOID HideHELP(WID wndid)
{
	HideTopWindow(TOPWND_HELP, TRUE, FALSE);
	ShowTopWindow(wndid, NULL, 0);
}


LRESULT HELP_WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	
	switch(Msg)
	{
	case WM_CREATE:
	{
	    HWND hControl;

        //标题栏
		hControl = SGL_CreateWindow(SMP_Titlebar_WndProc,
			0, 0, SCREEN_WIDTH, SMP_ITEM_HEIGHT,
			HELP_TITLEBAR, 
			SMP_TITLEBAR_STATIC, 0);
		_FGCOLOR(hControl) = COLOR_lightwhite;
		SMP_Titlebar_SetContent(hControl, RESID_INVALID, SGL_LoadString(STR_HELP),FALSE);
		SGL_AddChildWindow(hWnd, hControl);

		//创建工具栏
		hControl = SGL_CreateWindow(SMP_Toolbar_WndProc, 
			0,  _HEIGHT(hWnd) - SMP_ITEM_HEIGHT, 
			_WIDTH(hWnd), SMP_ITEM_HEIGHT,
			HELP_TOOLBAR,
			0, 0);
		SMP_Toolbar_SetStrings(hControl,  RESID_INVALID, RESID_INVALID, STR_BACK, FALSE);
		SGL_AddChildWindow(hWnd, hControl);

        //文本框
        hControl = SGL_CreateWindow(SMP_TextInfo_WndProc,
            0,SMP_ITEM_HEIGHT,SCREEN_WIDTH,SCREEN_HEIGHT - SMP_ITEM_HEIGHT*2,
            HELP_TEXTINFO,0,0);
        SMP_TextInfo_SetContent(hControl,SGL_LoadString(STR_HELPCONTENT));
		SGL_AddChildWindow(hWnd, hControl);
		break;
	}
    case WM_DESTROY:
    {
		break;
    }
	case WM_INITFOCUS:
	{
        HWND hText;

        hText = SGL_FindChildWindow(hWnd,HELP_TEXTINFO);
        SGL_SetFocusWindow(hWnd,hText);
		break;
	}
    case WM_SHOW:
    {   
        break;
    }
	case WM_PAINT:
	{   
		break;
	}
	case WM_KEYDOWN:
	case WM_KEYDOWNREPEAT:
	{
		break;
	}	
	case WM_KEYUP:
	{
		switch(wParam)
		{
		case MR_KEY_SOFTRIGHT:
            HideHELP((WID)_USERDATA(hWnd));
			return 1;
		}
		break;
	}
	case WM_COMMAND:
	{
		break;
	}

	}
	return 0;
}


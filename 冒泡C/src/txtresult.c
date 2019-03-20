
#include "txtresult.h"
#include "mainwnd.h"
#include "i18n.h"
#include "smp.h"
#include "smp_menu.h"
#include "smp_toolbar.h"
#include "smp_textinfo.h"
#include "smp_menuwnd.h"
#include "smp_edit.h"
#include "smp_button.h"
#include "smp_msgbox.h"
#include "bmp.h"
#include "smp_label.h"
#include "plt_msgbox.h"
#include "smp_progbar.h"
#include "smp_combobox.h"
#include "smp_list.h"
#include "topwnd.h"
#include "string.h"
#include "smp_titlebar.h"
#include "littlec.h"
#include "opendlg.h"

//子窗口ID
enum
{
	TXTRESULT_EDIT = 1,
    TXTRESULT_TITLEBAR,
	TXTRESULT_TOOLBAR,
    TXTRESULT_TEXTINFO
};

/////////////////////////////////////////////////////////

VOID ShowTxtResult(WID wndid)
{        
    StartLittleC(progmem);
    littleCStop();
	HideTopWindow(wndid, FALSE, FALSE);
	ShowTopWindow(TOPWND_TXTRESULT, NULL, (DWORD)wndid);
}

static VOID HideTxtResult(WID wndid)
{
	HideTopWindow(TOPWND_TXTRESULT, FALSE, FALSE);
	ShowTopWindow(wndid, NULL, 0);
}

LRESULT TXTRESULT_WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    PWSTR UINCODETXT = _GET_WINDATA(hWnd,PWSTR);
	
	switch(Msg)
	{
	case WM_CREATE:
	{
	    HWND hControl;

        UINCODETXT = (PWSTR)SGL_MALLOC(UNITEXTLEN);
        if(!UINCODETXT){
		//	SGL_TRACE("%s, %d: memory out\n", __FILE__, __LINE__);
			return 1;
		}
        SGL_MEMSET(UINCODETXT,0,UNITEXTLEN);
        _SET_WINDATA(hWnd, UINCODETXT);
        //标题栏
		hControl = SGL_CreateWindow(SMP_Titlebar_WndProc,
			0, 0, SCREEN_WIDTH, SMP_ITEM_HEIGHT,
			TXTRESULT_TITLEBAR, 
			SMP_TITLEBAR_STATIC, 0);
		_FGCOLOR(hControl) =  COLOR_lightwhite ;/* 标题文字颜色 */
		SMP_Titlebar_SetContent(hControl, RESID_INVALID, SGL_LoadString(STR_RESULT));
		SGL_AddChildWindow(hWnd, hControl);

		//创建工具栏
		hControl = SGL_CreateWindow(SMP_Toolbar_WndProc, 
			0,  _HEIGHT(hWnd) - SMP_ITEM_HEIGHT, 
			_WIDTH(hWnd), SMP_ITEM_HEIGHT,
			TXTRESULT_TOOLBAR,
			0, 0);
		SMP_Toolbar_SetStrings(hControl,  STR_SAVE, RESID_INVALID, STR_BACK, FALSE);
		SGL_AddChildWindow(hWnd, hControl);

        //文本框
        hControl = SGL_CreateWindow(SMP_TextInfo_WndProc,
            0,SMP_ITEM_HEIGHT,SCREEN_WIDTH,SCREEN_HEIGHT - SMP_ITEM_HEIGHT*2,
            TXTRESULT_TEXTINFO,0,0);
        //_BGCOLOR(hControl)=0xC0C0C0;
        SMP_TextInfo_SetContent(hControl,UINCODETXT);
		SGL_AddChildWindow(hWnd, hControl);

		break;
	}
    case WM_DESTROY:
    {
    	if(UINCODETXT) SGL_FREE(UINCODETXT);
		break;
    }
	case WM_INITFOCUS:
	{
        HWND hText = SGL_FindChildWindow(hWnd,TXTRESULT_TEXTINFO);
        SGL_SetFocusWindow(hWnd,hText);
		break;
	}
    case WM_SHOW:
    {   
        char *buf;
        UnicodeSt unicode;
        HWND hText = SGL_FindChildWindow(hWnd,TXTRESULT_TEXTINFO);

        buf = GetResult();
        SGL_MEMSET(UINCODETXT,0,UNITEXTLEN);
        CharToUnicode(buf, &unicode.pText, &unicode.len);
        wstrcpy(UINCODETXT, (PCWSTR)unicode.pText);
		mrc_freeOrigin(unicode.pText, unicode.len);
        SMP_TextInfo_SetContent(hText,UINCODETXT);
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
		case MR_KEY_SOFTLEFT://保存
            SGL_TextInput(hWnd);
			return 1;
		case MR_KEY_SOFTRIGHT:
            HideTxtResult((WID)_USERDATA(hWnd));
			return 1;
		}
		break;
	}
    case WM_GETEDITINFO:
    {
		SGL_EditInfo* info = (SGL_EditInfo*)wParam;
		info->title = SGL_LoadString(STR_INPUTNAME);
		info->buffer = (PWSTR)"\x00\x00";
		info->size = 256;
        break;
    }
    case WM_SETEDITTEXT:
	{
		PCWSTR pContent = (PCWSTR)wParam;
        PSTR code=NULL;
        int32 f = 0;

        code = UnicodeToChar(pContent);

        mrc_remove(code);
        f = mrc_open(code,4|8);
        SGL_FREE(code);
        if(f)
        {
            char *buf;
            buf = GetResult();
            mrc_write(f,buf,mrc_strlen(buf));
            mrc_close(f);
			SMP_MsgBox(0,hWnd, NULL,SGL_LoadString(STR_MSG),SGL_LoadString(STR_SAVESUC), ID_OK,NULL);
        }
        else
            SMP_MsgBox(0,hWnd, NULL,SGL_LoadString(STR_MSG),SGL_LoadString(STR_SAVEFAI), ID_OK,NULL);

		break;
	}

	}
	return 0;
}


#include "phiresult.h"
#include "mainwnd.h"
#include "topwnd.h"
#include "string.h"
#include "littlec.h"
#include "opendlg.h"
#include "momo.h"
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
#include "smp_progbar.h"
#include "smp_combobox.h"
#include "smp_list.h"
#include "smp_titlebar.h"

enum {
    PHI_MSGBOX=1
};

/////////////////////////////////////////////////////////

VOID ShowPhiResult(WID wndid)
{
	HideTopWindow(wndid, FALSE, FALSE);
	ShowTopWindow(TOPWND_PHIRESULT, NULL, (DWORD)wndid);
}

static VOID HidePhiResult(WID wndid)
{
	HideTopWindow(TOPWND_PHIRESULT, FALSE, FALSE);
	ShowTopWindow(wndid, NULL, 0);
}

LRESULT PHIRESULT_WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{	
    int ret=0;

	switch(Msg)
	{
	case WM_CREATE:
	{
		return 0;
	}
    case WM_DESTROY:
    {
		return 0;
    }
    case WM_SHOW:
    {
        mrc_clearScreen(0,0,0);
        TextMiddle("正在运行",0,1);
        SGL_SuspendDrawing();
        if(StartLittleC(progmem))
        {
            SGL_UnsuspendDrawing();
            SMP_MsgBox(PHI_MSGBOX,hWnd,NULL,SGL_LoadString(STR_MSG),SGL_LoadString(STR_RUNFAI),ID_OK,NULL);
        }
        break;
    }
	case WM_PAINT:
	{   
		return 0;
    }
	case WM_KEYDOWN:
        if(MR_KEY_SEND == wParam)
        {   
            goto lcexit;
            break;
        }
	case WM_KEYDOWNREPEAT:
        ret = StartLittleCEvent(MR_KEY_PRESS,wParam,lParam);
		break;
	case WM_KEYUP:	
        ret = StartLittleCEvent(MR_KEY_RELEASE,wParam,lParam);
		break;	
    case WM_MOUSEDOWN:
        ret = StartLittleCEvent(MR_MOUSE_DOWN,wParam,lParam);
        break;
    case WM_MOUSEUP:
        ret = StartLittleCEvent(MR_MOUSE_UP,wParam,lParam);
        break;
    case WM_MOUSEMOVE:
        ret = StartLittleCEvent(MR_MOUSE_MOVE,wParam,lParam);
        break;
    case WM_COMMAND:
	{
		WID id = LOWORD(wParam);
		WORD code = HIWORD(wParam);

		switch(id)
		{
		case PHI_MSGBOX:
			switch(code)
			{
			case ID_OK:
                HidePhiResult((WID)_USERDATA(hWnd));
				return 1;
			}
			break;
        }
		break;
	}

	}
    if(ret == -1)
    {
        ret=lc_state=0;
        SGL_UnsuspendDrawing();
        SMP_MsgBox(PHI_MSGBOX,hWnd,NULL,SGL_LoadString(STR_MSG),SGL_LoadString(STR_RUNFAI),ID_OK,NULL);
    }
    if(lc_state & LC_EXIT)
    {
        lc_state=0;
lcexit:
        littleCStop();
        SGL_UnsuspendDrawing();
        HidePhiResult((WID)_USERDATA(hWnd));
    }
    
    return 1;
}


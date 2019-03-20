/*
** smp_advbar.c: the implements of smp_advbar.h
** 
** update: 2011-12-09 by zengming00
**
** It may not be redistributed under any circumstances.
*/
#include "window.h"
#include "smp_advbar.h"
//#include "mrc_advbar.h"
#include "smp.h"
#include "i18n.h"
#include "gal.h"

#define SUCCESS     0x0001
#define FAILED      0x0002


HWND hAdvBar;
//////////////////////////////////////////////////////////////
static void AdvPauseCb(void)
{
    SGL_SuspendDrawing();
#ifdef ENABLE_SGL_KEYREPEAT		
    SGL_StopKeyRepeat();
#endif
    PauseApplication();
}

static void AdvResumeCb(void)
{
    extern int32 mrc_appResume(void);
    mrc_appResume();
}

static void AdvLoadCb(int32 result)
{
    _CLR_STYLE(hAdvBar,SUCCESS);//!!!
    if (MR_SUCCESS == result) 
    {
        _SET_STYLE(hAdvBar,SUCCESS);
//        mrc_advStartDraw();
    }
    else
    {
        _SET_STYLE(hAdvBar,FAILED);
    }
}

static void AdvDoneCb(NOTIFY_TYPE result)
{
    SGL_NotifyParent(hAdvBar, result, (DWORD)hAdvBar);
    //SGL_UpdateWindow(SGL_GetNextChild(HWND_DESKTOP, NULL));
}

LRESULT SMP_Advbar_WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch (Msg)
	{
	case WM_CREATE:
		{
		}
        break;

	case WM_DESTROY:
		{
		}
        break;

	case WM_SHOW:
	    {
            if (_IS_SET_ANY(hWnd,SUCCESS))
                mrc_advStartDraw();
	    }
        break;

	case WM_HIDE:
		{
            mrc_advStopDraw();
		}
        break;

	case WM_PAINT:
		{
            if(_IS_SET_ANY(hWnd,SUCCESS))
            {
				if(_IS_SET_ANY(_PARENT(hWnd), WS_INACTIVE) || SGL_IsSuspendDrawing())//处于非活动状态(有菜单或对话框弹出)
                {
                    mrc_advStopDraw();
                }
                else
                {
                    mrc_advStartDraw();//如果没有这句，那么广告将无法继续滚动，同时加上此句还可以达到切换广告的效果
                }
                mrc_advRefresh(FALSE);

            }else{
                mrc_drawRect(_LEFT(hWnd),_TOP(hWnd),_WIDTH(hWnd),_HEIGHT(hWnd),0xff,0xff,0xff);
                mrc_drawText("加载中",_LEFT(hWnd),_TOP(hWnd),0x80,0x00,0xff,FALSE,1);
            }

		}
        break;

    case WM_SETFOCUS: //获得焦点
        mrc_advFocus();
        break;

    case WM_KILLFOCUS: //失去焦点
        mrc_advUnFocus();
        break;
    }

	return 0;
}


HWND SMP_Advbar_AddToParent(HWND hParent, int x, int y, int w, WID id, HWND hListener)
{
	if(!hAdvBar){
		hAdvBar = SGL_CreateWindow(SMP_Advbar_WndProc, x, y, w, ADV_BAR_HEIGHT, id, WS_TABSTOP, 0);
	}else{
		_WID(hAdvBar) = id;
		_LEFT(hAdvBar) = x;
        _TOP(hAdvBar) = y;
        _WIDTH(hAdvBar) = w;
	}
	_LISTENER(hAdvBar) = hListener;

	//如果父窗口不是NULL表示之前添加在了别的父窗口必须先把他从之前的父窗口移除
	if(_PARENT(hAdvBar) != NULL){
		SGL_RemoveChildWindow(hAdvBar);
	}
	SGL_AddChildWindow(hParent, hAdvBar);

	return hAdvBar;
}

VOID SMP_Advbar_SetInfo(uint32 advChunkID, int8 IsSmallFont)
{
	if(!hAdvBar) return;
	mrc_advInit(advChunkID, 
		(uint16)_LEFT(hAdvBar), (uint16)_TOP(hAdvBar), (uint16)_WIDTH(hAdvBar), 
		IsSmallFont,
		AdvPauseCb, AdvResumeCb, AdvLoadCb);

	mrc_advSetNotify(AdvDoneCb);
	//mrc_setActiveKey(MR_KEY_SELECT);
}
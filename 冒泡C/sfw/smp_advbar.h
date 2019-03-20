#ifndef _SMP_ADVBAR_H
#define _SMP_ADVBAR_H

#include "window.h"
//#include "mrc_advbar.h"




/*
最好能在mrc_appPause中添加mrc_advStopDraw();

    在需要使用广告控件的窗的WM_CREATE，WM_SHOW等通知中增加如下代码
    SMP_Advbar_AddToParent(...);
    SMP_Advbar_SetInfo(...);

广告通知响应
typedef enum {
    NOTIFY_TYPE_DOWNLOAD,
    NOTIFY_TYPE_BROWSER
}NOTIFY_TYPE;

 * \code
 *	case WM_COMMAND:
 *		WID id = LOWORD(wParam);
 *		WORD code = HIWORD(wParam);
 *
 *		if(id == "the advbar id" && code == NOTIFY_TYPE)
 *		{
 *			HWND hAdvBar = (HWND)lParam;
 * 			//处理广告通知消息
 *		}
 * \endcode


此外可能还需要在父窗口中增加：
    case WM_MENUSHOW:
    case WM_MODALSHOW:
        mrc_advStopDraw();//如果没有这句，菜单显示时将导致菜单被覆盖

*/

extern HWND hAdvBar;

#define  SMP_Advbar_Hide() \
    mrc_advStopDraw()

#define  SMP_Advbar_Show() \
    mrc_advStartDraw()


HWND SMP_Advbar_AddToParent(HWND hParent, int x, int y, int w, WID id, HWND hListener);

VOID SMP_Advbar_SetInfo(uint32 advChunkID, int8 IsSmallFont);


#endif

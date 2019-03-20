#ifndef _PLT_OPEN_FILE
#define _PLT_OPEN_FILE

#include "window.h"


/*
  本地浏览器

  \code
  	//只需要调用这个函数来显示Plat_Open
  	Plat_Open(...);

  	//响应消息的通知
	case WM_COMMAND:
		WID id = LOWORD(wParam);
		WORD code = HIWORD(wParam);

		if(id == "the Plat_Open id")
		{
            PSTR path = (PSTR)lParam;

			if(code == MR_LOCALUI_KEY_OK)
			{
				//输入完成
			}else{    //MR_LOCALUI_KEY_CANCEL
				//输入取消
			}
		}	
  \endcode
*/

HWND Plt_Open(WID id, HWND hParent, HWND listener);


LRESULT Plt_Open_WndProc(HWND hDlg, UINT Msg, WPARAM wParam, LPARAM lParam);


#endif
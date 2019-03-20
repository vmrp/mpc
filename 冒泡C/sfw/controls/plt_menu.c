#include "plt_menu.h"


VOID PLT_Menu_SetInfo(HWND hMenu, PCWSTR title, Sint32 items)
{
	int32 hPltMenu = mrc_menuCreate((PCSTR)title, (int16)items);
	_SET_WINDATA(hMenu, hPltMenu);
}


VOID PLT_Menu_SetMenuItem(HWND hMenu, int index, PCWSTR title)
{
	mrc_menuSetItem(_GET_WINDATA(hMenu, int32), (PCSTR)title, (int32)index);
}


LRESULT PLT_Menu_WndProc(HWND hMenu, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	int32 pltMenu = _GET_WINDATA(hMenu, int32);
	
	switch(Msg)
	{

	case WM_CREATE:
	{
		_SET_STYLE(hMenu, WS_TRANSPARENT);
		break;
	}

	case WM_DESTROY:
	{
		if(pltMenu > 0) mrc_menuRelease(_GET_WINDATA(hMenu, int32));
		break;
	}

	case WM_PAINT:
	{
		if(pltMenu > 0)
		{
			if(!_USERDATA(hMenu)) 
			{
				mrc_menuShow(pltMenu);
				_SET_USERDATA(hMenu, TRUE);  //≤Àµ•œ‘ æ

			}else{
				mrc_menuRefresh(pltMenu);		
			}
		}
		break;
	}
			
	case MR_MENU_SELECT:
	case MR_MENU_RETURN:
	{
		WORD code = (WORD)Msg;
		SGL_NotifyParent(hMenu, code, wParam);
		break;
	}

	}
	
	return 0;
}


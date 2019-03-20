#include "plt_text.h"

typedef struct PLT_TextData
{
	PCWSTR title;
	PCWSTR text;
	int32 type;
	int32 hPltText;
}TEXTDATA, *PTEXTDATA;

/////////////////////////////////////////////////////////////////////////

VOID PLT_Text_SetInfo(HWND hWnd, PCWSTR title, PCWSTR text)
{
	PTEXTDATA pTextData = _GET_WINDATA(hWnd, PTEXTDATA);
	pTextData->title = title;
	pTextData->text = text;
}


LRESULT PLT_Text_WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	PTEXTDATA pTextData = _GET_WINDATA(hWnd, PTEXTDATA);

	switch(Msg)
	{

	case WM_CREATE:
	{
		pTextData = (PTEXTDATA)SGL_MALLOC(sizeof(TEXTDATA));
		if(!pTextData){
		//	SGL_TRACE("%s, %d: memory out\n", __FILE__, __LINE__);
			return 1;
		}

		SGL_MEMSET(pTextData, 0, sizeof(TEXTDATA));
		_SET_STYLE(hWnd, WS_TRANSPARENT);
		_SET_WINDATA(hWnd, pTextData);
		break;
	}

	case WM_DESTROY:
	{
		if(pTextData)
		{
			if(pTextData->hPltText) 
				mrc_textRelease(pTextData->hPltText);
			SGL_FREE(pTextData);
		}
		break;
	}

	case WM_PAINT:
	{
		if(!pTextData->hPltText) 
			pTextData->hPltText = mrc_textCreate((PCSTR)pTextData->title, (PCSTR)pTextData->text, (int32)(_STYLE(hWnd) & WS_MASK_USER));
		else
			mrc_textRefresh(pTextData->hPltText, (PCSTR)pTextData->hPltText, (PCSTR)pTextData->text);
		break;
	}
		
	case MR_DIALOG_EVENT:
	{
		WORD code = (WORD)wParam;
		SGL_NotifyParent(hWnd, code, (DWORD)hWnd);
		break;
	}
	
	}
	
	return 0;
}


#include "plt_msgbox.h"

//msgbox data
typedef struct PLT_MsgBoxData
{
	PCWSTR title;		//标题
	PCWSTR content;		//内容
	int32 pltMsgBox;	//句柄
}MSGBOXDATA, *PMSGBOXDATA;

static HWND hMsgBox;

//////////////////////////////////////////////////////////////////////

HWND PLT_MsgBox(WID id, HWND hParent, PCWSTR title, PCWSTR content, DWORD style, HWND listener)
{
	PMSGBOXDATA pData;

	if(!hMsgBox)
	{
		hMsgBox = SGL_CreateWindow(PLT_MsgBox_WndProc, 1, 1, 1, 1, id, style, 0);
	}else{
		_WID(hMsgBox) = id;
		_STYLE(hMsgBox) = style | WS_TRANSPARENT;
	}

	pData = _GET_WINDATA(hMsgBox, PMSGBOXDATA);
	pData->title = title;
	pData->content = content;
	
	_LISTENER(hMsgBox) = listener;
	SGL_DoModal(hMsgBox, hParent);
	return hMsgBox;
}


LRESULT PLT_MsgBox_WndProc(HWND hDlg, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	PMSGBOXDATA pData = _GET_WINDATA(hDlg, PMSGBOXDATA);
	
	switch(Msg)
	{

	case WM_CREATE:
	{
		pData = (PMSGBOXDATA)SGL_MALLOC(sizeof(MSGBOXDATA));
		if(!pData){
		//	SGL_TRACE("%s, %d: memory out\n", __FILE__, __LINE__);
			return 1;
		}

		SGL_MEMSET(pData, 0, sizeof(MSGBOXDATA));
		_SET_STYLE(hDlg, WS_TRANSPARENT);
		_SET_WINDATA(hDlg, pData);
		break;
	}
	
	case WM_DESTROY:
	{
		if(pData) SGL_FREE(pData);
		break;
	}

	case WM_HIDE:
	{
		//平台的信息框不会再显示
		if(pData && pData->pltMsgBox > 0)
		{
			mrc_dialogRelease(pData->pltMsgBox);
			pData->pltMsgBox = 0;
		}
		break;
	}

	case WM_PAINT:
	{
		if(pData->pltMsgBox > 0)
			mrc_dialogRefresh(pData->pltMsgBox, (PCSTR)pData->title, (PCSTR)pData->content, (int32)(_STYLE(hDlg) & WS_MASK_USER));
		else
			pData->pltMsgBox = mrc_dialogCreate((PCSTR)pData->title, (PCSTR)pData->content, (int32)(_STYLE(hDlg) & WS_MASK_USER));
		break;
	}

	case MR_DIALOG_EVENT:
	{
		SGL_ExitModal(hDlg, (WORD)wParam, (LPARAM)hDlg);		
		break;
	}
	
	}
	
	return 0;
}


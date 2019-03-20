#include "smp.h"
#include "smp_textinfo.h"
#include "smp_scrollbar.h"
#include "bmp.h"
#include "mainwnd.h"

#define TI_SCRBAR_ID		1

typedef struct SMP_TextInfoData
{
	Uint16 lines;
	Uint16 height;
	Uint16 begin[SMP_TEXTINFO_LINECOUNT];
	Uint16 end[SMP_TEXTINFO_LINECOUNT];
	Uint16 pagesize;
	Uint16 pagestart;
	HWND hscrbar;

}TEXTINFODATA, *PTEXTINFODATA;

/////////////////////////////////////////////////////////////////////////////

VOID SMP_TextInfo_SetContent(HWND hWnd, PCWSTR pContent)
{
    PTEXTINFODATA pTi = _GET_WINDATA(hWnd, PTEXTINFODATA);

    if(NULL == pContent)
    {
        pContent = (PCWSTR)"\x00\x20";//空格
    }
	_SET_USERDATA(hWnd, pContent);
    if(pTi->hscrbar)
    {
        SMP_Scrollbar_SetValue(pTi->hscrbar, 0, FALSE);
    }
	SMP_TextInfo_Refresh(hWnd);
}

int SMP_TextInfo_GetTextHeight(HWND hWnd)
{
	PTEXTINFODATA pTi = _GET_WINDATA(hWnd, PTEXTINFODATA);

	return (pTi->height * pTi->lines);
}

VOID SMP_TextInfo_Refresh(HWND hWnd)
{
	int32 w, h;
	UCHAR tmp[4] = {0};
	int i, lastspace, linewidth, word, wl = 0, maxwidth;
	HFONT font = SGL_GetSystemFont();
	PTEXTINFODATA pTi = _GET_WINDATA(hWnd, PTEXTINFODATA);
	PCWSTR pContent = _GET_USERDATA(hWnd, PCWSTR);

	SGL_MEMSET(pTi, 0, sizeof(TEXTINFODATA) - sizeof(HWND));

	maxwidth = pTi->hscrbar ?  _WIDTH(hWnd) - SMP_SCRBAR_WIDTH : _WIDTH(hWnd);
	word = FALSE;
	lastspace = -1;
	for(i = linewidth = 0;   (pContent[i] || pContent[i+1]) && (pTi->lines < SMP_TEXTINFO_LINECOUNT - 1);   i+=2)
	{		
		tmp[0] = pContent[i];
        tmp[1] = pContent[i+1];

		if(tmp[0] == '\0')
		{
			if(tmp[1] == ' ') //空格字符
			{
				lastspace = i;
				word = TRUE;
				wl = 0;
			}
            else if(tmp[1] == '\n')
            {
				lastspace = -1;
				linewidth = 0;
				word = TRUE;
				pTi->end[pTi->lines] = i;
				if (pTi->lines >= SMP_TEXTINFO_LINECOUNT - 1)
					break;
				pTi->lines++;
				pTi->begin[pTi->lines] = i + 2;
				continue;
			}
		}
        else
			word = FALSE;
        if(enable_smallfont)
            skyfontWidthHeight((PSTR)tmp,&w, &h);
        else
		    mrc_textWidthHeight((PSTR)tmp, TRUE, (uint16)font, &w, &h);
		if(h > pTi->height) pTi->height = (Uint16)h;

		if(word) wl += w;
		linewidth += w;
		if(linewidth > maxwidth)
		{
			if(word && lastspace != -1 && lastspace != i)
			{
				linewidth = wl;
				pTi->end[pTi->lines] = lastspace + 2;
			}
            else
            {
				pTi->end[pTi->lines] = i;
				linewidth = w;
			}
			if(pTi->lines >= SMP_TEXTINFO_LINECOUNT - 1)
			{
				break;
			}
			pTi->lines++;
			pTi->begin[pTi->lines] = pTi->end[pTi->lines-1];
			lastspace = -1;
			word = FALSE;
		}
	}

	pTi->end[pTi->lines] = i;
	pTi->lines++;//空行
	pTi->height+=2; 
	pTi->pagesize = DIV(_HEIGHT(hWnd), pTi->height);
    if(pTi->hscrbar)
    {
		SMP_Scrollbar_SetInfo(pTi->hscrbar, 0, pTi->lines - 1, pTi->pagesize);
    }
}


LRESULT SMP_TextInfo_WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	PTEXTINFODATA pTi = _GET_WINDATA(hWnd, PTEXTINFODATA);

	switch(Msg)
	{

	case WM_CREATE:
	{
		pTi = SGL_MALLOC(sizeof(TEXTINFODATA));
		if(!pTi){
		//	SGL_TRACE("%s, %d: memory out\n", __FILE__, __LINE__);
			return 1;
		}
		
		SGL_MEMSET(pTi, 0, sizeof(TEXTINFODATA));
		_SET_WINDATA(hWnd, pTi);

		if(!_IS_SET_ANY(hWnd, SMP_TEXTINFOS_NOSCRBAR))
        {
            pTi->hscrbar = SGL_CreateWindow(SMP_Scrollbar_WndProc, 
                0, 0, SMP_SCRBAR_WIDTH, _HEIGHT(hWnd), 
                TI_SCRBAR_ID, 0, 0);
            SGL_AddChildWindow(hWnd, pTi->hscrbar);
            _LEFT(pTi->hscrbar) = _WIDTH(hWnd) - SMP_SCRBAR_WIDTH;
            _TOP(pTi->hscrbar) = 0;
            _WIDTH(pTi->hscrbar) = SMP_SCRBAR_WIDTH;
            _HEIGHT(pTi->hscrbar) = _HEIGHT(hWnd);
		}
		
		break;
	}

	case WM_DESTROY:
	{
		if(pTi) SGL_FREE(pTi);
		return 0;
	}

	case WM_SHOW:
	{
		SMP_TextInfo_Refresh(hWnd);
        if(pTi->hscrbar)
        {
            SMP_Scrollbar_SetInfo(pTi->hscrbar, 0, pTi->lines - 1, pTi->pagesize);		
        }

		break;
	}
	
	case WM_PAINT:
	{
		int i, j, x, y, len, to;
		PCWSTR pContent = _GET_USERDATA(hWnd, PCWSTR);
		HFONT font = SGL_GetSystemFont();
		UCHAR line[70];
		mr_colourSt clr;
        skyfont_screenRect_t rect1;

        x=y^=y;
		SGL_WindowToScreen(hWnd, &x, &y);
		clr.r = PIXEL888RED(_FGCOLOR(hWnd)), clr.g = PIXEL888GREEN(_FGCOLOR(hWnd)), clr.b = PIXEL888BLUE(_FGCOLOR(hWnd));
        rect1.x=x,rect1.y=y,rect1.w=_WIDTH(hWnd),rect1.h=_HEIGHT(hWnd);
		i = pTi->pagestart;
		to = MIN(i+pTi->pagesize, pTi->lines);
		for(;       i < to;         i++, y += pTi->height)
		{
			len = pTi->end[i] - pTi->begin[i];
			if(len <= 0) continue; // 继续 for 循环

            SGL_MEMCPY(line, pContent + pTi->begin[i], len);
            for(j=0; j<len; j++)
            {
                if(line[j]==0x00)
                {
                    if(line[j+1] == 0x0d || line[j+1] == 0x0a)
                        line[j+1] = 0x20;
                }
            }
            line[len] = line[len+1] = 0;
            if(enable_smallfont)
                mrc_skyfont_drawTextLeft((PSTR)line, 0, -1, (int16)x, (int16)y, rect1, clr, 0);
            else
			    mrc_drawText((PSTR)line, (int16)x, (int16)y, clr.r, clr.g, clr.b, TRUE, (uint16)font);
		}
		break;
	}

    case WM_MOUSEDOWN:
        advFocus=FALSE;
        SGL_SendMessage(_PARENT(hWnd),WM_KEYUP,MR_KEY_5,0);//本句是手机C需要而增加的
        break;
    case WM_KEYDOWN:
	case WM_KEYDOWNREPEAT:
	{
		if(wParam == MR_KEY_UP)
		{
			SMP_Scrollbar_LineUp(pTi->hscrbar);
			return 1;
		}
        else if(wParam == MR_KEY_DOWN)
        {
			SMP_Scrollbar_LineDown(pTi->hscrbar);
			return 1;
		}
        else if(wParam == MR_KEY_LEFT)
        {
			SMP_Scrollbar_PageUp(pTi->hscrbar);
			return 1;
		}
        else if(wParam == MR_KEY_RIGHT)
        {
		    SMP_Scrollbar_PageDown(pTi->hscrbar);
			return 1;
		}
		break;
	}

	case WM_COMMAND:
	{
		WORD id = LOWORD(wParam);
		WORD code = HIWORD(wParam);
		if(id == TI_SCRBAR_ID && code == SMP_SCRBARN_VALUECHANGED)
		{
			if(pTi->pagestart == (int)lParam)
				return 0;

			pTi->pagestart = (int)lParam;
			SGL_UpdateWindow(hWnd);
		}
		break;
	}
	
	}

	return 0;
}

int SMP_TextInfo_GetCurrentLine(HWND hTextInfo)
{
    PTEXTINFODATA pTi = _GET_WINDATA(hTextInfo, PTEXTINFODATA);

    if(NULL == pTi->hscrbar)
    {
        return 0;
    }
    return SMP_Scrollbar_GetValue(pTi->hscrbar);
}





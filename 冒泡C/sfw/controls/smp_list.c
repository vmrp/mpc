#include "smp.h"
#include "smp_list.h"
#include "smp_scrollbar.h"
#include "bmp.h"

#define LIST_SCRBAR_ID		1


///////////////////////////////////////////////////////////////////////////////////////////////

typedef struct SMP_ListData
{
	int rheight;          // 行高 像素
	int total;            // 列表总条目数
	int pagestart;        // 当前页第一行的索引
	int hilight;          // 高亮显示行的索引
	int pagesize;         // 每一页显示的行数
	SMP_List_GetTotal fnGetTotalCount;  //获取总行数的回调函数
	SMP_List_GetRowData fnGetRowData;   //获取行数据的回调函数
	SMP_List_DrawRow fnDrawRow;
	HWND hScrBar;         // 列表滚动条句柄

}LISTDATA, *PLISTDATA;


/**
 * 默认的画行功能。
 * 注意：此函数才是真正绘制每行内容
 * r：该行绘制内容矩形
 * 每次改变列表的高亮行都会触发此事件
 */
VOID SMP_List_DefaultDrawRowCallback(int index, PRECT r, PSMPROWDATA pRowData, int hilight, BOOL flush)
{
	int32 i, w, h;
	mr_screenRectSt rect;
	mr_colourSt color;
	HFONT font = SGL_GetSystemFont();	

	memset(&color,0,sizeof(color));//设为黑色
	//绘制高亮样式
	if(index == hilight) 
	{
// 		GAL_FillBox(r->left+SMP_LIST_MARGIN, r->top, r->width-2*SMP_LIST_MARGIN, r->height, 0x002888D0);
        DrawShadeRect(r->left+SMP_LIST_MARGIN, r->top, r->width-2*SMP_LIST_MARGIN, r->height,0x3A9FFF,0x0762B4,SHADE_UPDOWN);
        memset(&color,0xff,sizeof(color));//设为白色
	}

	//绘制图标
	rect.x = r->left + pRowData->margin, rect.y = r->top, rect.h = r->height;
	if(pRowData->hBmp)
		mrc_bitmapShowEx(pRowData->hBmp, (int16)rect.x, (int16)(rect.y + DIV(rect.h-pRowData->colHeight0, 2)), pRowData->colWidth0, pRowData->colWidth0, pRowData->colHeight0, BM_TRANSPARENT, 0, 0);	

	rect.x += pRowData->colWidth0;
	for(i = 0; i < SMP_LIST_MAX_STR_COL; i++)
	{
		if(pRowData->cols[i].str)
		{
			rect.w = pRowData->cols[i].width;
			mrc_textWidthHeight((PSTR)pRowData->cols[i].str, 0, (uint16)font, &w, &h);
			mrc_drawTextEx((PSTR)pRowData->cols[i].str, rect.x + 5, (int16)(rect.y + DIV(rect.h - h, 2)), rect, color, 0, (Uint16)font);	
		}
		rect.x += pRowData->cols[i].width;
	}	

	if(flush)
		GAL_FlushRegion(r->left, r->top, r->width, r->height);
}

//列表绘制每一行
static VOID SMP_List_DrawItem(HWND hList, int index)
{
	RECT r;
	int top = 0;
	SMPROWDATA rowData;
	PLISTDATA pListData = _GET_WINDATA(hList, PLISTDATA);
	
	_WINRECT(hList, r);	
	SGL_WindowToScreen(_PARENT(hList), &r.left, &r.top);

	top = r.top + SMP_ITEM_MARGIN;
	r.left += SMP_LIST_MARGIN;
	r.width -= 2*SMP_LIST_MARGIN;
	if(pListData->hScrBar) r.width -= SMP_SCRBAR_WIDTH;
	r.height = pListData->rheight;
	r.top = top + (index - pListData->pagestart)*pListData->rheight;
	
	GAL_FillBox(r.left, r.top, r.width, r.height, _BGCOLOR(hList));
	SGL_MEMSET(&rowData, 0, sizeof(rowData));
	pListData->fnGetRowData(index, &rowData);
	pListData->fnDrawRow(index, &r, &rowData, pListData->hilight, TRUE);
}

//列表高亮行改变
static VOID SMP_List_HilightChanged(HWND hList, int from, int to)
{
	PLISTDATA pListData = _GET_WINDATA(hList, PLISTDATA);
	pListData->hilight = to;
	SMP_List_DrawItem(hList, from);
	SMP_List_DrawItem(hList, to);
	SGL_NotifyParent(hList, SMP_LISTN_HILICHANGED, to);
}

VOID SMP_List_SetDrawRowCallback(HWND hList, SMP_List_DrawRow fnDrawRow)
{
	PLISTDATA pListData = _GET_WINDATA(hList, PLISTDATA);
	pListData->fnDrawRow = fnDrawRow;
}

VOID SMP_List_SetDataProvider(HWND hList, SMP_List_GetTotal fnGetTotal, SMP_List_GetRowData fnGetRowData)
{
	PLISTDATA pListData = _GET_WINDATA(hList, PLISTDATA);

	if(pListData)
	{
		pListData->fnGetTotalCount = fnGetTotal;
		pListData->fnGetRowData = fnGetRowData;
	}
}

static VOID SMP_List_HilightRow(HWND hList, PLISTDATA pListData, int index)
{
	if(index >= 0)
	{
        if (index > pListData->total)
            index = pListData->total;
        
        pListData->hilight = index;

		if(index >= pListData->pagestart + pListData->pagesize)
			pListData->pagestart = index - pListData->pagesize + 1;
		else if(index < pListData->pagestart)
			pListData->pagestart = index;
		
		if(pListData->hScrBar)
            SMP_Scrollbar_SetValue(pListData->hScrBar, pListData->pagestart, FALSE);
		SGL_UpdateWindow(hList);
		SGL_NotifyParent(hList, SMP_LISTN_HILICHANGED, index);
	}
}

//通过行序改变高亮行
VOID SMP_List_HilightByIndex(HWND hList, int index)
{
	PLISTDATA pListData;
	if(!IS_NORMAL_WINDOW(hList)) return;

	pListData = _GET_WINDATA(hList, PLISTDATA);
	SMP_List_HilightRow(hList, pListData, index);
}

//获取当前高亮行的行序
int SMP_List_GetHilightIndex(HWND hList)
{
	PLISTDATA pListData;
	if(!IS_NORMAL_WINDOW(hList)) 
		return 0;

	pListData = _GET_WINDATA(hList, PLISTDATA);
	return pListData->hilight;
}

//获取列表页面信息
VOID SMP_List_GetPageInfo(HWND hList, int* pagestart, int* pagesize)
{
	PLISTDATA pListData;
	if(!IS_NORMAL_WINDOW(hList)) 
		return;
	
	pListData = _GET_WINDATA(hList, PLISTDATA);
	if(pagestart) *pagestart = pListData->pagestart;
	if(pagesize) *pagesize = pListData->pagesize;
}

//列表上翻页
VOID SMP_List_PageUp(HWND hList)
{
	int oldHili;
	PLISTDATA pListData = _GET_WINDATA(hList, PLISTDATA);		
	if(pListData->pagestart <= 0)
		return;

	pListData->pagestart -= pListData->pagesize;
	if(pListData->pagestart < 0)
		pListData->pagestart = 0;

	oldHili = pListData->hilight;
	if(pListData->hilight >= pListData->pagestart + pListData->pagesize)
		pListData->hilight = pListData->pagestart + pListData->pagesize - 1;

	if(pListData->hScrBar) 
        SMP_Scrollbar_SetValue(pListData->hScrBar, pListData->pagestart, FALSE);
	SGL_UpdateWindow(hList);
	if(oldHili != pListData->hilight)
		SGL_NotifyParent(hList, SMP_LISTN_HILICHANGED, pListData->hilight);
}

//列表下翻页
VOID SMP_List_PageDown(HWND hList)
{
	int oldHili;	
	PLISTDATA pListData = _GET_WINDATA(hList, PLISTDATA);		
	int maxstart = pListData->total - pListData->pagesize;
	if(pListData->pagestart >= maxstart)
		return;
	
	pListData->pagestart += pListData->pagesize;
	if(pListData->pagestart > maxstart)
		pListData->pagestart = maxstart;
	
	oldHili = pListData->hilight;
	if(pListData->hilight < pListData->pagestart)
		pListData->hilight = pListData->pagestart;
	
	if(pListData->hScrBar)
        SMP_Scrollbar_SetValue(pListData->hScrBar, pListData->pagestart, FALSE);
	SGL_UpdateWindow(hList);
	if(oldHili != pListData->hilight)
		SGL_NotifyParent(hList, SMP_LISTN_HILICHANGED, pListData->hilight);
}

//列表上翻行
VOID SMP_List_LineUp(HWND hList)
{
	PLISTDATA pListData = _GET_WINDATA(hList, PLISTDATA);	

	if(pListData->hilight > 0) 
	{
		if(pListData->hilight > pListData->pagestart)
			SMP_List_HilightChanged(hList, pListData->hilight, pListData->hilight-1);
		else
			SMP_List_HilightRow(hList, pListData, pListData->hilight - 1);
	}else
		SMP_List_HilightRow(hList, pListData, pListData->total - 1);
}

//列表下翻行
VOID SMP_List_LineDown(HWND hList)
{
	int step = 1;
	PLISTDATA pListData = _GET_WINDATA(hList, PLISTDATA);
	
	if(pListData->hilight < pListData->total - 1) 
	{
		if(pListData->hilight + step < pListData->pagestart + pListData->pagesize)
			SMP_List_HilightChanged(hList, pListData->hilight, pListData->hilight + step);
		else
			SMP_List_HilightRow(hList, pListData, pListData->hilight + step);
	}else
		SMP_List_HilightRow(hList, pListData, 0);		
}

//刷新列表
VOID SMP_List_Refresh(HWND hList)
{
	int maxpagestart;
	PLISTDATA pListData = _GET_WINDATA(hList, PLISTDATA);

    if(pListData)
    {
        pListData->total = 0; 
        pListData->pagestart = 0; 
        pListData->hilight = 0; 
    }else return;
	pListData->total = pListData->fnGetTotalCount();
	if(pListData->hilight > pListData->total-1) 
		pListData->hilight = MAX(pListData->total - 1, 0);
	
	if(pListData->hScrBar) 
        SMP_Scrollbar_SetInfo(pListData->hScrBar, 0, MAX(pListData->total - 1, 0), pListData->pagesize);
	
	maxpagestart = MAX(pListData->total - pListData->pagesize, 0);
	if(pListData->pagestart > maxpagestart)
		pListData->pagestart = maxpagestart;
    if(pListData->hScrBar) 
        SMP_Scrollbar_SetValue(pListData->hScrBar, pListData->pagestart, FALSE);		
}

//列表窗口事件
LRESULT SMP_List_WndProc(HWND hList, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	PLISTDATA pListData = _GET_WINDATA(hList, PLISTDATA);
	switch(Msg)
	{

	case WM_CREATE:
	{
		//申请内存并初始化成员变量
		pListData = (PLISTDATA)SGL_MALLOC(sizeof(LISTDATA));
		if(!pListData) return 1;
		
		SGL_MEMSET(pListData, 0, sizeof(LISTDATA));	
		pListData->fnDrawRow = SMP_List_DefaultDrawRowCallback;
		_SET_WINDATA(hList, pListData);		
		//创建滚动条
		if(!_IS_SET_ANY(hList, SMP_LISTS_NOSCRBAR))
		{
			pListData->hScrBar = SGL_CreateWindow(SMP_Scrollbar_WndProc, 0, 0, 1, 1, 
				LIST_SCRBAR_ID, 0, 0);
			SGL_AddChildWindow(hList, pListData->hScrBar);
		}
		return 0;
	}

	case WM_DESTROY:
	{
		if(pListData) SGL_FREE(pListData);
		return 0;
	}

	case WM_SHOW: //刷新列表
	{
        if(pListData->hScrBar)
        {
            _LEFT(pListData->hScrBar) = _WIDTH(hList) - SMP_SCRBAR_WIDTH;
            _TOP(pListData->hScrBar) = 0;
            _WIDTH(pListData->hScrBar) = SMP_SCRBAR_WIDTH;
            _HEIGHT(pListData->hScrBar) = _HEIGHT(hList);
        }
        pListData->rheight = strH("鼎",0,(uint16)SGL_GetSystemFont());
        pListData->rheight += 2;
        pListData->pagesize = DIV(_HEIGHT(hList) - SMP_ITEM_MARGIN, pListData->rheight);
		SMP_List_Refresh(hList);
		return 0;
	}

	case WM_PAINT:
	{
		RECT r;
		int32 from, to;
		SMPROWDATA rowData;
		
		_WINRECT(hList, r);
		SGL_WindowToScreen(_PARENT(hList), &r.left, &r.top);

		r.top += SMP_ITEM_MARGIN;
		r.left += SMP_LIST_MARGIN;
		r.width -= pListData->hScrBar ? SMP_SCRBAR_WIDTH + 2*SMP_LIST_MARGIN : 2*SMP_LIST_MARGIN;
		r.height = pListData->rheight;

		to = pListData->total - pListData->pagestart;
		if(pListData->pagesize < to) 
            to = pListData->pagesize;

		for(from = 0; from < to; from++, r.top += pListData->rheight)
		{
			SGL_MEMSET(&rowData, 0, sizeof(SMPROWDATA));
			pListData->fnGetRowData(from + pListData->pagestart, &rowData);
			pListData->fnDrawRow(pListData->pagestart + from, &r, &rowData, pListData->hilight, FALSE);
		}
		return 0;
	}

	case WM_MOUSEDOWN: //高亮显示点击的行
	{
		int row = DIV(lParam-SMP_ITEM_MARGIN, pListData->rheight) + pListData->pagestart;
		if(row >= 0 && row < pListData->total && row < pListData->pagestart + pListData->pagesize)
		{			
			if(row != pListData->hilight )
				SMP_List_HilightChanged(hList, pListData->hilight, row);
		}
		return 0;
	}

	case WM_MOUSEUP:  //通知父窗口列表点击
	{
		int row = DIV(lParam-SMP_ITEM_MARGIN, pListData->rheight) + pListData->pagestart;
		if(row >= 0 && row  <pListData->total && row < pListData->pagestart + pListData->pagesize)
		{
			if(row == pListData->hilight)
				SGL_NotifyParent(hList, SMP_LISTN_CLICKED, pListData->hilight);
		}
		return 0;
	}

	case WM_KEYDOWN:  //处理列表翻行翻页
	case WM_KEYDOWNREPEAT:
	{
		switch(wParam)
		{
		case MR_KEY_DOWN:
			SMP_List_LineDown(hList);
			return 1;
		case MR_KEY_UP:
			SMP_List_LineUp(hList);
			return 1;
		case MR_KEY_LEFT:
			SMP_List_PageUp(hList);
			return 1;
		case MR_KEY_RIGHT:
			SMP_List_PageDown(hList);
			return 1;
		}
		return 0;
	}
	
	case WM_KEYUP:
	{
		if( (wParam == MR_KEY_SELECT) || (wParam == MR_KEY_5) )
		{
			SGL_NotifyParent(hList, SMP_LISTN_SELECTED, pListData->hilight);
			return 1;
		}
		return 0;
	}

	case WM_COMMAND:  //主要是处理滚动条位置的变化
	{
		WORD id = LOWORD(wParam);
		WORD code = HIWORD(wParam);
		
		if(LIST_SCRBAR_ID == id && code == SMP_SCRBARN_VALUECHANGED)
		{
			RECT r = {0,};
			int oldHi = pListData->hilight;

			if(pListData->pagestart == (int)lParam)
				return 0;

			pListData->pagestart = (int)lParam;
			if(pListData->hilight < pListData->pagestart)
				pListData->hilight = pListData->pagestart;
			else if(pListData->hilight >= pListData->pagestart + pListData->pagesize)
				pListData->hilight = pListData->pagestart + pListData->pagesize - 1;

			//只为了防止重新绘制滚动条
			r.width = _WIDTH(hList) - SMP_SCRBAR_WIDTH;
			r.height = _HEIGHT(hList);
			SGL_UpdateWindowEx(hList, &r);
			if(oldHi != pListData->hilight)
				SGL_NotifyParent(hList, SMP_LISTN_HILICHANGED, pListData->hilight);
		}
		return 0;
	}

	}

	return 0;
}



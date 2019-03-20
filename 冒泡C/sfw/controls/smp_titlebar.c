#include "smp_titlebar.h"
#include "string.h"
#include "smp.h"
#include "bmp.h"
#include "momo.h"

/*白天模式颜色*/
#define COLOR_001   0x00c4dfe9 //外围边框

//标题栏数据结构
typedef struct SMP_TitlebarData
{
	DWORD bmp;       // BMP图片id
	PCWSTR str;      // 显示的标题文字
	PCWSTR str2;     // 右边标题文字(时间)
}TITLEBARDATA, *P_TITLEBARDATA;

//设置标题栏内容
VOID SMP_Titlebar_SetContentR(HWND hWnd, PCWSTR content, BOOL redraw)
{
    P_TITLEBARDATA pData = _GET_WINDATA(hWnd, P_TITLEBARDATA);

    pData->str2 = content;
    if(redraw && SGL_IsWindowVisible(hWnd))
        SGL_UpdateWindow(hWnd);
}
VOID SMP_Titlebar_SetContent(HWND hWnd, DWORD bmpID, PCWSTR content, BOOL redraw)
{
	P_TITLEBARDATA pData = _GET_WINDATA(hWnd, P_TITLEBARDATA);

	pData->str = content;
	pData->bmp = bmpID;
    if(redraw && SGL_IsWindowVisible(hWnd))
        SGL_UpdateWindow(hWnd);
}

//标题栏窗口事件
LRESULT SMP_Titlebar_WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	P_TITLEBARDATA pData = _GET_WINDATA(hWnd, P_TITLEBARDATA);
	
	switch(Msg)
	{
	case WM_CREATE:
	{
		pData = (P_TITLEBARDATA)SGL_MALLOC(sizeof(TITLEBARDATA));
		if(!pData)
			return 1;
		SGL_MEMSET(pData, 0, sizeof(TITLEBARDATA));
		_SET_WINDATA(hWnd, pData);
		break;
	}

	case WM_DESTROY:
	{
		//释放控件数据结构
		if(pData) SGL_FREE(pData);
		break;
	}
	case WM_SHOW:
		break;
	case WM_PAINT:
	{
		mr_colourSt color;
		mr_screenRectSt rect;

		color.r = PIXEL888RED(_FGCOLOR(hWnd));
		color.g = PIXEL888GREEN(_FGCOLOR(hWnd));
		color.b = PIXEL888BLUE(_FGCOLOR(hWnd));

		rect.x = SMP_ITEM_CONTENT_MARGIN;
		rect.w = (uint16)(_WIDTH(hWnd) - 2*SMP_ITEM_CONTENT_MARGIN); 
		rect.h = (uint16)_HEIGHT(hWnd);

        DrawShadeRect(0,0,_WIDTH(hWnd),_HEIGHT(hWnd),0x414446,0x0f1214,SHADE_UPDOWN);//内容

#if 0 //在需要图片时打开
		/* 加载logo图标 */
		if(RESID_INVALID != pData->bmp)
		{
            int w = 0, h = 0;
			HBITMAP bmp;

            bmp = SGL_LoadBitmap(pData->bmp, &w, &h);
			if(bmp)
			{
				mrc_bitmapShowEx((uint16*)bmp
					, (int16)rect.x, (int16)DIV(_HEIGHT(hWnd)-h, 2)
					, (int16)w, (int16)w
					, (int16)h, BM_TRANSPARENT, 0, 0); //显示标题图片

				w = w + SMP_ITEM_CONTENT_MARGIN;
				rect.x += w;
				rect.w -= w;
			}
			SGL_ReleaseBitmap(pData->bmp);
		}
#endif
		rect.x += SMP_ITEM_CONTENT_MARGIN;
		rect.y = DIV(_HEIGHT(hWnd) - FONT_H, 2)-1;
		//显示标题文字
        if(enable_smallfont)
            skyfontdraw((PSTR)pData->str, rect.x, rect.y, rect, color, 0);
        else
            mrc_drawTextLeft((PSTR)pData->str, rect.x, rect.y, rect, color
			            , DRAW_TEXT_EX_IS_UNICODE, (uint16)SGL_GetSystemFont());
#if 0
        if(pData->str2)
        {
            int32 w,h;

            if(enable_smallfont)
            {
                skyfontWidthHeight((PSTR)pData->str2, &w, &h);
                rect.x=(int16)(_WIDTH(hWnd) - w - SMP_ITEM_CONTENT_MARGIN);
                skyfontdraw((PSTR)pData->str2, rect.x, rect.y, rect, color, 0);
            }
            else
            {
                mrc_textWidthHeight((PSTR)pData->str2, TRUE, (uint16)SGL_GetSystemFont(), &w, &h);
                rect.x=(int16)(_WIDTH(hWnd) - w - SMP_ITEM_CONTENT_MARGIN);
                mrc_drawTextLeft((PSTR)pData->str2, rect.x, rect.y, rect, color
                    , DRAW_TEXT_EX_IS_UNICODE, (uint16)SGL_GetSystemFont());
            }

        }
#endif

		//ShowLocalTime(COLOR_lightwhite);
		break;
	}

	}

	return 0;
}


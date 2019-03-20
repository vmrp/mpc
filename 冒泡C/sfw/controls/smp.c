#include "gal.h"
#include "smp.h"
#include "window.h"
#include "bmp.h"


static void draw(int x,int y)
{
    GAL_Rectangle(x,y,19,11,0);
    //GAL_Rectangle(0,x+1,y+1,17,9,0xffffff);
    //GAL_FillBox(0,x+2,y+2,15,7,0);
    GAL_Line(x+7,y+3,x+11,y+7,0xffffff);
    GAL_Line(x+7,y+7,x+11,y+3,0xffffff);
}

VOID SMP_DrawCheckBox(int x, int y, BOOL checked)
{
	GAL_Rectangle(x, y, SMP_CHECKBOX_SIZE, SMP_CHECKBOX_SIZE, COLOR_border);	

	if(checked)
	{
		x += DIV(SMP_CHECKBOX_SIZE-7, 2);
		GAL_Line(x, y + DIV(SMP_CHECKBOX_SIZE, 2), x + 3, y + DIV(SMP_CHECKBOX_SIZE*2, 3), COLOR_focus);
		GAL_Line(x + 3, y + DIV(SMP_CHECKBOX_SIZE*2, 3), x + 7, y + DIV(SMP_CHECKBOX_SIZE, 3), COLOR_focus);
	}
}

VOID SMP_DrawMsgBoxFrame(int x, int y, int w, int h)
{
    int16 hh;
    hh=SMP_ITEM_HEIGHT-2;
	GAL_Rectangle2(x, y, w, h, 2, 0);//外框
	GAL_FillBox(x+2, y+2, w-4, hh/3, 0x4C5052);//标题
	GAL_FillBox(x+2, y+2+hh/3, w-4, hh-hh/3, 0);//标题
    draw(w-25,y);
}

/*
VOID SMP_DrawWndHeader(int x, int y, int w, int h, Uint32 bgcolor, Uint32 fgcolor, DWORD bmpID, PCWSTR str)
{
	int wBmp = 0, hBmp = 0;
	int32 wText, hText;
	mr_screenRectSt rect;
	mr_colourSt color={0,};
	HBITMAP bmp;

	//绘制背景
	GAL_FillBox(PHYSICALGC, x, y, w, h, bgcolor);

	//绘制位图
	wBmp = 0;
	if(bmpID < RES_BITMAP_COUNT)
	{
		bmp = SGL_LoadBitmap(bmpID, &wBmp, &hBmp);
		mrc_bitmapShowEx(bmp, SMP_ITEM_CONTENT_MARGIN, (int16)DIV(h - hBmp, 2), (int16)wBmp, (int16)wBmp, (int16)hBmp, BM_TRANSPARENT, 0, 0);
		wBmp += SMP_ITEM_CONTENT_MARGIN;
	}

	//绘制标题
	if(str)
	{
		HFONT font = SGL_GetSystemFont();
		rect.x = (int16)(SMP_ITEM_CONTENT_MARGIN + wBmp);
		mrc_textWidthHeight((PSTR)str, TRUE, (uint16)font, &wText, &hText);
		rect.y = (int16)DIV(h - hText, 2); rect.h = h; rect.w = (uint16)(SCREEN_WIDTH - rect.x - SMP_ITEM_CONTENT_MARGIN);

		color.r = PIXEL888RED(fgcolor);
		color.g = PIXEL888GREEN(fgcolor);
		color.b = PIXEL888BLUE(fgcolor);
		mrc_drawTextEx((PSTR)str, rect.x, rect.y, rect, color, 1, (uint16)font);
	}
}
*/
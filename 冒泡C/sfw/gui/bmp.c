/*
** bmp.c: the implements of bmp.h
** 
** Copyright (C) 2007-2008 SKY-MOBI AS.  All rights reserved.
**
** Create date: 2007-12-27 by wandonglin
**
** This file is part of the simple gui library.
** It may not be redistributed under any circumstances.
*/

#include "bmp.h"
#include "platform.h"


//包含位图资料
#include "res_bmp.rc"


//位图句柄
static HBITMAP bmpDatas[RES_BITMAP_COUNT];


HBITMAP GUIAPI
SGL_LoadBitmap(DWORD bmp, int* width, int* height)
{

	if(bmp >= RES_BITMAP_COUNT)
		return NULL;
	
	if(!bmpDatas[bmp])
	{	
		int32 len;

		mrc_readFileFromMrpEx(NULL, bmpInfo[bmp].filename, (uint8**)&bmpDatas[bmp], &len, 0);
		
	}

	if(width) *width = bmpInfo[bmp].width;
	if(height) *height = bmpInfo[bmp].height;
	return bmpDatas[bmp];
}


VOID GUIAPI
SGL_ReleaseBitmap(DWORD bmp)
{
	if(bmp >= RES_BITMAP_COUNT)
		return;

	if(bmpDatas[bmp])
	{
		SGL_FREE(bmpDatas[bmp]);
		bmpDatas[bmp] = NULL;
	}
}


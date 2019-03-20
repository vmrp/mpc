/*
** gal.c: implements of gal.h
** 
** Copyright (C) 2007-2008 SKY-MOBI AS.  All rights reserved.
**
** Create date: 2007-12-27 by wandonglin
**
** This file is part of the simple gui library.
** It may not be redistributed under any circumstances.
*/

#include "gal.h"
#include "smp.h"
#include "momo.h"

uint8 SMP_ITEM_HEIGHT;


void GAL_Initialize (VOID)
{	
    SMP_ITEM_HEIGHT = FONT_H+6;
}

void GAL_Terminate(VOID)
{
}

void GAL_Rectangle(int x, int y, int w, int h, Uint32 pixel)
{
	int16 r = x + w - 1;
	int16 b = y + h - 1;
	mrc_drawLine((int16)x, (int16)y, r, (int16)y, PIXEL888RED(pixel), PIXEL888GREEN(pixel), PIXEL888BLUE(pixel));
	mrc_drawLine((int16)x, b, r, b, PIXEL888RED(pixel), PIXEL888GREEN(pixel), PIXEL888BLUE(pixel));
	mrc_drawLine((int16)x, (int16)y, (int16)x, b, PIXEL888RED(pixel), PIXEL888GREEN(pixel), PIXEL888BLUE(pixel));
	mrc_drawLine(r, (int16)y, r, b, PIXEL888RED(pixel), PIXEL888GREEN(pixel), PIXEL888BLUE(pixel));
}

void GAL_Rectangle2(int x, int y, int w, int h, int line, Uint32 pixel)
{
	int i;
	for(i = 0; i < line; i++)
		GAL_Rectangle(x+i, y+i, w-2*i, h-2*i, pixel);
}


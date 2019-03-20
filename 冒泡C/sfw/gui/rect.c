/*
** rect.c: implements of rect.h
** 
** Copyright (C) 2007-2008 SKY-MOBI AS.  All rights reserved.
**
** Create date: 2007-12-27 by wandonglin
**
** This file is part of the simple gui library.
** It may not be redistributed under any circumstances.
*/

#include "rect.h"


BOOL GUIAPI 
DoesIntersect(PRECT psrc1, PRECT psrc2)
{
	int left, top, right, bottom;

	left = (psrc1->left > psrc2->left) ? psrc1->left : psrc2->left;
	top  = (psrc1->top > psrc2->top) ? psrc1->top : psrc2->top;
	right = (RECT_RIGHT(psrc1) < RECT_RIGHT(psrc2)) ? RECT_RIGHT(psrc1) : RECT_RIGHT(psrc2);
	bottom = (RECT_BOTTOM(psrc1) < RECT_BOTTOM(psrc2)) 
		? RECT_BOTTOM(psrc1) : RECT_BOTTOM(psrc2);

	if(left >= right || top >= bottom)
	    return FALSE;

	return TRUE;
}


VOID GUIAPI 
UnionRect(PRECT pdst, PRECT psrc)
{
	if(psrc->left < pdst->left)//左边比较
	{
		pdst->width += pdst->left - psrc->left;
		pdst->left = psrc->left;
	}

	if(psrc->top < pdst->top)//顶边比较
	{
		pdst->height += pdst->top - psrc->top;
		pdst->top = psrc->top;
	}

	if(RECT_RIGHT(psrc) > RECT_RIGHT(pdst))//右边比较
		pdst->width = psrc->left + psrc->width - pdst->left;
	
	if(RECT_BOTTOM(psrc) > RECT_BOTTOM(psrc))//底边比较
		pdst->height = psrc->top + psrc->height - pdst->height;
}


VOID GUIAPI
IntersectRect(PRECT pdst, PRECT psrc1, PRECT psrc2)
{
	int left, top, right, bottom;
    
	left = (psrc1->left > psrc2->left) ? psrc1->left : psrc2->left;
	top  = (psrc1->top > psrc2->top) ? psrc1->top : psrc2->top;
	right = (RECT_RIGHT(psrc1) < RECT_RIGHT(psrc2)) ? RECT_RIGHT(psrc1) : RECT_RIGHT(psrc2);
	bottom = (RECT_BOTTOM(psrc1) < RECT_BOTTOM(psrc2)) 
		? RECT_BOTTOM(psrc1) : RECT_BOTTOM(psrc2);
	
	pdst->left = left;
	pdst->top = top;
	pdst->width = right - left + 1;
	pdst->height = bottom - top + 1;
}


/*
 * 图形绘制代码
 * 版权所有 (C) 2012-2012 DeepBlue
 * [25/10/2011]
 */

#include <mrc_base.h>
#ifndef SDK_MOD
#include <mrc_skyfont.h>
#else
#include <fontread.h>
#endif
#include <mrc_bmp.h>

#include "String.h"
#include "Entrance.h"

#include "nv_ui.h"

static mr_screeninfo ms;

static void small_font_proc(int32 result)
{
	Entrance_Start();
}

void font_exit(void)
{
	Entrance_ReleaseSource();	
#ifndef SDK_MOD
	mrc_skyfont_unload();
#else
	FontModuleRelease();
#endif
}

void nv_ui_init(void)
{	
	mrc_getScreenInfo(&ms);
	String_Init();
#ifndef SDK_MOD
	if(mrc_skyfont_load(0,small_font_proc)==MR_FAILED)
	{
		mrc_exit();
	}
#else
	FontModuleInit(1, 1, 1);
	Entrance_Start();
#endif
}

void fms(int16 x,int16 y,int16 w,int16 h)
{
	mrc_refreshScreen(x,y,(uint16)w,(uint16)h);
}

void cls(int32 r,int32 g,int32 b)
{  
	mrc_clearScreen(r, g, b);
}

void fresh()
{
	mrc_refreshScreen(0,14,(uint16)ms.width,(uint16)ms.height-14);
}

void fresht()
{
	mrc_refreshScreen(0,0,(uint16)ms.width,(uint16)ms.height);
}

int16 MSpw()
{
	int16 w = (int16)ms.width;
	return w;
}

int16 MSph()
{
	int16 h = (int16)ms.height;
	return h;
}

void clsr(int32 w,int32 h)
{  
	uint16 *pSBuf;

	pSBuf = w_getScreenBuffer();     
    mrc_getScreenSize(&w, &h);
    mrc_memset((uint8*)pSBuf, 0, w * h * 2);
}

void txt(char* szText,int16 x,int16 y,uint8 r,uint8 g,uint8 b)
{	
	char *pUnStr;
	mr_colourSt clr;
#ifndef SDK_MOD
	skyfont_screenRect_t rect2;
#else	
	mr_screenRectSt rect1;
#endif
	clr.r=r;
	clr.g=g;
	clr.b=b;

	pUnStr = strc2u(szText);

#ifndef SDK_MOD
	rect2.x=0;
	rect2.y=0;
	rect2.w=(uint16)ms.width;
	rect2.h=(uint16)ms.height;

	mrc_skyfont_drawTextLeft(pUnStr, 0, -1, x, y, rect2, clr, 0);
#else
	rect1.x=0;
	rect1.y=0;
	rect1.w=(uint16)ms.width;
	rect1.h=(uint16)ms.height;

	_drawText(pUnStr,x,y,clr,1,1);
#endif

	mrc_free(pUnStr);
}

void txtJG(char* szText,int16 x,int16 y,colorct bs)
{	
	char *pUnStr;
	mr_colourSt clr;
#ifndef SDK_MOD
	skyfont_screenRect_t rect2;
#else	
	mr_screenRectSt rect1;
#endif
	clr.r=bs.r;
	clr.g=bs.g;
	clr.b=bs.b;

	pUnStr = strc2u(szText);

#ifndef SDK_MOD
	rect2.x=0;
	rect2.y=0;
	rect2.w=(uint16)ms.width;
	rect2.h=(uint16)ms.height;

	mrc_skyfont_drawTextLeft(pUnStr, 0, -1, x, y, rect2, clr, 0);
#else
	rect1.x=0;
	rect1.y=0;
	rect1.w=(uint16)ms.width;
	rect1.h=(uint16)ms.height;

	_drawText(pUnStr,x,y,clr,1,1);
#endif

	mrc_free(pUnStr);
}

void txt2(char* szText,int16 x,int16 y,uint8 r,uint8 g,uint8 b)
{	
	mr_colourSt clr;
#ifndef SDK_MOD
	skyfont_screenRect_t rect2;
#else	
	mr_screenRectSt rect1;
#endif
	clr.r=r;
	clr.g=g;
	clr.b=b;
#ifndef SDK_MOD
	rect2.x=0;
	rect2.y=0;
	rect2.w=(uint16)ms.width;
	rect2.h=(uint16)ms.height;

	mrc_skyfont_drawTextLeft(szText, 0, -1, x, y, rect2, clr, 0);
#else
	rect1.x=0;
	rect1.y=0;
	rect1.w=(uint16)ms.width;
	rect1.h=(uint16)ms.height;

	_drawText(szText,x,y,clr,1,1);
#endif
}

int32 txtwh(char *text,int32 *tw,int32 *th)
{
	char * pUnStr = NULL;
	int32 w=0,h=0,xing=0;

#ifndef SDK_MOD	
	skyfont_screenRect_t rect2;

	rect2.x=0;
	rect2.y=0;
	rect2.w=(uint16)ms.width;
	rect2.h=(uint16)ms.height;
	pUnStr=strc2u(text);
	xing=mrc_skyfont_textWidthHeight(pUnStr,0,-1,0,0,rect2,1|SKYFONT_SUPPORT_AUTO_NEWLINE,&w,&h);
#else
	pUnStr=strc2u(text);
	_textWidthHeight(pUnStr,&w,&h,1,MR_FONT_MEDIUM);
#endif

	*tw=w;
	*th=h;

	mrc_free(pUnStr);

	return xing;
}

void ysrectJG(int16 x,int16 y,int16 w,int16 h,colorct bs,colorct bs2)
{
	srect(x+1,y+1,w-2,h-2,bs.r,bs.g,bs.b,bs2.r,bs2.g,bs2.b);
	nrect(x,y,w,h,bs2.r,bs2.g,bs2.b,1);
}

void ysrect(int16 x,int16 y,int16 w,int16 h,uint8 r1,uint8 g1,uint8 b1,uint8 r2,uint8 g2,uint8 b2)
{
	srect(x+1,y+1,w-2,h-2,r1,g1,b1,r2,g2,b2);
	nrect(x,y,w,h,r2,g2,b2,1);
}

void srect(int16 x,int16 y,int16 w,int16 h,uint8 r1,uint8 g1,uint8 b1,uint8 r2,uint8 g2,uint8 b2)
{
	int16 i,r,g,b;
	i=0;
	r=(int16)(r2-r1);
	g=(int16)(g2-g1);
	b=(int16)(b2-b1);
	while(i<h)
	{
		mrc_drawLine(x, y+i, x+w, y+i, (uint8)(r1+r*i/h), (uint8)(g1+g*i/h), (uint8)(b1+b*i/h));
		i=i+1;
	}
}

void wsrect(int16 x,int16 y,int16 w,int16 h,uint8 r1,uint8 g1,uint8 b1,uint8 r2,uint8 g2,uint8 b2)
{
	int16 i,r,g,b;
	i=0;
	r=(int16)(r2-r1);
	g=(int16)(g2-g1);
	b=(int16)(b2-b1);
	while(i<w)
	{
		mrc_drawLine(x+i, y, x+i, y+h, (uint8)(r1+r*i/w), (uint8)(g1+g*i/w), (uint8)(b1+b*i/w));
		i=i+1;
	}
}


void urect(int16 x,int16 y,int16 tx,int16 ty,uint8 r,uint8 g,uint8 b,int16 hu)
{
	int16 i=0;
	while(i<=hu)
	{
		mrc_drawLine(x+i, y, tx+i, ty, r, g, b);
		i=i+1;
	}
}

void drect2(int16 x,int16 y,int16 w,int16 h,uint8 r1,uint8 g1,uint8 b1,uint8 r2,uint8 g2,uint8 b2)
{
	int16 i,r,g,b,qu=0;
	i=0;
	r=(int16)(r2-r1);
	g=(int16)(g2-g1);
	b=(int16)(b2-b1);
	while(i<=h)
	{
		mrc_EffSetCon(x+h-qu, y+i, w-2*h+2*qu, 1, (uint8)(r1+r*i/h), (uint8)(g1+g*i/h), (uint8)(b1+b*i/h));
		i++;
		qu++;
	}
}

void drect(int16 x,int16 y,int16 w,int16 h,uint8 r1,uint8 g1,uint8 b1,uint8 r2,uint8 g2,uint8 b2)
{
	int16 i,r,g,b;
	i=0;
	r=(int16)(r2-r1);
	g=(int16)(g2-g1);
	b=(int16)(b2-b1);
	while(i<=h)
	{
		mrc_EffSetCon(x, y+i, w, 1, (uint8)(r1+r*i/h), (uint8)(g1+g*i/h), (uint8)(b1+b*i/h));
		i=i+1;
	}
}

void san(int16 x,int16 y,int16 w,uint8 r,uint8 g,uint8 b,int16 hu)
{
	int16 i;
	for(i=0;i<=w;i++)
	{
		switch(hu)
		{
			//向左
		case 0:
			mrc_drawLine(x+w-i,y+i,x+w-i,y+2*w-i,r,g,b);
			break;
		case 1:
			mrc_EffSetCon(x+w-i,y+i,1,2*w-2*i,r,g,b);
			break;
			//向右
		case 2:
			mrc_drawLine(x+i,y+i,x+i,y+2*w-i,r,g,b);
			break;
		case 3:
			mrc_EffSetCon(x+i,y+i,1,2*w-2*i,r,g,b);
			break;
			//向上
		case 4:
			mrc_drawLine(x+i,y+w-i,x+2*w-i,y+w-i,r,g,b);
			break;
		case 5:
			mrc_EffSetCon(x+i,y+w-i,2*w-2*i,1,r,g,b);
			break;
			//向下
		case 6:
			mrc_drawLine(x+i,y+i,x+2*w-i,y+i,r,g,b);
			break;
		case 7:
			mrc_EffSetCon(x+i,y+i,2*w-2*i,1,r,g,b);
			break;
		}
	}
}

void rectJG(int16 x,int16 y,int16 w,int16 h,colorct bs)
{
	mrc_drawRect(x,y,w,h,bs.r,bs.g,bs.b);
}

void yrect(int16 x,int16 y,int16 w,int16 h,uint8 r,uint8 g,uint8 b)
{
	mrc_drawRect(x+1,y,w-2,h,r,g,b);
	mrc_drawRect(x,y+1,w,h-2,r,g,b);
}

void yrectJG(int16 x,int16 y,int16 w,int16 h,colorct bs)
{
	mrc_drawRect(x+1,y,w-2,h,bs.r,bs.g,bs.b);
	mrc_drawRect(x,y+1,w,h-2,bs.r,bs.g,bs.b);
}

void trect(int16 x,int16 y,int16 w,int16 h,uint8 r,uint8 g,uint8 b)
{
	mrc_EffSetCon(x,y+1,w,h-2,r,g,b);
	mrc_EffSetCon(x+1,y,w-2,1,r,g,b);
	mrc_EffSetCon(x+1,y+h-1,w-2,1,r,g,b);
}

void nrect(int16 x,int16 y,int16 w,int16 h,uint8 r,uint8 g,uint8 b,int16 hu)
{
	mrc_drawRect(x,y+1,hu,h-2,r,g,b);
	mrc_drawRect(x+w-hu,y+1,hu,h-2,r,g,b);
	mrc_drawRect(x+1,y,w-2,hu,r,g,b);
	mrc_drawRect(x+1,y+h-hu,w-2,hu,r,g,b);
}

void ntrect(int16 x,int16 y,int16 w,int16 h,uint8 r,uint8 g,uint8 b,int16 hu)
{
	mrc_EffSetCon(x,y+1,hu,h-2,r,g,b);
	mrc_EffSetCon(x+w-hu,y+1,hu,h-2,r,g,b);
	mrc_EffSetCon(x+1,y,w-2,hu,r,g,b);
	mrc_EffSetCon(x+1,y+h-hu,w,hu,r,g,b);
}

void mrect(int16 x,int16 y,int16 w,int16 h,uint8 r,uint8 g,uint8 b,int16 hu)
{
	mrc_drawRect(x,y,hu,h,r,g,b);
	mrc_drawRect(x+w-hu,y,hu,h,r,g,b);
	mrc_drawRect(x,y,w,hu,r,g,b);
	mrc_drawRect(x,y+h-hu,w,hu,r,g,b);
}

void vrect(int16 x,int16 y,int16 w,int16 h,uint8 r,uint8 g,uint8 b,int t)
{
	switch(t)
	{
		case 0:
			mrc_drawRect(x+2,y,w-4,h,r,g,b);
			mrc_drawRect(x+1,y+1,w-2,h-2,r,g,b);
			mrc_drawRect(x,y+2,w,h-4,r,g,b);
			break;
		case 2:
			mrc_drawRect(x+3,y,w-6,h,r,g,b);
			mrc_drawRect(x+1,y+1,w-2,h-2,r,g,b);
			mrc_drawRect(x,y+3,w,h-6,r,g,b);
			break;
		case 4:
			mrc_drawRect(x+4,y,w-8,h,r,g,b);
			mrc_drawRect(x+2,y+1,w-4,h-2,r,g,b);
			mrc_drawRect(x+1,y+2,w-2,h-4,r,g,b);
			mrc_drawRect(x,y+4,w,h-8,r,g,b);
			break;
		case 6:
			mrc_EffSetCon(x+4,y,w-8,1,r,g,b);
			mrc_EffSetCon(x+2,y+1,w-4,1,r,g,b);
			mrc_EffSetCon(x+1,y+2,w-2,2,r,g,b);
			mrc_EffSetCon(x,y+4,w,h-8,r,g,b);
			mrc_EffSetCon(x+4,y+h-1,w-8,1,r,g,b);
			mrc_EffSetCon(x+2,y+h-2,w-4,1,r,g,b);
			mrc_EffSetCon(x+1,y+h-4,w-2,2,r,g,b);
			break;
	}
}

void svrect(int16 x,int16 y,int16 w,int16 h,uint8 r1,uint8 g1,uint8 b1,uint8 r2,uint8 g2,uint8 b2,int t)
{
	int16 i=0,r,g,b;
	r=(int16)(r2-r1);
	g=(int16)(g2-g1);
	b=(int16)(b2-b1);
	while(i<t)
	{
		vrect(x+i, y+i, w-2*i, h-2*i, (uint8)(r1+r*i/t), (uint8)(g1+g*i/t), (uint8)(b1+b*i/t),6);
		i++;
	}
}


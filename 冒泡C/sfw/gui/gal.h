/*
** gal.h: graphic absract layer header file.
**
** Copyright (C) 2007-2008 SKY-MOBI AS.  All rights reserved.
**
** This file is part of the simple gui library.
** It may not be redistributed under any circumstances.
*/

#ifndef _SGL_GAL_H
#define _SGL_GAL_H

#include "types.h"
#include "momo.h"

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */






/**
 * \标准颜色
 */
#define COLOR_transparent	0x00000000
#define COLOR_darkred		0x00800000
#define COLOR_darkgreen		0x00008000
#define COLOR_darkyellow	0x00808000
#define COLOR_darkblue		0x00000080
#define COLOR_darkmagenta	0x00800080
#define COLOR_darkcyan		0x00008080
#define COLOR_lightgray		0x00c0c0c0
#define COLOR_darkgray		0x00808080
#define COLOR_red			0x00ff0000
#define COLOR_green			0x0000ff00
#define COLOR_yellow		0x00ffff00
#define COLOR_blue			0x000000f0
#define COLOR_magenta		0x00ff00ff
#define COLOR_cyan			0x0000ffff
#define COLOR_lightwhite	0xF0F0F0
#define COLOR_black			0x00000000


/** 常用字体颜色 */
#define COLOR_FONT_lpur     0x008028F8  //浅紫色(purple)很常用 
#define COLOR_CF            0x00415b74  //黑蓝色
#define COLOR_user3         0x0087ceeb  //天蓝色，夜间模式常用字体色
#define COLOR_FONT_night    0x004A6D8C  //QQ浏览器夜间字体色

/* 常用前景色 */
#define COLOR_userlabel		0x00949494   
#define COLOR_Combox_range  0x009A9A9A  //combox外围矩形边框
#define COLOR_Combox_bg     0x00202020  //combox背景

/** 夜间模式颜色 */
#define COLOR_bgNight		0x00a1a1a1   //列表背景，线  
#define COLOR_NFM		    0x0004a6d8
#define COLOR_menuNight		0x00081c10
#define COLOR_Listface	    0x00171a1d  //列表皮肤
#define COLOR_scrbarbg	    0x00e7ece7
#define COLOR_nlabelfont    0x00660008 

/** 其他自定义颜色 */
#define COLOR_user1			0x00808080
//#define COLOR_user2			0x009090f0
#define COLOR_user4         0x00201c3a


/**
 * \图形抽象层
 */
void GAL_Initialize (VOID);

/**
 * \终止图形抽象层
 */
void GAL_Terminate (VOID);

/**
 * \获得每个像素的字节
 */
#define GAL_BytesPerPixel(gc) \
	DIV((gc)->scrInfo.bit + 7 , 8)

/**
 * \获得每像素的位
 */
#define GAL_BitsPerPixel(gc) \
	((gc)->scrInfo.bit)

/**
 * \获取屏幕的宽度。
 */
#define GAL_Width(gc) SCREEN_WIDTH

/**
 * \获取屏幕的高度
 */
#define GAL_Height(gc) SCREEN_HEIGHT

/**
 * \获得支持的颜色
 
#define GAL_Colors(gc) \
	(((gc)->scrInfo.bit >= 24)? (1 << 24): (1 << (gc)->scrInfo.bit))
*/
#define GAL_Colors(gc) 16
/**
 * \颜色矩形
 */
#define GAL_FillBox(x, y, w, h, pixel) \
	mrc_drawRect((int16)(x), (int16)(y), (int16)(w), (int16)(h), PIXEL888RED((pixel)), PIXEL888GREEN((pixel)), PIXEL888BLUE((pixel)))

/**
 * \画一条水平线
 */
#define GAL_DrawHLine(x, y, w, pixel) \
	mrc_drawLine((int16)(x), (int16)(y), (int16)((x)+(w)-1), (int16)(y), PIXEL888RED(pixel), PIXEL888GREEN(pixel), PIXEL888BLUE(pixel))

/**
 * \画一条垂直线
 */
#define GAL_DrawVLine(x, y, h, pixel) \
	mrc_drawLine((int16)(x), (int16)(y), (int16)(x), (int16)((y)+(h)-1), PIXEL888RED(pixel), PIXEL888GREEN(pixel), PIXEL888BLUE(pixel))

/**
 * \绘制一个像素
 */
#define GAL_DrawPixel(x, y, pixel) \
	mrc_drawPointEx((int16)(x), (int16)(y), PIXEL888RED(pixel), PIXEL888GREEN(pixel), PIXEL888BLUE(pixel))

/**
 * \画一条直线
 */
#define GAL_Line(x1, y1, x2, y2, pixel) \
	mrc_drawLine((int16)(x1), (int16)(y1), (int16)(x2), (int16)(y2), PIXEL888RED(pixel), PIXEL888GREEN(pixel), PIXEL888BLUE(pixel))

/**
 * \画一个矩形框
 */
void GAL_Rectangle(int x, int y, int w, int h, Uint32 pixel);

/**
 * \绘制一个具有特定宽度的矩形框
 */
void GAL_Rectangle2(int x, int y, int w, int h, int line, Uint32 pixel);

/**
 * \刷新屏幕
 */
#define GAL_Flush() \
	mrc_refreshScreen(0, 0, (uint16)SCREEN_WIDTH, (uint16)SCREEN_HEIGHT)

/**
 * \刷新屏幕的区域
 */
#define GAL_FlushRegion(x,  y,  w, h) \
	mrc_refreshScreen((int16)(x), (int16)(y), (uint16)(w), (uint16)(h))



#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* _SGL_GAL_H */


/*
** types.h: the types header file, contains all the base types for simple gui library.
**
** Copyright (C) 2007-2008 SKY-MOBI AS.  All rights reserved.
**
** This file is part of the simple gui library.
** It may not be redistributed under any circumstances.
*/
#ifndef _SGL_TYPES_H
#define _SGL_TYPES_H

#include "platform.h"

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

/**
 * \8位无符号字符类型的定义
 */
typedef unsigned char   Uint8;
/**
 * \8位有符号字符类型的定义
 */
typedef signed char     Sint8;
/**
 * \16位无符号整数类型定义
 */
typedef unsigned short  Uint16;
/**
 * \16位有符号整数类型定义
 */
typedef signed short    Sint16;
/**
 * \32位无符号整数类型定义
 */
typedef unsigned int    Uint32;
/**
 * \32位有符号整数类型定义
 */
typedef signed int      Sint32;

/**
 * \布尔值类型的定义
 */
typedef int BOOL;

/**
 * \FALSE值，定义为0
 */
#ifndef FALSE
#define FALSE       0
#endif

/**
 * \TRUE值，定义为1
 */
#ifndef TRUE
#define TRUE        1
#endif

/**
 * \空指针
 */
#ifndef NULL
#define NULL            ((void *)0)
#endif

/**
 * \void类型的定义
 */
#define VOID            void

/**
 * \GUIAPI类型定义
 */
#define GUIAPI

/**
 * \窗口或控件的句柄类型定义
 */
typedef struct SGL_Window* HWND;

/**
 * \菜单的句柄类型定义
 */
typedef HWND HMENU;

/**
 * \字体的句柄类型定义。
 */
typedef Uint32 HFONT;

/**
 * \位图的句柄类型定义
 */
typedef VOID* HBITMAP;

/**
 * \窗口类型的定义
 */
typedef Uint16 WID;

/**
 * \8位有符号字符类型的定义
 */
//typedef char char;

/**
 * \8位无符号字符类型的定义
 */
typedef Uint8 UCHAR;

/**
 * \wstring指针的类型定义
 */
typedef UCHAR* PWSTR;

/**
 * \const wstring指针的类型定义
 */
typedef const UCHAR* PCWSTR;

/**
 * \字符串指针的类型定义
 */
typedef char* PSTR;

/**
 * \const字符串指针的类型定义
 */
typedef const char* PCSTR;

/**
 * \无符号字节的类型定义
 */
typedef unsigned char   BYTE;

/**
 * \字节指针的类型定义
 */
typedef BYTE*		PBYTE;

/**
 * \有符号字符的类型定义
 */
typedef signed char     SBYTE;

/**
 * \无符号短整数（字）的类型定义
 */
typedef unsigned short  WORD;

/**
 * \有符号短整数类型的定义。
 */
typedef signed short    SWORD;

/**
 * \无符号长整型（双字）类型定义。
 */
typedef unsigned long   DWORD;

/**
 * \一种有符号长整数类型的定义。
 */
typedef signed long     SDWORD;

/**
 * \无符号整数类型定义
 */
typedef unsigned int    UINT;

/**
 * \长整数类型定义
 */
typedef long            LONG;

/**
 * \第一条消息参数类型定义
 */
typedef UINT            WPARAM;

/**
 * \第二个消息参数类型定义
 */
typedef DWORD           LPARAM;

/**
 *\结果类型定义
 */
typedef DWORD		LRESULT;

/**
 * \字的低八位字节
 */
#define LOBYTE(w)           ((BYTE)(w))

/**
 * \字的高八位字节
 */
#define HIBYTE(w)           ((BYTE)(((WORD)(w) >> 8) & 0xFF))

/**
 * \高低字节合并为字
 */
#define MAKEWORD(low, high) ((WORD)(((BYTE)(low)) | (((WORD)((BYTE)(high))) << 8)))

/**
 * \返回双字的低字
 */
#define LOWORD(l)           ((WORD)(DWORD)(l))

/**
 * \返回双字的高字
 */
#define HIWORD(l)           ((WORD)((((DWORD)(l)) >> 16) & 0xFFFF))

/**
 * \返回双字的低字
 */
#define LOSWORD(l)          ((SWORD)(DWORD)(l))

/**
 * \返回双字的高字
 */
#define HISWORD(l)          ((SWORD)((((DWORD)(l)) >> 16) & 0xFFFF))

/**
 * \高低字合并为双字
 */
#define MAKELONG(low, high) ((DWORD)(((WORD)(low)) | (((DWORD)((WORD)(high))) << 16)))

/**
 * \距形区域定义
 */
typedef struct _RECT
{
    int left;		/*矩形的左上角X坐标*/
    int top;		/*矩形的左上角Y坐标*/
    int width;		/*矩形的宽度*/
    int height;		/*矩形的高度*/

} RECT, *PRECT;

/**
 * \宏返回的RECT正确位置
 */
#define RECT_RIGHT(r) (r->left + r->width - 1)

/**
 * \宏返回的矩形底部位置
 */
#define RECT_BOTTOM(r) (r->top + r->height - 1)

/**
 * \点结构类型定义
 */
typedef struct _POINT
{
    int x;		/*X坐标的点*/
    int y;		/*Y坐标的点*/

}POINT, *PPOINT;

/**
 * \2维对象结构
 */
typedef struct _SIZE
{
    int cx;
    int cy;
}SIZE, *PSIZE;

/* 创建24位RGB888格式RGB像素（0x00RRGGBB）*/
#define RGB2PIXEL888(r,g,b)   (((r) << 16) | ((g) << 8) | (b))

/* 创建16位RGB565格式像素*/
#define RGB2PIXEL565(r,g,b)   ((((r) & 0xf8) << 8) | (((g) & 0xfc) << 3) | (((b) & 0xf8) >> 3))

/* 创建24位从RGB （0x00RRGGBB）* RGB888格式像素*/
#define COLOR2PIXEL888(c)    \
    ((((c) & 0xff) << 16) | ((c) & 0xff00) | (((c) & 0xff0000) >> 16))

/* 创建16位从RGB （0x00RRGGBB）* RGB565格式像素*/
#define COLOR2PIXEL565(c)    \
    ((((c) & 0xf8) << 8) | (((c) & 0xfc00) >> 5) | (((c) & 0xf80000) >> 19))

/* 返回24位像素RGB888位R，G或B组分*/
#define PIXEL888RED(pixel)        (Uint8)(((pixel) >> 16) & 0xff)
#define PIXEL888GREEN(pixel)      (Uint8)(((pixel) >> 8) & 0xff)
#define PIXEL888BLUE(pixel)       (Uint8)((pixel) & 0xff)

/* 返回16位像素RGB565位R，G或B组分*/
#define PIXEL565RED(pixel)        (Uint8)(((pixel) >> 11) & 0x1f)
#define PIXEL565GREEN(pixel)      (Uint8)(((pixel) >> 5) & 0x3f)
#define PIXEL565BLUE(pixel)       (Uint8)((pixel) & 0x1f)

/**
 * \宏返回数组的元素个数
 */
#define TABLESIZE(table)    (sizeof(table)/sizeof(table[0]))


#ifndef MAX
#define MAX(x, y)           (((x) > (y))?(x):(y))
#endif


#ifndef MIN
#define MIN(x, y)           (((x) < (y))?(x):(y))
#endif

/**
 * \宏返回绝对值
 */
#ifndef ABS
#define ABS(x)              (((x)<0) ? -(x) : (x))
#endif


#ifndef DIV
#define DIV(x, y)		((x) / (y))
#endif


#ifndef MOD
#define MOD(x, y)		((x) % (y))
#endif

/**
 * \无效资源
 */
#ifndef RESID_INVALID
#define RESID_INVALID	0xFFFFFFFFL
#endif

/** @} end of sgltypes */

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* _SGL_TYPES_H */


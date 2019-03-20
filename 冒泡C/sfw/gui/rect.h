/*
** funs.h: the functions header file.
**
** Copyright (C) 2007-2008 SKY-MOBI AS.  All rights reserved.
**
** This file is part of the simple gui library.
** It may not be redistributed under any circumstances.
*/

#ifndef _SGL_RECT_H
#define _SGL_RECT_H

#include "gal.h"

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

/**
 * \检查2矩形是否相交
 *
 * \参数psrc1	第一个矩形
 * \参数psrc2	第二个矩形
 */
BOOL GUIAPI DoesIntersect(PRECT psrc1, PRECT psrc2);

/**
 * \brief union psrc to pdst
 *
 * \参数pdst	dest矩形
 * \参数psrc	src矩形
 */
VOID GUIAPI UnionRect(PRECT pdst, PRECT psrc);

/**
 * \相交矩形
 *
 * \参数pdst	结果矩形
 * \参数psrc1	第一矩形
 * \参数psrc2	第二个矩形
 */
VOID GUIAPI IntersectRect(PRECT pdst, PRECT psrc1, PRECT psrc2);

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* _SGL_RECT_H */


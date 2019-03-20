/*
** bmp.h: the bitmap header file.
**
** Copyright (C) 2007-2008 SKY-MOBI AS.  All rights reserved.
**
** This file is part of the simple gui library.
** It may not be redistributed under any circumstances.
*/

#ifndef _SGL_BMP_H
#define _SGL_BMP_H

#include "types.h"

#ifdef __cplusplus
extern "C"{
#endif  /* __cplusplus */


/**
  位图管理

  位图管理是管理的东西，例如字符串，但不同的是
  位图管理子模块只是不停的位图文件的信息，但不是数据。

  工作的步骤与位图子模块。
 - 位图文件的信息填写在Excel表（见图片）。
	不同的屏幕尺寸可能有不同的位图文件的信息。
 - 启用一个屏幕大小，只要定义该列宏。
	当SCREEN_SIZE_176220定义，即SCREEN_SIZE_176220列信息将被使用。
 - 编译正确的位图文件到MRP的文件。
 - 加载与它位图的ID

	\code
		#include "bmp.h"
		...
		//位图的ID“BMP_BUSY”

		int w, h; //宽度和高度

		HBITMAP bmp = SGL_LoadBitmap(BMP_BUSY, &w, &h);
	\endcode

   <br><br>
 */
	 
#include "res_bmp.h"

/**
 * \由位图ID加载位图，用户也可以调用SGL_ReleaseBitmap来释放位图
 *
 * \参数bmp			位图的ID
 * \参数[出]		位图的宽度，不需要时可以为NULL
 * \参数[出]height	位图的高度，不需要时可以为NULL
 * \返回
 *	- 成功，加载的位图句柄
 *	- 否则，空
 * \对应 SGL_ReleaseBitmap.
 */
HBITMAP GUIAPI SGL_LoadBitmap(DWORD bmp, int* width, int* height);

/**
 * \销毁位图并释放内存，不能对没有加载的位图使用。
 * \参数bmp		位图的ID
 * \对应 SGL_LoadBitmap
 */
VOID GUIAPI SGL_ReleaseBitmap(DWORD bmp);


#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* _SGL_BMP_H */


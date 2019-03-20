/*
** sound.h: the sound header file.
**
** Copyright (C) 2007-2008 SKY-MOBI AS.  All rights reserved.
**
** This file is part of the simple gui library.
** It may not be redistributed under any circumstances.
*/

#ifndef _SGL_SOUND_H
#define _SGL_SOUD_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */


/**
  \声音管理

    \code
		#include "sound.h"
		...
		//一个声音的ID为“SOUND_ONLINE”

		SGL_PlaySound(SOUND_ONLINE, FALSE);
	\endcode
  */

#include "res_sound.h"

/**
 * \由ID播放声音。
 * \参数sound	声音识别码
 * \参数loop	播放模式
 *	-TRUE		循环播放
 *	-FALSE		播放一次
 * \成功返回true，否则返回FALSE
 */
BOOL GUIAPI SGL_PlaySound(DWORD sound, BOOL loop);

/**
 * \停止声音
 *
 * \参数sound	声音识别码
 * \成功返回true，否则返回FALSE
 */
BOOL GUIAPI SGL_StopSound(DWORD sound);

/**
 * \释放声音资源
 *
 * \参数sound	声音识别码
 * \没有加载时不要使用
 */
VOID GUIAPI SGL_ReleaseSound(DWORD sound);


#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif

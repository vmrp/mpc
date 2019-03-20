/*
** i18n.h: the i18n header file.
**
** Copyright (C) 2007-2008 SKY-MOBI AS.  All rights reserved.
**
** This file is part of the simple gui library.
** It may not be redistributed under any circumstances.
*/

#ifndef _SGL_I18N_H
#define _SGL_I18N_H

#include "types.h"


#ifdef __cplusplus
extern "C"{
#endif  /* __cplusplus */


#define SGL_MAX_LNG_COUNT		39
#define MR_PLAT_VALUE_BASE	1000

//include the string ids包含的字符串ID

#include "res_str.h"


/**
 * \资源管理

  \国际化和字符串管理。

  管理子系统包括两个componets：
  - Excel的managemented字符串资源（见图片）。
	填写所有的应用程序在Excel字符串资源，并给他们每个人一个编号
	该语言列可以根据需要删除或添加。编辑Excel后
	按一下按钮“的CodeGen”来生成代码
  - 由应用程序使用某些API。
	  \code
		#include "i18n.h"
		...
		//有字符串ID“STR_OK”

		PWCSTR str = SGL_LoadString(STR_OK);
	  \endcode

  <br><br>
  \image html strings.gif "Excel Manangement String Resource"

  @ingroup resource
  @{

	 

 * 初始化国际化子系统
 *
 * 此功能之前必须调用任何其他国际化功能。该框架将调用时，系统为你带来了。
 * 所以不要在乎它。
 */
VOID GUIAPI SGL_I18nInitialize(VOID);

/**
 * \设置当前的语言，这并不会影响字符串有由SGL_LoadString加载
 * \参数新的语言识别码
 * \return 
 * 	- 成功返回TRUE
 * 	- 否则返回FALSE
 */
BOOL GUIAPI SGL_SetCurrentLanguage(DWORD lang);

/**
 * \获取当前使用的语言
 *
 * \返回当前语言
 */
DWORD GUIAPI SGL_GetCurrentLanguage(VOID);

/**
 * \获取当前语言设置下字符串ID代表的字符串 
 * \参数str		字符串ID
 * \return 
 * 	- Unicode字符串的指针
 * 	- NULL时无效
 */
PCWSTR GUIAPI SGL_LoadString(DWORD str);

/**
 * \释放语言内存资源
 * \参数lang	语言
 * \成功返回true，否则返回FALSE
 */
BOOL GUIAPI SGL_ReleaseResource(DWORD lang);

/** @} end of i18n  */

/** @} end of resource */

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* _SGL_I18N_H */

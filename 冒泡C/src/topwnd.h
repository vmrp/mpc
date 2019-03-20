#ifndef _TOPWND_H
#define _TOPWND_H

#include "window.h"

/**
 * \顶部窗口的ID起始值
 */
#define TOPWND_ID_START		300


/**
 * \顶层窗口ID
 *
 * 一个窗口ID应该有一个条目
 * InitTopWindow使用
 */
enum {
	TOPWND_MAINWND = TOPWND_ID_START,
    TOPWND_OPENDLG,
    TOPWND_HELP,
	//在这里添加窗口的ID
	MAX_TOPWND_COUNT //保持
};

/**
 * \初始化顶部窗口
 *
 * 此函数应该在InitApplication调用
 */
VOID InitTopWindow(VOID);

/**
 * \获取顶层窗口
 *
 * \参数wid	窗口的ID
 * \return 
 */
HWND GetTopWindow(WID wid);

/**
 * \显示一个顶级窗口
 *
 * \参数顶端窗口的ID
 * \参数listener顶级窗口listener
 * \参数窗体数据，用户定义的数据
 * \返回顶部窗口句柄，NULL时失败
 */
HWND ShowTopWindow(WID wid, HWND listener, DWORD userdata);

/**
 * \隐藏一个顶级窗口
 *
 * \参数wid，顶端窗口的ID
 * \参数destroy，是否销毁窗口
 * \参数redraw，是否刷新顶级窗口
 */
VOID HideTopWindow(WID wid, BOOL destroy, BOOL redraw);


#endif

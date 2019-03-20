#ifndef _PLT_MENU_H
#define _PLT_MENU_H

#include "window.h"
/**
   \平台菜单窗口

  若要使用该平台菜单：
   - 创建平台菜单窗口
   - 设置菜单的信息
   - 设置菜单项信息
   - 添加到桌面
   - 对应MR_MENU_SELECT

  \代码

  //创建一个平台菜单和显示它
  hMenu = SGL_CreateWindow（PLT_Menu_WndProc，
  0，0，SCREEN_WIDTH，SCREEN_HEIGHT，
  身份证，0，0）;
  PLT_Menu_SetInfo（hMenu，标题，项目）;
  PLT_Menu_SetMenuItem（hMenu，0，...);
  PLT_Menu_SetMenuItem（hMenu，1，...);
  PLT_Menu_SetMenuItem（hMenu，2，...);
  ...
  _LISTENER（hMenu）=hWnd，//不要忘记设置监听器
  SGL_AddChildWindow（HWND_DESKTOP，hMenu）;

  //响应了WM_COMMAND
  案件的WM_COMMAND：
  妇女参与发展的ID = LOWORD做为（wParam参数）;
  Word代码= HIWORD（wParam参数）;
  如果（编号==“平台菜单ID”）
  {
  如果（代码== MR_MENU_SELECT）
  {
  诠释指数=（int）的lParam的; / /选择菜单项的索引
  / /你的代码
  ......
  }否则如果（代码== MR_MENU_RETURN）{
/ /你的代码
......
  }
  }
  
  \ endcode

 * \设置菜单的信息。
 *
 * 此函数应该菜单窗口后调用创建
 * 设置菜单的信息。
 *
 * \参数hMenu	平台菜单窗口
 * \参数title	标题字符串
 * \参数title	菜单项字符串
 */
VOID PLT_Menu_SetInfo(HWND hMenu, PCWSTR title, Sint32 items);

/**
 * \设置一个菜单项
 *
 * \参数hMenu	平台菜单句柄
 * \参数index	菜单项的索引
 * \参数title	菜单项字符串
 */
VOID PLT_Menu_SetMenuItem(HWND hMenu, int index, PCWSTR title);

/**
 * \平台菜单的窗口过程。
 *
 * \参数hMenu		窗口句柄
 * \参数Msg			窗口消息
 * \参数wParam		第一个参数
 * \参数lParam		第二个参数
 * \返回消息的处理结果
 */
LRESULT PLT_Menu_WndProc(HWND hMenu, UINT Msg, WPARAM wParam, LPARAM lParam);



/** @} end of pltmenu 结束pltmenu */

#endif

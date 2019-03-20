#ifndef _SMP_MENU_H
#define _SMP_MENU_H

#include "window.h"

/**
 * 控制类菜单控件

  若要使用菜单：
  - 设置菜单项
  - 菜单调用API来显示菜单
  - 响应通知消息的菜单

下面是设置菜单项的不同方式，并显示一个弹出菜单：

  
 示例代码 1:
  \code
  	SMP_Menu_ClearMenuItems();
  	SMP_Menu_SetMenuItem(0, ..., 1);
  	SMP_Menu_SetMenuItem(1, ..., 2);
  	SMP_Menu_SetMenuItem(2, ..., 3);
  	...
  	SMP_Menu_Popup(...);
  \endcode

 示例代码 2:
  \code
  	//全局菜单项信息
	static const DWORD miOptions [] =
	{
		STR_CONNECT,
		STR_EXIT,
		STR_REGISTER,
		...
	};
	//你的代码

  	SMP_Menu_ClearMenuItems();
	SMP_Menu_SetMenuItem2(0, miOptions, sizeof(miOptions)/sizeof(miOptions[0]));
	...
  	SMP_Menu_Popup(...);
  \endcode

 示例代码3:
  \code
 /当是一个无子菜单的菜单例:

  	//全局菜单项信息
	static const DWORD miOptions [] =
	{
		STR_CONNECT,
		STR_EXIT,
		STR_REGISTER,
		...
	};

	SMP_Menu_Popup2(..., miOptions, sizeof(miOptions)/sizeof(miOptions[0]));
  \endcode

菜单响应通知事件
  \code
	case WM_COMMAND:
		WID id = LOWORD(wParam);
		WORD code = HIWORD(wParam); // 菜单项的ID

		if(id == "the menu id")
		{
			DWORD userdata = (DWORD)lParam; //if setted with the menu item/如果setted的菜单项

			switch(code)
			{
			case STR_CONNECT:
				//handle the notify event.
				break;
			case STR_EXIT:
				break;
			case STR_REGISTER:
				break;
			...
			}
		}	
  \endcode

 * \菜单中的最大菜单项数量。根据需要进行重新配置
 */
#define SMP_MENU_MAX_MENUITEM	25

/*
窗口样式

表示在弹出的函数（x，y）的位置是菜单（左按钮）
*/
#define SMP_MENUS_BOTTOMLEFT			0x0001L	

/**

表示在弹出的函数（x，y）的位置是菜单（右按钮）
 */
#define SMP_MENUS_BOTTOMRIGHT		0x0002L

/**
 * \brief Indicates the (x, y) position in popup function is the menu (left, top)
 \表示在弹出的函数（x，y）的位置是菜单（左，上）
 */
#define SMP_MENUS_TOPLEFT				0x0004L

#if 0 //尚未实现
/**
 * \brief Indicates the (x, y) position in popup function is the menu (right, top)
 \表示在弹出的函数（x，y）的位置是菜单（右，上）
 */
#define SMP_MENUS_TOPRIGHT			0x0008L
#endif

/**
 * \表示是一个固定的宽度单位菜单
 *
 *仅在SMP_Menu_PopupFlat用来显示平面风格的菜单这种风格
 *菜单只是一个平坦的水平，所以不要使用此为正常菜单样式，
 *它是使用在组合框和一些其他控件中
 */
#define SMP_MENUS_FLAT				0x0010L

/**
 * \分离菜单项ID
 */
#define SMP_MENU_SEPERATOR		0xffff

/**
 * \清除菜单项
 */
VOID SMP_Menu_ClearMenuItems(VOID);

/**
 * \设置菜单项
 * 
 * \参数index	菜单项的索引
 * \参数id		菜单项的ID
 * \参数str		菜单项显示的字符串
 * \参数data	菜单项数据
 * \参数next	下一个菜单项，-1表示没有下一个菜单项
 */
VOID SMP_Menu_SetMenuItem(int index, WID id, PCWSTR str, DWORD data, int next);

/**
 * \设置菜单项字符串资源
 *
 * \参数start	初始项
 * \参数items	字符串资源数组
 * \参数size	字符串资源数组的大小
 */
VOID SMP_Menu_SetMenuItem2(int start, const DWORD* items, int size);

/**
 * \设置一个子菜单
 *
 * \参数index	子菜单位置
 * \参数sub		有子菜单的菜单长度
 */
VOID SMP_Menu_SetSubMenu(int index, int sub);

/**
 * \选中菜单项
 * 
 * \参数index	菜单项的索引
 * \参数check	TRUE选中，FALSE不选中
 */
VOID SMP_Menu_CheckMenuItem(int index, BOOL check);

/**
 * \禁用菜单项
 *
 * \参数index	菜单项的索引
 * \参数disable	TRUE禁用，FALSE使用
 */
VOID SMP_Menu_DisableMenuItem(int index, BOOL disable); 

/**
 * \弹出全局菜单
 *
 * \参数id			菜单的窗口ID
 * \参数style		菜单的风格
 * \参数hParent		窗口句柄
 * \参数x			左边的位置
 * \参数Y			顶边的位置
 * \参数listener	处理WM_COMMAND消息的监听窗口
 * \返回全局菜单句柄
 */
HMENU SMP_Menu_Popup(WID id, DWORD style, HWND hParent, int x, int y, HWND listener);

/**
 * \弹出菜单和全局菜单项
 *
 * \参数id			菜单窗口的ID
 * \参数style		菜单的风格
 * \参数hParent		窗口句柄
 * \参数x			左边位置
 * \参数Y			顶边位置
 * \参数items		菜单项字符串数组
 * \参数size		菜单项字符串数组的大小
 * \参数listener	处理WM_COMMAND消息的监听窗口
 * \返回全局菜单句柄
 */
HMENU SMP_Menu_Popup2(WID id, DWORD style, HWND hParent, int x, int y, const DWORD* items, int size, HWND listener);

/**
 * \弹出全局固定宽度风格的菜单
 *
 * \参数id			菜单窗口的ID
 * \参数style		菜单的风格
 * \参数hParent		窗口句柄
 * \参数x			左边位置
 * \参数Y			顶边位置
 * \参数w			菜单的宽度
 * \参数listener	处理WM_COMMAND消息的监听窗口
 * \返回全局菜单句柄
 */
HMENU SMP_Menu_PopupFlat(WID id, DWORD style, HWND hParent, int x, int y, int w, HWND listener);

/*
窗口程序
 * \菜单窗口
 *
 * \参数hMenu	窗口句柄
 * \参数Msg		窗口消息
 * \参数wParam	第一个参数
 * \参数lParam	第二个参数
 * \返回消息的处理结果
 */
LRESULT SMP_Menu_WndProc(HMENU hMenu, UINT Msg, WPARAM wParam, LPARAM lParam);


#endif /* _SMP_MENU_H */


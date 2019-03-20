#include "i18n.h"
#include "smp.h"
#include "smp_menu.h"
#include "momo.h"
/**
 * \菜单项的样式
 */
#define MIS_HASSUBMENU		0x0001
#define MIS_DISABLED		0x0002
#define MIS_CHECKED			0x0004 

#define MENU_ITEM_SPACE			    4
#define MENU_BORDER				    3  //外围边框
#define MENU_SEPERATOR_HEIGHT		3
#define MENU_ITEM_MARGIN			9
#define MENU_STATUS_WIDTH			6
#define MENU_SUBICON_WIDTH		    3 //奇数
#define MENU_SUBICON_HEIGHT         7 //奇数

#define HAS_SUBMENU(pItem) \
	((pItem->style & MIS_HASSUBMENU) && pItem->su.submenu)

#define GET_SUBMENU(pItem) \
	pItem->su.submenu

#define GET_USRDATA(pItem) \
	pItem->su.userdata

#define HAS_TITLE(pItem) \
	(NULL != pItem->title)

#define HAS_CHECKED(pItem) \
	(pItem->style & MIS_CHECKED)

#define HAS_DISABLED(pItem) \
	(pItem->style & MIS_DISABLED)

#define IS_SEPERATOR(pItem) \
	(pItem->id == SMP_MENU_SEPERATOR)

///////////////////////////////////////////////////////////////////////////////////////////////

//菜单项数据
typedef struct SMP_MenuItem
{
	WID id;								//菜单项的ID
	WORD style; 
	PCWSTR title;						//显示字符串
	union{
		DWORD userdata;					//用户数据，只有MIS_HASSUBMENU未设置
		struct SMP_MenuItem* submenu;	//子菜单项，只设置MIS_HASSUBMENU
	}su;
	struct SMP_MenuItem* next;			//下一菜单项
}MENUITEM, *PMENUITEM;

//菜单数据
typedef struct SMP_MenuData
{
	int height;						//最大菜单项的高度
	int width;						//最大菜单项的宽度
	PMENUITEM highlight;			//当前选定的索引从0									
	PMENUITEM item_list;			//菜单项列表
	HMENU hSubMenu;					//子菜单窗口句柄
}MENUDATA, *PMENUDATA;

//全局数据
static HMENU sMenu;
static MENUITEM sMenuItems[SMP_MENU_MAX_MENUITEM];

//函数实现
/////////////////////////////////////////////////////////////////////////////////////////////
VOID SMP_Menu_ClearMenuItems(VOID)
{
	SGL_MEMSET((sMenuItems), 0, sizeof((sMenuItems)));
}

VOID SMP_Menu_SetMenuItem(int index, WID id, PCWSTR str, DWORD data, int next)
{
	if(index < 0 || index >= SMP_MENU_MAX_MENUITEM
		|| next < -1 || next >= SMP_MENU_MAX_MENUITEM)
		return;
		
	sMenuItems[(index)].id = (WID)(id);
	sMenuItems[(index)].title = (str);
	sMenuItems[(index)].su.userdata = (data);
	sMenuItems[(index)].next = ((next)==-1? NULL : &sMenuItems[(next)]);
}

VOID SMP_Menu_SetMenuItem2(int start, const DWORD* items, int size)
{
	int i;
	
	if(start < 0 || start >= SMP_MENU_MAX_MENUITEM
		|| !items || size <= 0 || size > SMP_MENU_MAX_MENUITEM - start)
		return;

	for(i = 0; i < size; i++, start++)
	{
		sMenuItems[start].id = (WID)items[i];
		sMenuItems[start].title = SGL_LoadString(items[i]);
		sMenuItems[start].su.userdata = 0;
		sMenuItems[start].next = (i == size - 1? NULL : &sMenuItems[start+1]);
	}
}

VOID SMP_Menu_SetSubMenu(int index, int sub)
{
	if(index < 0 || index >= SMP_MENU_MAX_MENUITEM
		|| sub < 0 || sub >= SMP_MENU_MAX_MENUITEM)
		return;

	sMenuItems[(index)].style |= MIS_HASSUBMENU;
	sMenuItems[(index)].su.submenu = &sMenuItems[(sub)];
}

VOID SMP_Menu_CheckMenuItem(int index, BOOL check)
{
	if(index < 0 || index >= SMP_MENU_MAX_MENUITEM)
		return;

	if(check)
		sMenuItems[(index)].style |= MIS_CHECKED;
	else
		sMenuItems[(index)].style &= ~MIS_CHECKED;
}

VOID SMP_Menu_DisableMenuItem(int index, BOOL disable)
{
	if(index < 0 || index >= SMP_MENU_MAX_MENUITEM)
		return;

	if(disable)
		sMenuItems[(index)].style |= MIS_DISABLED;
	else
		sMenuItems[(index)].style &= ~MIS_DISABLED;
}

HMENU SMP_Menu_Popup(WID id, DWORD style, HWND hParent, int x, int y, HWND listener)
{
	if(!sMenu)
	{
		sMenu = SGL_CreateWindow(SMP_Menu_WndProc, 1, 1, 1, 1, id, style, (DWORD)sMenuItems);
	}else{
		_WID(sMenu) = id;
		_STYLE(sMenu) = style;
		_SET_USERDATA(sMenu, sMenuItems);
	}

	_LISTENER(sMenu) = listener;
	SGL_PopupMenu(sMenu, x, y, hParent);
	return sMenu;
}

HMENU SMP_Menu_Popup2(WID id, DWORD style, HWND hParent, int x, int y, const DWORD* items, int size, HWND listener)
{
	SMP_Menu_ClearMenuItems();
	SMP_Menu_SetMenuItem2(0, items, size);
	return SMP_Menu_Popup(id, style, hParent, x, y, listener);
}

HMENU SMP_Menu_PopupFlat(WID id, DWORD style, HWND hParent, int x, int y, int w, HWND listener)
{
	if(NULL == sMenu)
	{
		sMenu = SGL_CreateWindow(SMP_Menu_WndProc, x, y, w, 1, id, style | SMP_MENUS_FLAT, (DWORD)sMenuItems);
	}else{
		_WID(sMenu) = id;
		_STYLE(sMenu) = style | SMP_MENUS_FLAT;
		_WIDTH(sMenu) = w;
		_SET_USERDATA(sMenu, sMenuItems);
	}

	_LISTENER(sMenu) = listener;
	SGL_PopupMenu(sMenu, x, y, hParent);
	return sMenu;
}

//确定菜单/子菜单弹出位置
static VOID SMP_Menu_AdjustMenuLocation(HMENU hMenu, int width, int height)
{
	int x= _LEFT(hMenu);
	int y= _TOP(hMenu);

	SGL_WindowToScreen(_PARENT(hMenu), &x, &y);

	if(_IS_SET_ANY(hMenu, SMP_MENUS_FLAT)) //combox 的弹出菜单
		width = _WIDTH(hMenu);

	if(x + width > SCREEN_WIDTH)
		x = SCREEN_WIDTH - width;

	if(_IS_SET_ANY(hMenu, SMP_MENUS_BOTTOMLEFT)) //菜单弹出位置为左下角
	{
		y -= height;
	}else if(_IS_SET_ANY(hMenu, SMP_MENUS_BOTTOMRIGHT)){ //菜单弹出位置为右下角
		x = SCREEN_WIDTH - width;
		y -= height;
	}else{	//默认弹出位置
		if(y + height > SCREEN_HEIGHT - SMP_MENU_BOTTOM_HEIGHT) y = SCREEN_HEIGHT - height - SMP_MENU_BOTTOM_HEIGHT;
	}
	if(y < 0) y = 0;
	SGL_ScreenToWindow(_PARENT(hMenu), &x, &y);
	_LEFT(hMenu) = x;
	_TOP(hMenu) = y;
	_WIDTH(hMenu) = width;
	_HEIGHT(hMenu) = height;
}

//获取菜单的尺寸
static VOID SMP_Menu_GetMenuDimension(HMENU hMenu, int* width, int* height)
{
	int32 w, h, itemCount = 0, spCount = 0;
	HFONT font = SGL_GetSystemFont();
	PMENUDATA pMenuData =  _GET_WINDATA(hMenu, PMENUDATA);
	PMENUITEM pItem = _GET_USERDATA(hMenu, PMENUITEM);
	
	for(pMenuData->height = pMenuData->width = 0; pItem; pItem = pItem->next)
	{
		//分离器菜单项名
		if(IS_SEPERATOR(pItem))
		{
			spCount ++;
			continue;
		}		
		++itemCount;//菜单项计数
		
		if(HAS_TITLE(pItem))//测试菜单项宽度和高度
		{
            if(enable_smallfont)
                skyfontWidthHeight((PSTR)pItem->title,&w, &h);
            else
                mrc_textWidthHeight((PSTR)pItem->title, TRUE, (Uint16)font, &w, &h);
			if(w > pMenuData->width) pMenuData->width = w;
			if(h > pMenuData->height) pMenuData->height = h;
		}
	}

	//计算信息
	if(_IS_SET_ANY(hMenu, SMP_MENUS_FLAT))
	{
		pMenuData->width +=  2*SMP_ITEM_CONTENT_MARGIN;
		*width = pMenuData->width;
		pMenuData->height += MENU_ITEM_SPACE;
		*height = itemCount * (pMenuData->height) + spCount * MENU_SEPERATOR_HEIGHT + 2;
	}else{
		pMenuData->width += 3 * MENU_ITEM_MARGIN + 2*MENU_BORDER;
		*width = pMenuData->width;
		pMenuData->height += MENU_ITEM_SPACE;
		*height = itemCount * (pMenuData->height) + spCount * MENU_SEPERATOR_HEIGHT + 2*MENU_BORDER;
	}
}

static PMENUITEM SMP_Menu_GetMenuItemByPosition(PMENUITEM pItem, int height, int x, int y)
{
	for(; pItem; pItem = pItem->next)
	{
		if(IS_SEPERATOR(pItem))
		{
			y -= MENU_SEPERATOR_HEIGHT;
			continue;
		}
		
		y -= height;
		if(y < 0) break;
	}

	return pItem;
}

static VOID SMP_Menu_ShowSubMenu(HMENU hMenu, PMENUDATA pMenuData)
{
	PMENUITEM pItem;
	PMENUDATA pSubMenuData; 
	int y = _TOP(hMenu);
	
	if(pMenuData->hSubMenu)
		_WID(pMenuData->hSubMenu) = _WID(hMenu);
	else	
		pMenuData->hSubMenu = SGL_CreateWindow(SMP_Menu_WndProc, 1, 1, 1, 1, _WID(hMenu), 0, 0);
	_LISTENER(pMenuData->hSubMenu) = _LISTENER(hMenu);

	pSubMenuData = _GET_WINDATA(pMenuData->hSubMenu, PMENUDATA);
	_SET_USERDATA(pMenuData->hSubMenu, GET_SUBMENU(pMenuData->highlight));
	
	for(pItem = _GET_USERDATA(hMenu, PMENUITEM); pItem != pMenuData->highlight; pItem = pItem->next)
		y += IS_SEPERATOR(pItem)? MENU_SEPERATOR_HEIGHT : pMenuData->height;
	
	_LEFT(pMenuData->hSubMenu) = _LEFT(hMenu) + pMenuData->width;
	_TOP(pMenuData->hSubMenu) = y;
	SGL_AddChildWindow(_PARENT(hMenu), pMenuData->hSubMenu);
}

static VOID SMP_Menu_HilightMenuItem(HMENU hMenu, PMENUITEM pItem, BOOL press)
{
	PMENUDATA pMenuData =  _GET_WINDATA(hMenu, PMENUDATA);

	if(pItem && (HAS_DISABLED(pItem) || IS_SEPERATOR(pItem)))
		return;
	
	if(pItem == pMenuData->highlight)
	{
		if(HAS_SUBMENU(pItem) && !SGL_IsWindowVisible(pMenuData->hSubMenu))
			SMP_Menu_ShowSubMenu(hMenu, pMenuData);
		return;
	}

	pMenuData->highlight = pItem;
	if(SGL_IsWindowVisible(pMenuData->hSubMenu))
	{
		HWND hParent = _PARENT(hMenu);
		SGL_SuspendDrawing();
		SGL_RemoveChildWindow(pMenuData->hSubMenu);
		if(press && HAS_SUBMENU(pItem))
			SMP_Menu_ShowSubMenu(hMenu, pMenuData);
		SGL_UnsuspendDrawing();
		SGL_UpdateWindow(hParent);
	}else{
		SGL_UpdateWindow(hMenu);
		if(press && HAS_SUBMENU(pItem))
			SMP_Menu_ShowSubMenu(hMenu, pMenuData);
	}
}

static VOID SMP_Menu_FocusItemNext(HMENU hMenu)
{
	int i;
	PMENUDATA pMenuData =  _GET_WINDATA(hMenu, PMENUDATA);	
	PMENUITEM pItem = pMenuData->highlight;

	/*
	* 两次循环主要是用来处理循环滚动的效果。在第一次循环
	* 中如果找不到下一个MenuItem。 那么pItem的值最后是NULL，在第
	* 二个循环时发现这个值是NULL将会从新从表头查找。
	*/
	for(i = 0; i < 2; i++)
	{
		for(pItem = pItem? pItem->next : _GET_USERDATA(hMenu, PMENUITEM)
			; pItem && (IS_SEPERATOR(pItem) || HAS_DISABLED(pItem))
			; pItem = pItem->next);

		if(pItem && pItem != pMenuData->highlight) 
		{
			SMP_Menu_HilightMenuItem(hMenu, pItem, FALSE);
			break;
		}
	}	
}

static VOID SMP_Menu_FocusItemPrev(HMENU hMenu)
{
	int i;
	PMENUDATA pMenuData =  _GET_WINDATA(hMenu, PMENUDATA);	
	PMENUITEM pItem = pMenuData->highlight;
	PMENUITEM pPrev, hili;

	pPrev = pItem = _GET_USERDATA(hMenu, PMENUITEM);
	hili = pMenuData->highlight;

	/*
	* 这里的两次的循环和mnFocusItemNext中的处理是相同的道理。
	*/
	for(i = 0; i < 2; i++)
	{
		while(pItem != hili)
		{
			for(pPrev = pItem, pItem = pItem->next
				; pItem && (IS_SEPERATOR(pItem) || HAS_DISABLED(pItem))
				; pItem = pItem->next);
		}
		
		if(pPrev && !IS_SEPERATOR(pPrev) && !HAS_DISABLED(pPrev) && pPrev != pMenuData->highlight)
		{
			SMP_Menu_HilightMenuItem(hMenu, pPrev, FALSE);
			break;
		}else
			hili = NULL;
	}
	
}

static VOID SMP_Menu_DrawNormal(HWND hMenu)
{
	PMENUDATA pMenuData =  _GET_WINDATA(hMenu, PMENUDATA);
	Uint32 color;
	int x=0, y=0, drawx;
	PMENUITEM pItem = _GET_USERDATA(hMenu, PMENUITEM);
	
	if(!pItem) return;
	SGL_WindowToScreen(hMenu, &x, &y);
	//绘制背景
	GAL_FillBox(x, y, _WIDTH(hMenu), _HEIGHT(hMenu), 0xffffff);//背景
	GAL_Rectangle(x, y, _WIDTH(hMenu), _HEIGHT(hMenu), 0);//外边
	
	x += MENU_BORDER;
	y += MENU_BORDER;

	GAL_FillBox(x, y, MENU_ITEM_MARGIN+3, _HEIGHT(hMenu)-2*MENU_BORDER, 0xDFDEDA);//左边
	//绘制菜单项
	for(; pItem; pItem = pItem->next)
	{
		//画分离器
		if(IS_SEPERATOR(pItem))
		{
			GAL_DrawHLine(x, y + DIV(MENU_SEPERATOR_HEIGHT, 2), _WIDTH(hMenu) - 2*MENU_BORDER, COLOR_controlhili);	
			y += MENU_SEPERATOR_HEIGHT;
			continue;
		}
		
		//绘制高光背景
		if(pItem == pMenuData->highlight)
        {
// 			GAL_FillBox(x, y, _WIDTH(hMenu)-2*MENU_BORDER, pMenuData->height, 0x0A246A);
            DrawShadeRect(x, y, _WIDTH(hMenu)-2*MENU_BORDER, pMenuData->height,0xFEE48C,0xFDC45E,SHADE_UPDOWN);
            GAL_Rectangle(x, y, _WIDTH(hMenu)-2*MENU_BORDER, pMenuData->height, 0xFCB419);//外边
            GAL_Rectangle(x+1, y+1, _WIDTH(hMenu)-2*MENU_BORDER-2, pMenuData->height-2, 0xFEEFAC);//次外边
        }
		

        //高光字体、禁用字体的颜色设置
// 		color = HAS_DISABLED(pItem)? COLOR_lightgray :  (pItem == pMenuData->highlight? COLOR_lightwhite : COLOR_black);
		color = HAS_DISABLED(pItem)? COLOR_lightgray :  COLOR_black;

		//绘制标题
		if(HAS_TITLE(pItem))
        {       
            mr_colourSt clr;
            skyfont_screenRect_t rect1;

            clr.r = PIXEL888RED(color), clr.g = PIXEL888GREEN(color), clr.b = PIXEL888BLUE(color);
            rect1.x=x + MENU_ITEM_MARGIN+7;    rect1.y=y + DIV(MENU_ITEM_SPACE, 2);
            rect1.w=_WIDTH(hMenu);           rect1.h=_HEIGHT(hMenu);

            if(enable_smallfont)
                mrc_skyfont_drawTextLeft((PSTR)pItem->title, 0, -1, rect1.x,rect1.y, rect1, clr, 0);
            else
			    mrc_drawText((PSTR)pItem->title, rect1.x, rect1.y, 
				    clr.r, clr.g, clr.b, TRUE, (Uint16)SGL_GetSystemFont());
		}
		
		//绘制子菜单标志
		if(HAS_SUBMENU(pItem))
		{
			int i;
			drawx = x + _WIDTH(hMenu) - MENU_ITEM_MARGIN - 2 - MENU_BORDER;
			for(i=0; i<4; i++)
				GAL_DrawVLine(drawx+i, y + DIV(pMenuData->height, 2) - MENU_SUBICON_WIDTH+i, MENU_SUBICON_HEIGHT-2*i, color);
		}
        //绘制选中样式
        if(HAS_CHECKED(pItem))
        {
            int yy = y + DIV(pMenuData->height-7, 2);
            int x1= x + 2;
            int xx=x1+2;
#define CHECKCOLOR 0xf3,0x99,0x00

            mrc_drawLine(x1,yy+2,xx,yy+4,CHECKCOLOR);
            mrc_drawLine(x1,yy+3,xx,yy+5,CHECKCOLOR);
            mrc_drawLine(x1,yy+4,xx++,yy+6,CHECKCOLOR);

            mrc_drawLine(xx,yy+3,xx+3,yy,CHECKCOLOR);
            mrc_drawLine(xx,yy+4,xx+3,yy+1,CHECKCOLOR);
            mrc_drawLine(xx,yy+5,xx+3,yy+2,CHECKCOLOR);
        }

		y += pMenuData->height;
	}		
}


static VOID SMP_Menu_DrawFlat(HWND hMenu)
{
	PMENUDATA pMenuData =  _GET_WINDATA(hMenu, PMENUDATA);

	int x=0, y=0;
	HFONT font = SGL_GetSystemFont();
	PMENUITEM pItem = _GET_USERDATA(hMenu, PMENUITEM);
	mr_screenRectSt rect;
	mr_colourSt color;
	Uint32 tmp;
	
	if(!pItem) return;
	
	SGL_WindowToScreen(hMenu, &x, &y);
	
	//绘制背景
	GAL_Rectangle(x, y, _WIDTH(hMenu), _HEIGHT(hMenu), COLOR_black);

	++y; //跳过上边距
	rect.x = (uint16)(x + SMP_ITEM_CONTENT_MARGIN), rect.w = (uint16)(_WIDTH(hMenu)-2*SMP_ITEM_CONTENT_MARGIN), rect.h = (uint16)pMenuData->height;
	
	//绘制菜单项
	for(; pItem; pItem = pItem->next)
	{
		//draw seperator/画分离器
		if(IS_SEPERATOR(pItem))
		{
			GAL_DrawHLine(x + MENU_ITEM_SPACE, y + DIV(MENU_SEPERATOR_HEIGHT, 2), _WIDTH(hMenu) - 2*MENU_ITEM_SPACE, COLOR_controlhili);	
			y += MENU_SEPERATOR_HEIGHT;
			continue;
		}
		
		//绘制高光背景
		if(pItem == pMenuData->highlight)
			GAL_FillBox(x+1, y, _WIDTH(hMenu)-2, pMenuData->height, COLOR_controlhili);
		
		tmp = HAS_DISABLED(pItem)? COLOR_lightgray :  (pItem == pMenuData->highlight? COLOR_lightwhite : COLOR_black);
		color.r = PIXEL888RED(tmp), color.g = PIXEL888GREEN(tmp), color.b = PIXEL888BLUE(tmp);

		//绘制标题
		rect.y = (uint16)y;
        if(enable_smallfont)
        {
            skyfont_screenRect_t rect1;
            memcpy(&rect1,&rect,sizeof(rect));
            mrc_skyfont_drawTextLeft((PSTR)pItem->title, 0, -1, rect.x, (int16)(rect.y + DIV(MENU_ITEM_SPACE, 2)), rect1, color, 0);
        }
        else
		    mrc_drawTextEx((PSTR)pItem->title, rect.x, (int16)(rect.y + DIV(MENU_ITEM_SPACE, 2)), rect, color, TRUE, (Uint16)font);
		
		y += pMenuData->height;
	}		
}


LRESULT SMP_Menu_WndProc(HMENU hMenu, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	PMENUDATA pMenuData =  _GET_WINDATA(hMenu, PMENUDATA);

	switch(Msg)
	{
		
	case WM_CREATE:
	{
		pMenuData = SGL_MALLOC(sizeof(MENUDATA));
		if(!pMenuData)
            return 1;
		SGL_MEMSET(pMenuData, 0, sizeof(MENUDATA));
		_SET_WINDATA(hMenu, pMenuData);
		return 0;
	}
		
	case WM_DESTROY:
	{
		if(!pMenuData) return 0;
		
		//如果有子菜单，销毁
		if(pMenuData->hSubMenu)
		{
			_SET_USERDATA(pMenuData->hSubMenu, 0); //应该先清除它
			SGL_DestroyWindow(pMenuData->hSubMenu);
		}
		
		SGL_FREE(pMenuData);
		return 0;
	}
	
	case WM_SHOW:
	{
		int w, h;

		pMenuData->highlight = 0; 
		SMP_Menu_GetMenuDimension(hMenu, &w, &h); //获取菜单窗口宽高
		SMP_Menu_AdjustMenuLocation(hMenu, w, h); //根据菜单窗口宽高确定其弹出位置
		SMP_Menu_FocusItemNext(hMenu);	//使菜单项获取焦点	

		return 0;
	}
	
	case WM_HIDE:
	{
		//如果子菜单中可见被关闭
		if(SGL_IsWindowVisible(pMenuData->hSubMenu))
			SGL_RemoveChildWindow(pMenuData->hSubMenu);
		
		_CLR_STYLE(hMenu, WS_PRESSED);
		_CLR_STYLE(hMenu, WS_KEYDOWN);
		return 0;
	}
		
	case WM_PAINT:
	{
		if(_IS_SET_ANY(hMenu, SMP_MENUS_FLAT))
			SMP_Menu_DrawFlat(hMenu);
		else
			SMP_Menu_DrawNormal(hMenu);
		return 0;
	}
			
	case WM_MOUSEDOWN:
	case WM_MOUSEUP:		
	{
		PMENUITEM pItem;
		int top = _IS_SET_ANY(hMenu, SMP_MENUS_FLAT)? 1 : MENU_BORDER;
		int left = _IS_SET_ANY(hMenu, SMP_MENUS_FLAT)? SMP_ITEM_CONTENT_MARGIN : MENU_BORDER;
		
		//点击不在菜单项范围内
		if((int)wParam < left 
			|| (int)wParam > _WIDTH(hMenu) - left
			|| (int)lParam < top 
			|| (int)lParam > _HEIGHT(hMenu) - top)
			return 0;

		//获取菜单项
		if(NULL == (pItem = SMP_Menu_GetMenuItemByPosition(_GET_USERDATA(hMenu, PMENUITEM), 
			pMenuData->height, wParam - left, lParam - top)))
			return 0;

		if(WM_MOUSEDOWN == Msg) //鼠标点下获得焦点
		{
			_SET_STYLE(hMenu, WS_PRESSED);
			SMP_Menu_HilightMenuItem(hMenu, pItem, TRUE);								
		}//鼠标弹起响应事件
		else if(_IS_SET_ANY(hMenu, WS_PRESSED) //没按下
			&& pItem == pMenuData->highlight //没获得焦点
			&& !HAS_SUBMENU(pItem)) //没有子菜单(有子菜单呢?)
		{
			//if(SGL_IsWindowVisible(pMenuData->hSubMenu))
			//	return 0;
			SGL_SelectMenuItem(hMenu, pItem->id, pItem->su.userdata);
			_CLR_STYLE(hMenu, WS_PRESSED);
		}
		
		return 0;
	}
	case WM_KEYDOWN:
	case WM_KEYDOWNREPEAT:
	{		
		//pass events to subment/传递活动，以subment
		if(SGL_IsWindowVisible(pMenuData->hSubMenu))
			return SGL_SendMessage(pMenuData->hSubMenu, Msg, wParam, lParam);
		
		//_SET_STYLE(hMenu, WS_KEYDOWN);
		if(wParam == MR_KEY_DOWN)
			SMP_Menu_FocusItemNext(hMenu);
		else if(wParam == MR_KEY_UP)
			SMP_Menu_FocusItemPrev(hMenu);
		return 0;
	}
	case WM_KEYUP:
	{
		if(wParam == MR_KEY_SOFTRIGHT || wParam == MR_KEY_LEFT)
        {
#if 0 //关闭整个菜单

			return 1;
#else //单步关闭菜单

			if(!SGL_IsWindowVisible(pMenuData->hSubMenu))
				return 1; //只是关闭自己
			SGL_RemoveChildWindow(pMenuData->hSubMenu);
			return 0;
#endif
		}

		//pass events to subment /传递活动，以subment
		if(SGL_IsWindowVisible(pMenuData->hSubMenu))
			return SGL_SendMessage(pMenuData->hSubMenu, Msg, wParam, lParam);

		/*
		* for the simple toolbar will generate a single WM_KEYUP from click event
		* so these two code will simple menu can not cowork with simple toolbar
		对于简单的工具栏*会生成一个单一WM_KEYUP Click事件
		因此这两个代码将简单的菜单不能用简单的工具栏cowork
		*/
		//if(!_IS_SET_ANY(hMenu, WS_KEYDOWN)) return 0;
		//_CLR_STYLE(hMenu, WS_KEYDOWN);

		if(wParam == MR_KEY_SELECT || wParam == MR_KEY_SOFTLEFT || wParam == MR_KEY_RIGHT)
		{	
			//用户选择一个菜单项
			if(pMenuData->highlight)
			{
				if(!HAS_SUBMENU(pMenuData->highlight))
					SGL_SelectMenuItem(hMenu, pMenuData->highlight->id, pMenuData->highlight->su.userdata);					
				else if(!SGL_IsWindowVisible(pMenuData->hSubMenu))
					SMP_Menu_ShowSubMenu(hMenu, pMenuData);
			}
		}

		return 0;
	}

	case WM_GETSUBMENU:
	{
		HMENU hSub = hMenu;
		PMENUDATA pMenuData2 =  _GET_WINDATA(hSub, PMENUDATA);
		int x = (int)wParam, y = (int)lParam;

		//获取最后一级子菜单,且触点在子菜单内
		while(pMenuData2->hSubMenu && SGL_IsWindowVisible(pMenuData2->hSubMenu))
		{
			hSub = pMenuData2->hSubMenu;
			pMenuData2 =  _GET_WINDATA(hSub, PMENUDATA);
			if(x >= hSub->left && x <= hSub->left+hSub->width
				&& y >= hSub->top && y <= hSub->top+hSub->height)
				break;
		}

		return (LRESULT)hSub;
	}
	
	}
	
	return 0;
}


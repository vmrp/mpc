#include "mrc_base.h"
#include "mrc_win.h"
#include "mrc_menu.h"

static void mrc_platMenuKeyHandler(int32 data, int32 type,int32 p1,int32 p2);
static void mrc_platMenuWinHandler(int32 data,int32 eventId);

int32 mrc_platMenuNew(char* title, int32 num)
{
	void* data = MR_MALLOC(sizeof(T_MENU_ITEM) * num + sizeof(T_MENU_ITEM)+sizeof(T_MENU_ITEM));
      T_MENU *m = (T_MENU *)data;
     int32 menu;
	
	if(data == NULL)
		return MR_FAILED;

   MR_MEMSET(data,0,sizeof(T_MENU_ITEM) * num + sizeof(T_MENU_ITEM)+sizeof(T_MENU_ITEM));
   menu = mrc_winNew((int32)data, mrc_platMenuWinHandler, mrc_platMenuKeyHandler);

	if(menu == MR_FAILED)
	{
		mrc_free(data);
		return MR_FAILED;
	}
   m->handle = mrc_menuCreate(title, (int16)num);  
   if (m->handle>0){
      m->itemNum = num;
	return (int32)m;
   }else
   {
      mrc_free(data);
      return MR_FAILED;
   }
}

/*
int32 mrc_platMenuDelete(void)
{
     int32 data;
     if(mrc_winGetActiveWinData(&data) == MR_SUCCESS){
         mrc_free(data);
      }
	return mrc_winClose();
}

int32 menuDeleteById(int32 menuId)
{
	return winDeleteById(menuId);
}
*/

int32 mrc_platMenuSetItem(int32 menu, const char *text, int32 index, PtrHilight cb, int32 data)
{
   T_MENU *m = (T_MENU *)menu;
   if ((m)&&(index < m->itemNum))
   	{
      m->subMenu[index].hCb = cb;
      m->subMenu[index].data = data;
      return mrc_menuSetItem(m->handle, text, index);
   }else if((m)&&(index == m->itemNum))
   	{//处理按下“返回”键的回调函数。
	   	m->EscMenu[0].hCb=cb;
	   	m->EscMenu[0].data=data;
	   	return MR_SUCCESS;
   	}
   else
   {
      return MR_FAILED;
   }
}

int32 mrc_platMenuShow(int32 menu)
{
   T_MENU *m = (T_MENU *)menu;
   return mrc_menuShow(m->handle);
}

int32 mrc_platMenuRefresh(int32 menu)
{
   T_MENU *m = (T_MENU *)menu;
   return mrc_menuRefresh(m->handle);
}


static void mrc_platMenuKeyHandler(int32 data, int32 type,int32 p1,int32 p2)
{
     T_MENU *menu = (T_MENU *)data;
	if(type == MR_MENU_SELECT)
	{
	   if ((p1 < menu->itemNum) && (menu->subMenu[p1].hCb))
	   	{
      	   menu->subMenu[p1].hCb(menu->subMenu[p1].data);
        }
	}else if(type == MR_MENU_RETURN){
		if(menu->EscMenu[0].hCb==0)
		{			
	   		mrc_winClose();
		}
		else
		{
			menu->EscMenu[0].hCb(menu->EscMenu[0].data);
		}
     }
}

static void mrc_platMenuWinHandler(int32 data,int32 eventId)
{
   T_MENU *m = (T_MENU *)data;
	
	switch(eventId)
	{
		case WIN_EVENT_SHOW:
		{		
                //mrc_menuShow(m->handle);
			break;
		}
		case WIN_EVENT_PAUSE:
			break;
		case WIN_EVENT_REFRESH:
                mrc_menuRefresh(m->handle);
			break;
		case WIN_EVENT_EXIT:
                mrc_menuRelease(m->handle);

                mrc_free((void*)data);
			break;
		default:
			break;
	}
}
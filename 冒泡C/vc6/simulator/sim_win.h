#ifndef __WIN_H__
#define  __WIN_H__
extern "C"
{
	#include "mrporting.h"	
}
typedef void (*PtrWinEvent)(void *p,int32,int32);   
typedef void (*PtrKeyEvent)(int16,int32,int32);         
#define MAX_WIN_NO       30
#define MAX_ITEMS_NO       256
#define MAX_TITLE_LEN       128
typedef enum
{
	WIN_EVENT_SHOW,
	WIN_EVENT_EXIT           
}T_WIN_EVENT;

typedef enum 
{
    	WIN_ID_NORMAL,
		WIN_ID_MENU,
		WIN_ID_EDIT,
		WIN_ID_TEXT,
		WIN_ID_DLG,
		WIN_ID_SUSPEND,
		WIN_ID_MAX
}T_WIN_ID;      
typedef struct
{
	int32 id;
	PtrKeyEvent keyCb;
	PtrWinEvent winCb;
	void *data;
}T_FRAME_WIN;

typedef struct  {
	char title[MAX_TITLE_LEN];
	char *items[MAX_ITEMS_NO];
	int num;
	int index;
}T_WIN_MENU;

typedef struct  {
	char title[MAX_TITLE_LEN];
	char *text;
	int32 type;
}T_WIN_DLG;

typedef struct  {
	char title[MAX_TITLE_LEN];
	char *text;
	uint16 *items[MAX_ITEMS_NO];
	int32 num;
	int32 index;
	int32 type;
}T_WIN_TEXT;


typedef enum 
{
	CEDIT_CREATE,
	CEDIT_SHOW,
	CEDIT_DESTROY,
	CEDIT_GETTEXT
}T_CEIDT_TYPE;   


typedef struct  {
	char title[MAX_TITLE_LEN];
	char *text;
	int32 max_size;
	int32 messagehandle;
	int32 type;
}T_WIN_EDIT;

typedef struct  {
	char title[MAX_TITLE_LEN];
	char text[MAX_TITLE_LEN];
}T_WIN_SUSPEND;

 void winInit(void);
 int32 winCreate(int32 winId,void *data,PtrWinEvent winevent,PtrKeyEvent keyevent);
 void  winKeyEvent(int16 type ,int32 x,int32 y);
 void  winWinEvent(void *p,int32 winId);
 int32 winDelete(void);
 int32 winDeleteAll(void);
 int32 getWinTop(void);
 void* getWindata(int32 id);
 void* getWindataTop(void);
 int32 isWintopID(int32 id);
 void  winShow();
 void  winShow(int32 winId);
 void startMrp(void);
 void winNormalEvent(void *p,int32 id,int32 flag);
 void dealNormalEvent(int16 type,int32 x,int32 y);

 void winMenuEvent(void *p,int32 id,int32 flag);
 void dealMenuEvent(int16 type,int32 x,int32 y);

 void winDlgEvent(void *p,int32 id,int32 flag);
 void dealDlgEvent(int16 type,int32 x,int32 y);

 void winTextEvent(void *p,int32 id,int32 flag);
 void dealTextEvent(int16 type,int32 x,int32 y);

 void winEditEvent(void *p,int32 id,int32 flag);
 void dealEditEvent(int16 type,int32 x,int32 y);

 void winSuspendEvent(void *p,int32 id,int32 flag);
 void dealSuspendEvent(int16 type,int32 x,int32 y);

 
 void drawText(uint16* text,int x,int y, int r,int g,int b);
 void drawTextEx(uint16* text,int x,int y, int width,int height,int r,int g,int b);

 int32 getSize_x(void);

 int32 getSize_y(void);

#endif


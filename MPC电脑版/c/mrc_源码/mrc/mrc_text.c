#include "mrc_base.h"
#include "mrc_win.h"
#include "mrc_text.h"

static void mrc_platTextKeyHandler(int32 data, int32 type,int32 p1,int32 p2);
static void mrc_platTextWinHandler(int32 data,int32 eventId);

int32 mrc_platTextNew(char* title, char* texttext, int32 type, int32 textdata, PtrHilight hOk, PtrHilight hCancel)
{
	void* data = MR_MALLOC(sizeof(T_PLAT_TEXT));
      T_PLAT_TEXT *t = (T_PLAT_TEXT *)data;
     int32 text;
	
	if(data == NULL)
		return MR_FAILED;

   MR_MEMSET(data,0,sizeof(T_PLAT_TEXT));
   text = mrc_winNew((int32)data, mrc_platTextWinHandler, mrc_platTextKeyHandler);

	if(text == MR_FAILED)
	{
		mrc_free(data);
		return MR_FAILED;
	}
   t->handle = mrc_textCreate((const char *)title, (const char *)texttext, type);
   if (t->handle>0){
      t->hOk = hOk;
      t->hCancel = hCancel;
      t->data = textdata;
	return (int32)t;
   }else
   {
      mrc_free(data);
      return MR_FAILED;
   }
}

/*
int32 mrc_platTextRelease(int32 text)
{
   T_PLAT_TEXT *t = (T_PLAT_TEXT *)text;
   return mrc_textRelease(t->handle);
}
*/

int32 mrc_platTextRefresh(int32 text, char * title, char * texttext)
{
   T_PLAT_TEXT *t = (T_PLAT_TEXT *)text;
   return mrc_textRefresh(t->handle, (const char *)title, (const char *)texttext);
}


static void mrc_platTextKeyHandler(int32 data, int32 type,int32 p1,int32 p2)
{
   T_PLAT_TEXT *t = (T_PLAT_TEXT *)data;
   if(type == MR_DIALOG_EVENT)
   {
      if (p1 == MR_DIALOG_KEY_OK){
         if (t->hOk){
            t->hOk(t->data);
         }
      }else if (p1 == MR_DIALOG_KEY_CANCEL) {
         if (t->hCancel){
            t->hCancel(t->data);
         }else{
            mrc_winClose();
         }
      }//(p1 == MR_DIALOG_KEY_CANCEL)
      
   }//(type == MR_DIALOG_EVENT)
}

static void mrc_platTextWinHandler(int32 data,int32 eventId)
{
   T_PLAT_TEXT *t = (T_PLAT_TEXT *)data;
	
	switch(eventId)
	{
		case WIN_EVENT_SHOW:
		{		
			break;
		}
		case WIN_EVENT_PAUSE:
			break;
		case WIN_EVENT_REFRESH:
			break;
		case WIN_EVENT_EXIT:
                mrc_textRelease(t->handle);

                mrc_free((void*)data);
			break;
		default:
			break;
	}
}

static void mrc_platWinKeyHandler(int32 data, int32 type,int32 p1,int32 p2)
{
   T_PLAT_WIN *w = (T_PLAT_WIN *)data;
   w->keyEvent(data, type, p1, p2);
}

static void mrc_platWinWinHandler(int32 data,int32 eventId)
{
   T_PLAT_WIN *w = (T_PLAT_WIN *)data;

   if (w->winEvent)
      w->winEvent(data, eventId);
   switch(eventId)
   {
      case WIN_EVENT_SHOW:
      {		
         break;
      }
      case WIN_EVENT_PAUSE:
         break;
      case WIN_EVENT_REFRESH:
         break;
      case WIN_EVENT_EXIT:
         if (w->handle)
            mrc_winRelease(w->handle);
         mrc_free((void*)data);
         break;
      default:
         break;
   }
}

int32 mrc_platWinNew(int32 windata, PtrWinEvent winEvent, PtrKeyEvent keyEvent)
{
	void* data = MR_MALLOC(sizeof(T_PLAT_WIN));
      T_PLAT_WIN *w = (T_PLAT_WIN *)data;
     int32 win;
	
	if(data == NULL)
		return MR_FAILED;

   MR_MEMSET(data,0,sizeof(T_PLAT_WIN));
   w->data = windata;
   w->keyEvent = keyEvent;
   w->winEvent = winEvent;
   win = mrc_winNew((int32)data, mrc_platWinWinHandler, mrc_platWinKeyHandler);

   if(win == MR_FAILED)
   {
   	mrc_free(data);
   	return MR_FAILED;
   }
   w->handle = mrc_winCreate();
   if (w->handle>0){
	return (int32)w;
   }else
   {
      mrc_free(data);
      return MR_FAILED;
   }
}

int32 mrc_platBaseWinNew(int32 windata, PtrWinEvent winEvent, PtrKeyEvent keyEvent)
{
	void* data = MR_MALLOC(sizeof(T_PLAT_WIN));
      T_PLAT_WIN *w = (T_PLAT_WIN *)data;
     int32 win;
	
	if(data == NULL)
		return MR_FAILED;

   MR_MEMSET(data,0,sizeof(T_PLAT_WIN));
   w->data = windata;
   w->keyEvent = keyEvent;
   w->winEvent = winEvent;
   win = mrc_winNew((int32)data, mrc_platWinWinHandler, mrc_platWinKeyHandler);

   if(win == MR_FAILED)
   {
   	mrc_free(data);
   	return MR_FAILED;
   }
   w->handle = 0;
   return (int32)w;
}


static void mrc_platDialogKeyHandler(int32 data, int32 type,int32 p1,int32 p2);
static void mrc_platDialogWinHandler(int32 data,int32 eventId);

int32 mrc_platDialogNew(char* title, char* texttext, int32 type, int32 textdata, PtrHilight hOk, PtrHilight hCancel)
{
	void* data = MR_MALLOC(sizeof(T_PLAT_TEXT));
      T_PLAT_TEXT *t = (T_PLAT_TEXT *)data;
     int32 text;
	
	if(data == NULL)
		return MR_FAILED;

   MR_MEMSET(data,0,sizeof(T_PLAT_TEXT));
   text = mrc_winNew((int32)data, mrc_platDialogWinHandler, mrc_platDialogKeyHandler);

	if(text == MR_FAILED)
	{
		mrc_free(data);
		return MR_FAILED;
	}
   t->handle = mrc_dialogCreate((const char *)title, (const char *)texttext, type);
   if (t->handle>0){
      t->hOk = hOk;
      t->hCancel = hCancel;
      t->data = textdata;
	return (int32)t;
   }else
   {
      mrc_free(data);
      return MR_FAILED;
   }
}

int32 mrc_platDialogRefresh(int32 text, char * title, char * texttext, int32 type)
{
   T_PLAT_TEXT *t = (T_PLAT_TEXT *)text;
   return mrc_dialogRefresh(t->handle, (const char *)title, (const char *)texttext, type);
}


static void mrc_platDialogKeyHandler(int32 data, int32 type,int32 p1,int32 p2)
{
   T_PLAT_TEXT *t = (T_PLAT_TEXT *)data;
   if(type == MR_DIALOG_EVENT)
   {
      if (p1 == MR_DIALOG_KEY_OK){
         if (t->hOk){
            t->hOk(t->data);
         }
      }else if (p1 == MR_DIALOG_KEY_CANCEL) {
         if (t->hCancel){
            t->hCancel(t->data);
         }else{
            mrc_winClose();
         }
      }//(p1 == MR_DIALOG_KEY_CANCEL)
      
   }//(type == MR_DIALOG_EVENT)
}

static void mrc_platDialogWinHandler(int32 data,int32 eventId)
{
   T_PLAT_TEXT *t = (T_PLAT_TEXT *)data;
	
	switch(eventId)
	{
		case WIN_EVENT_SHOW:
		{		
			break;
		}
		case WIN_EVENT_PAUSE:
			break;
		case WIN_EVENT_REFRESH:
			break;
		case WIN_EVENT_EXIT:
                mrc_dialogRelease(t->handle);

                mrc_free((void*)data);
			break;
		default:
			break;
	}
}
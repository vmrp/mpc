#include "smp_flashbox.h"
#include "bmp.h"


//flash box data /闪光灯框数据
typedef struct SMP_FlashBoxData
{
	const DWORD* frames; //bitmap frames /位图帧

	Uint32 size; // the size of the frames array/该数组的大小的帧
	Uint32 interval; // the timer interval /定时器的时间间隔
	BOOL loop; // loop mode playing/循环模式下播放

	BOOL playing; // the timer is working /定时器工作
	int32 flash_timer; // the timer handle/定时器处理
	Uint32 cur; // current bitmap index in frames array/定时器处理
}FLASHBOXDATA, *PFLASHBOXDATA;


//////////////////////////////////////////////////////////////////////////

static void SMP_FlashBox_Flash(int32 data)
{
	HWND hWnd = (HWND)data;
	PFLASHBOXDATA pData = _GET_WINDATA(hWnd, PFLASHBOXDATA);

	if(!pData->loop && pData->cur >= (pData->size - 1))
		SMP_FlashBox_Stop(hWnd);

	pData->cur = MOD(pData->cur + 1, pData->size); 
	SGL_UpdateWindow(hWnd);
}


VOID SMP_FlashBox_SetInfo(HWND hWnd, const DWORD* frms, Uint32 size, Uint32 interval, BOOL loop)
{
	PFLASHBOXDATA pData = _GET_WINDATA(hWnd, PFLASHBOXDATA);

	SMP_FlashBox_Stop(hWnd);
	
	pData->frames = frms;
	pData->size = size;
	pData->cur = 0;
	pData->interval = interval;
	pData->loop = loop;
}


VOID SMP_FlashBox_Start(HWND hWnd, Uint32 index)
{
	PFLASHBOXDATA pData = _GET_WINDATA(hWnd, PFLASHBOXDATA);

	pData->cur = index;
	SMP_FlashBox_Play(hWnd);
}


VOID SMP_FlashBox_Play(HWND hWnd)
{
	PFLASHBOXDATA pData;

	if(0 == hWnd)
	{
		return;
	}

	pData = _GET_WINDATA(hWnd, PFLASHBOXDATA);

	if(pData->playing) return;
		
	if(!pData->flash_timer)
		pData->flash_timer = mrc_timerCreate();

	pData->playing = TRUE;

	if(pData->size > 1)
		mrc_timerStart(pData->flash_timer, (int32)pData->interval, (int32)hWnd, SMP_FlashBox_Flash, TRUE);
}


VOID SMP_FlashBox_Stop(HWND hWnd)
{
	PFLASHBOXDATA pData = 0;

	if(0 == hWnd)
	{
		return;
	}

	pData = _GET_WINDATA(hWnd, PFLASHBOXDATA);

	pData->playing = FALSE;
	if(pData->flash_timer)
		mrc_timerStop(pData->flash_timer);
}


LRESULT SMP_FlashBox_WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	PFLASHBOXDATA pData = _GET_WINDATA(hWnd, PFLASHBOXDATA);

	switch(Msg)
	{
	
	case WM_CREATE:
	{
		pData = (PFLASHBOXDATA)SGL_MALLOC(sizeof(FLASHBOXDATA));
		if(NULL == pData){
		//	SGL_TRACE("%s, %d: memory out\n", __FILE__, __LINE__);
			return 1;
		}
		
		SGL_MEMSET(pData, 0, sizeof(FLASHBOXDATA));
		_SET_WINDATA(hWnd, pData);
		break;
	}

	case WM_DESTROY:
	{
		if(pData) 
		{
			if(pData->flash_timer)
				mrc_timerDelete(pData->flash_timer);
			
			SGL_FREE(pData);
		}
		break;
	}
	
	case WM_SHOW:
	{
		SMP_FlashBox_Play(hWnd);
		break;
	}
	
	case WM_HIDE:
	{
		SMP_FlashBox_Stop(hWnd);
		break;
	}
	
	case WM_PAINT:
	{
		int x = 0, y = 0, w = 0, h =  0;
		HBITMAP hBmp = SGL_LoadBitmap(pData->frames[pData->cur], &w, &h);
		
		if(hBmp) 
		{
			SGL_WindowToScreen(hWnd, &x, &y);
			x += DIV(_WIDTH(hWnd) - w, 2);
			y += DIV(_HEIGHT(hWnd) - h, 2);

			mrc_bitmapShowEx((uint16 *)hBmp, (int16)x, (int16)y, (int16)w, (int16)w, (int16)h, BM_COPY, 0, 0);
		}
		break;
	}
	
	}

	return 0;
}


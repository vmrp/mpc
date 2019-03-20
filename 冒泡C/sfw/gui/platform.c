/*
** platform.c: implements of platform.h
** 
** Copyright (C) 2007-2008 SKY-MOBI AS.  All rights reserved.
**
** Create date: 2007-12-27 by wandonglin
**
** This file is part of the simple gui library.
** It may not be redistributed under any circumstances.
*/

#include "window.h"
#include "i18n.h"
#include "mrc_network.h"
#include "platform.h"
#include "momo.h"
//#include "mrc_advbar.h"
#include "interpreter.h"
#include "picoc.h"
#include "application.h"
#include "mainwnd.h"

#define PICINITSUC 0x01
#define PLATRUMMODE 0x02


int32 _code;
int32 _p0;
int32 _p1;
char suc;

int32 mrc_appResume(void);
int32 mrc_appPause(void);

void SafeExit(int32 data)
{
    suc=0;
    if(!data)
    {
        mrc_clearScreen(0,0,0);
        mrc_drawText("结束运行",0,0,240,0,0,0,1);
        mrc_refreshScreen(0,0,SCREEN_WIDTH,SCREEN_HEIGHT);
        data=mrc_timerCreate();
        mrc_timerStart(data,1000,data,SafeExit,0);
        return;
    }
    else
    {
        mrc_timerDelete(data);
        PicocCleanup();
        mrc_appResume();
    }
}

int CheckBaseCall(char *FuncName,struct ValueType *ReturnType,int NumParams)
{
    struct Value *FuncValue = NULL;

    if(FuncName==NULL) return -1;
    if (!VariableDefined(TableStrRegister(FuncName)))
        ProgramFail(NULL, "%s()未定义",FuncName);

    VariableGet(NULL, TableStrRegister(FuncName), &FuncValue);
    if (FuncValue->Typ->Base != TypeFunction)
        ProgramFail(NULL, "无法使用%s",FuncName);

    if (FuncValue->Val->FuncDef.ReturnType == ReturnType)
    {
        if (FuncValue->Val->FuncDef.NumParams != NumParams) return -1;
    }else return -1;

    return 0;
}

int PicocCallInit(void)
{
    if(CheckBaseCall("event",&IntType,3)) return -1;
    if(CheckBaseCall("init",&IntType,0)) return -1;
    if(CheckBaseCall("pause",&IntType,0)) return -1;
    if(CheckBaseCall("resume",&IntType,0)) return -1;

    VariableDefinePlatformVar(NULL, "_code", &IntType, (union AnyValue *)&_code, FALSE);
    VariableDefinePlatformVar(NULL, "_p0", &IntType, (union AnyValue *)&_p0, FALSE);
    VariableDefinePlatformVar(NULL, "_p1", &IntType, (union AnyValue *)&_p1, FALSE);
    VariableDefinePlatformVar(NULL, "_ret", &IntType, (union AnyValue *)&FuncRet, TRUE);

    return 0;
}

void PicocRun(int32 data)
{
    if(!data)
    {
        mrc_appPause();
        _SET_BIT(suc,PLATRUMMODE);
        mrc_clearScreen(0,0,0);
        mrc_drawText("启动运行",0,0,240,0,0,0,1);
        mrc_refreshScreen(0,0,SCREEN_WIDTH,SCREEN_HEIGHT);
        data=mrc_timerCreate();
        mrc_timerStart(data,500,data,PicocRun,0);
    }
    else
    {
        mrc_timerDelete(data);
        PicocInitialise(HEAP_SIZE);
        if (PicocPlatformSetExitPoint())
        {
            SafeExit(0);
            return;
        }

        _SET_BIT(suc,PICINITSUC);
        PicocPlatformScanFile((const char*)tempfile_path);
        if(PicocCallInit())
            SafeExit(0);
        else
            PicocParse(" ", "_ret=init();", strlen("_ret=init();"), TRUE, TRUE, FALSE);
    }
}

int32 MRC_EXT_INIT(void)
{
	return momo_init();
}

int32 mrc_appEvent(int32 code, int32 param0, int32 param1)
{
    if(!_IS_SET_BIT(suc,PLATRUMMODE))//非运行模式，处理编辑器
    {
        switch(code)
        {
        case MR_MOUSE_DOWN:
            SGL_DispatchEvents(WM_MOUSEDOWN, param0, param1);
            break;
        case MR_MOUSE_UP:
            SGL_DispatchEvents(WM_MOUSEUP, param0, param1);
            break;
        case MR_KEY_PRESS:
            SGL_DispatchEvents(WM_KEYDOWN, param0, param1);
            break;
        case MR_KEY_RELEASE:
            SGL_DispatchEvents(WM_KEYUP, param0, param1);
            break;
        case MR_MOUSE_MOVE:
            SGL_DispatchEvents(WM_MOUSEMOVE, param0, param1);
            break;
        default:
            SGL_DispatchEvents(code, param0, param1);
            break;
        }
        return 0;
    }
    else
    {
        if(_IS_SET_BIT(suc,PICINITSUC))
        {
            _code=code;
            _p0=param0;
            _p1=param1;

            if (PicocPlatformSetExitPoint())
            {
                SafeExit(0);
                return FuncRet;
            }
            PicocParse(" ", "_ret=event(_code,_p0,_p1);", strlen("_ret=event(_code,_p0,_p1);"), TRUE, TRUE, FALSE);
        }

        return FuncRet;
    }

}

int32 mrc_appPause(void)
{	
    if(!_IS_SET_BIT(suc,PLATRUMMODE))//非运行模式，处理编辑器
    {
//        mrc_advStopDraw();
        SGL_SuspendDrawing();
#ifdef ENABLE_SGL_KEYREPEAT		
        SGL_StopKeyRepeat();
#endif
        //用户应用程序指定的函数
        return PauseApplication();
    }
    else
    {
        if(_IS_SET_BIT(suc,PICINITSUC))
        {
            if (PicocPlatformSetExitPoint())
            {
                SafeExit(0);
                return FuncRet;
            }
            PicocParse(" ", "_ret=pause();", strlen("_ret=pause();"), TRUE, TRUE, FALSE);
        }
        return FuncRet;
    }

}

int32 mrc_appResume(void)
{
    if(!_IS_SET_BIT(suc,PLATRUMMODE))//非运行模式，处理编辑器
    {
        HWND hTopWnd = SGL_GetNextChild(HWND_DESKTOP, NULL);

        SGL_UnsuspendDrawing();
        if(hTopWnd)
            SGL_UpdateWindow(hTopWnd);
        //用户应用程序指定的函数
        return ResumeApplication();
    }
    else
    {
        if(_IS_SET_BIT(suc,PICINITSUC))
        {
            if (PicocPlatformSetExitPoint())
            {
                SafeExit(0);
                return FuncRet;
            }
            PicocParse(" ", "_ret=resume();", strlen("_ret=resume();"), TRUE, TRUE, FALSE);
        }
        return FuncRet;
    }
}

int32 MRC_EXT_EXIT(void)
{
	//调用用户应用功能
	int r;

    PlatformCleanup();//关闭打印文件
	SGL_SuspendDrawing();
	r = ExitApplication();
	
	//终止sky GUI库
	SGL_Terminate();

#if !OMIT_NETWORK_MODULE
	//终止网络模块

	mrc_Socket_Terminate();
	mrc_Http_Terminate();
#endif

	return r;
}


#define _EXT_RELEASE
#ifndef _EXT_RELEASE
void sgl_assert(const char* file, int line, int exp)
{
	if(!exp)
	{
	//	SGL_TRACE("assert failed %s:%d\n", file, line);
		mrc_exit();
	}
}


#define MAX_LOGFILE_SIZE	1024*10
void sgl_log(const char* file, void* buffer, int size)
{
	char tmp[32];
	int32 stream, sendbytes;
	char * data = (char*)buffer;
	if(size <= 0) return;

	if(mrc_getLen(file) > MAX_LOGFILE_SIZE)
	{
		SGL_STRCPY(tmp, file);
		SGL_STRCAT(tmp, ".bak");
		mrc_remove(tmp);
		mrc_rename(file, tmp);
	}
	
	stream = mrc_open(file, MR_FILE_WRONLY | MR_FILE_CREATE);
	if(stream)
	{
		mrc_seek(stream, 0, MR_SEEK_END);
		while(size > 0 && (sendbytes = mrc_write(stream, data, size)) >= 0) 
		{
			SGL_TRACE("sgl_log: %d, %d\n", size, sendbytes);
			size -= sendbytes;
			data += sendbytes;
		}
		mrc_close(stream);
	}
}

#endif


#ifdef WIN32
#include "mrc_bmp.h"
int32 mrc_Img2bmp(uint8 *pImg, int32 ImgSize, MRC_IMG_TYPE ImgType, uint16 **bmp, int32 *w, int32 *h)
{
	return MR_FAILED;
}
#endif
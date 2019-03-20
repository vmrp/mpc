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

//#include "window.h"
//#include "i18n.h"
// #include "mrc_network.h"
#include "platform.h"
#include "momo.h"
//#include "mrc_advbar.h"
#include "interpreter.h"
#include "picoc.h"
//#include "application.h"
//#include "mainwnd.h"

#define PICINITSUC 0x01
#define PLATRUMMODE 0x02


int32 _code;
int32 _p0;
int32 _p1;
char suc;


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
        mrc_exit();
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
        PicocPlatformScanFile((const char*)filename);
        if(PicocCallInit())
            SafeExit(0);
        else
            PicocParse(" ", "_ret=init();", strlen("_ret=init();"), TRUE, TRUE, FALSE);
    }
}

int32 mrc_init(void)
{
 //   mrc_Socket_Initialize();
	return momo_init();
}

int32 mrc_event(int32 code, int32 param0, int32 param1)
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

int32 mrc_pause(void)
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

int32 mrc_resume(void)
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

int32 mrc_exitApp(void)
{

    PlatformCleanup();//关闭打印文件
    mrc_clearScreen(0,0,0);
    mrc_drawText("程序正常结束",0,0,255,250,250,0,1);
    mrc_refreshScreen(0,0,SCREEN_WIDTH,SCREEN_HEIGHT);
	return 0;
}
int32 mrc_extRecvAppEvent(int32 app, int32 code, int32 param0, int32 param1)
{
    return 0;
}
int32 mrc_extRecvAppEventEx(int32 code, int32 p0, int32 p1, int32 p2, int32 p3, int32 p4,
                            int32 p5)
{
    return 0;
}

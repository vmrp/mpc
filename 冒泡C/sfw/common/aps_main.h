#ifndef _APS_MAIN_H_
#define _APS_MAIN_H_


#include "mrc_base_i.h"
#ifdef __cplusplus
extern "C" {
#endif


#define APS_EXT_ID_MAINMODULE    1   
#define APS_EXT_ID_GRAPHICS      10008
#define APS_EXT_ID_COMMENT	     10045
#define APS_EXT_ID_APP           255
/***************************************************
开发者可为自己的插件定义除以上的插件ID外的任意ID，
遵循的原则是同一应用里用到的插件ID不可重复
 ***************************************************/

// 当app模块调用 mrc_exit()时，不是直接调用虚拟机的mrc_exit()，
//而是向主模块发送一个消息。
#define APS_MAIN_CODE_EXITAPP          0
#define APS_MAIN_CODE_RUNMRP           1
#define APS_MAIN_CODE_PAUSE           34
#define APS_MAIN_CODE_RESUME          35
#define APS_MAIN_CODE_REG_ENTRY       36
#define APS_MAIN_CODE_DUMP_LOAD           41
#define APS_MAIN_CODE_DUMP_UNLOAD         42
#define APS_MAIN_CODE_FWD_CALLBACK        44
#define APS_MAIN_CODE_SWITCH_CDR          45
#define APS_MAIN_CODE_RESTORE_CDR         46
#define APS_MAIN_CODE_PAUSE_EXCEPT        47
#define APS_MAIN_CODE_RESUME_EXCEPT       48
#define APS_MAIN_CODE_VM_MALLOC           180
#define APS_MAIN_CODE_VM_FREE             181

/* 获取某一个mrp的appid，appVer，shortName */
#define APS_MAIN_CODE_GETMRPINFO               201


typedef void (*loadExt_dump_cb)(int32 result);
#ifdef SDK_MOD
	typedef int32 (*mrc_init_t)(void);
	typedef int32 (*mrc_event_t)(int32 code, int32 param0, int32 param1);
	typedef int32 (*mrc_pause_t)(void);
	typedef int32 (*mrc_resume_t)(void);
	typedef int32 (*mrc_exitApp_t)(void);
	
	typedef struct
	{
		mrc_init_t		mrc_init_f;
		mrc_event_t		mrc_event_f;
		mrc_pause_t		mrc_pause_f;
		mrc_resume_t	mrc_resume_f;
		mrc_exitApp_t	mrc_exitApp_f;
		loadExt_dump_cb load_cb;/*加载失败时需要此回调*/
	}mrcEnterFuncSt;
#endif

#ifndef SDK_MOD
/*
  aps_main_loadExt_dump
  加载extID指定的插件
*/
#define aps_main_loadExt_dump(extID,extname,cb) \
	mrc_extSendAppEventEx(APS_EXT_ID_MAINMODULE, APS_MAIN_CODE_DUMP_LOAD, (int32)(extID),0,0,(int32)(extname),(int32)cb)
#else
#define aps_main_loadExt_dump(extID,extname,enterFunc) \
	mrc_extSendAppEventEx(APS_EXT_ID_MAINMODULE, APS_MAIN_CODE_DUMP_LOAD, (int32)(extID),0, 0,(int32)(extname),(int32)enterFunc)
#endif

/*卸载插件*/
#define aps_main_unloadExt_dump(extID) \
	mrc_extSendAppEventEx(APS_EXT_ID_MAINMODULE,APS_MAIN_CODE_DUMP_UNLOAD,(int32)(extID),0,0,0,0)


#define aps_main_pauseExt(extID) \
    mrc_extSendAppEventEx(APS_EXT_ID_MAINMODULE, APS_MAIN_CODE_PAUSE,(int32)(extID),0,0,0,0)

#define aps_main_resumeExt(extID) \
    mrc_extSendAppEventEx(APS_EXT_ID_MAINMODULE, APS_MAIN_CODE_RESUME,(int32)(extID),0,0,0,0)


#define aps_main_runMrpLater(mrp_name,parameter) \
	mrc_extSendAppEventEx(APS_EXT_ID_MAINMODULE,APS_MAIN_CODE_RUNMRP,(int32)(mrp_name),(int32)(parameter),0,0,0)

#define aps_main_switchCDR(oldDirBuffer) \
	mrc_extSendAppEventEx(APS_EXT_ID_MAINMODULE,APS_MAIN_CODE_SWITCH_CDR,(int32)(oldDirBuffer),0,0,0,0)

#define aps_main_restoreCDR(oldDirBuffer) \
	mrc_extSendAppEventEx(APS_EXT_ID_MAINMODULE,APS_MAIN_CODE_RESTORE_CDR,(int32)(oldDirBuffer),0,0,0,0)


/*暂停所有其他插件，除了extID之外*/
#define aps_main_pauseExt_except(extID) \
	mrc_extSendAppEventEx(APS_EXT_ID_MAINMODULE,APS_MAIN_CODE_PAUSE_EXCEPT,(int32)(extID),0,0,0,0)

/*恢复所有其他插件，除了extID之外 */
#define aps_main_resumeExt_except(extID) \
	mrc_extSendAppEventEx(APS_EXT_ID_MAINMODULE,APS_MAIN_CODE_RESUME_EXCEPT,(int32)(extID),0,0,0,0)

/*插件的入口函数原型*/
/* p0:  消息号 */
/* p1~p5: 参数 */
typedef int32 (*ExtEntry)(int32 p0, int32 p1, int32 p2, int32 p3, int32 p4, int32 p5);

/*注册插件的入口函数*/
#define aps_main_regEntry(selfExtid, entryAddr) \
    mrc_extSendAppEventEx(APS_EXT_ID_MAINMODULE, APS_MAIN_CODE_REG_ENTRY,(int32)(selfExtid),(int32)(entryAddr),0,0,0)

/* 获取某一个mrp的appid，appVer，shortName */
#define aps_main_getMrpInfo(packName,appID,appVer,shortName) \
	mrc_extSendAppEventEx(APS_EXT_ID_MAINMODULE, APS_MAIN_CODE_GETMRPINFO, (int32)packName, (int32)appID, (int32)appVer,(int32)shortName,0)
	


			
typedef struct _tagCBParam
{
	int32 p0;
	int32 p1; 
	int32 p2; 
	int32 p3; 
	int32 p4; 
	int32 p5;
}APS_CBParam;

/*************************************************************
  aps_main_fwd_cb用来委托主模块异步地调用回调函数,在调用此函数前
  应调用aps_main_unloadExt_dump
  func:   mpsFpFuncType                                    
  param:  APS_CBParam*    需要指向一块动态内存,在回调执行函数中
                          释放
*************************************************************/
#define aps_main_fwd_cb(func,param) \
	mrc_extSendAppEventEx(APS_EXT_ID_MAINMODULE,APS_MAIN_CODE_FWD_CALLBACK,(int32)func,(int32)param,0,0,0)

/***********************************************************************
aps_vm_malloc和aps_vm_free用来申请和释放采用DUMP装载内存情况下仍旧安全
的内存空间。

如A插件在释放自己之前可以使用aps_vm_malloc来申请一块内存，用来存放需要给
Game.Ext的信息，即使自身释放之后对Game.Ext来说，这块内存仍旧是安全的。

可以被用来进行aps_vm_malloc的内存非常有限,只能用来申请小块的内存。
*************************************************************************/
#define aps_vm_malloc(size)\
	(void*)mrc_extSendAppEventEx(APS_EXT_ID_MAINMODULE,APS_MAIN_CODE_VM_MALLOC,(int32)(size),0,0,0,0)

#define aps_vm_free(p)\
	mrc_extSendAppEventEx(APS_EXT_ID_MAINMODULE,APS_MAIN_CODE_VM_FREE,(int32)(p),0,0,0,0)


#ifdef __cplusplus
}
#endif

#endif

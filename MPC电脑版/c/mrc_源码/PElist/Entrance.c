/*
 * 应用程序入口代码
 * 版权所有 (C) 2012-2012 DeepBlue
 * [6/8/2012]
 */

#include <mrc_base.h>
#include <mrc_base_i.h>
#include <mrc_exb.h>

#include "AppTimer.h"
#include "KeyTimer.h"
#include "nv_base.h"
#include "nv_ui.h"
#include "AppMain.h"
//#include "AppSmallwin.h"
#include "MDskin.h"

#include "Entrance.h"

// logo启动图
static void logo(void)
{
	int32 w=0,h=0,get=0;

    mrc_bitmapNew(0, 240,320);
    mrc_bitmapLoad(0, "ebony.bmp", 0, 0, 240,320,240);

    mrc_bitmapNew(2, 147,53);
    mrc_bitmapLoad(2, "mdplorepe.bmp", 0, 0, 147,53,147);

	mrc_bitmapShow(0, mspw/2-120, msph/2-160, 2, 0, 0, 240,320);
	mrc_EffSetCon(0,0,240,320,200,200,200);

	mrc_bitmapShow(2, mspw/2-74, msph/2-40, 6, 0, 0, 147,53);

	txtwh("欢迎进入MDplorePE v1.0",&w,&h);
	txt("欢迎进入MDplorePE v1.0",(int16)(mspw/2-w/2),(int16)(msph/2-40+53+25),120,120,120);

	txtwh("DeepBlue at 2012",&w,&h);
	txt("DeepBlue at 2012",(int16)(mspw/2-w/2),(int16)(msph-h-5),80,80,80);

	fresht();

	for(get=0;get<=mspw;get+=5)
	{
		mrc_drawRect(0,msph-1,(int16)get,1,(uint8)get,(uint8)get,(uint8)get);
		fresht();
	}

	mrc_bitmapLoad(0, "*", 0, 0, 0, 0, 0);
	mrc_bitmapLoad(2, "*", 0, 0, 0, 0, 0);
}

// 应用初始化
static void GetView(void)
{
	if(mrc_fileState("MDs") == MR_IS_INVALID||mrc_fileState("MDs") == MR_IS_FILE)
	{
		mrc_mkDir("MDs");
		mrc_mkDir("MDs/plugins");
	}
	else
	{
		if(mrc_fileState("MDs/Plugins") == MR_IS_INVALID || mrc_fileState("MDs/Plugins") == MR_IS_FILE)
			mrc_mkDir("MDs/Plugins");
		if(mrc_fileState("MDs/Reader") == MR_IS_INVALID || mrc_fileState("MDs/Reader") == MR_IS_FILE)
			mrc_mkDir("MDs/Reader");
		if(mrc_fileState("MDs/Musicer") == MR_IS_INVALID || mrc_fileState("MDs/Musicer") == MR_IS_FILE)
			mrc_mkDir("MDs/Musicer");
	}
    mrc_bitmapNew(1, 25,33);
    mrc_bitmapLoad(1, "cim.bmp", 0, 0, 25,33,25);
}

void Entrance_Start(void)
{
#ifndef SDK_MOD
	SwitchDisk("C:/");
#endif
	GetView();
	
#ifndef SDK_MOD
	logo();
#endif

	MDskin_main("C:/","C:/",0);

	AppTimerReDo();	

	FileViewBrowser_Main();
}

void Entrance_Pause(void)
{

}

void Entrance_Resume(void)
{
	FileViewBrowser_UI();
}

void Entrance_ReleaseSource(void)
{

}

void Entrance_KeyCheck(int32 code, int32 p, int32 q)
{
	FileViewBrowser_Event( code, p, q);
}
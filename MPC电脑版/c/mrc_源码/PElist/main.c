#include <mrc_base.h>

#include "Entrance.h"
#include "nv_ui.h"

int32 MRC_EXT_INIT(void)
{
	nv_ui_init();
	return MR_SUCCESS;
}

int32 MRC_EXT_EXIT(void)
{	
	font_exit();
	return MR_SUCCESS;
}

int32 mrc_appEvent(int32 code, int32 param0, int32 param1)
{//计费状态下，自动返回
	Entrance_KeyCheck(code, param0, param1);
	return MR_SUCCESS;
}

int32 mrc_appPause()
{//计费状态下，自动返回
	Entrance_Pause();
	return MR_SUCCESS;	
}

int32 mrc_appResume()
{
	Entrance_Resume();
	return MR_SUCCESS;
}

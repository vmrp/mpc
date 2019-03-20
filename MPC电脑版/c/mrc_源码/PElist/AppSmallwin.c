/*
 * 浏览器小窗口代码
 * 版权所有 (C) 2012-2012 DeepBlue
 * [6/8/2012]
 */

#include <mrc_base.h>
#include <mrc_base_i.h>

#include "nv_ui.h"
#include "AppMain.h"

#include "AppSmallwin.h"

// 删除提示窗口
void RmSmallwin(void)
{
	svrect(2,msph/2-30,mspw-4,59,255,255,255,230,230,230,4);
	mrc_drawRect(6,msph/2-27,mspw-12,60-8,35,35,35);

	ysrect(7,msph/2+25-19,90,18,200,200,200,160,160,160);
	ysrect(5+mspw-12-90,msph/2+25-19,90,18,200,200,200,160,160,160);

	txt("确定删除已选择的文件/文件夹?",15,msph/2-27+10,200,200,200);

	txt("确定",7+45-12,msph/2+25-19+3,0,0,0);
	txt("取消",5+mspw-12-90+45-12,msph/2+25-19+3,0,0,0);

	fresh();
}

// 退出提示窗口
void ExitSmallwin(void)
{
	svrect(1,msph/2-34,mspw-2,68,255,255,255,230,230,230,4);
	mrc_drawRect(5,msph/2-30,mspw-10,60,35,35,35);

	yrect(7,msph/2+30-18,90,16,200,200,200);
	yrect(mspw-97,msph/2+30-18,90,16,200,200,200);

	srect(8,msph/2+30-17,88,14,190,190,190,180,180,180);
	srect(mspw-96,msph/2+30-17,88,14,190,190,190,180,180,180);

	mrc_drawRect(7,msph/2-13,100,1,100,100,100);

	txt("系统提示",7,msph/2-27,200,200,200);

	txt("确定退出MDplore?",15,msph/2-27+20,210,210,210);

	txt("确定",7+45-12,msph/2+30-17+1,0,0,0);
	txt("取消",mspw-96+45-12,msph/2+30-17+1,0,0,0);

	fresh();
}

// 删除提示窗口按键控制
void RmSmallwin_Event(int32 win,int32 code, int32 p, int32 q)
{
	if(win == APPWINDOWS_DELREAME)
	{
		if(code == MR_KEY_PRESS) // 选择列表项
		{
			switch(p)
			{
			case MR_KEY_SELECT:
			case MR_KEY_SOFTLEFT:
				trect(7,msph/2+30-18,90,16,180,180,180);	
				fresh();
				Noback();
				break;
			}
		}
	}
	else if(win == APPWINDOWS_EXITREAME)
	{
		if(code == MR_KEY_PRESS) // 选择列表项
		{
			switch(p)
			{
			case MR_KEY_SELECT:
			case MR_KEY_SOFTLEFT:
				trect(7,msph/2+30-18,90,16,180,180,180);	
				fresh();
				mrc_exit();
				break;
			}
		}
	}
	if(code == MR_KEY_PRESS) // 选择列表项
	{
		switch(p)
		{
		case MR_KEY_POUND:
		case MR_KEY_SOFTRIGHT:
			trect(mspw-97,msph/2+30-18,90,16,180,180,180);
			fresh();
			Noback();
			FileViewBrowser_UI();
			break;
		}
	}
}

/*
 * MD皮肤获取代码
 * 版权所有 (C) 2012-2012 DeepBlue
 * [6/8/2012]
 */

#include <mrc_base.h>
#include <mrc_base_i.h>

#include "nv_ui.h"
#include "nv_base.h"

#include "MDskin.h"

#define SkinbigLen 1024*5 // 皮肤大小限制最大

static char *buf=NULL; // 皮肤内存
static int number=0;

colorct MDskin_Getcolor(char *name)
{
	char qname[50]={0},*q=NULL,*w=NULL;
	colorct cvb;

	mrc_sprintf(qname,">%s[",name);
	q=strfind(buf,qname,"]");

	if(q != NULL)
	{
		w=strfindEx(q,qname,",");
		if(w != NULL)
		{
			cvb.r=(uint8)mrc_atoi(w);

			mrc_sprintf(qname,"%s%s,",qname,w);
			mrc_free(w);
	
			w=strfindEx(q,qname,",");
			if(w != NULL)
			{
				cvb.g=(uint8)mrc_atoi(w);
			
				mrc_sprintf(qname,"%s%s,",qname,w);
				mrc_free(w);
			
				w=strfindEx(q,qname,"]");
				if(w != NULL)
				{
					cvb.b=(uint8)mrc_atoi(w);
				}
			}
		}
		mrc_free(w);
	}
	mrc_free(q);

	return cvb;
}

void MDskin_main(char *oname,char *tname,int num)
{
	char bufs[30]={0},*bbd=NULL;
	int32 fd=0,len=0;

#ifndef SDK_MOD
	SwitchDisk(oname);
#endif
	number=num;

	mrc_sprintf(bufs,"MDs/MDs1000-%d.kin",number);

RET:if(mrc_fileState(bufs) != MR_IS_INVALID && mrc_fileState(bufs) != MR_IS_DIR)
	{
		len=mrc_getLen(bufs);
		fd=mrc_open(bufs,MR_FILE_RDONLY);
		if(fd != 0 && len < SkinbigLen)
		{
			buf=(char *)mrc_malloc(len+1);
			mrc_memset(buf,0,len+1);
			mrc_read(fd,buf,len);
		}
		mrc_close(fd);
	}
	else
	{
		bbd=mrc_readFileFromMrp("mds1000-0.kin",&len,0);

		fd=mrc_open(bufs,MR_FILE_CREATE | MR_FILE_WRONLY);
		if(fd != 0)
		{
			mrc_seek(fd,0,MR_SEEK_SET);
			mrc_write(fd,bbd,len);
		}
		mrc_close(fd);

		mrc_freeFileData(bbd,len);
		goto RET;
	}
#ifndef SDK_MOD
	SwitchDisk(tname);
#endif
}

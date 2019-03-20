/*
 * 字符串函数代码
 * 版权所有 (C) 2012-2012 DeepBlue
 * [6/8/2012]
 */

#include <mrc_base.h>
#include <mrc_base_i.h>
#include <mrc_exb.h>

#include "nv_base.h"

// 如:"C:/","Z:/"
void SwitchDisk(const char* strDisk)
{  
	unsigned char* output=NULL; 
	int32 output_len=0;

    mrc_platEx(1204, (uint8*)strDisk, mrc_strlen(strDisk), &output, &output_len, NULL );
}

// 指针偏移
char* strplen(char *text,int32 poss,int32 str_len)
{
	char *p=NULL;
	
	p=(char *)mrc_malloc(strlen(text)+1);
	mrc_memset(p,0,strlen(text)+1);
	mrc_memcpy(p,text+poss,str_len);

	return p;
}

// 获取指定磁盘整个文件
void* ReadAllex(const char* strDisk,const char* returnstrDisk,const char* filename)
{	
	void *txtbuf=NULL;
	int32 ret=0,filelen=0;

	SwitchDisk(strDisk);
	
	ret=mrc_open(filename,MR_FILE_RDONLY);
	if(ret!=0)
	{
		filelen=mrc_getLen(filename)+1;
		txtbuf=(void *)mrc_malloc(filelen);	
		mrc_memset(txtbuf,0,filelen);
		mrc_read(ret,txtbuf,filelen-1);
	}

	SwitchDisk(returnstrDisk);

	return txtbuf;
}

// 字符查找,包括查找字符
char *strfind(char *text,char *s,char *t)
{
	int32 textlen=0;
	char *p=NULL,*n=NULL,*ts=NULL,*ftext=NULL;

	p=mrc_strstr(text,s);// Find s in text
	if(p==NULL)
		return p;
	ftext=strplen(p,mrc_strlen(s),mrc_strlen(p)-mrc_strlen(s));// Poss get next text

	n=mrc_strstr(ftext,t);// Find t in text
	if(n==NULL)
		return n;
	textlen=mrc_strlen(ftext)-mrc_strlen(n)+mrc_strlen(s);//Get mallloc txtlen

	ts=(char *)mrc_malloc(textlen+mrc_strlen(t)+1);
	mrc_memset(ts,0,textlen+mrc_strlen(t)+1);

	mrc_memcpy(ts,p,textlen);// Get txtlen p
	mrc_strcat(ts,t);// Get t

	mrc_free(ftext);// Free ftext internal storage

	return ts;
}

// 字符查找,返回内容,不包括查找字符
char *strfindEx(char *text,char *s,char *ts)
{
	char *p=NULL,*t=NULL;

	p=strfind(text,s,ts);
	if(p==NULL)
		return p;

	t=strplen(p,mrc_strlen(s),mrc_strlen(p)-mrc_strlen(s)-mrc_strlen(ts));
	mrc_free(p);

	return t;
}

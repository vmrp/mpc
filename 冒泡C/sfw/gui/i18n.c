/*
** i18n.c: the implements of i18n.h
** 
** Copyright (C) 2007-2008 SKY-MOBI AS.  All rights reserved.
**
** Create date: 2007-12-27 by wandonglin
**
** This file is part of the simple gui library.
** It may not be redistributed under any circumstances.
*/

#include "i18n.h"


#define RES_FILENAME	"res_lang%d.rc"
#define MAGIC_CODE		0x0774660D


//文件头
typedef struct RES_FILE_HEADER_T
{
	DWORD magic;		//代码
	DWORD appid;		//应用程序ID
	DWORD version;		//该资源的版本
	DWORD strings;		//字符串总数
	DWORD reserve1;		//预留空间
	DWORD reserve2;		//预留空间
	BYTE supprts[8];	//one language has a bit information in this array一个语言有一个在这个数组位信息
	DWORD count; //the language contains in this file/的语言在这个文件中包含
	//DWORD offset[]; //the offset for the language/使用的语言抵消

}RES_FILE_HEADER;


//语言片段头

typedef struct RES_LANG_HEADER_T
{
	DWORD id;			//语言id从0号开始
	DWORD size;			//将字符串数据的大小
	DWORD checksum;		//字符串数据的校验
	DWORD reserve;		//预留空间

}RES_LANG_HEADER;


//语言表

static struct LANGUAGE_T
{
	const Uint16* offset;	//数据偏移
	const Uint8* data;		//数据

}languages[SGL_MAX_LNG_COUNT];


//局部变量

static DWORD cur_lang;

//////////////////////////////////////////////////////////////////////////


#if BIG_ENDIAN
static VOID
I18n_BigEndian2(Uint16* pArr, DWORD size)
{
	DWORD i;
	PBYTE pb;
	BYTE tmp;
	
	for(i = 0; i < size; i++)
	{
		pb = (PBYTE)&pArr[i];
		tmp = pb[0];
		pb[0] = pb[1];
		pb[1] = tmp;
	}
}


static VOID 
I18n_BigEndian4(DWORD* pArr, DWORD size)
{
	DWORD i;
	PBYTE pb;
	BYTE tmp;
	
	for(i = 0; i < size; i++)
	{
		pb = (PBYTE)&pArr[i];
		tmp = pb[0];
		pb[0] = pb[3];
		pb[3] = tmp;
		tmp = pb[1];
		pb[1] = pb[2];
		pb[2] = tmp;
	}
}
#endif


VOID GUIAPI
SGL_I18nInitialize(VOID)
{
	//设置语言为中文
	SGL_SetCurrentLanguage(1000);
}


BOOL GUIAPI
SGL_SetCurrentLanguage(DWORD lang)
{
	lang -= MR_PLAT_VALUE_BASE;
	
	if(lang == cur_lang || lang >= SGL_MAX_LNG_COUNT) 
		return FALSE;

	cur_lang = lang;
	return TRUE;
}


DWORD GUIAPI
SGL_GetCurrentLanguage(VOID)
{
	return cur_lang + MR_PLAT_VALUE_BASE;
}

#ifdef RES_LOAD_FILENAME
static BOOL 
SGL_CheckFileHeader(RES_FILE_HEADER* pHeader, DWORD lang)
{
	mrc_appInfoSt * pAppInfo = mrc_getAppInfo();
	
	if(pHeader->magic != MAGIC_CODE
		|| pHeader->appid != (DWORD)pAppInfo->id 
		|| pHeader->strings != RES_STRING_COUNT
		|| pHeader->count == 0 
		|| pHeader->count > SGL_MAX_LNG_COUNT
		|| !(pHeader->supprts[lang/8] & (1 << lang%8)))
		return FALSE;

	return TRUE;
}


static DWORD
SGL_GenerateCheckSum(PBYTE pData, DWORD size)
{
	DWORD i = 0;
	DWORD checksum = 0;

	for(i = 0; i < size; i++)
		checksum += pData[i];

	return checksum;
}
#endif

static BOOL
SGL_LoadResourceFromMrp(DWORD lang)
{
	char filename[24];
	uint8* pBuf = NULL;
	int32 len = 0;

	SGL_SPRINTF(filename, RES_FILENAME, lang);
	if(MR_SUCCESS != mrc_readFileFromMrpEx(NULL, filename, (uint8**)&pBuf, &len, 0))
	{
	///	SGL_TRACE("Failed to read resource: %s\n", filename);
		return FALSE;
	}

#if BIG_ENDIAN
	I18n_BigEndian2((Uint16*)pBuf, RES_STRING_COUNT);
#endif

	languages[lang].offset = (Uint16*)pBuf;
	languages[lang].data = pBuf + RES_STRING_COUNT*2;
	return TRUE;
}


#ifdef RES_LOAD_FILENAME
static BOOL
SGL_LoadResourceFromFile(PCSTR filename, DWORD lang)
{
	int32 stream;
	BOOL r = FALSE;
	RES_FILE_HEADER filehdr;
	RES_LANG_HEADER langhdr;	
	DWORD offset[SGL_MAX_LNG_COUNT];
	PBYTE pBuf = NULL;

	if(0 == (stream = mrc_open(filename, MR_FILE_RDONLY)))
	{
		SGL_TRACE("%s, %d: failed to open file : %s\n", __FILE__, __LINE__, filename);
		return FALSE;
	}

	if(sizeof(RES_FILE_HEADER) != mrc_read(stream, &filehdr, sizeof(RES_FILE_HEADER))
		|| !SGL_CheckFileHeader(&filehdr, lang))
	{
		SGL_TRACE("%s, %d: failed to read file header\n", __FILE__, __LINE__);
		goto loadfile_error;
	}

#if BIG_ENDIAN
	I18n_BigEndian4((DWORD*)pBuf, 4);
	I18n_BigEndian4((DWORD*)&filehdr.count, 1);
#endif

	if(filehdr.version <= RES_VERSION)
	{
		SGL_TRACE("%s, %d: small resource version \n", __FILE__, __LINE__);
		goto loadfile_error;
	}

	if((int32)(filehdr.count*sizeof(DWORD)) != mrc_read(stream, offset, filehdr.count*sizeof(DWORD)))
	{
		SGL_TRACE("%s, %d: failed to load offset information \n", __FILE__, __LINE__);
		goto loadfile_error;
	}

#if BIG_ENDIAN
	I18n_BigEndian4((DWORD*)offset, filehdr.count);
#endif

	if(MR_SUCCESS != mrc_seek(stream, sizeof(RES_FILE_HEADER) + sizeof(DWORD)*filehdr.count + offset[lang], MR_SEEK_SET))
	{
		SGL_TRACE("%s, %d: failed to seek \n", __FILE__, __LINE__);
		goto loadfile_error;
	}
	
	if(sizeof(RES_LANG_HEADER) != mrc_read(stream, &langhdr, sizeof(RES_LANG_HEADER)))
	{
		SGL_TRACE("%s, %d: failed to read language header \n", __FILE__, __LINE__);
		goto loadfile_error;
	}
	
#if BIG_ENDIAN
	I18n_BigEndian4((DWORD*)pBuf, 4);
#endif

	pBuf = (PBYTE)SGL_MALLOC(sizeof(Uint16) * RES_STRING_COUNT + langhdr.size);
	if(pBuf == NULL)
	{
		SGL_TRACE("%s, %d: failed to malloc memory \n", __FILE__, __LINE__);
		goto loadfile_error;
	}
	
	if((int32)(sizeof(Uint16) * RES_STRING_COUNT + langhdr.size) != mrc_read(stream, pBuf, sizeof(Uint16) * RES_STRING_COUNT + langhdr.size))
	{
		SGL_TRACE("%s, %d: failed to read language data \n", __FILE__, __LINE__);
		SGL_FREE(pBuf);
		goto loadfile_error;
	}

	if(langhdr.checksum != SGL_GenerateCheckSum(pBuf + RES_STRING_COUNT*sizeof(Uint16), langhdr.size))
	{
		SGL_TRACE("%s, %d: buffer length error\n", __FILE__, __LINE__);
		SGL_FREE(pBuf);
		goto loadfile_error;
	}	

#if BIG_ENDIAN
	I18n_BigEndian2((Uint16*)pBuf, RES_STRING_COUNT);
#endif

	languages[lang].offset = (Uint16*)pBuf;
	languages[lang].data = pBuf + sizeof(Uint16) * RES_STRING_COUNT;
	r = TRUE;
	
loadfile_error:
	if(stream) mrc_close(stream);
	return r;
}

#endif


BOOL GUIAPI 
SGL_LoadResource(DWORD lang)
{	
#ifdef RES_LOAD_FILENAME
    //尝试从文件中加载资源	
	if(SGL_LoadResourceFromFile(RES_LOAD_FILENAME, lang))
		return TRUE;
#endif
	
	//load from mrp
	return SGL_LoadResourceFromMrp(lang);
}


BOOL GUIAPI
SGL_ReleaseResource(DWORD lang)
{
	lang -= MR_PLAT_VALUE_BASE;

	if(languages[cur_lang].offset)
	{
		SGL_FREE((void*)languages[lang].offset);		
		languages[lang].data = NULL;
		languages[lang].offset = NULL;
		return TRUE;
	}

	return FALSE;
}


PCWSTR GUIAPI
SGL_LoadString(DWORD str)
{
	if(str >= RES_STRING_COUNT) 
		return NULL;

	if(!languages[cur_lang].offset)
	{
		if(!SGL_LoadResource(cur_lang))
			return NULL;
	}
	
	return (PCWSTR)(languages[cur_lang].data + languages[cur_lang].offset[str]);
}

/*
** sound.c: implement of sound.h
** 
** Copyright (C) 2007-2008 SKY-MOBI AS.  All rights reserved.
**
** Create date: 2008-01-17 by wandonglin
**
** This file is part of the simple gui library.
** It may not be redistributed under any circumstances.
*/

#include "sound.h"

#include "res_sound.rc"

typedef struct SOUND_DATA_T{
	int32 len;
	VOID* data;
}SOUND_DATA;

 static SOUND_DATA soundDatas[RES_SOUND_COUNT];

////////////////////////////////////////////////////////////////////////////////////////

BOOL GUIAPI
SGL_PlaySound(DWORD sound, BOOL loop)
{
	if(sound >= RES_SOUND_COUNT)
		return FALSE;
	
	if(!soundDatas[sound].data)
		 mrc_readFileFromMrpEx(NULL, soundsInfo[sound].filename, (uint8**)&soundDatas[sound].data, &soundDatas[sound].len, 0);

	if(soundDatas[sound].data)
		return mr_playSoundEx(soundsInfo[sound].type, soundDatas[sound].data, soundDatas[sound].len, loop) == MR_SUCCESS;

	return FALSE;
}


BOOL GUIAPI 
SGL_StopSound(DWORD sound)
{
	if(sound < RES_SOUND_COUNT)
		return mr_stopSoundEx(soundsInfo[sound].type) == MR_SUCCESS;

	return FALSE;
}


VOID GUIAPI 
SGL_ReleaseSound(DWORD sound)
{
	if(sound >= RES_SOUND_COUNT)
		return;

	if(soundDatas[sound].data)
	{
		SGL_FREE(soundDatas[sound].data);
		soundDatas[sound].data = NULL;
	}
}


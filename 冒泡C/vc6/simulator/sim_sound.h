#ifndef __SOUND_H__
#define  __SOUND_H__
extern "C"
{
	#include "mrporting.h"	
}
#include "mmsystem.h" //mciSendCommand 头文件，添加winmm.lib

#include <dmusici.h>//direct
#include <dshow.h>//direct

#define ACI_MP3_INIT    2013
#define ACI_MP3_FILE    2023
#define ACI_MP3_STREAM    2033
#define ACI_MP3_PALY    2043
#define ACI_MP3_PAUSE    2053
#define ACI_MP3_RESUME   2063
#define ACI_MP3_STOP   2073
#define ACI_MP3_CLOSE   2083
#define ACI_MP3_STATUS   2093
#define ACI_MP3_SETTIME  2103
#define ACI_MP3_SETLEN  2113
#define ACI_MP3_TOTALTIME 2123
#define ACI_MP3_PLAYTIME  2133
#define ACI_MP3_PLAYLEN  2143

#define ACI_WAV_INIT    2012
#define ACI_WAV_FILE    2022
#define ACI_WAV_STREAM    2032
#define ACI_WAV_PALY    2042
#define ACI_WAV_PAUSE    2052
#define ACI_WAV_RESUME   2062
#define ACI_WAV_STOP   2072
#define ACI_WAV_CLOSE   2082
#define ACI_WAV_STATUS   2092
#define ACI_WAV_SETTIME  2102
#define ACI_WAV_SETLEN  2112
#define ACI_WAV_TOTALTIME 2122
#define ACI_WAV_PLAYTIME  2132


#define ACI_MID_INIT    2011
#define ACI_MID_FILE    2021
#define ACI_MID_STREAM    2031
#define ACI_MID_PALY    2041
#define ACI_MID_PAUSE    2051
#define ACI_MID_RESUME   2061
#define ACI_MID_STOP   2071
#define ACI_MID_CLOSE   2081
#define ACI_MID_STATUS   2091
#define ACI_MID_SETTIME  2101
#define ACI_MID_SETLEN  2111
#define ACI_MID_TOTALTIME 2121
#define ACI_MID_PLAYTIME  2131
#define  MR_PLAT_VALUE_BASE 1000
typedef enum tagT_MEDIA_STAT
{
	MEDIA_IDLE = 1+MR_PLAT_VALUE_BASE,
		MEDIA_INITED,
		MEDIA_LOADED,
		MEDIA_PLAYING,
		MEDIA_PAUSED,
		MEDIA_SUSPENDED,        
		MEDIA_SUSPENDING
}T_MEDIA_STAT;
typedef struct
{
	int32 pos;
}T_MEDIA_SEEK;

int32 plagMusic(int32 code, uint8* input, int32 input_len, uint8** output, int32* output_len, MR_PLAT_EX_CB *cb);
void closeMusic(void);
MCI_OPEN_PARMS* getMCI_OPEN_PARMS();
MCI_PLAY_PARMS* getMCI_PLAY_PARMS();
uint32 get_info(uint32 item,int32 flag);
 bool PlayMIDI(unsigned char* data,unsigned int size,int32 loop) ;
  void StopMIDI() ;
#endif























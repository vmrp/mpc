#include <sound.h>
enum
{
_MIDI=1, _WAVE, _MP3, _AMR, _PCM, _M4A, _AMR_WB
};

typedef struct
{
 int32 pos;
}T_DSM_AUDIO_POS;


struct
{
// uint32 timer;//定时器句柄
 char *filename;//文件路径
 uint32 type;//播放设备
 uint8 state;//播放状态
 uint8 block;//播放模式
 uint8 volume;//音量0-5
 uint8 loop;//播放请求类型
 //uint32 tolaltime;//音乐总时间
// uint32 curtime;//当时播放时间
}PALY_MUSIC;

uint32 GetMusicKind(char *filename)
{
 return _MP3;
}
;
void PalyMusic(char *filename)
{
 PALY_MUSIC.type=GetMusicKind(filename);
PALY_MUSIC.state=1;
PALY_MUSIC.block=0;
PALY_MUSIC.loop=0;
PALY_MUSIC.filename=filename;
PALY_MUSIC.volume=1;


soundinit(PALY_MUSIC.type);
soundloadfile(PALY_MUSIC.type,PALY_MUSIC.filename);
soundplay(PALY_MUSIC.type,PALY_MUSIC.block,PALY_MUSIC.loop);
setvolume(PALY_MUSIC.volume);



}

void StopPaly()
{
 if(PALY_MUSIC.state)
 soundstop(PALY_MUSIC.type);
 soundclose(PALY_MUSIC.type);

}


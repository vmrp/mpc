/*
* MusicPlay(简单音乐播放器)
* By:天天
* Date:2012.8.28
* 接口:
 初始化 MPInit(char *name);
 按键入口 MPEvent(int type,int p1,int p2);
* 帮助:上(2)、下(8)键加减音量，中心(5)键 暂停/播放、0键停止播放。
*/

#include <sound.h>
//字体
#define MFONT 1

enum
{
_MIDI=1, _WAVE, _MP3, _AMR, _PCM, _M4A, _AMR_WB
};

typedef struct
{
 int32 pos;
}T_DSM_AUDIO_POS;

//播放状态
enum
{
Is_Play=0,//播放中
Is_Pause,//暂停
Is_Stop//停止
};

//播放状态字符串
char* GetPlayStateStr(int type)
{
char *PlayStateStr[3]={"播放中","暂停","停止"};
 return PlayStateStr[type];
}

struct PMusic
{
 int timer;//定时器句柄
 char *name;//文件路径
 int8 type;//播放请求类型
 int8 state;//播放状态
 int8 block;//播放(循环)模式
 int8 volume;//音量(0~5)
 int8 loop;//播放循环模式
 int totaltime;//音乐总时间
 int curtime;//当时播放时间
};

//获取音乐文件类型
int GetMusicKind(char *FileName)
{
 char *str=strrchr(FileName,'.');
 switch(0)
 {
 case strcmp(str,".mp3"):
 return _MP3;
 break;
 case strcmp(str,".amr"):
 return _AMR;
 break;
 case strcmp(str,".mid"):
 return _MIDI;
 break;
 case strcmp(str,".wave"):
 return _WAVE;
 break;
 case strcmp(str,".m4a"):
 return _M4A;
 break;
 case strcmp(str,".pcm"):
 return _PCM;
 break;
 }
 return -1;
}

struct PMusic *PM;

/*初使化入口,FileName:音乐文件名，成功返回0，失败返回-1*/
int MPInit(char *FileName)
{
 if(getlen(FileName)<0)
 return -1;
 int t=GetMusicKind(FileName);
 if(-1==t)
 return -1;

 int l=sizeof(struct PMusic);
 PM=(struct PMusic*)malloc(l);
 memset(PM,NULL,l);

 int len=strlen(FileName);
 PM->name=(char*)malloc(len);
 strcpy(PM->name,FileName);

 PM->timer=timercreate();
 PM->type=t;
 PM->block=0;
 PM->loop=1;
 PM->state=Is_Play;
 PM->volume=1;
 soundinit(PM->type);//打开播放设备
soundloadfile(PM->type,PM->name);//加载文件
 soundplay(PM->type,PM->block,PM->loop);//播放
 PM->totaltime=GetMusicTime(PM->type,1);//音乐总时间
 setvolume(PM->volume);//设置音量
 timerstart(PM->timer,500,500,"PMtimer",1);
 return 0;
}

//退出
int PMExit()
{
 timerdel(PM->timer);
 soundstop(PM->type);//停止
 soundclose(PM->type);//关闭播放设备
 free(PM->name);
 free(PM);
 BackFV();
 return 0;
}

//定时器会调函数
void PMtimer(int p1)
{
 PM->curtime=GetMusicTime(PM->type,0);//获取播放时间
 PMDraw();
}

//画播放界面
void PMDraw()
{
 cls(120,245,230);
 char *str=(char*)malloc(200);
 sprintf(str,"音乐播放器\n%s\n%d:%d %d:%d\n%s 音量:%d",PM->name,(PM->curtime)/60,(PM->curtime)%60,(PM->totaltime)/60,(PM->totaltime)%60,GetPlayStateStr(PM->state),PM->volume);

 colorst *clr=malloc(3);
 memset(clr,0,3);
 rectst *rect=malloc(8);
 memset(rect,0,8);
 rect->w=SCRW;
 rect->h=SCRH;
 dtextex(str,0,0,rect,clr,2,MFONT);
 free(str);
 free(clr);
 free(rect);
 ref(0,0,SCRW,SCRH);
}

/*获取播放时间(秒S)
* type:设备类型
* is_alltime:取真值返回播放总时间、否则返回已播放时间
*/
int GetMusicTime(int32 type,int is_alltime)
{
 uint8 *p;
 if(is_alltime)
 getsoundtotaltime(type,&p);
 else
 getsoundcurtime(type,&p);
 if(p==NULL)return 0;
 return (int)(*p);
}

//按键入口
int MPEvent(int type,int p1,int p2)
{
 if(KY_DOWN==type)
 switch(p1)
 {
 case _SRIGHT:
 PMExit();
 break;

 case _0:
 if(PM->state!=Is_Stop)
 {
 soundstop(PM->type);
 PM->state=Is_Stop;
 timerstop(PM->timer);
 PM->curtime=0;
 PMDraw();
 }
 break;

 case _5:
 case _SELECT:
 if(PM->state==Is_Stop)
 {
 soundplay(PM->type,PM->block,PM->loop);
 timerstart(PM->timer,500,500,"PMtimer",1);
 PM->state=Is_Play;
 }
 else
 if(PM->state==Is_Pause)
 {
 PM->state=Is_Play;
 soundresume(PM->type);
 timerstart(PM->timer,500,500,"PMtimer",1);
 }
 else
 {
 soundpause(PM->type);
 timerstop(PM->timer);
 PM->state=Is_Pause;
 }
 PMDraw();
 break;

 case _2:
 case _UP:
 if(PM->volume<5)
 {
 PM->volume++;
 setvolume(PM->volume);
 PMDraw();
 }
 break;

 case _8:
 case _DOWN:
 if(PM->volume>0)
 {
 PM->volume--;
 setvolume(PM->volume);
 PMDraw();
 break;
 }

 }
 return 0;
}


//设置播放时间(ms)
void SetPlayTime(int type,int time)
{
 T_DSM_AUDIO_POS *p=malloc(sizeof(T_DSM_AUDIO_POS));
 p->pos=time;
 setplaytime(type,p);
 free(p);
}

//By:天天
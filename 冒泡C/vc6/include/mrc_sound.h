#ifndef _MRC_SOUND_H_
#define _MRC_SOUND_H_



#define ACI_MIDI_DEVICE 1
#define ACI_WAVE_DEVICE 2
#define ACI_MP3_DEVICE 3
#define ACI_AMR_DEVICE 4
#define ACI_PCM_DEVICE 5
#define ACI_M4A_DEVICE 6
#define ACI_AMR_WB_DEVICE 7


typedef void (*ACI_PLAY_CB)(int32 result);

typedef struct
{
    int32 pos;
}T_DSM_AUDIO_POS;

/*
	每次播放前需调用
	[in]:播放设备类型ACI_MIDI_DEVICE－ACI_AMR_WB_DEVICE
	[return]:	MR_SUCCESS 初始化成功
			MR_FAILED 初始化失败
			MR_IGNORE 不支持该功能
*/
int32 mrc_playSoundExInit(int32 type);
/*
	加载音频文件
	注意:该方法下resume时手机会有问题，后续移植版本解决，建议将
	文件写入T卡然后mrc_playSoundExLoad加载
	[in]
	type:设备类型
	data:输入数据
	buf_len:数据长度
	[return]:	MR_SUCCESS 初始化成功
			MR_FAILED 初始化失败
			MR_IGNORE 不支持该功能
*/
int32 mrc_playSoundExLoad(int32 type, const void * data, int32 buf_len);
/*
	加载音频文件
	[in]
	type:设备类型
	filename:数据文件名称，相对路径，GB编码，这里注意的是从mrp外部读取文件
	[return]:	MR_SUCCESS 初始化成功
			MR_FAILED 初始化失败
			MR_IGNORE 不支持该功能
*/
int32 mrc_playSoundExLoadFile(int32 type, char* filename);
/*
	播放音频文件
	[in]
	type:设备类型
	block:1：阻塞式向底层发送播放请求；0：非阻塞凡是向底层发送播放请求
	loop:0：单次播放；1：循环播放；2：nes pcm 播放方式
	cb:回调函数  ,无则填NULL
	[return]:	MR_SUCCESS 初始化成功
			MR_FAILED 初始化失败
			MR_IGNORE 不支持该功能
*/
int32 mrc_playSoundEx(int32 type, int32 block, int32 loop, ACI_PLAY_CB cb);

/*
	暂停播放音频文件
	[in]
	type:设备类型
	[return]:	MR_SUCCESS 初始化成功
			MR_FAILED 初始化失败
			MR_IGNORE 不支持该功能
*/
int32 mrc_pauseSoundEx(int32 type);
/*
	继续播放音频文件
	[in]
	type:设备类型
	[return]:	MR_SUCCESS 初始化成功
			MR_FAILED 初始化失败
			MR_IGNORE 不支持该功能
*/
int32 mrc_resumeSoundEx(int32 type);
/*
	停止播放音频文件
	[in]
	type:设备类型
	[return]:	MR_SUCCESS 初始化成功
			MR_FAILED 初始化失败
			MR_IGNORE 不支持该功能
*/
int32 mrc_stopSoundEx(int32 type);
/*
	关闭设备
	[in]
	type:设备类型
	[return]:	MR_SUCCESS 初始化成功
			MR_FAILED 初始化失败
			MR_IGNORE 不支持该功能
*/
int32 mrc_closeSoundEx(int32 type);
/*
	音量调节
	[in]
	volume:音量大小0~5
	[return]:	MR_SUCCESS 初始化成功
			MR_FAILED 初始化失败
			MR_IGNORE 不支持该功能
*/
int32 mrc_setVolume(int32 volume);

/*
	获取音乐的总时间秒S
	[out]
	p:指向这个数据结构（T_DSM_AUDIO_POS）的指针的指针，
	这个数据结构的值就是总的时间
	[return]:	MR_SUCCESS 初始化成功
			MR_FAILED 初始化失败
			MR_IGNORE 不支持该功能

	注：mtk上此接口在以文件形式播放的时候有效。
*/
int32 mrc_getSoundTotalTime(int32 type, uint8** p);

/*
	获取当前已经播放的时间秒S
	[out]
	p:指向这个数据结构（T_DSM_AUDIO_POS）的指针的指针，
	这个数据结构的值就是已经播放的时间
	[return]:	MR_SUCCESS 初始化成功
			MR_FAILED 初始化失败
			MR_IGNORE 不支持该功能

	注：mtk上此接口在以文件形式播放的时候有效。
*/

int32 mrc_getSoundCurTime(int32 type, uint8** p);


/*
	获取当前已经播放的时间毫秒ms
	[out]
	p:指向这个数据结构（T_DSM_AUDIO_POS）的指针的指针，
	这个数据结构的值就是已经播放的时间
	[return]:	MR_SUCCESS 初始化成功
			MR_FAILED 初始化失败
			MR_IGNORE 不支持该功能

	注：mtk上此接口在以文件形式播放的时候有效。
*/

int32 mrc_getSoundCurTimeMs(int32 type, uint8** p);

/*
设置播放位置，相对文件或者缓冲的起始位置的偏移量
［IN］
	SOUND_POS pos：相对文件或者缓冲的起始位置的偏移量
［return］
	MR_SUCCESS	成功
	MR_FAILED	失败
	MR_IGNORE   不支持该功能
*/
int32 mrc_setPlayPos(int32 type,T_DSM_AUDIO_POS pos);

/*
设置播放时间，相对文件或者缓冲的起始时间的偏移量
［IN］
	SOUND_POS pos：相对文件或者缓冲的起始时间的偏移量，单位：ms
［return］
	MR_SUCCESS	成功
	MR_FAILED	失败
	MR_IGNORE   不支持该功能
*/
int32 mrc_setPlayTime(int32 type,T_DSM_AUDIO_POS pos);

/*
获取当前设备的状态
［IN］
	无
［return］
	设备状态值  成功
	MR_FAILED	失败
	MR_IGNORE   不支持该功能

*/
int32 mrc_getDeviceState(int32 type);
#endif

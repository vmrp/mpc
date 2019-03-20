#ifndef __MR_HELPER_WIN_H__
#define  __MR_HELPER_WIN_H__

/*
本文件以及对应的C代码文件，对mythroad提供的基本函数
进行了封装，提供一种基本的窗体架构。
*/



#define MAX_WIN_NO       30

typedef enum
{
	WIN_EVENT_SHOW,                  //窗口创建好之后就会收到这个事件，可以在这个事件中画窗口的界面
	WIN_EVENT_PAUSE,                 //当创建一个新的窗口后，父窗口会收到这个事件
	WIN_EVENT_REFRESH,               //当子窗口销毁掉后，父窗口会收到这个事件
	WIN_EVENT_UPDATE,               //外部可以修改窗体中要显示的功能，然后向该窗体发送这个事件使窗体将更新的内容画到屏幕上
	WIN_EVENT_EXIT                     //调用winDelete后，系统会向将要销毁的窗口发送这个事件，已处理窗口特殊的退出处理
}T_WIN_EVENT;


/*
窗体事件回调

输入:
data          窗体数据
eventId     窗体事件
      WIN_EVENT_SHOW,                  //窗口创建好之后就会收到这个事件，
                                                        可以在这个事件中画窗口的界面
      WIN_EVENT_PAUSE,                 //当创建一个新的窗口后，父窗口会收
                                                         到这个事件
      WIN_EVENT_REFRESH,               //当子窗口销毁掉后，父窗口会收到这
                                                         个事件
      WIN_EVENT_UPDATE,               //外部可以修改窗体中要显示的功能，
                                                         然后向该窗体发送这个事件使窗体将
                                                         更新的内容画到屏幕上
      WIN_EVENT_EXIT                     //调用winDelete后，系统会向将要销毁的窗
                                                         口发送这个事件，已处理窗口特殊的
                                                         退出处理
*/
typedef void (*PtrWinEvent)(int32 data,int32 eventId); 

/*
窗体按键事件回调

输入:
data                                 窗体数据
code, param0, param1       与mrc_event函数的code, param0, param1参数含义相同

*/
typedef void (*PtrKeyEvent)(int32 data, int32 code,int32 param0,int32 param1);


typedef struct
{
	int32 id;
	PtrKeyEvent keyCb;
	PtrWinEvent winCb;
	int32 data;
}T_FRAME_WIN;

/*
窗体架构初始化
*/
void mrc_winInit(void);

/*
函数为本窗体框架的事件驱动函数，由外部在收到事件时
调用，一般在mrc_event中调用该函数以驱动窗体框架。

创建自定义窗体时，注册了两个窗口函数，一个是键盘事件
处理函数，一个是窗体事件处理函数。而这里提供的两个函数
mrc_winKeyEvent和mrc_winWinEvent分别用于触发键盘事件处理函数和窗体
事件处理函数。
	mrc_winEvent()函数设计时只实现了mrc_winKeyEvent的功能，日后将被删除，
	不建议再继续使用，留着主要用于保持现有代码的兼容性。

输入:
type,p1,p2     与mrc_event函数的code, param0, param1参数含义相同
eventId 的有效取值范围是该窗体的窗体处理函数中定义。

返回:
      MR_SUCCESS     成功
      MR_FAILED         失败
*/
int32 mrc_winEvent(int32 type, int32 p1, int32 p2);
int32 mrc_winKeyEvent(int32 type, int32 p1, int32 p2);
int32 mrc_winWinEvent(int32 eventId);
/*
函数为本窗体框架的恢复(被暂停的应用的)驱动函数，由外
部在收到恢复事件时调用，一般在mrc_resume中调用该函数以
驱动窗体框架
*/
void mrc_winResume(void);

/*
新建窗体

输入:
data          窗体数据
winEvent   窗体事件处理函数
keyEvent   按键事件处理函数

返回:
      >=0                   窗体id
      MR_FAILED         失败
*/
int32 mrc_winNew(int32 data,PtrWinEvent winEvent,PtrKeyEvent keyEvent);

/*
关闭最上层的窗体

返回:
      MR_SUCCESS     成功
      MR_FAILED         失败
*/
int32 mrc_winClose(void);


/*
关闭最上层的窗体，不刷新其父窗体(即下一层窗体)

返回:
      MR_SUCCESS     成功
      MR_FAILED         失败
*/
int32 mrc_winCloseNotShow(void);

/*
通过窗体id关闭窗体

输入:
id                  窗体id

返回:
      MR_SUCCESS     成功
      MR_FAILED         失败
*/
int32 mrc_winCloseById(int32 id);

/*
通过窗体id取得窗体数据

输入:
id                  窗体id

输出:
data              窗体数据

返回:
      MR_SUCCESS     成功
      MR_FAILED         失败
*/
int32 mrc_getWinDataById(int32 id, int32* data);

/*
取得最上层窗体(活跃窗体)的窗体id

返回:
      >=0                   窗体id
      MR_FAILED         失败
*/
int32 mrc_getActiveWinId(void);

/*
取得最上层窗体(活跃窗体)的窗体数据

输出:
data              窗体数据

返回:
      MR_SUCCESS     成功
      MR_FAILED         失败
*/
int32 mrc_winGetActiveWinData(int32* data);

/*
向窗体发送窗体事件

输入:
winId                  窗体id
eventId              窗体事件

返回:
      MR_SUCCESS     成功
      MR_FAILED         失败
*/
int32 mrc_sendEventTo(int32 winId,int32 eventId);

/*
检查窗体是否已经创建

输入:
winId                  窗体id

返回:
      TRUE          窗体已经创建
      FALSE         窗体尚未创建
*/
int32 mrc_isTheWinCreated(int32 winId);


int32 getVMStat(void);
int32 setVMStat(int32 stat);
int32 isVMIn(void);

#endif


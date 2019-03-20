#ifndef _MYADV_H_
#define _MYADV_H_

#ifdef __cplusplus
extern "C" {
#endif 

#define MAX_PATH_LEN 64

typedef struct tag_myData
{
    char* text;    //广告内容(介绍)
    char  path[MAX_PATH_LEN]; //短名称加.MRP
    int32 appid;
    int32 isExist; //是否存在这个广告应用
}st_myData;


//当点击广告时启动下载插件,插件销毁后调用这个回调来重画屏幕(背景)
typedef void (*advPaintBack)(void);

//广告暂停
void advPause(void);

//广告恢复
void advResume(void);

//换一条广告
void advNext(void);

//点击广告
void advDl(void);

//使用广告(初始化时调用)
void advInit(advPaintBack callback);

//设置显示区域
void advSetRect(int16 x, int16 y, int16 w, int16 h);

//停止广告(结束时调用)
void advDelete(void);



#ifdef __cplusplus
}
#endif 

#endif

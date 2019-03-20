#ifndef  INCLUDE_MRC_CAMERA_H
#define INCLUDE_MRC_CAMERA_H

//#include "mrc_base.h"

typedef enum
{
	CAMERA_STATUS_UNINIT,//未初始化状态
	CAMERA_STATUS_INIT,//初始化成功状态
	CAMERA_STATUS_INIT_FAILED,//初始化失败状态 
	CAMERA_STATUS_START,//启动成功状态
	CAMERA_STATUS_START_FAILED,//启动失败状态
	CAMERA_STATUS_CAPTURE,//拍照成功状态
	CAMERA_STATUS_CAPTURE_FAILED,//拍照失败状态
	CAMERA_STATUS_PAUSE,//暂停状态
	CAMERA_STATUS_RESUME_FAILED,//恢复失败状态
	CAMERA_STATUS_CLOSE//关闭状态
}CAMERA_STATUS_T;


typedef enum
{
	CAMERA_CAPTURE_MODE_MULTIPLE = 1,//多张连拍模式
	CAMERA_CAPTURE_MODE_SINGLE = 2//单张拍照模式,拍照完成以后，如果要继续拍照要调用mrc_Camera_resume
}CAMERA_CAPTURE_MODE_T;


typedef enum
{
	CAMERA_IMG_QUALITY_LOW,//较差的图片质量
	CAMERA_IMG_QUALITY_NORMAL,//一般效果的图片质量
	CAMERA_IMG_QUALITY_HIGH//高品质的图片质量
}CAMERA_IMG_QUALITY_T;


typedef struct
{
	void* img_data;//拍照返回的jpg格式图片数据指针 
	int32 img_data_len;//拍照返回的jpg格式图片数据长度
}CAMERA_IMG_DATA_T;


/*
函数名:mrc_Camera_init
功能:摄像头设备初始化
输入：无
输出:无
返回：
    MR_SUCCESS  成功
    MR_FAILED   失败
    MR_IGNORE 不支持该功能

备注:使用摄像头之前必须调用该接口，否则无法正常拍照
*/
int32 mrc_Camera_init(void);

/*
函数名:mrc_Camera_creat
功能:创建摄像头预览界面
输入：
x:预览区域的x坐标
y:预览区域的y坐标
prevW:预览区域的宽度
prevH:预览区域的高度
imgW:拍下来的照片的宽度
imgH:拍下来的照片的高度
img_quality_type:照片质量类型
	CAMERA_IMG_QUALITY_LOW:较差的图片质量
	CAMERA_IMG_QUALITY_NORMAL:一般效果的图片质量
	CAMERA_IMG_QUALITY_HIGH:高品质的图片质量

输出:无
返回：
    MR_SUCCESS  成功
    MR_FAILED   失败
    MR_IGNORE 不支持该功能

注意:prevW和prevH最大，分别为屏幕的宽和高，如果屏幕大小是240x320，
那么它们的最大值分别为240和320，
imgW和imgH的最大值与摄像头的像素有关系，可以大于屏幕的宽和高。
*/
int32 mrc_Camera_creat(uint16 x, uint16 y, uint16 prevW, uint16 prevH, uint16 imgW, uint16 imgH,uint32 img_quality_type);

/*
函数名:mrc_Camera_capture
功能:拍照
输入：
mode:拍照模式，分连拍和单拍两种模式
	CAMERA_CAPTURE_MODE_MULTIPLE: 多张连拍模式
	CAMERA_CAPTURE_MODE_SINGLE: 单张拍照模式,拍照完成以后，如果要继续拍照要调用mrc_Camera_resume

输出:
img_data:用于输出拍照的jpge图片数据，包括数据指针和数据大小,函数返回以后
用户需要自己创建内存空间，将图片进行拷贝
返回：
    MR_SUCCESS  成功
    MR_FAILED   失败
    MR_IGNORE 不支持该功能
*/
int32 mrc_Camera_capture(uint8 mode,CAMERA_IMG_DATA_T* img_data);

/*
函数名:mrc_Camera_destroy
功能:关闭摄像头设备
输入:无
输出:无
返回:无
备注:调用该接口以后，如果想再次使用，那么必须重新初始化摄像头
*/
void mrc_Camera_destroy(void);

/*
函数名:mrc_Camera_pause
功能:暂停摄像头，如果先前处于预览状态，
那么将会退出预览状态
输入：无
输出:无
返回：
    MR_SUCCESS  成功
    MR_FAILED   失败
    MR_IGNORE 不支持该功能
*/
int32 mrc_Camera_pause(void);

/*
函数名:mrc_Camera_resume
功能:恢复摄像头，如果先前处于预览状态，
那么将会恢复到预览状态
输入：无
输出:无
返回：
    MR_SUCCESS  成功
    MR_FAILED   失败
    MR_IGNORE 不支持该功能
*/
int32 mrc_Camera_resume(void);

/*
函数名:mrc_Camera_getZoom
功能:获取摄像头缩放等级
输入：无
输出:无
返回：摄像头缩放等级
*/
int32 mrc_Camera_getZoom(void);

/*
函数名:mrc_Camera_zoomOut
功能:摄像头缩小功能
输入：无
输出:无
返回：
    MR_SUCCESS  成功
    MR_FAILED   失败
    MR_IGNORE 不支持该功能
*/
int32 mrc_Camera_zoomOut(void);

/*
函数名:mrc_Camera_zoomIn
功能:摄像头放大功能
输入：无
输出:无
返回：
    MR_SUCCESS  成功
    MR_FAILED   失败
    MR_IGNORE 不支持该功能
*/
int32 mrc_Camera_zoomIn(void);

/*
函数名:mrc_Camera_status
功能:获取摄像头当前状态
输入：无
输出:无
返回：摄像头状态值，具体状态值如下:
	CAMERA_STATUS_UNINIT 未初始化状态
	CAMERA_STATUS_INIT 初始化成功状态
	CAMERA_STATUS_INIT_FAILED 初始化失败状态 
	CAMERA_STATUS_START 启动成功状态
	CAMERA_STATUS_START_FAILED 启动失败状态
	CAMERA_STATUS_CAPTURE 拍照成功状态
	CAMERA_STATUS_CAPTURE_FAILED 拍照失败状态
	CAMERA_STATUS_PAUSE 暂停状态
	CAMERA_STATUS_RESUME_FAILED 恢复失败状态
	CAMERA_STATUS_CLOSE 关闭状态

*/
uint8 mrc_Camera_status(void);

#endif//INCLUDE_MRC_CAMERA_H

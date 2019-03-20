#ifndef _MRC_IMAGE_H_
#define _MRC_IMAGE_H_

typedef struct
{
	char *src;
	int32 len;
	int32 src_type;// MRAPP_SRC_TYPE
}MRAPP_IMAGE_ORIGIN_T, *PMRAPP_IMAGE_ORIGIN_T;

typedef struct
{
	int32 width;    //图片的宽度
	int32 height;   //图片的高度
	int32 type;		//图片类型
}MRAPP_IMAGE_WH, *PMRAPP_IMAGE_WH;

typedef struct
{
	char *src; //文件名，或是数据流的buf的地址，如果是文件名，是GB格式
	int32 len; //src所指的buf的大小
	int32 width;//用户图片显示的区域的宽度
	int32 height;//用于图片显示的区域的高度
	int32 src_type;// MRAPP_SRC_TYPE
	char *dest;//解码后的图片数据存放的buf//rgb565 
}MRAPP_IMAGE_DECODE_T, *PMRAPP_IMAGE_DECODE_T;


typedef struct _MRAPP_GIF_FRAME_INFO
{
	int32 fwidth; //本帧的宽度
	int32 fheight; //本帧的高度
	int32 ox; //本帧左上角的x坐标(相对逻辑屏幕)
	int32 oy; //本帧左上角的y坐标(相对逻辑屏幕)
	int32 transparent_flag; //是否需要透明显示
	int32 transparent_color; //透明显示的颜色
	int32 delay_time; //本帧显示的时间
	char *pdata; //解压好的图片数据
	struct _MRAPP_GIF_FRAME_INFO *next; //指向下一帧的数据结构
}MRAPP_GIF_FRAME_INFO_T, *PMRAPP_GIF_FRAME_INFO_T;

typedef struct
{
	int32 id;
	int32 width; //gif的宽度
	int32 height; //gif的高度
	int bg_color; //gif的背景色
	int frame_count; //gif总的帧数
	MRAPP_GIF_FRAME_INFO_T *first; //指向gif的第一帧的数据结构
}MRAPP_GIF_HEADER, *PMRAPP_GIF_HEADER;

typedef enum
{
	SRC_NAME = 0,    //传到移植接口是文件名(部分版本不支持)
	SRC_STREAM   //传到移植接口的是图片数据流
}MRAPP_SRC_TYPE;

typedef struct 
{
	char *src;//文件名，或是数据流的buf的地址，如果是文件名，是GB格式
	int32 src_len; //src所指的buf的大小
	int32 src_type;//// MRAPP_SRC_TYPE
	int32 ox;//rect
	int32 oy;
	int32 w;
	int32 h;
}T_DRAW_DIRECT_REQ, *PT_DRAW_DIRECT_REQ;

typedef struct 
{
	int32 p1;//gif动画句柄
}T_DSM_COMMON_RSP, *PT_DSM_COMMON_RSP;


/*
获取图片宽高
[in]PMRAPP_IMAGE_ORIGIN_T
[out]PMRAPP_IMAGE_WH 宽高类型的结构体
code:
PMRAPP_IMAGE_WH p;

mrc_readFileFromMrpEx(NULL, "2.jpg", (uint8**)&input2.src, &input2.len, 0);
input2.src_type = SRC_STREAM;
mrc_getImageInfo(&input2, (uint8**)&p);


*/
int32 mrc_getImageInfo(PMRAPP_IMAGE_ORIGIN_T image, uint8** output);//PMRAPP_IMAGE_WH

/*
直接调用平台接口将jpeg图片绘制到指定的矩形区域内
[in] PT_DRAW_DIRECT_REQ
返回值：
MR_SUCCESS： 成功
MR_FAILED ：失败

sample code:

T_DRAW_DIRECT_REQ input;

input.src_type = SRC_STREAM;
input.ox = 80;
input.oy = 80;
input.w = 80;
input.h = 80;
mrc_readFileFromMrpEx(NULL, "1.jpg", (uint8**)&input.src, &input.src_len, 0);
mrc_drawJpegToFrameBuffer(&input);
*/
int32 mrc_drawJpegToFrameBuffer(PT_DRAW_DIRECT_REQ input);


/*
显示gif动画的接口（直接绘制到frame buffer上），每一帧动画的显示也都是mtk来控制.
不需要显示时需要调用mrc_stopGif停止播放。
[in] PT_DRAW_DIRECT_REQ
[out] T_DSM_COMMON_RSP,gif动画句柄
返回值：
MR_SUCCESS： 成功
MR_FAILED ：失败

code:

PT_DSM_COMMON_RSP gp;
T_DRAW_DIRECT_REQ input1;
memset(&input1, 0, sizeof(T_DRAW_DIRECT_REQ));
input1.src_type = SRC_STREAM;

input1.ox = 120;
input1.oy = 60;
input1.w = 60;
input1.h = 60;
mrc_readFileFromMrpEx(NULL, "1.gif", (uint8**)&input1.src, &input1.src_len, 0);
mrc_drawGifToFrameBuffer(&input1, (uint8**)&gp);

*/
int32 mrc_drawGifToFrameBuffer(PT_DRAW_DIRECT_REQ input, uint8** output);

/*
停止播放动画
[in] T_DSM_COMMON_RSP,gif动画句柄
返回值：
MR_SUCCESS： 成功
MR_FAILED ：失败

code:
PT_DSM_COMMON_RSP input;

mrc_stopGif(input);
*/
int32 mrc_stopGif(PT_DSM_COMMON_RSP input);

/*
解码一张GIF图片。
[in] 
返回值：
MR_SUCCESS： 成功
MR_FAILED ：失败

code:
mrc_readFileFromMrpEx(NULL, "1.gif", (uint8**)&test.src, &test.len, 0);
test.src_type = SRC_STREAM;
input2.src = test.src;
input2.len = test.len;
input2.src_type = SRC_STREAM;
ret = mrc_getImageInfo(&input2, (uint8**)&p);
mrc_printf("output.h is %d----output.w is %d", p->height, p->width);
mrc_printf("output.type is %d", p->type);
//pp = malloc(sizeof(uint16)*p->height*p->width);//这里没有BUFFER竟然也OK？
//memset(pp, 0, sizeof(pp));
//test.dest = (char*)pp;
test.height = 90;
test.width = 90;
ret = mrc_decodeGifToBuffer(&test, (uint8**)&outl);

mrc_printf("mrc_decodeGifToBuffer ret is %d", ret);
mrc_bitmapShowEx((uint16*)outl->first->pdata, 0, 0, outl->first->fwidth, outl->first->fwidth,
	outl->first->fheight, BM_COPY, 0, 0);//这里没有BUFFER竟然也OK？
mrc_releaseDecGifRes();
*/
int32 mrc_decodeGifToBuffer(PMRAPP_IMAGE_DECODE_T input, uint8** output);//PMRAPP_GIF_HEADER
/*
应用通知移植释放先前gif解码用到的资源。
[in] 
返回值：
MR_SUCCESS： 成功
MR_FAILED ：失败

code:
	mrc_releaseDecGifRes();
*/
int32 mrc_releaseDecGifRes(void);


#endif
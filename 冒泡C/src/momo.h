#ifndef _MOMO_H_
#define _MOMO_H_

#include "mrc_base.h"
#include "mrc_bmp.h"
#include "mrc_exb.h"
#include "mrc_image.h"
#include "mrc_base_i.h"
#include "mrc_skyfont.h"

#define _SET_BIT(var,bit) \
    (var |= (bit))

#define _CLR_BIT(var,bit) \
    (var &= ~(bit))

#define _IS_SET_BIT(var,bit) \
    (var & (bit))


#define _FUNCSET_STYLE(state) \
	(func_state |= (state))

#define _FUNCCLR_STYLE(state) \
	(func_state &= ~(state))

#define _FUNCIS_SET_ANY(state) \
	(func_state & (state))

#define FUNC_INITSUC 1
#define FUNC_EXIT    2
#define FUNC_LOAD    4
#define FUNC_RELOADTXT 8 //运行后重新加载编辑器内容





#define FPRINTF(...) \
    do{\
    int32 h31415926len;\
    char msgBuffer[256];\
    int32 h31415926 = mrc_open("debug.txt", MR_FILE_CREATE|MR_FILE_WRONLY);\
    mrc_seek(h31415926, 0, MR_SEEK_END);\
    h31415926len = MR_SPRINTF(msgBuffer, __VA_ARGS__);\
    mrc_write(h31415926, msgBuffer, h31415926len);\
    mrc_write(h31415926, "\r\n", 2);\
    mrc_close(h31415926);\
    }while(0)

extern char STOREDIR[];//存储目录（带/）
extern char enable_smallfont;//是否开启了小字体
extern char FONT_W;
extern char FONT_H;
extern uint16 SCREEN_WIDTH;
extern uint16 SCREEN_HEIGHT;
extern int32 func_state;//程序运行状态位



enum {
    CFG_SMALLFONT,
    CFG_FIRSTRUN
};
//读取配置，成功返回配置值，失败返回FALSE
#define readCfg(item)    RdWrCfg(item,0,TRUE)

//写配置,返回值无意义
#define writeCfg(item,data)   RdWrCfg(item,data,FALSE)

int32 RdWrCfg(uint32 item, int32 data, int32 isRead);



enum {
    DATA_COPY
};
//读取数据，成功返回0失败返回-1，output需要自己申请释放
#define readData(item,output,len) RdWrData(item,output,len,TRUE)
//写入数据，成功返回0失败返回-1
#define writeData(item,data,len) RdWrData(item,data,len,FALSE)
int32 RdWrData(uint32 item, void* data, uint32 len, int32 isRead);


//为通用全局变量初始化
int momo_init(void);

int momo_exit(void);

typedef struct STACKDATA {
    int16 stack[64];
    uint8 sp;
}STACKDATA;

//初始化虚拟栈空间
void StackInit(void);

void push(int16 data);

int16 pop(void);

//复制文件
int CopyFile(const char* dst,const char* src);


//画圆type是否为实心
void circle(int x0,int y0,int r,int color,int type);

//获取当前系统目录(初始化时调用,之前不要有文件操作,成功返回路径长度、失败返回MR_FAILED)
int32 GetCurSystemPath(char * out);

//获取默认系统目录
int32 GetDefSystemPath(char* out);

//从文件路径获得文件名例:"c:/mythroad/a.txt"返回a.txt,需要free
char* ExtractFileName(char* path);


//字符串加解密
char *stred(char *str, int len);

//画中心渐变线
void MidShadeLine(int x, int y, int w, uint32 pixelA, uint32 pixelB);

/*读入一行

参数
f       文件句柄
out     输出字符串
成功返回0,失败返回-1
*/
int readline(int32 f, char out[256]);


/*文字屏幕(非小字体)*/
void TextScreen(const char *text, int8 is_unicode);

//删除一个文件夹(完整删除)
int32 removeDir(const char* dirName);

//检查是否被修改
int32 Check(char *key);

//增加空格
char *AddWhite(char *str, int32 len);

/*
 切换MRP工作目录
 strDisk为目录字符串,如"A:/","B:/"
*/
int32 SwitchDisk(const char* strDisk);  

enum
{
	SHADE_UPDOWN,		//从上到下
	SHADE_LEFTRIGHT,	//从左到右
	SHADE_DOWNUP,		//从下到上
	SHADE_RIGHTLEFT		//从右到左
};
/*
画一个渐变矩形
输入：
xy			xy位置
wh			宽度高度		
pixelA		初始颜色
pixelB		最终颜色
*/
void DrawShadeRect(int16 x, int16 y, int16 w, int16 h, uint32 pixelA, uint32 pixelB, int8 mode);

//同上
void ShadeRect(int x, int y, int w, int h, int AR, int AG, int AB, int BR, int BG, int BB, int mode);

/*画一个矩形阴影*/
void DrawHatching(int x, int y, int w, int h);

/**/
/*画一个线框*/
void DrawRectangle(int x, int y, int w, int h, uint32 pixel);

/*画一个具有一定宽度的线框*/
void DrawRectangle2(int x, int y, int w, int h, int line, uint32 pixel);

//确定磁盘是否有足够的空间
int32 getDiskSpace(void);


#define PIXEL888R(pixel)      (uint8)(((pixel) >> 16) & 0xff)
#define PIXEL888G(pixel)      (uint8)(((pixel) >> 8) & 0xff)
#define PIXEL888B(pixel)      (uint8)((pixel) & 0xff)

/* 画一个颜色框 */
#define FillBox(x, y, w, h, pixel) \
	mrc_drawRect((int16)(x), (int16)(y), (int16)(w), (int16)(h), PIXEL888R((pixel)), PIXEL888G((pixel)), PIXEL888B((pixel)))

/* 画一条水平线 */
#define DrawHLine(x, y, w, pixel) \
	mrc_drawLine((int16)(x), (int16)(y), (int16)((x)+(w)-1), (int16)(y), PIXEL888R(pixel), PIXEL888G(pixel), PIXEL888B(pixel))

void DrawHatchingText(const char *Text, int16 x, int16 y, int is_unicode);

typedef struct COPYMEN{
	int (*arry)[32][32];
	int x;
	int y;
	int w;
	int h;
}COPYMENDATA;

//复制内存块
void copymen(COPYMENDATA *dest, COPYMENDATA *src);

void BMPScreen(const char *name, int16 w, int16 h,uint16 Mode, int32 BackColor);

//获取本地时间
void GetLocalTimer(char output[6]);

/* 截图 , 参数ToDir例："bmp/" */
void PrintScreen(const char *ToDir);


/* 屏幕中间显示文字 (非小字体)*/
void TextMiddle(const char *text, int8 is_unicode, int8 show);

//计算字符串宽度(非小字体)
int32 strW(char* str, int is_uincode, uint16 font);

//计算字符串高度(非小字体)
int32 strH(char* str, int is_uincode, uint16 font);

/*
从文件名获取扩展名
成功返回文件扩展名指针,失败返回空
*/
char* GetFileExt(const char* path);


//测试一定长度的GB字符串是否安全，返回安全的长度
int32 TestStr(char *str, int32 len);

//创建一条路径,成功返回0失败返回-1
int32 MakePath(char *path);

//画渐变线
void ShadeLine(int x, int y, int w, uint32 pixelA, uint32 pixelB);

//测试12号字体宽高(只unicode)
int32 skyfontWidthHeight(char *szText, int32 *width, int32 *height);

int32 skyfontdraw(char* szText, int16 x, int16 y, mr_screenRectSt rect, mr_colourSt colorst, uint16 flag);


//删除字符串左右两边的空格
char *strtrim(char *s);


/*
获取MRP内部文件信息
MRPhandle[i]        MRP文件句柄
filename[i]         要获取信息的文件名
filepos[o]          文件在MRP中的位置
filelen[o]          文件在MRP中的长度
iszip[o]            文件在MRP中否是以压缩形式存放
成功返回0，失败返回-1
*/
int32 GetMRPFileInfo(int32 MRPhandle, char *filename, int32 *filepos, int32 *filelen, int32 *iszip);

//测试GB字符串的字符数
int32 StrNum(char *str);

//根据文件名画图片,图片格式可以为BMP24,JPG,GIF等平台支持的格式
//成功返回MR_SUCCESS,失败返回MR_FAILED，未实现返回MR_IGNORE.
int DrawIMG(char* filename, int x,int y);


#endif
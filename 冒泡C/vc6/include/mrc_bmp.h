#ifndef _mrc_bmpH_
#define _mrc_bmpH_

//当采用旧的MyThroad方式时，需要打开下面这行的编译开关。
//#define MR_OLD

#ifdef MR_OLD
#include "cfunction.h"
#endif

/* Bitmap header */
typedef struct _mrc_bmpHeader
{
	uint16		Magic;				/* Magic identifier: "BM" */
	uint32		FileSize;			/* Size of the BMP file in bytes */
	uint16		Reserved1;			/* Reserved */
	uint16		Reserved2;			/* Reserved */
	uint32		DataOffset;			/* Offset of image data relative to the file's start */
	uint32		HeaderSize;			/* Size of the header in bytes */
	uint32		Width;				/* Bitmap's width */
	uint32		Height;				/* Bitmap's height */
	uint16		Planes;				/* Number of color planes in the bitmap */
	uint16		BitsPerPixel;		/* Number of bits per pixel */
	uint32		CompressionType;	/* Compression type */
	uint32		ImageDataSize;		/* Size of uncompressed image's data */
	uint32		HPixelsPerMeter;	/* Horizontal resolution (pixels per meter) */
	uint32		VPixelsPerMeter;	/* Vertical resolution (pixels per meter) */
	uint32		ColorsUsed;			/* Number of color indexes in the color table that are actually used by the bitmap */
	uint32		ColorsRequired;		/* Number of color indexes that are required for displaying the bitmap */
} mrc_bmpHeader;//程序员应当通过封装的函数访问BMP数据。


/* Private data structure */
struct _BMP
{
	mrc_bmpHeader	Header;
	uint8*		Palette;
	uint8*		Data;
	uint32		      PaletteType;    // 0:no need free;1:need free;
	uint32		      DataType;    // 0:no need free;1:need free;
	uint8*	MrpBuffer;//20071126李波为了实现mrc_bmpLoadFromMrp而增加的
	//字段，使得本来符合BMP位图格式规范的_BMP头不再符合规范。因此，
	//凡是在设计BMP头操作的地方，比如使用sizeof(_BMP)、memcpy之类，都要
	//需要特别注意。
};



/* Bitmap image */
typedef  struct _BMP BMP;

/* Error codes */
typedef enum
{
	mrc_bmpOK = 0,				/* No error */
	mrc_bmpERROR,				/* General error */
	mrc_bmpOUT_OF_MEMORY,		/* Could not allocate enough memory to complete the operation */
	mrc_bmpIO_ERROR,			/* General input/output error */
	mrc_bmpFILE_NOT_FOUND,		/* File not found */
	mrc_bmpFILE_NOT_SUPPORTED,	/* File is not a supported BMP variant */
	mrc_bmpFILE_INVALID,		/* File is not a BMP image or is an invalid BMP */
	mrc_bmpINVALID_ARGUMENT,	/* An argument is invalid or out of range */
	mrc_bmpTYPE_MISMATCH,		/* The requested action is not compatible with the BMP's type */
	mrc_bmpERROR_NUM
} mrc_bmpSTATUS;


/*********************************** Public methods **********************************/


/* Construction/destruction */
BMP*		mrc_bmpCreate( uint32 width, uint32 height, uint16 depth );
void			mrc_bmpFree( BMP* bmp );


/* I/O */
//从磁盘中读取256色bmp文件，返回256色位图句柄；
//当句柄不再使用时，需要调用mrc_bmpFree释放内存。
BMP*			mrc_bmpLoadFromFile				( char* filename );
//从缓冲区中读取256色bmp文件，返回256色位图句柄；
//当句柄不再使用时，需要先调用mrc_bmpFree释放内存。
//Buffer的内存空间是否需要释放，由程序员根据Buffer的来源确定。
BMP* 			mrc_bmpLoadFromBuffer( uint8* buf, int32 len );
//从mrp文件中提取256色bmp文件，返回256色位图句柄；
//当句柄不再使用时，需要先调用mrc_bmpFree释放内存。
BMP*			mrc_bmpLoadFromMrp				( char* filename );
void				mrc_bmpWriteFile				( BMP* bmp, char* filename );


/* Meta info */
uint32			mrc_bmpGetWidth				( BMP* bmp );
uint32			mrc_bmpGetHeight				( BMP* bmp );
uint16			mrc_bmpGetDepth				( BMP* bmp );


/* Pixel access */
void			mrc_bmpGetPixelRGB				( BMP* bmp, uint32 x, uint32 y, uint8* r, uint8* g, uint8* b );
void			mrc_bmpSetPixelRGB				( BMP* bmp, uint32 x, uint32 y, uint8 r, uint8 g, uint8 b );//不可用于16色、256色基于索引的位图格式中。
void			mrc_bmpGetPixelIndex			( BMP* bmp, uint32 x, uint32 y, uint8* val );
void			mrc_bmpSetPixelIndex			( BMP* bmp, uint32 x, uint32 y, uint8 val );


/* Palette handling */
/*
获取调色版颜色数。
输入:图片句柄；
输出:图片调色板色彩数。若不是16色和256色位图，则返回0。
若16色、256色位图的色彩数少于16、256，则返回句柄头中ColorUsed的值。
*/
uint32 mrc_bmpGetColorUsed( BMP* bmp);
void			mrc_bmpGetPaletteColor			( BMP* bmp, uint8 index, uint8* r, uint8* g, uint8* b );
void			mrc_bmpSetPaletteColor			( BMP* bmp, uint8 index, uint8 r, uint8 g, uint8 b );

/*
获取指定屏幕缓冲区的信息。
输入
di:目标屏幕缓冲区编号；
输出:
bitmapInfo目标屏幕缓冲区信息。
返回:
      MR_SUCCESS     成功
      MR_FAILED         失败

*/
int32 mrc_bitmapGetInfo(uint32 di,mr_bitmapSt *bitmapInfo);


/*
设置指定屏幕缓冲区的信息。
输入
di:目标屏幕缓冲区编号；
bitmapInfo:待设置的屏幕缓冲区参数。
返回:
      MR_SUCCESS     成功
      MR_FAILED         失败
      
警告:
1、调用mrc_bitmapSetInfo前，必须调用mrc_bitmapGetInfo保存初值；
2、在使用完毕后，必须恢复初值；
3、该屏幕的所有参数都必须设置。

*/
int32 mrc_bitmapSetInfo(uint32 di,mr_bitmapSt *bitmapInfo);


//取屏幕缓冲区的内存地址
uint16 * w_getScreenBuffer(void);
//设置屏幕缓冲区的内存地址
void w_setScreenBuffer(uint16 *ptr);
//获取屏幕缓冲区宽高
void mrc_getScreenSize(int32 *w,int32 *h);
void w_getScreenSize(int32 *w,int32 *h);//功能同mrc_getScreenSize
//设置屏幕缓冲区宽高
void mrc_setScreenSize(int32 w, int32 h);
void w_setScreenSize(int32 w, int32 h);//功能同mrc_setScreenSize

//从位图的y行sx列开始的w个象素复制到目标row中
//当使用256色位图时，若Palette!=NULL，则从Palette中获得16bit的颜色，从而提升性能。
//其他情况下Palette必须设置为NULL。
void mrc_bmpGet16BitLineFrom8BitData( BMP* bmp, int32 y, int32 sx, int32 w, uint16* row,uint16 * Palette);

/*
将hBMP指向的windows bmp 256色或16色格式的图片，从(sx, sy)开始的
宽高为w, h的区域，绘制到图片缓存di从(x, y)开始的地方。

输入:
di               目标图片缓冲序号
hBMP        指向保存windows bmp 256色格式的图片的内存
x,y           windows bmp 256色或16色BMP图片句柄。
w,h             欲绘制区域的宽高

sx,sy          欲绘制区域在源图片的起始位置

返回:
      MR_SUCCESS     成功
      MR_FAILED         失败
*/
int32 mrc_bmpBufferDraw(uint16 di, BMP*hBMP, int32 x, int32 y, int32 w, int32 h, uint16 rop,int32 sx, int32 sy);

//mrc_bmpBufferDrawEX调用mrc_bmpLoadFromBuffer后再调用mrc_bmpBufferDraw、mrc_bmpFree()，所以实际意义有限。
int32 mrc_bmpBufferDrawEX(uint16 di,uint8* buf,  int32 len, int32 x, int32 y,int32 w, int32 h,uint16 rop, int32 sx, int32 sy);

/*
	将句柄为hBMP的256色、16色bmp缓冲中的图片，从缓冲中图片的(sx, sy)
	开始的宽高为w, h的区域，绘制到(x, y)开始的屏幕缓冲中。

输入:
p        源图片指针
x,y              屏幕位置
rop              选择如下：
   BM_OR,         			//SRC .OR. DST*   半透明效果
   BM_XOR,        			//SRC .XOR. DST*
   BM_COPY,       		//DST = SRC*      覆盖
   BM_NOT,        			//DST = (!SRC)*
   BM_MERGENOT,   	//DST .OR. (!SRC)
   BM_ANDNOT,     		//DST .AND. (!SRC)
   BM_TRANSPARENT,//透明色不显示，图片的第一个象素（左上角坐标为(0,0)的象素）是透明色
   BM_AND,        			//DST AND SRC
   BM_GRAY,        		//灰度绘图， 相当于BM_TRANSPARENT＋灰度绘图：
                              DST灰度 ＝ 30%R + 59%G + 11%B图片序号
   BM_REVERSE     		//反向绘图，相当于BM_TRANSPARENT＋反向绘图（V1939）

sx,sy              源图片的起始位置
w,h             欲加载图片的宽高

返回:
      MR_SUCCESS     成功
      MR_FAILED         失败
*/
int32 mrc_bmpShow(BMP* hBMP,
   int16 x,
   int16 y,
   int16 w,
   int16 h,
   uint16 rop,
   int16 sx,
   int16 sy
);

//mrc_bmpShowEx实际上是先调用mrc_bmpLoadFromBuffer获得BMP句柄，
//然后调用mrc_bmpShow，最后调用mrc_bmpFree释放句柄。
int32 mrc_bmpShowEx(uint8 *buffer,
   int16 x,
   int16 y,
   int16 w,
   int16 h,
   uint16 rop,
   int16 sx,
   int16 sy
);



mrc_bmpSTATUS mrc_bmpGetError(void);
/* Useful macro that may be used after each BMP operation to check for an error */
#define mrc_bmpCHECK_ERROR( output_file, return_value ) \
	if ( mrc_bmpGetError() != mrc_bmpOK )													\
	{																				\
		fprintf( ( output_file ), "BMP error: %s\n", mrc_bmpGetErrorDescription() );	\
		return( return_value );														\
	}																				\

   /*下面的MAKERGB是MR平台内部的*/
#ifdef MR_ANYKA_MOD
#define MAKERGB(r, g, b)     (uint32) ( ((uint32)r<<16) | ((uint32)g << 8) | ((uint32)b) )
#else
#define MAKERGB(r, g, b)     (uint16) ( ((uint32)(r>>3) << 11) + ((uint32)(g>>2) << 5) + ((uint32)(b>>3)) )
#endif

/*************************will be delete************************************/


/*
将buf和len中的windows bmp 256色格式的图片，从(sx, sy)开始的
宽高为w, h的区域，绘制到图片缓存di从(x, y)开始的地方。

输入:
di               目标图片缓冲序号
buf,len        指向保存windows bmp 256色格式的图片的内存
w,h             欲绘制区域的宽高
x,y             欲绘制区域在屏幕上的起始位置
sx,sy          欲绘制区域在源图片的起始位置

返回:
      MR_SUCCESS     成功
      MR_FAILED         失败
*/
int32 mrc_bmpBufferDrawOld(uint16 di, uint8* buf,  int32 len, int32 x, int32 y, int32 w, int32 h, int32 sx, int32 sy);

/*
将mrp中名为filename的windows bmp 256色格式的图片，从(sx, sy)开
始的宽高为w, h的区域，绘制到屏幕从(x, y)开始的地方。

在mythroad工程中，名为"*.bmp"的文件被期望为16Bit或24Bit色的
windows bmp格式，所以该接口使用的图片文件名，需要将
后缀修改为非"*.bmp"，比如可以修改为"*.bm"。


输入:
filename     文件名
w,h             欲绘制区域的宽高
x,y             欲绘制区域在屏幕上的起始位置
sx,sy          欲绘制区域在源图片的起始位置

返回:
      MR_SUCCESS     成功
      MR_FAILED         失败
*/
int32 mrc_bmpShowOld(char* filename, int32 x, int32 y, int32 w, int32 h, int32 sx, int32 sy);


/*************************above will be delete************************************/



/* 图片格式*/
typedef enum
{
	MRC_IMG_GIF = 0,
	MRC_IMG_JPG
} MRC_IMG_TYPE;


/*
功能描述:
	将其他格式的图片转为16bitBMP。

输入:
*pImg    	 图片的原始数据；
ImgSize   图片的字节数；
ImgType	图片格式:

**bmp	用于存放解码得到的图片的缓冲区指针。缓冲区由SDK分配，
	由用户负责释放。
*w 	解码得到的图片宽的象素数；
*h 	解码得到的图片高的象素数；

GIF图片转BMP时，将GIF图片的第一帧转换为16bitBMP。
返回:
      MR_SUCCESS     成功
      MR_FAILED         失败
      
 备注:
 	该函数目前模拟器上尚不支持。      
*/
int32 mrc_Img2bmp(uint8 *pImg,int32 ImgSize,MRC_IMG_TYPE ImgType,uint16 **bmp,int32 *w,int32 *h);

int32 mrc_PrintScreen(char *filename);

#define FLIP_NONE	0x0000
#define	FLIP_HORI	0x0100
#define FLIP_VERT	0x0200


int32 bitmapShowFlip(uint16* p, int16 x, int16 y, uint16 mw, uint16 w, uint16 h, uint16 rop, 
		int16 sx, int16 sy, uint16 transcolor);


#define TRANS_NONE					0x0000
#define TRANS_ROT90					0x0100
#define TRANS_ROT180				0x0200
#define TRANS_ROT270				0x0300
#define TRANS_MIRROR				0x0400
#define TRANS_MIRROR_ROT90		0x0500
#define TRANS_MIRROR_ROT180		0x0600
#define TRANS_MIRROR_ROT270		0x0700

/*
rop取值:
1、TRANS_NONE： 没有进行转动和翻转； 
2、TRANS_ROT90： 90度转动； 
3、TRANS_ROT180： 180度转动； 
4、TRANS_ROT270： 270度转动； 
5、TRANS_MIRROR： 左右翻转； 
6、TRANS_MIRROR_ROT90： 左右翻转后90度转动； 
7、TRANS_MIRROR_ROT180： 左右翻转后180度转动； 
8、TRANS_MIRROR_ROT270： 左右翻转后270度转动。 
输入:
p        源图片指针
x,y     屏幕	显示位置
mw		图片的原始宽度，即完整宽度。
rop : (BM_TRANSPARENT 或BM_COPY（仅两项），其余模式默认BM_COPY处理) | (TRANS_NONE至TRANS_MIRROR_ROT270,不填
		功能与mrc_bitmapshowEx相同)
sx,sy           源图片的起始位置
w,h             欲加载图片的宽高
transcolor : 透明色 (16bit),可以用MAKERGB宏将(r,g,b)转换为16bit的格式。

返回:
      MR_SUCCESS     成功
      MR_FAILED         失败
*/
int32 mrc_bitmapShowFlip(uint16* p, int16 x, int16 y, uint16 mw, uint16 w, uint16 h, uint16 rop, 
		int16 sx, int16 sy, uint16 transcolor);
#endif

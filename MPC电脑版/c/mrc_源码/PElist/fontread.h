/***************************************************************************************************************
***************************************************************************************************************
*Version:V2.0.0616
*New Features:Support Memory or TCard load, config this by Function FontmoduleInit's  parameter: BOOL isMemLoad. If game's memory 
			 is enough,you can set isMemLoad as TRUE to accelerate the speed of drawing.
*Others:Support Unicode string only; 
*           Font with unequal width; 
*           OutPut file as sky_X_XXX.bin.
***************************************************************************************************************
***************************************************************************************************************/

#ifndef _DRAWTEXT_H
#define _DRAWTEXT_H

#ifndef BOOL_T
#define BOOL_T
typedef int	BOOL;
#endif

typedef enum charset_e
{
	MR_SET_GB,				//BG字符集
	MR_SET_UNICODE		//UNICODE字符集
}E_CHR_SET;
/*
功	  能：绘制单行文本，对应于SKY SDK mrc_drawText接口
参数说明：
	      	  char*chr					［IN ］定义欲绘制文本内容
		  int16 x					［IN ］定义欲绘制文本的起始x坐标（相对于屏幕坐标原点(0,0)）
		  int16 y					［IN ］定义欲绘制文本的起始y坐标（相对于屏幕坐标原点(0,0)）
		  mr_colourSt  colorst      		［IN ］定义欲绘制文本的顔色值，详见mr_colourSt定义
		  E_CHRSET flag			［IN ］对应于E_CHRSET枚举体，取值为SET_GB,MR_SET_UNICODE，定义欲绘制文本的字符集类型
		  uint16 font				［IN ］对应于 SKY SDK MR_FONT_TYPE枚举体，取值为MR_FONT_BIG, MR_FONT_MEDIUM, MR_FONT_SMALL
返回值:
		MR_SUCCESS  绘制成功
		MR_FAILED   绘制失败
*/
int32 _drawText(char* chr, int16 x, int16 y,  mr_colourSt colorst,E_CHR_SET flag,  uint16 font);
/*
功	  能：绘制单个字符
参数说明：
	      	  char*chr					［IN ］定义欲绘制文本内容
		  int16 x					［IN ］定义欲绘制文本的起始x坐标（相对于屏幕坐标原点(0,0)）
		  int16 y					［IN ］定义欲绘制文本的起始y坐标（相对于屏幕坐标原点(0,0)）
		  mr_colourSt  colorst      		［IN ］定义欲绘制文本的顔色值，详见mr_colourSt定义
		  E_CHRSET flag			［IN ］对应于E_CHRSET枚举体，取值为SET_GB,MR_SET_UNICODE，定义欲绘制文本的字符集类型
		  uint16 font				［IN ］对应于 SKY SDK MR_FONT_TYPE枚举体，取值为MR_FONT_BIG, MR_FONT_MEDIUM, MR_FONT_SMALL
返回值:
		MR_SUCCESS  绘制成功
		MR_FAILED   绘制失败
*/
int32 _drawChar(char* chr, int16 x, int16 y, mr_colourSt colorst,E_CHR_SET flag,  uint16 font);

/*
功	  能：绘制指定的文本串，根据显示内容在指定的矩形区域内自动换行
		      绘制时以自右至左显示
参数说明：
	      	  char*chr					［IN ］定义欲绘制文本内容
		  int16 x					［IN ］定义欲绘制文本的起始x坐标，相对于rect的右上顶点，切记
		  int16 y					［IN ］定义欲绘制文本的起始y坐标（相对于屏幕坐标原点(0,0)）
		  mr_screenRectSt  rect              ［IN ］定义欲绘制文本的显示区域，详见mr_screenRectSt定义
		  mr_colourSt  colorst      		［IN ］定义欲绘制文本的顔色值，详见mr_colourSt定义
		  E_CHRSET flag			［IN ］对应于E_CHRSET枚举体，取值为SET_GB,MR_SET_UNICODE，定义欲绘制文本的字符集类型
		  uint16 font				［IN ］对应于 SKY SDK MR_FONT_TYPE枚举体，取值为MR_FONT_BIG, MR_FONT_MEDIUM, MR_FONT_SMALL
返回值:
		MR_SUCCESS  绘制成功
		MR_FAILED   绘制失败
*/
int32 _drawTextFromRight(char* pcText, int16 x, int16 y, mr_screenRectSt rect, mr_colourSt colorst, E_CHR_SET flag, uint16 font);

/*
功	  能：绘制指定的文本串，根据显示内容在指定的矩形区域内自动换行
参数说明：
	      	  char*chr					［IN ］定义欲绘制文本内容
		  int16 x					［IN ］定义欲绘制文本的起始x坐标（相对于屏幕坐标原点(0,0)）
		  int16 y					［IN ］定义欲绘制文本的起始y坐标（相对于屏幕坐标原点(0,0)）
		  mr_screenRectSt  rect              ［IN ］定义欲绘制文本的显示区域，详见mr_screenRectSt定义
		  mr_colourSt  colorst      		［IN ］定义欲绘制文本的顔色值，详见mr_colourSt定义
		  E_CHRSET flag			［IN ］对应于E_CHRSET枚举体，取值为SET_GB,MR_SET_UNICODE，定义欲绘制文本的字符集类型
		  uint16 font				［IN ］对应于 SKY SDK MR_FONT_TYPE枚举体，取值为MR_FONT_BIG, MR_FONT_MEDIUM, MR_FONT_SMALL
返回值:
		MR_SUCCESS  绘制成功
		MR_FAILED   绘制失败
*/
int32 _drawTextLeft(char* pcText, int16 x, int16 y, mr_screenRectSt rect, mr_colourSt colorst, E_CHR_SET flag, uint16 font);


/*
功	  能：绘制指定的文本串，根据显示内容在指定的矩形区域内自动换行，支持从局部行lineStart开始绘制。
参数说明：
	      	  char*chr					［IN ］定义欲绘制文本内容
		  int16 x					［IN ］定义欲绘制文本的起始x坐标（相对于屏幕坐标原点(0,0)）
		  int16 y					［IN ］定义欲绘制文本的起始y坐标（相对于屏幕坐标原点(0,0)）
		  mr_screenRectSt  rect              ［IN ］定义欲绘制文本的显示区域，详见mr_screenRectSt定义
		  mr_colourSt  colorst      		［IN ］定义欲绘制文本的顔色值，详见mr_colourSt定义
		  E_CHRSET flag			［IN ］对应于E_CHRSET枚举体，取值为SET_GB,MR_SET_UNICODE，定义欲绘制文本的字符集类型
		  uint16 font				［IN ］对应于 SKY SDK MR_FONT_TYPE枚举体，取值为MR_FONT_BIG, MR_FONT_MEDIUM, MR_FONT_SMALL
		  int32  lineStart	 [IN]  绘制多行串时，欲显示的首行索引
返回值:
		MR_SUCCESS  绘制成功
		MR_FAILED   绘制失败
备注：由于行数是通过_textWidthHeightLine接口以参数linewidth为准计算得到的，请在确定Rect区域时保持两者一致。
*/
int32 _drawTextLeftByLineN(char* pcText, int16 x, int16 y, mr_screenRectSt rect, mr_colourSt colorst, E_CHR_SET flag, uint16 font, int32 lineStart);
/*
功	  能：获取字符串在指定字体中的宽、高值
参数说明：
	      	  char*pcText				［IN ］定义文本内容
		  int32 width				［OUT］字符串的宽度，单行计
		  int32 height				［OUT ］字符串的高度
		  mr_screenRectSt  rect              ［IN ］ 定义欲绘制文本的显示区域，详见mr_screenRectSt定义
		  mr_colourSt  colorst      		［IN ］定义欲绘制文本的顔色值，详见mr_colourSt定义
		  E_CHRSET flag			［IN ］对应于E_CHRSET枚举体，取值为SET_GB,MR_SET_UNICODE，定义欲绘制文本的字符集类型
		  uint16 font				［IN ］对应于 SKY SDK MR_FONT_TYPE枚举体，取值为MR_FONT_BIG, MR_FONT_MEDIUM, MR_FONT_SMALL
返回值:
		MR_SUCCESS  绘制成功
		MR_FAILED   绘制失败
*/
int32 _textWidthHeight(char *pcText,int32 *width,int32 *height,E_CHR_SET flag, uint16 font);
/*
功	  能：获取字符串在指定字体中的宽、高值
参数说明：
	      	  char*pcText				［IN ］定义文本内容
		  int32 width				［OUT］字符串的宽度，单行计
		  int32 height				［OUT ］字符串的高度
		  int32 lines					 [OUT] 字符串在以lineWidth为宽时，需要多少行。
		  mr_screenRectSt  rect              ［IN ］ 定义欲绘制文本的显示区域，详见mr_screenRectSt定义
		  mr_colourSt  colorst      		［IN ］定义欲绘制文本的顔色值，详见mr_colourSt定义
		  E_CHRSET flag			［IN ］对应于E_CHRSET枚举体，取值为SET_GB,MR_SET_UNICODE，定义欲绘制文本的字符集类型
		  uint16 font				［IN ］对应于 SKY SDK MR_FONT_TYPE枚举体，取值为MR_FONT_BIG, MR_FONT_MEDIUM, MR_FONT_SMALL
		  int32  linewidth   [IN]	 一行的宽度
返回值:
		MR_SUCCESS  绘制成功
		MR_FAILED   绘制失败
*/
int32 _textWidthHeightLine(char *pcText,int32 *width,int32 *height,int32 *lines,E_CHR_SET flag, uint16 font, int32 linewidth);
/*
功	  能：游戏自带字体模块初始化，请在mrc_init中调用。
参数说明：
	      	  BOOL isBigFontSupport					［IN ］自带字库是否存在BIG字库 ，对应于PC工具MR_FONT_BIG选 项
		  BOOL isMediumFontSupport				［IN］ 自带字库是否存在MEDIUM字库  ，对应于PC工具MR_FONT_MEDIUM选 项
		  BOOL isSmallFontSupport				［IN ］自带字库是否存在SMALL字库  ，对应于PC工具MR_FONT_SMALL选 项
		  BOOL isMemLoad						 [   IN ]     字库点阵是否Load到内存中，TRUE内存加载,FALSE T卡加载 V1.0.9版本新增
返回值:
		MR_SUCCESS  初始化成功
		MR_FAILED   初始化失败
*/
int32 FontModuleInit(BOOL isUnicodeIndexSupport,BOOL isGBIndexSupport, BOOL isMemLoad);
/*
功	  能：游戏自带字体模块资源释放，请务必在mrc_exitApp中调用。
参数说明：
	         无
返回值:
		无
*/
void FontModuleRelease(void);
#endif

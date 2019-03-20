#ifndef _PictureSee_LIB_H_
#define _PictureSee_LIB_H_

#define PictureSee_EXT_ID               20120515
#define PictureSee_FUNCTION_DrawImg	    0x10				 //插件中的函数ID，开发者可以自行定义及扩展
//#define PictureSee_FUNCTION_...	    0x12				 //插件中的函数ID，开发者可以自行定义及扩展

#define PictureSee_MRPNAME              "picturesee_mtk.mrp" //存放PictureSee插件EXT代码段的MRP名称，由于本版本要求自定义功能插件做为主包MRP内置 插件，所以置为NULL
#define PictureSee_MRPFOLD              "MDs"     			 //存放PictureSee插件EXT代码段的MRP路径，由于本版本要求自定义功能插件做为主包MRP内置 插件，所以置为NULL
#define PictureSee_EXTNAME              "picturesee.ext"	 //插件EXT代码段的名称

/*注意：
1、本期模板工程创建的插件示例，仅演示MRP内置插件，如：verdload下载插件
2、所谓MRP内置插件，即：插件的mrp、lib都打包在最终的输出MPR中，如:
	[config_mtk240]
	define=...
	output=...
	c_compiler=thumb
	bmp_mode=normal
	plat=mtk
	loader=c

	[config_game_mtk240x320]
	plugin=app

	[files_mtk240x320]
	#游戏/应用的主逻辑代码段
	file2=game_mtk240x320.ext(name=game.ext)
	#内置插件
	file3=${MRPBUILDER_HOME}\PictureSees\mainext\mainext_mtk_d.mrp
	#内置插件
	file4=${MRPBUILDER_HOME}\PictureSees\verdload\verdload_mtk.mrp	
	file1=..\res\GJYWLZ_RES_MTK_240X320.mrp

	[files_game_mtk240x320]
	file11=..\lib\GJYWLZ_MTK_240X320_24_T5N.lib
	#非内存插件
	file12=${MRPBUILDER_HOME}\PictureSees\netpay\netpay_mtkt.lib
	#内置插件
	file13=${MRPBUILDER_HOME}\PictureSees\verdload\verdload_mtkt.lib
	#非内存插件
	file15=${MRPBUILDER_HOME}\PictureSees\platreq\mr_platreq_mtkt.lib
*/

/*
功能：
	插件的处理回调，一般是PictureSee逻辑完成后对主程序的结果通知
参数说明：
	int32 p0、p1、p2、p3、p4、p5  回调形参，应用开发者可以根据需要自行设计
	本模板示例中：
	p0   定义见E_PictureSee_CBRESULT枚举取值
	p1、p2、p3、p4、p5			无效
返回值：
	无
*/

typedef enum
{
	PictureSee_RESULT_EXT_LOADSUCC,				//插件动态加载成功
	PictureSee_RESULT_EXT_LOADFAILED,			//插件动态加载失败
	PictureSee_RESULT_EXT_MRSUCCESS,				//插件功能执行成功
	PictureSee_RESULT_EXT_MRFAILED				//插件功能执行失败
}E_PictureSee_CBRESULT;

typedef void (*PictureSee_Func_CB)(int32 p0, int32 p1, int32 p2, int32 p3, int32 p4, int32 p5);


/*
功能：
	演示程序，演示插件从MRP中读取资源图片"Hello" 并显示
参数说明：
	char* bmpFileName     主程序传递过来的从主包MRP中读取的资源图片名称
	Func_CB cb		    PictureSee插件完成功能后通知回调
返回值：
	无
*/
void PictureSee_Func_DrawImg(PictureSee_Func_CB cb);

#endif

#ifndef __FILETYPE_H__
#define __FILETYPE_H__

enum
{
	// 文件夹
	FILE_DIR,
	// 文本
	FILE_TXT,FILE_WML,
	FILE_LRC,
	FILE_C,FILE_H,
	// 应用程序
	FILE_MRP,
	// 图片
	FILE_JPG,FILE_GIF,
	FILE_BMP,FILE_PNG,
	// 压缩
	FILE_ZIP,FILE_RAR,
	FILE_GZ,FILE_7Z,
	// 声音
	FILE_MP3,FILE_AMR,
	FILE_WAV,FILE_AWB,
	FILE_MID,
	// 影像
	FILE_MP4,FILE_F4V,
	FILE_RMVB,FILE_3GP,
	// 其它
	FILE_OTHER
};

// 文件图标
void MDtub(int32 dtd,int16 x,int16 y);
// 分析文件类型
int typeanalyse(char *name);
// 分析文件类型,返回名称
char *typeanalysetxt(int num);

#endif // __FILETYPE_H__
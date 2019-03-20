/**
 ** filelist.h the declare of filelist.c
 ** 
 ** 文件列表 链表操作封装 方法
 **
 ** @Author Jianbin Zhu 2011/9/13
 **
 ** 最后修改日期：2011/11/16
 **/

#ifndef	_RX_FILELIST_ 
#define _RX_FILELIST_


//#include "appdef.h"
#include "mrc_base.h"


typedef unsigned char boolean;	//定义 布尔类型

#ifndef false
#define false 0
#endif

#ifndef true
#define true 1
#endif

//求绝对值
#ifndef ABS
#define ABS(x)              (((x)<0) ? -(x) : (x))
#endif

//检测一个字符是不是小写字母
#ifndef IS_SMLETTER
#define IS_SMLETTER(i)		((i >= 97) && (i <= 122))
#endif

//文件格式枚举（1字节|）
typedef enum
{
	FILE_UNDEFINE=-1,		//初始状态未定义
	FILE_FOLDER=0x0001,		//文件夹
	FILE_TEXT=0x0002,		//文本文件
	FILE_ARCHIVE=0x0004,	//压缩文件
	FILE_AUDIO=0x0008,		//声音文件
	FILE_IMAGE=0x0010,		//图像文件
	FILE_MRP=0x0020,		//MRP文件
	FILE_REG=0x0040,		//游戏注册文件
	FILE_UNTYPE=0xffff		//无类型文件(亦指文件过滤中的所有文件)
}FILE_Format;

//比较两个文件的条件
typedef enum
{
	CMPBY_NAME,
	CMPBY_SIZE,
	CMPBY_DATE
}FILE_CmpCond;

//文件实体
typedef struct _file
{
	char	*name;		//名指针(GB编码)
	int		format;		//格式
	int		length;		//大小

	boolean b_isHiden;	//是否隐藏
	boolean b_checked;	//是否选中
	boolean	b_isFile;	//类型

	struct _file *next;		//下一个
	struct _file *prev;		//上一个
}FILE, *PFILE;


//列表初始化（必须在下面任何方法之前调用）
void FL_init(boolean dirFirst, boolean order_ascending, int order_by);
//用默认数据初始化
#define  FL_initDef() \
	FL_init(true, true, CMPBY_NAME)

//文件构造方法
PFILE F_new(char *name, int nameLen, boolean isFile, int format, boolean isHiden);

#define F_new1(name, nameLen, isFile) \
	F_new(name, nameLen, isFile, FILE_UNDEFINE, false)

#define F_new2(name, nameLen, isFile, format) \
	F_new(name, nameLen, isFile, format, false)

//向列表的尾部追加指定的文件
void FL_add(PFILE pf);
//向列表追加并按名称排序（实时动态排序）
void FL_add_sortByName(PFILE pf);

//从列表中移除所有文件 
void FL_clear(void);

//返回列表中指定位置的文件
PFILE FL_get(int index);
//获取指定序号文件名
char* FL_getName(int index);
//获取指定序号文件格式
FILE_Format FL_getFormat(int index);
//获取列表文件数
int FL_getFCount(void);
//获取列表目录数
int FL_getDCount(void);
//获取指定序号文件是否选中
boolean FL_isChecked(int index);
//获取指定序号不是文件
boolean FL_isFile(int index);
//获取指定序号文件是否隐藏
boolean FL_isHiden(int index);

//返回列表中首次出现指定元素的索引，如果列表不包含此元素，则返回 -1。
int FL_indexOf(PFILE pf);

//如果列表不包含元素，则返回 true.
boolean FL_isEmpty(void);

//移除列表中指定位置的元素（可选操作）。 
void FL_remove(int index);

//用指定元素替换列表中指定位置的元素（可选操作）。
void FL_set(int index, PFILE pf); 

//返回列表中的元素数。 
int FL_size(void);

//选中指定文件
void FL_check(int index, boolean checked);

#endif
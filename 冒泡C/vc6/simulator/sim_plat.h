#ifndef __PLAT_H__
#define  __PLAT_H__
extern "C"
{
	#include "mrporting.h"	
}
#define MAX_LIST_NO       256
#define MAX_LIST_LEN       128
#define PLATEX_LISTBOXSELECT 1 //创建单、多选框
#define PLATEX_LISTBOXITEM 2//设置单，多选框
 enum
{
	MR_LIST_NO_ICON  ,   //(没有显示序号的) 
	MR_LIST_NORMAL_ICON ,//(一般的list的，建议调用menu的接口来实现)
	MR_LIST_SINGLE_SEL , //(单选框)
	MR_LIST_MUTI_SEL    //(多选框)
		
};

typedef struct
{
	int32 menu;     //list的句柄
	char *text;     //这一项的名称(大端unicode)
	int32 index;    //这一项在整个list中的位置(从0开始)
	int32 selected; //这一项是否标记为选中(1：选中；0：未选中)
}MMIMR_LISTBOXITEM_T;

typedef struct
{
	char *title;     //标题(大端unicode 编码)
	int16 itemNum;   //list 的项目个数
	int16 listType;  //如下
}MMIMR_LISTBOXSELECT_T;

typedef struct  {
	char title[MAX_LIST_LEN];
	char *items[MAX_LIST_NO];
	int32 selected[MAX_LIST_NO];
	int16 itemNum;
	int32 index;
	int16 listType;
}T_WIN_LIST;

int32 createListBox(uint8 *p);
int32 setListBox(uint8 *p);
#endif




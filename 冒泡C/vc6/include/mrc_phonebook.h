#ifndef _MRC_PB_H_
#define _MRC_PB_H_

#define MR_PHB_SUCCESS 				    (1000)
#define MR_PHB_ERROR 					(1001)
#define MR_PHB_READY 					(1002)
#define MR_PHB_NOT_SUPPORT 				(1003)
#define MR_PHB_NOT_READY 				(1004)
#define MR_PHB_NOT_FOUND 				(1005)
#define MR_PHB_STORAGE_FULL 			(1006) 
#define MR_PHB_NUMBER_TOO_LONG 			(1007)
#define MR_PHB_OUT_OF_INDEX 			(1008)
#define MR_PHB_MAX_NAME 				(36*2) 
#define MR_PHB_MAX_NUMBER 				(48*2)
#define MR_PHB_MAX_SEARCH_PATTERN 		(60*2)


enum PB_SEARCH_TYPE{
	TYPE_SEARCH_NONE,
	TYPE_SEARCH_NAME,
	TYPE_SEARCH_NUM
	};
enum PB_TYPE{
	TYPE_NONE,
	TYPE_PB_SIM,
	TYPE_PB_PHONE,
	TYPE_PB_ALL
};

typedef struct 
{
	char name[MR_PHB_MAX_NAME];
	char number[MR_PHB_MAX_NUMBER];
	char homeNumber[MR_PHB_MAX_NUMBER];
	char officeNumber[MR_PHB_MAX_NUMBER];
	char faxNumber[MR_PHB_MAX_NUMBER];
	char emailAddress[MR_PHB_MAX_NUMBER];
}T_PHB_ENTRY;
		
typedef struct
{
    char pattern[MR_PHB_MAX_SEARCH_PATTERN]; /*搜索条件大端unicode*/
    uint16 search_type;  /*搜索类型，1：通过名称；2：通过号码*/
    uint16 storage;       /*不关心*/
}T_PHB_SEARCH_REQ;

typedef struct
{
	uint16 index;                   /*在列表中的位置，起始是 0*/
	uint16 store_index;              /*在sim卡或者手机中物理的位置*/
	T_PHB_ENTRY phb_entry; /*具体信息*/
}T_PHB_SEARCH_RSP;

typedef struct 
{
     uint16 storage;//信息存储的位置，sim卡，还是手机
     uint16 index; //要修改的条目在列表中的索引（0开始），如果是 0xfffff表示增加一个条目
     T_PHB_ENTRY entry; //具体的信息
}T_PHB_SET_ENTRY_REQ;

typedef struct
{
     uint16 index;       //条目在列表中的索引
     uint16 storage;    //条目保存的位置。
}T_PHB_GET_ENTRY_REQ;

typedef struct
{
	 T_PHB_ENTRY entry;//本条目的具体信息
}T_PHB_GET_ENTRY_RSP;

typedef struct 
{
	int32 storage; /*1：sim卡电话本；2：手机电话本；3；全部*/
}T_PHB_GET_COUNT_REQ;

typedef struct
{
	int32 count;/*记录的个数*/
}T_PHB_GET_COUNT_RSP;

/**************************************************************************************
电话本按键事件说明
1、	电话本的操作事件采用mr_event的形式通知上层应用。
2、	Mr_event的三个参数的具体说明如下：
    Mr_event(p1,p2,p3);
			 p1 = 14
			 p2 = 1：读取条目未使用
			 p3 = MR_FAILED 操作失败
			      MR_SUCCESS 操作成功
***************************************************************************************/

/*

输入:
	T_PHB_SEARCH_REQ 
					char pattern[MR_PHB_MAX_SEARCH_PATTERN]; 搜索条件大端unicode
    				unsigned short int search_type;  搜索类型，TYPE_SEARCH_NAME或TYPE_SEARCH_NUM
    				unsigned short int storage;      不关心
输出:
	T_PHB_SEARCH_RSP 
					unsigned short int index;                   在列表中的位置，起始是 0
	              	unsigned short int store_index;             在sim卡或者手机中物理的位置,取值为TYPE_PB_SIM, TYPE_PB_PHONE,TYPE_PB_ALL
	              	T_PHB_ENTRY phb_entry;       具体信息

返回:
			 MR_PHB_NOT_READY 	 //电话本初始化中
             MR_PHB_NOT_FOUND    //2种情况，1：搜索参数为空；2：未找到
             MR_PHB_SUCCESS      //搜索成功

*/
int32 mrc_searchPBRecord(T_PHB_SEARCH_REQ *t_req,T_PHB_SEARCH_RSP *t_rsp);



/*
输入:
	T_PHB_GET_ENTRY_REQ 
					unsigned short int 	index;       //条目在列表中的索引，从0开始
     				unsigned short int  	storage;     //条目保存的位置，取值为TYPE_PB_SIM, TYPE_PB_PHONE,TYPE_PB_ALL

输出:
	T_PHB_GET_ENTRY_RSP
					T_PHB_ENTRY entry;//本条目的具体信息

返回:
			MR_PHB_OUT_OF_INDEX：索引越界
            MR_PHB_SUCCESS       读取成功
			MR_PHB_NOT_READY     电话本初始化中
备注:这个操作是一个异步的过程，操作的结构会通过mr_event的形式上送。 具体看前半部分事件说明。
*/

int32 mrc_readPBRecord(T_PHB_GET_ENTRY_REQ *t_req, T_PHB_GET_ENTRY_RSP *t_rsp);

/*
输入:
	T_PHB_GET_COUNT_REQ
					long int storage;  TYPE_PB_SIM, TYPE_PB_PHONE,TYPE_PB_ALL

输出:
	T_PHB_GET_COUNT_RSP
					long int count;记录的个数

返回:
			 MR_PHB_SUCCESS      获得记录成功
             MR_PHB_NOT_READY    电话本初始化中
             MR_PHB_NOT_SUPPORT  输入参数错误

*/
int32 mrc_getPBRecordCount(T_PHB_GET_COUNT_REQ *t_req, T_PHB_GET_COUNT_RSP *t_rsp);

/*
输入:
		 	void

输出:	
			void

返回:
			 MR_PHB_NOT_READY （1004）初始化中
      		 MR_PHB_READY      （1002）可以进行操作
*/
int32 mrc_getPBState(void);

/*
输入:
			void

输出:
			void

返回:
			 MR_SUCCESS ：创建浏览对象成功
             MR_FAILED  ：创建浏览对象失败
             MR_IGNORE  ：不支持该功能
*/
int32 mrc_createPBObj(void);

/*
输入:
			void

输出:
			void

返回:
			 MR_SUCCESS ：成功销毁
             MR_FAILED  ：销毁失败
             MR_IGNORE  ：不支持该功能

*/
int32 mrc_destoryPBObj(void);

/*
输入:
		

输出:
		T_PHB_ENTRY
				char	name[MR_PHB_MAX_NAME];          //姓名
	          	char	number[MR_PHB_MAX_NUMBER];		//号码
	          	char	homeNumber[MR_PHB_MAX_NUMBER];	//家庭号码
	          	char	officeNumber[MR_PHB_MAX_NUMBER];//办公号码
	          	char	faxNumber[MR_PHB_MAX_NUMBER];	//传真号码
	          	char	emailAddress[MR_PHB_MAX_NUMBER];//邮件地址
				

返回:
		MR_SUCCESS ：获得信息成功
        MR_FAILED  ：获得信息失败
        MR_IGNORE  ：不支持该功能
*/
int32 mrc_getPBInfo(T_PHB_ENTRY *t_recInfo);

#endif

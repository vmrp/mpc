#ifndef __GET_ADV_H__
#define __GET_ADV_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


#include "mrc_base.h"



typedef struct advInfo
{
	int32 price;            //广告单价（人民币单位：分）
	int32 quota;            //总投放配额（人民币单位：元）
	int32 remain;           //可用配额（人民币单位：分）
	int32 appid;
	char shortname[8];
	char app_name[50];		//应用名称
	char app_summary[250];	//应用简介
}st_advInfo;

typedef struct advData
{
	int32 realnum;        //真实得到的数量
	st_advInfo* infoList;
}st_advData;

/*
获取广告信息的回调函数
code = 0时有效
数据存在结构体st_advData，用完需要释放
*/
typedef void (*getAdv_cb)(int32 code,st_advData* pdata);

/*
  函数名：getAdv
  输入参数:
  num:需要获取的广告数量最多100，默认返回10
  getAdv_cb:获取到广告时的回调
*/
int32 getAdv(int32 num,getAdv_cb CB);

/*
  函数名：getadv_uninit
  输入参数:
  如果游戏退出时尚未收到回调，需要调用此函数处理一下网络
*/
void getadv_uninit(void);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __GET_ADV_H__ */
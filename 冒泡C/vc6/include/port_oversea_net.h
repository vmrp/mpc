#ifndef MRC_PORT_NET_OVERSEA_H
#define MRC_PORT_NET_OVERSEA_H

#include "mrc_base.h"
#include "mrc_exb.h"
#include "mrc_network.h"

typedef struct
{
	uint8 authentication_type;
	uint8 user_name[32];
	uint8 password[32];
	uint8 dns[4];
	uint8 apn[100];
}T_DSM_DATA_ACCOUNT;

#define MR_GET_DATA_ACCOUNT_REQ          1107  /*发送获得账号资料的请求，应用异步等待结果返回*/
#define MR_GET_DATA_ACCOUNT              1108  /*等获得账号资料的请求异步返回成功后调用本接口读取结果*/
#define MR_SET_DATA_ACCOUNT              1109  /*设置账号资料*/
#define MR_SET_DATA_ACCOUNT_USE_FLAG     1110  /*设置标志位：1、在mr_initNetWork的时候不会重新nvram中的值（保证用户手动设置的值不对被修改），*/
#define MR_SET_DATA_ACCOUNT_MTK_UI       1111  /*调用mtk的设置账号资料的界面进行设置，调用这个接口后VM会pause。VM无法得知后续的操作和结果*/

enum
{
	MR_DATA_ACCOUNT_OP_GET,
	MR_DATA_ACCOUNT_OP_SET
};

/**********************************************************************************
   读取统一位置的apn.ini文件，获取apn.ini值
   输入:
   返回:
   0    自动设置
   1    手动设置
**********************************************************************************/

uint8 mrc_GetApnSettings(void);


/**********************************************************************************
   mr_initNetworkEx 在这个函数中，先调用接口来或者配置信息（apn.ini），根据配置信息来设置参数
   ，然后调用mr_initNetwork
   输入:
   返回:
   MR_FAILED     失败
   MR_SUCCESS    成功
**********************************************************************************/

//int32 mrc_initNetworkOversea(MR_INIT_NETWORK_CB cb, const char *mode);


void mrc_GPRS_ConnectOversea(GPRS_MODE mode, gprs_event_handler handler, DWORD timeout, APN_MODE_E apn_mode);


/**********************************************************************************
   设置apn方式，并保存到apn.ini文件中
   输入:
   flag：1 手动设置 0 手动设置
**********************************************************************************/

void mrc_setApnFlag(uint8 flag);

/**********************************************************************************
   设置apn标志为1，并调用本地apn设置界面。
   输入:
   返回:
**********************************************************************************/

void mrc_set_apn(void);

/**********************************************************************************
   获取帐号资料请求
   输入:
   返回:
**********************************************************************************/
int32 mrc_getApnRequest(void);
/**********************************************************************************
   获取帐号资料
   输入:
   返回:
**********************************************************************************/
int32 mrc_getApnAccount(T_DSM_DATA_ACCOUNT*info);

/*
函数名:mrc_editNetworkAccount
	功能：用户编译网络接入帐号。只有海外的联网应用需要使用这个接口。
	
	因为海外的运营商很多，各个运营商使用的帐号资料也不尽相同。而且网关地址也可能不一样。所以为了处理这样复杂的情况。我们做了如下假定：
	A． 海外版本只支持直接连接互联网的模式（避免网关地址不同的情况）
	B． 开放接口，在连接网络失败的情况下，让用户来设置正确的帐号资料

	1.	在海外的版本中需要增加一个菜单。这个菜单的功能就是让用户来设置正确的帐号资料。调用的接口是：mrc_editNetworkAccount();调用这个接口之后应用会收到PAUSE事件，
	等用户输入正确的信息后，应用会收到RESUME事件，同时需要应用保存一个标志位来表明用户是否已经设置过新的帐号资料，以后每次运行该应用在调用mr_initNetWork的前都需要判断这个标志位，并调用接口来设置这个标志位：
	设置标志位的接口是：mrc_selectNetworkAccount(option) 的值的含义如下：
	option：
		0：使用用户设置的帐号资料
	   	1：使用出厂预设的帐号资料

	
	输入：
		  无
	返回：
	MR_SUCCESS :	设置成功。
	MR_FAILED	:	设置失败。
	MR_IGNORE	:	不支持。

*/
int32 mrc_editNetworkAccount(void);

#endif

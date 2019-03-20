#ifndef SKY_PAYMENT_H
#define SKY_PAYMENT_H
#include "mrc_base.h"

#define PAYMENT
#define PAYMENT_TOM

#define PAY_MAXRESENDTIMES 3
 //共有几个通道，
 typedef enum{
	ZHANGSHANGLINGTONG_2RMB=0,		// 0表示2元掌上灵通的通道，移动1元，联通2元
	PAYMENT_TEST,										// 1表示测试
	ZHIYUNSHIDAI_2RMB, 							// 2 表示指云时代的通道，移动1元，联通2元
	ZHANGSHANGLINGTONG_2RMB_MMS,	// 3表示掌上灵通的2块钱彩信通道，只有移动。从20080808开始用10662000300350的15省2元短信代替。
	ZHIYUNSHIDAI_1RMB,							// 4指云时代1RMB联通通道。
	XINQINGHUDONG_1RMB,						// 5心情互动1RMB联通通道。
	GUNSHIYIDONG_2RMB,							// 6滚石移动2RMB，移动、联通均2元。
	CHUANGYIHEXIAN_2RMB,						// 7创意和弦2RMB，移动、联通均2元。
	TOM_2RMB,												// 8TOM通道，移动、联通均2元。
	TOM_TEST2RMB,										// 9TOM测试通道，移动、联通均2元。
	ZHIYUNSHIDAI_2RMB_JS,						// 10指云时代移动1元联通2元，限江苏省使用。其他省份不可用。
	ZHIYUNSHIDAI_2RMB_MMS,					// 11 指云时代彩信通道，移动联通均2元。
	ZHIYUNSHIDAI_1RMB_MT,						// 12指云时代移动多MO单MT，联通多MO多MT通道，移动联通均1元。	
	ZHANGSHANGLINGTONG_2RMB_318,	//13掌上灵通2RMB。
	ZHIYUNSHIDAI_ZMCC10MT_UNICOM20,// 14指云时代移动1元，采用单MT下行屏蔽指令、联通2元
	BEIWEI_CMCC10_UNICOM20,				// 15北纬通道，移动1元，联通2元。	
	ZHANGSHANGLINGTONG_20,					//16 和0号通道完全一致，主要是为了和阅读产品兼容而 增加的通道。表示2元掌上灵通的通道，移动1元，联通2元
	PAY_CHN_ZY31,										//17 指云时代移动3元彩信通道，联通使用1元指令。
	PAY_CHN_XQ21,										//18 心情互动移动2元通道，联通使用1元指令。
	INDONESIA_2000RB,								//19印度尼西亚2000卢比通道。
	PAY_CHN_TMGAME22,							//20TOM彩信游戏通道彩信，移动联通均2元
	PAY_CHN_TMBOOK22,							//21TOM彩信电子书通道，移动联通均2元
	PAY_CHN_ZS22,										//22掌上灵通10662000300350通道，移动、联通均2元；
	PAY_CHN_CY12,										//23创艺和弦通道，移动1元，联通2元
	PAY_CHN_GS22_GUANGDONG,				//24滚石广东通道。移动2源，联通2源。
	PAY_CHN_ZX11,										//25移动长沙掌讯1元，联通指云1元
	PAY_CHN_ZX22,										//26移动长沙掌讯2元，联通指云2元
	PAY_CHN_ZYRTT22,								// 27指云时代彩信通道，移动联通均2元。
	PAY_CHN_ZSSIK22,									// 28掌上灵通短信通道，移动联通均2元。是318通道。
	PAY_CHN_CYAKJ22,								// 29创意和弦短信通道，移动联通均2元。
	PAY_CHN_ZYTGT22,								// 30指云时代短信通道，移动联通均2元。
	PAY_CHN_KZ11,									// 31短信通道，移动联通均1元。
	PAY_CHN_KZ22,									// 32短信通道，移动联通均2元。	
	PAY_CHN_ZSD12,									// 33短信通道，移动1元，联通2元。
	PAY_CHN_ZYSIGN22,								// 34短信通道，移动2元，联通2元。
	PAY_CHN_ZSDJF22,								// 35短信通道，移动2元，联通2元。	
	PAY_CHN_YXA22,									// 36短信通道，移动2元，联通2元。
	PAY_CHN_HDGC11,									// 37短信通道，移动1元，联通1元。	
	PAY_CHN_HY12,									// 38短信通道，移动1元，联通2元。		
	PAY_CHN_ZYYBT12,								// 39短信通道，移动1元，联通2元。				
	PAY_CHN_40GD22,							// 40短信通道，移动2元，联通2元。广东专用。
	PAY_CHN_41GD22,							// 41短信通道，移动2元，联通2元。广东专用。
	PAY_CHN_42GD22,							// 42短信通道，移动2元，联通2元。广东专用。			
	PAY_CHN_43ZY11,							// 43短信通道，移动1元，联通1元。
	PAY_CHN_44HY22,							// 44短信通道，移动2元，联通2元。
	PAY_CHN_45HY22,							// 45短信通道，移动2元，联通2元。				
	PAY_CHN_46ZY22,							// 46短信通道，移动2元，联通2元。
	PAY_CHN_47TMG11,						// 47短信通道，移动1元，联通1元。					
	PAY_CHN_48KZ22,							// 48短信通道，移动2元，联通2元。
	PAY_CHN_49KZ11,							// 49短信通道，移动1元，联通1元。						
	PAY_CHN_50ZX22,							// 50短信通道，移动2元，联通2元。					
	PAY_CHN_51YZ22, 						// 51短信通道，移动2元，联通2元。
	PAY_CHN_52DT11, 						// 52短信通道，移动1元，联通1元。						
	PAY_CHN_53CY22, 						// 53短信通道，移动2元，联通2元。							
	PAY_CHN_54GG22,							// 54短信通道，移动2元，联通2元。
	PAY_CHN_55NJ11,							// 55短信通道，移动2元，联通2元。	
	PAY_CHN_56BW22, 						// 56短信通道，移动2元，联通2元。	
	PAY_CHN_57GS22, 						// 57短信通道，移动2元，联通2元。	
	PAY_CHN_58KZ32, 						// 58短信通道，移动3元，联通2元。		
	PAY_CHN_59GZ52, 						// 59短信通道，移动5元，联通x元。		
	PAY_CHN_60NJ11, 						// 60短信通道，移动1元，联通x元。		
	PAY_CHN_61HD22, 						// 61短信通道，移动2元，联通x元。		
	PAY_CHN_62HD31, 						// 62短信通道，移动3元，联通x元。		

	PAY_CHN_63XZ1, 							// 58短信通道，移动1元
	PAY_CHN_64XZ1, 							// 59短信通道，移动1元
	PAY_CHN_65XL2, 							// 60短信通道，移动2元
	PAY_CHN_66XL1, 							// 61短信通道，移动1元
	PAY_CHN_67XL2, 							// 62短信通道，移动2元

	PAY_CHN_68NJ1, 							// 68短信通道，移动1元
	PAY_CHN_69ZY1, 							// 69短信通道，移动1元
	PAY_CHN_70XZ2, 							// 70短信通道，移动2元
	MAX_PAY_CHANNEL				
}PAY_CHANNEL;


typedef void (*mrc_PayCB)(int32 data);
typedef struct{
		uint16 isReg;//是否已注册
		uint32 Option;//是否生成签名文件，1:生成；0:不生成；
		uint16 Channel;//收费通道
		uint16 value;//收费金额
		int32 timeOut;//超时时间，若为0则不等待，立刻返回
		int32 data;//传给回调函数的参数。
		mrc_PayCB fcb;
}PAY_STATUS_t;
typedef struct{
		uint32 Option;//是否生成签名文件，1:生成；0:不生成；
		uint16 Channel;//收费通道
		uint16 value;//收费金额
		int32 timeOut;//超时时间，若为0则不等待，立刻返回
		int32 data;//传给回调函数的参数。
		mrc_PayCB fcb;
}PAY_CALLBACK_t;

 typedef enum{
	CHECK_IS_REGISTER=0x00,
	CHECK_IS_FREE=0x01,
	CHECK_IS_MONTHUSER=0x02,//用户是包月用户
	CHECK_IS_REREG=0x03		//用户需要补注册。
}PAY_CHECK_OPTION;

#define MR_REREG 3


 typedef enum{
	PAY_MSG_MAGIC=0,	//道具短信
	PAY_MSG_REGISTER,	//注册短信
	PAY_MSG_SCORE,	//收费积分短信
	PAY_MSG_UNIPAY,	//统一充值短信
	PAY_MSG_DATA,
	PAY_MSG_REREG=7	//补注册。
}PAY_MSG_OPTION;






typedef struct{
		int32 MCC;			//收费提示、收费金额的语言ID
		int32 AppLanguageID;		//操作说明、帮助介绍的语言ID
}PAY_CTR_INFO_T;
/*
mrc_initChargeOverSea功能说明:
	初始化计费模块，申请各种资源。必须首先调用。
	在多国语言版本中，资费说明和收费金额、操作说明和游戏中
	语言文字都保存在EXCEL表生成的资源文件中。
1、计费初始化时，取得计费控制信息；
2、根据计费控制信息，取得各个收费点的收费金额；
3、如果注册费为0，则不应该弹出注册提示；
4、如果道具收费为0，可以弹出道具信息，用户点击确定后调用短信发送函数，
短信发送函数将返回成功。

输入:
PAY_CTR_INFO_T *tPayCtrlInfo;
	
输出:
PAY_CTR_INFO_T *tPayCtrlInfo;


注意:
		当处于飞行模式、无网络等情况时，计费模块将初始化失败。
		
返回值:
MR_SUCCESS:初始化成功；
MR_FAILED:初始化失败。
*/
int32 mrc_initChargeOverSea(PAY_CTR_INFO_T *tPayCtrlInfo);

/*
mrc_checkChargeExOverSea功能说明:
	mrc_checkChargeExOverSea(CHECK_IS_REGISTER)==MR_SUCCESS，用户已注册应用；
	目前只支持CHECK_IS_REGISTER属性。
输入:
	无。
	
输出:
MR_SUCCESS:条件匹配；
MR_FAILED:查找失败。
*/
int32 mrc_checkChargeExOverSea(int32 Option); 

/*
checkCharge功能说明:
	检查应用所在目录的appname子目录有无成功发送计费短信的签名文件，
若有，则返回成功，否则返回失败。	

输入:
	无。
	
输出:
MR_REREG:需要补注册。
MR_FAILED:没有找到指定的签名文件或者签名文件不是本手机的。
MR_SUCCESS:找到成功发送了计费短信的合法签名文件；
*/

int32 mrc_checkCharge(void); 

/*
checkChargeEx功能说明:
	mrc_checkChargeEx(CHECK_IS_REGISTER)==MR_SUCCESS，用户已注册应用；
	mrc_checkChargeEx(CHECK_IS_FREE)==MR_SUCCESS，游戏在该运营商免费；	
	mrc_checkChargeEx(CHECK_IS_MONTHUSER)==MR_SUCCESS，用户是包月用户。	

输入:
	无。
	
输出:
MR_SUCCESS:条件匹配；
MR_FAILED:查找失败。
*/
int32 mrc_checkChargeEx(int32 Option); 
/*
mrc_RegisterSid功能说明:
给当前游戏、当前用户生成注册文件；
(1)、若用户已注册，则不做任何操作；
(2)、若注册文件不存在，则新生成注册文件；
(3)、若注册文件已存在，则将用户的IMSI添加到注册文件中；

输入:
	无。
	
输出:
MR_SUCCESS:注册成功。
MR_FAILED:注册失败。
*/
int32 mrc_RegisterSid(void); 

/*
函数名称:	mrc_ChargeExOverSea
功能说明:	提供带回调函数的灵活扩展的海外版收费功能。
	在最后一条MO短信返回成功的状态报告后，回调函数将被执行。

输入:
Option:	值为0时，表示不写记录，用于购买道具等多次发送的场合；
				值为1时，表示要写记录，用于产品购买注册等场合。若检查
				到已付费的存盘记录，则本函数立刻返回成功。
				
Channel:	收费渠道，由斯凯定义、分配。
value:		收费金额，单位为通道的计费单位。最大
				取值范围为10条该通道SP短信的对应值，例如，1.5的通道最大值为150。
uint16 mcc:
				从Excel表中取得的MCC值，主要是用于校验，避免出错。
				
uint16 ChargePoint:
				计费点编号。

int32 data:回调函数的参数。
				
mrc_PayCB f:回调函数，当f=NULL时，回调函数将被忽略，这时本函数行为
	和mrc_ChargeEx完全一致。
	
int32 timeOut:
	超时保护定时器等待的毫秒数。如果超时时间还是没有收到所有的状态报告，
	，当超时时间到达时，回调函数必须被调用。在传给应用 的data数据中指明尚未
	收到的短信条数。若该值为0，则所有短信提交协议栈成功后立刻返回，若该值
	为负数，则永久等待，直到收到足够的短信发送成功状态报告。
	
输出:
MR_SUCCESS:第一条MO短信成功提交协议栈；后续短信将异步MO。
MR_FAILED:收费失败。
*/
int32 mrc_ChargeExOverSea(uint32 Option,uint16 Channel,uint16 value,uint16 mcc,uint16 ChargePoint,int32 data,mrc_PayCB f,int32 timeOut);

/*
releaseCharge功能说明:
	释放计费模块相关资源。
	
输入:
	无。

输出:
MR_SUCCESS:成功释放资源。
MR_FAILED:释放失败。
*/
int32 mrc_releaseChargeOverSea(void);



/*
mrc_initDefaultChn功能说明:
	配置SDK计费模块的默认主、副SP计费通道。
	SDK计费模块总是尝试优先使用主通道进行收费；
	如果通道单价高于收费金额，则尝试使用副通道进行收费；
	目前暂时还不支持将主通道收费余额转给副通道收取的方式，
	日后将视情况支持。
在SDKv1.0.2.42以及之前的版本中，不支持副通道，主通道的移动联通也是相同地进行切换。
在SDKv1.0.2.43版本中，只有主通道支持通道切换，副通道不支持切换；
在SDKv1.0.3.0到SDKv1.0.3.15版本中，主副通道不支持移动、联通分开配置。
在SDKv1.0.3.16之后的版本中，主副通道均支持切换。

输入:
uint16 priChn:	
	主通道ID；
	高字节为联通主通道ID，低字节为移动主通道ID。
	当高字节为0时，联通主通道ID和移动主通道ID相同。

uint16 slvChn:	
	副通道ID。
	高字节为联通副通道ID，低字节为移动副通道ID。
	当高字节为0时，联通副通道ID和移动副通道ID相同。	
	
输出:
无。

注意:
	在计费初始化之后，调用发送收费短信的接口之前，必须调用mrc_initDefaultChn。
返回值:
MR_SUCCESS:设置成功。
MR_FAILED:设置失败。
*/
int32 mrc_initDefaultChn(uint32 priChn,uint32 slvChn);
/*
mrc_ConfirmSms功能说明:	用户进行二次确认。
	在mrc_ChargeExConfirm调用后，如果通道需要二次确认，在收到确认提醒
短信后，回调函数cbQueConfirm将被调用，然后应用将提示用户是否确认
购买行为，应用均需要调用mrc_ConfirmSms函数，告知用户态度。

(1)、如果用户选择购买，则计费模块将很快就调用mrc_PayCB f；并在后台异步的
完成二次确认以及后续其他短信的自动二次确认；
(2)、如果用户选择不购买，则计费模块将清空短信发送队列，回调函数
mrc_PayCB f将不会被执行。
	
输入:
int32 isBuy:
	0:用户放弃二次确认；
	1:用户选择二次确认。

*/
void mrc_ConfirmSms(int32 isBuy);

/*
函数名称:	mrc_scoreConfirm
功能说明:	支持二次确认和积分上传的短信计费接口。
1、如果最终使用的通道不需要二次确认，则本函数的行为和mrc_ChargeExOversea一致；
2、如果最终使用的通道需要二次确认，则在收到运营商回复的第一条的回复请求后，
调用回调函数cbQueConfirm。

输入:
Option:	值为0时，表示不写记录，用于购买道具等多次发送的场合；
				值为1时，表示要写记录，用于产品购买注册等场合。若检查
				到已付费的存盘记录，则本函数立刻返回成功。
				
value:		收费金额，单位为通道的计费单位。最大
				取值范围为10条该通道SP短信的对应值，例如，1.5的通道最大值为150。
				
mrc_PayCB f:短信发送完毕的回调函数，当f=NULL时，回调函数将被忽略，这时本函数行为
	和mrc_ChargeEx完全一致。
	
int32 data:
	当f函数被调用时的回调函数参数。
	
int32 timeOut:
	超时保护定时器等待的毫秒数。如果超时时间还是没有收到所有的状态报告，
	，当超时时间到达时，回调函数必须被调用。在传给应用 的data数据中指明尚未
	收到的短信条数。若该值为0，则所有短信提交协议栈成功后立刻返回，若该值
	为负数，则永久等待，直到收到足够的短信发送成功状态报告。

mrc_PayCB 	cbQueConfirm:	二次确认短信通知函数。
	当该通道需要二次确认时，收到二次确认提醒后，该函数将被执行。

uint16 mcc:	
	从Excel表中取得的MCC值，主要是用于校验，避免出错。
	
int32 score:
	若需要上传积分，则调用本函数。
输出:
MR_SUCCESS:第一条MO短信成功提交协议栈；后续短信将异步MO。
MR_FAILED:收费失败。
*/

int32 mrc_scoreConfirm(uint32 Option,uint16 value,mrc_PayCB f,int32 data,int32 timeOut,mrc_PayCB cbQueConfirm,uint16 mcc,int32 score);

/*
函数名称:	mrc_payMoney
功能说明:	带记录计费点功能的付费接口。积分上传时，不能记录计费点。

输入:
Option:	值为0时，表示不写记录，用于购买道具等多次发送的场合；
				值为1时，表示要写记录，用于产品购买注册等场合。若检查
				到已付费的存盘记录，则本函数立刻返回成功。
				
value:		收费金额，单位为通道的计费单位。最大
				取值范围为10条该通道SP短信的对应值，例如，1.5的通道最大值为150。
				
mrc_PayCB f:短信发送完毕的回调函数，当f=NULL时，回调函数将被忽略，这时本函数行为
	和mrc_ChargeEx完全一致。
	
int32 data:
	当f函数被调用时的回调函数参数。
	
int32 timeOut:
	超时保护定时器等待的毫秒数。如果超时时间还是没有收到所有的状态报告，
	，当超时时间到达时，回调函数必须被调用。在传给应用 的data数据中指明尚未
	收到的短信条数。若该值为0，则所有短信提交协议栈成功后立刻返回，若该值
	为负数，则永久等待，直到收到足够的短信发送成功状态报告。

mrc_PayCB 	cbQueConfirm:	二次确认短信通知函数。
	当该通道需要二次确认时，收到二次确认提醒后，该函数将被执行。

uint16 mcc:	
	从Excel表中取得的MCC值，主要是用于校验，避免出错。

int32 score:
	若需要上传积分，则调用本函数。
	
uint8 chargePont:
	计费点编号，取值范围为0到255，默认值为0.

输出:
MR_SUCCESS:提交协议栈成功；
MR_FAILED:提交协议栈失败，没有任何计费短信发送。
*/
int32 mrc_payMoney(uint32 Option,uint16 value,mrc_PayCB cbFinishPay,int32 data,int32 timeOut,mrc_PayCB cbConfirmAgain,uint16 mcc,int32 score,uint8 chargePont);

//启动WAP浏览器查询积分。
void mrc_LookScore(void);


/*
函数名称:	mrc_getPolicy
功能说明:	获取SDK的收费策略等级。

输入:
int32 inputPolicy:	应用的默认收费策略等级，不能用0，必须是真实存在的的等级。
int32 lockFlag:	应用是否锁定策略、不采用策略文件默认配置，
0表示不锁定，1表示锁定。对于锁定的应用，下载的策略文件
必须列举APPID或者设置强制解锁标志。

输出:
	无。

返回值:
	应用的收费策略。

*/

int32 mrc_getPolicy(int32 inputPolicy,int32 lockFlag);

/*
函数名称:	mrc_setPolicy
功能说明:	设置应用的收费策略等级。

输入:
int32 finalPolicy:	应用最终采用的收费策略等级，不能用0，必须是真实存在的的等级。
输出:
	无。

返回值:
	返回成功，说明设置的策略生效；
	若返回失败，则应用要检查，如果应用默认策略是高收费策略，则应当降级到常规策略。

*/

int32 mrc_setPolicy(int32 finalPolicy);

typedef struct{
	int32 smsNum;		//需要发送的短信条数
	int32 price;		//短信价格单位，人民币角
	int32 spStrEnable;	//值为1时表示提示语有效。
	int32 spStrLen;		//SP短信提示语字节数。	
	char  *spStrBuf;	//SP短信提示语接收缓冲区指针，采用unicode编码，该内存块由SDK计费模块管理，禁止应用释放该内存块							
}PAY_CTR_SPINFO_T;
/*
函数名称:	mrc_getSPInfo
功能说明:	获取付费信息

输入:
uint32 Option:		付费类型
uint16 value:		收费总金额，单位是人民币角。
uint16 mcc:		中国大陆，MCC默认填写为460。
输出:

PAY_CTR_SPINFO_T *spInfo	:函数若返回成功，则该缓冲区有意义。	

返回值:
	若返回成功，则spInfo有意义；但是返回成功仍然可能出现
	mrc_getSPInfo->spStrBuf为NULLd的情况，需要根据spInfo[0].spStrEnable判定。
	若返回失败，则spInfo无意义。

*/
int32 mrc_getSPInfo(uint32 Option,uint16 value,uint16 mcc,PAY_CTR_SPINFO_T *spInfo);


/*
函数名称:	付费回调函数mrc_uniPayCB
功能说明:	付费流程完毕后的回调函数

输入:
int32 result:	
		MR_FAILED:	用户点击取消
		MR_SUCCESS:	用户点击确定，并且付费成功
		MR_INGNORE:	用户点击确定，但是实际付费彻底失败

int32 cbData:	应用付费时通过mrc_uniPayUI传给付费回调函数的参数。
*/
typedef void (*mrc_uniPayCB)(int32 result,int32	cbData);
typedef struct{
uint32	mcc;				//国家MCC
uint32	Option;				//付费类型，增加一个补注册类型PAY_MSG_REREG。检测注册时添加一个是否补注册的状态。
int32	value;				//收费金额。在中国大陆，收费金额的单位为人民币角
uint8	chargePoint;		//计费点编号。当有多个收费点时，每个收费点赋予不同的计费点编号，以便进行运营数据分析。
char	*uincodeTitle;		//付费页面的标题；根据运营商政策和实际情况需要，斯凯可能添加、修改、删除该字段内容。
char	*unicodePayText;	//付费页面的正文。根据运营商政策和实际情况需要，斯凯可能添加、修改、删除该字段内容。
int32	dataLen;			//需要携带的数据信息字节数
char	*data;				//需要携带的数据指针
int32	timeOut;			//用户确认付费后、发送短信的等候超时时间，单位是毫秒数。对于F420等非规范移植版本机器，计费模块将自动修改为30000ms。
int32	cbData;				//传给回调函数的参数。
mrc_uniPayCB uniPayCallBack;	//付费回调函数。回调函数的result参数小于0表示用户选择取消付费，大于等于0表示用户进行了确认。
}PAY_CTR_UNIPAY_T;

/*
函数名称:	mrc_uniPay
功能说明:	斯凯提供的统一的支付处理流程界面。
	目前暂时不支持字库。

输入:
	PAY_CTR_UNIPAY_T *ctrlUnipay:	付费相关的控制信息。
输出:
	无，处理结果将通过回调函数异步返回。

返回值:
	MR_SUCCESS:	参数合法有意义，将出现斯凯提供的UI付费界面。
	MR_FAILED:参数非法，或者上一次付费还在处理中，不会出现斯凯UI付费界面，回调函数不会被执行。

*/
void mrc_uniPayUI(PAY_CTR_UNIPAY_T *ctrlUnipay);


/*
函数名称:	mrc_uniPayUI_active
功能说明:	判断应用是否处于uniPayUI的付费界面中。

输入:
	无。
输出:
	无。
返回值:
	若返回0，表示不在付费界面中；
	若返回非0值，表示在付费界面中。
*/
int32 mrc_uniPayUI_active(void);

enum GAME_NETWORK_STYLE
{
	GAME_NETWORK_NONE,			// 不需要连接网络
	GAME_NETWORK_CONNECT		// 需要连接网络
};

typedef struct{
	int32 networkStyle;			// 网络连接类型 GAME_NETWORK_STYLE
	int32 gameRegPrice;			// Game注册时需要的金额，单位:角
	int32 reserve1;				// 保留1
	int32 reserve2;				// 保留2
	int32 reserve3;				// 保留3
	int32 reserve4;				// 保留4
}UNIPAY_SETPARAM_T;
/*
函数名称:	mrc_uniPaySetParam
功能说明:	调用mrc_uniPayUI付费时预设参数。
注:
	需要在mrc_initChargeOverSea之前调用
输入:
	UNIPAY_SETPARAM_T。
输出:
	无。
返回值:
*/
void mrc_uniPaySetParam(UNIPAY_SETPARAM_T *setParam);
#endif

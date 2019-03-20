#ifndef _MRP_VERFIY_H_

#define _MRP_VERFIY_H_

typedef enum
{
	E_VERIFY_RESULT_SUC,			//签名校验通过
	E_VERIFY_RESULT_FAILED,		//签名校验失败
	E_VERIFY_RESULT_NOCER,		//未发现签名证书
	E_VERIFY_RESULT_MAX
	//...	
}E_MRPVERIFY_RESULT;

/*
函数功能:验证MRP签名证书的合法性

参数说明:mrpname-->指定对哪个MRP进行签名校验
						 要求相对于Mythroad系统根目录的完整路径
						 NULL标识Check本运行MRP
						 

函数返回值 : 见E_MRPVERIFY_RESULT 定义
	
*/
E_MRPVERIFY_RESULT  mrc_verifyMrpCertificate_V1(const char *mrpName);

E_MRPVERIFY_RESULT  mrc_verifyMrpCertificate_V2(const char *mrpName);

E_MRPVERIFY_RESULT  mrc_verifyMrpCertificate_V3(const char *mrpName);

E_MRPVERIFY_RESULT  mrc_verifyMrpCertificate_V4(const char *mrpName);
#endif
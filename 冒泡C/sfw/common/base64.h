#ifndef TOMCRYPT_H_
#define TOMCRYPT_H_

#ifdef __cplusplus
extern "C" {
#endif

/* 错误代码 */
enum {
   CRYPT_OK=0,             /* 结果成功*/
   CRYPT_ERROR,            /* 一般错误*/
   CRYPT_NOP,              /* Not a failure but no operation was performed 不是失败，但没有执行操作*/

   CRYPT_INVALID_KEYSIZE,  /* 给予的密钥大小无效 */
   CRYPT_INVALID_ROUNDS,   /* Invalid number of rounds无效的号码 */
   CRYPT_FAIL_TESTVECTOR,  /* Algorithm failed test vectors 算法未能测试向量*/

   CRYPT_BUFFER_OVERFLOW,  /* 没有足够的空间输出 */
   CRYPT_INVALID_PACKET,   /* 无效的输入数据包 */

   CRYPT_INVALID_PRNGSIZE, /* Invalid number of bits for a PRNG 位无效的PRNG的数*/
   CRYPT_ERROR_READPRNG,   /* Could not read enough from PRNG 无法读取足够的PRNG的*/

   CRYPT_INVALID_CIPHER,   /* Invalid cipher specified 无效的密码指定*/
   CRYPT_INVALID_HASH,     /* Invalid hash specified 无效的哈希指定*/
   CRYPT_INVALID_PRNG,     /* Invalid PRNG specified 无效的PRNG的指定*/

   CRYPT_MEM,              /* 内存不足*/

   CRYPT_PK_TYPE_MISMATCH, /* Not equivalent types of PK keys 键PK的不等价类型*/
   CRYPT_PK_NOT_PRIVATE,   /* Requires a private PK key 需要一个私人的PK键*/

   CRYPT_INVALID_ARG,      /* Generic invalid argument 通用无效的参数*/
   CRYPT_FILE_NOTFOUND,    /* 文件未找到*/

   CRYPT_PK_INVALID_TYPE,  /* Invalid type of PK key 无效的PK型键*/
   CRYPT_PK_INVALID_SYSTEM,/* Invalid PK system specified 无效的PK系统指定**/
   CRYPT_PK_DUP,           /* Duplicate key already in key ring 重复键已经在钥匙圈*/
   CRYPT_PK_NOT_FOUND,     /* Key not found in keyring 键未找到钥匙圈*/
   CRYPT_PK_INVALID_SIZE,  /* Invalid size input for PK parameters 无效的PK参数*尺寸输入*/

   CRYPT_INVALID_PRIME_SIZE,/* Invalid size of prime requested无效请求的黄金尺寸 */
   CRYPT_PK_INVALID_PADDING /* Invalid padding on input 无效的填充上输入*/
};


/**
   base64 Encode a buffer (NUL terminated)
   @param in      The input buffer to encode
   @param inlen   The length of the input buffer
   @param out     [out] The destination of the base64 encoded data
   @param outlen  [in/out] The max size and resulting size
   @return CRYPT_OK if successful
*/
/**
   Base64编码（以NUL终止）
   @在输入缓冲区参数进行编码
   @参数inlen的输入缓冲区的长度
   @参数进行[输出]的Base64编码数据目标
   @参数outlen [输入/输出]和由此产生的规模最大尺寸
   如果成功返回CRYPT_OK @
*/

int base64_encode(const unsigned char *in,  unsigned long inlen, 
                        unsigned char *out, unsigned long *outlen);


/**
   Base64解码

   参数in         base64的解码数据
   参数inlen      base64的数据长度
   参数out        [输出]解码的二进制数据
   参数outlen     [输入/输出]最大尺寸以及由此产生的解码数据的大小
   如果成功, 返回CRYPT_OK
*/
int base64_decode(const unsigned char *in,  unsigned long inlen, 
                        unsigned char *out, unsigned long *outlen);

#ifdef __cplusplus
   }
#endif

#endif /* TOMCRYPT_H_ */

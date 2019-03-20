#include "md5.h"

/*  
 * \用MD5算法取散列值
 * \参数szSour			源字符串
 * \参数iLen			源字符串的长度
 * \参数[输出]szDest	输出16个字节
 */
VOID MD5(BYTE *szSour, int iLen, BYTE *szDest)
{
	md5_state_t context;

	SGL_MEMSET(&context, 0, sizeof(context));
	mrc_md5_init(&context);
	mrc_md5_append(&context, (md5_byte_t *)szSour, iLen);
	mrc_md5_finish(&context, (md5_byte_t *)szDest);
}


#ifndef _MD5_H
#define _MD5_H

#include "types.h"

/*  
 * \用MD5算法取散列值
 * \参数szSour			源字符串
 * \参数iLen			源字符串的长度
 * \参数[输出]szDest	输出16个字节
 */

VOID MD5(BYTE *szSour, int iLen, BYTE *szDest);

#endif /*_MD5_H*/
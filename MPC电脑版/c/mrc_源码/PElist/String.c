/*
 * 字符串函数代码
 * 版权所有 (C) 2011-2012 eleqian
 * [6/7/2012]
 */

#include <mrc_base.h>
#include <mrc_exb.h>

#include "String.h"

//转换Unicode字符串为数值
int32 watoi(const char *uniStr)
{
    int32 sLen, i, rValue;
    char *asciiStr;

    sLen = wstrlen(uniStr) / 2;
    asciiStr = (char*)malloc(sLen + 1);

    for (i = 0; i < sLen; i++)
    {
        asciiStr[i] = uniStr[i * 2 + 1];
    }

    asciiStr[sLen + 1] = '\0';
    rValue = atoi(asciiStr);
    free(asciiStr);

    return rValue;
}

//转换数值为Unicode字符串
char *witoa(int32 value)
{
    char inValue[13] = {0};
    char *outStr;
    int32 sLen, i;

    sprintf(inValue, "%d", value);
    sLen = strlen(inValue);
    outStr = (char*)malloc(sLen * 2 + 2);
    memset(outStr, 0, sLen * 2 + 2);

    for (i = 0; i < sLen; i++)
    {
        outStr[i * 2 + 1] = inValue[i];
    }

    return outStr;
}

/*---------------------------------------------------------------------------*/

//将字符串转换为大写
char *strupr(char *str)
{
    char *p;

    for (p = str; '\0' != *p; p++)
    {
        if (*p >= 'a' && *p <= 'z')  // 小写字母
        {
            *p -= 32;
        }
    }

    return str;
}

//将字符串转换为大写
char *strlwr(char *str)
{
    char *p;

    for (p = str; '\0' != *p; p++)
    {
        if (*p >= 'A' && *p <= 'Z')  // 大写字母
        {
            *p += 32;
        }
    }

    return str;
}

// 从字符串末尾搜索字符
char *strrchr(const char * str, int ch)
{

    char *p = (char *)str;

    while (*p) 
        p++;

    while (p-- != str && *p != (char)ch)
        ;

    if (*p == (char)ch)
        return p;

    return NULL;
}

//不区分大小写比较字符串
int32 stricmp(const char *str1, const char *str2)
{
    uint8 *dst = (uint8*)str1;
    uint8 *src = (uint8*)str2;
    uint8 f, l;
    
    do {
        if (((f = *(dst++)) >= 'A') && (f <= 'Z'))
            f -= ('A' - 'a');
        
        if (((l = *(src++)) >= 'A') && (l <= 'Z'))
            l -= ('A' - 'a');
    } while (f && (f == l));
    
    return (f - l);
}

// 不区分大小写比较字符串前n个字符
int32 strnicmp(const char *dst, const char *src, int32 count)
{
    int ch1, ch2;

    do {
        if (((ch1 = (unsigned char)(*(dst++))) >= 'A') &&(ch1 <= 'Z'))
            ch1 += 0x20;

        if (((ch2 = (unsigned char)(*(src++))) >= 'A') &&(ch2 <= 'Z'))
            ch2 += 0x20;

    } while (--count && ch1 && (ch1 == ch2));

    return (ch1 - ch2);
}

//复制字符串
char *strdup(const char *pcStr)
{
    char *pcTmp = NULL;
    int slen = 0;

    if(NULL == pcStr)
        return NULL;

    slen = strlen(pcStr);
    pcTmp = (char *)mrc_malloc(slen + 1);
    if(NULL == pcTmp)
        return NULL;
    mrc_memcpy(pcTmp, pcStr, slen + 1);

    return pcTmp;
}

// 转换gb字符串为unicode，需free
char *strc2u(const char *gbstr)
{
    int32 slen;
    char *uniStr;

    if (NULL == gbstr)
        return NULL;

    slen = strlen(gbstr);
    slen = slen * 2 + 2;
    uniStr = (char *)malloc(slen);
    if(NULL == uniStr)
        return NULL;
    GBToUni(gbstr, uniStr, slen);

    return uniStr;
}

// 转换unicode字符串为gb，需free
char *stru2c(const char *unistr)
{
    int32 slen;
    char *gbStr;

    if (NULL == unistr)
        return NULL;

    slen = wstrlen(unistr);
    slen = slen + 1;
    gbStr = (char *)malloc(slen);
    if(NULL == gbStr)
        return NULL;
    UniToGB(unistr, gbStr, slen);

    return gbStr;
}

/*---------------------------------------------------------------------------*/

// unicode字符串长度
int32 wstrlen(const char *str)
{
    int i = 0;

    while (str[i] || str[i + 1]) 
        i += 2;

    return i;
}

// unicode字符串比较
int32 wstrcmp(const char *str1, const char *str2)
{
    while ((str1[0] || str1[1]) && str1[0] == str2[0] && str1[1] == str2[1])
    {
        str1 += 2; 
        str2 += 2;
    }

    return str1[0] == str2[0] ? str1[1] - str2[1] : str1[0] - str2[0];
}

// unicode字符串复制
char *wstrcpy(char *dst, const char *src)
{
    int i = 0;

    while (src[i] || src[i + 1]) 
    {
        dst[i] = src[i];
        dst[i+1] = src[i + 1];
        i += 2;
    }

    dst[i] = dst[i + 1] = '\0';

    return dst;
}

// unicode字符串复制指定长度
char *wstrncpy(char *dst, const char *src, int32 size)
{
    int i = 0;

    size -= 2;

    while (src[i] || src[i + 1]) 
    {
        if(i >= size) break;

        dst[i] = src[i];
        dst[i + 1] = src[i + 1];
        i += 2;
    }

    dst[i] = dst[i + 1] = '\0';

    return dst;
}

// unicode字符串附加
char *wstrcat(char *dst, const char *src)
{
    int len = wstrlen(dst);
    return wstrcpy(dst + len, src);
}

// unicode字符串复制
char *wstrdup(const char *str)
{
    int len = 0;
    char *pDest = NULL;

    if (NULL == str)
        return NULL;

    len = wstrlen(str) + 2;

    pDest = (char*)malloc(len);
    if(NULL == pDest)
        return NULL;

    memcpy(pDest, str, len);

    return pDest;
}

/*---------------------------------------------------------------------------*/

//UTF8转换为Unicode
//参数：UTF8字符串，Unicode缓冲区，缓冲区大小
//返回：缓冲区使用大小
int32 UTF8ToUni(const char *utf8str, char *unistr, int32 size)
{
	int32 i = 0, u = 0;
    uint8 *utf8, *unicode;

    utf8 = (uint8*)utf8str;
    unicode = (uint8*)unistr;

	while (utf8[i] && u < size - 2)
	{
		if((utf8[i] & 0x80) == 0)
		{
			// one byte 0...
            unicode[u] = 0;
			unicode[u + 1] = utf8[i++];
		}else if((utf8[i] & 0x20) == 0){
			// two bytes 110... 10...
			unicode[u] = (uint8)((utf8[i] & 0x1f) >> 2);
			unicode[u + 1] = (uint8)((utf8[i] << 6) & 0xff) | (utf8[i + 1] & 0x3f);
			i += 2;
		}else{
			// three bytes 1110... 10... 10...
			unicode[u] = (uint8)((utf8[i] << 4) & 0xff) | ((utf8[i + 1] & 0x3f) >> 2);
			unicode[u + 1] = (uint8)((utf8[i + 1] << 6) & 0xff) | (utf8[i + 2] & 0x3f);
			i += 3;
		}

		u += 2;
	}

    unicode[u] = '\0';
    unicode[u + 1] = '\0';

	return u;
}

//Unicode转换为UTF8
//参数：Unicode字符串，UTF8缓冲区，缓冲区大小
//返回：缓冲区使用大小
int32 UniToUTF8(const char *unistr, char *utf8str, int32 size)
{
	int32 u = 0, i = 0;
    uint8 *unicode, *utf8;

	unicode = (uint8 *)unistr;
    utf8 = (uint8 *)utf8str;

	while ((unicode[u] || unicode[u + 1]) && i < size - 1)
	{
		if (unicode[u] == 0 && unicode[u + 1] < 128) {
			// 0 - 7 bits
			utf8[i++] = unicode[u + 1];
		} else if((unicode[u] & 0xf8) == 0) {
			// 8 - 11 bits
			utf8[i++] = 0xc0 | ((unicode[u] << 2) & 0xff) | (unicode[u + 1] >> 6);
			utf8[i++] = 0x80 | (unicode[u + 1] & 0x3f);
		} else {
			// 12 - 16 bits
			utf8[i++] = 0xe0 | (unicode[u] >> 4);
			utf8[i++] = 0x80 | ((unicode[u] << 2) & 0x3f) | (unicode[u + 1] >> 6);
			utf8[i++] = 0x80 | (unicode[u + 1] & 0x3f);
		}

		u += 2;
	}

    utf8[i] = '\0';

	return i;
}

// 转换ascii字符串为unicode字符串
int32 Asc2Uni(const char *input, char *output, int32 outlen)
{
    int32 i;

    if (NULL == input || NULL == output)
        return -1;

    for (i = 0; input[i] != '\0' && outlen >= 4; i++)
    {
        output[i * 2] = '\0';
        output[i * 2 + 1] = input[i];
        outlen -= 2;
    }

    output[i * 2] = '\0';
    output[i * 2 + 1] = '\0';

    return (i * 2);
}

// 转换unicode字符串为ascii字符串
int32 Uni2Asc(const char *input, char *output,int32 outlen)
{
    int32 i;

    if (NULL == input || NULL == output)
        return -1;

    for (i = 0; input[i * 2 + 1] != '\0' && outlen >= 2; i++)
    {
        output[i] = input[i * 2 + 1];
        outlen--;
    }

    output[i] = '\0';

    return (i);
}

// 转换Unicode为GB2312
// 参数：Unicode字符串，GB2312缓冲区，缓冲区大小
// 返回：gb串大小
int32 UniToGB(const char *uniStr, char *gbBuf, int32 bufSize)
{
    char *gbStr;
    int32 ret;
    int32 gbSize;
    int32 uniSize;
    int32 outSize = 0;

	gbStr = (char*)malloc(bufSize);//输出缓冲
    if (NULL != gbStr)
    {
        gbSize = bufSize;
        uniSize = wstrlen(uniStr);

        ret = mrc_unicodeToGb2312((uint8*)uniStr, uniSize, (uint8**)&gbStr, &gbSize);
        if (MR_SUCCESS == ret)
        {
            gbSize = strlen(gbStr);
            outSize = (gbSize > bufSize - 1) ? (bufSize - 1) : gbSize;
            memcpy(gbBuf, gbStr, outSize);
        }
        free(gbStr);
    }

    gbBuf[outSize] = '\0';

    return outSize;
}

// 打开这个宏使用系统函数转换
// 貌似系统函数有bug，所以目前使用代码实现转换
//#define USE_SYS_GBCONV  

#ifdef USE_SYS_GBCONV

// 转换GB2312为Unicode
// 参数：GB2312字符串，Unicode缓冲区，缓冲区大小
// 返回：unicode串大小
int32 GBToUni(const char *gbStr, char *uniBuf, int32 bufSize)
{
    char *uniStr = NULL;
    int32 uniSize = 0;
    int32 outSize = 0;

    uniStr = (char*)mrc_c2uVM((char*)gbStr, NULL, &uniSize);
    if (NULL != uniStr)
    {
        outSize = (uniSize > bufSize - 2) ? (bufSize - 2) : uniSize;
        memcpy(uniBuf, uniStr, outSize);
        mrc_c2uVMfree(uniStr, uniSize);
    }
    uniBuf[outSize] = '\0';
    uniBuf[outSize + 1] = '\0';

    return outSize - 2;
}

#else

static uint8 *gb2uTable;  // 编码转换表
static int32 gb2uCount;   // 编码表长度

// 初始化gb转unicode码表
static int32 GBCodeTableInit(void)
{
    char *fbuf;
    int32 fsize;
    int32 ret = -1;

    fbuf = mrc_readFileFromMrp("gb.bin", &fsize, 0);
    if (NULL != fbuf)
    {
        gb2uTable = (uint8*)malloc(fsize);
        if (NULL != gb2uTable)
        {
            memcpy(gb2uTable, fbuf, fsize);
            gb2uCount = fsize;
            ret = 0;
        }
        mrc_freeFileData(fbuf, fsize);
    }

    return ret;
}

// 编码转换，从GB2312转成unicode
static uint8 *GBCodeToUnicode(uint8 *gbCode)
{
	uint8 *p_map = NULL;
	uint32 i;

	if ((*(gbCode + 1) >= 0xa1) && (*(gbCode + 1) <= 0xfe))
	{
		if ((*gbCode >= 0xa1) && (*gbCode <= 0xa9))
		{
			i= ((*gbCode - 0xa1) * 94 + (*(gbCode + 1) - 0xa1)) * 2;
			p_map = &gb2uTable[i];
		}
        else if ((*gbCode >= 0xb0) && (*gbCode <= 0xf7))
		{
			i = ((*gbCode - 0xb0 + 9) * 94 + (*(gbCode + 1) - 0xa1)) * 2;
			p_map = &gb2uTable[i];
		} 
	} 

	return p_map;
}

/*
 * 说明
 * 参数： gbCode：待转换的gb字符串
 *		   gbLen：gb字符串字节长度
 *		   unicode：保存转换后的unicode缓冲区（该内存自己申请）
 * 返回： 成功：转换后unicode长度
 *		  失败：-1
 */
int32 GBToUni(const char *gbStr, char *uniBuf, int32 bufSize)
{
    uint8 *gbCode = (uint8 *)gbStr;
    uint8 *unicode = (uint8 *)uniBuf;
    uint8 *pReturn = NULL;
    int32 i = 0, j = 0;

	if(!gbCode || !unicode || bufSize <= 0) 
		return -1;

    // 初始化gb转unicode码表
    if (NULL == gb2uTable)
    {
        int32 ret;

        ret = GBCodeTableInit();
        if (0 != ret || NULL == gb2uTable)
        {
            return -1;
        }
    }

    // 循环转换各个字符
	while ('\0' != gbCode[i] && j < bufSize - 2)
	{
		if (gbCode[i] < 0x80)
		{
			unicode[j] = 0;
			unicode[j + 1] = gbCode[i];
			i++;
		}
        else
		{
			pReturn = GBCodeToUnicode(&gbCode[i]);

			if (NULL != pReturn)
			{
				unicode[j] = *(pReturn + 1);
				unicode[j + 1] = *pReturn;
                i++;

                // 进行判断防止越过'\0'而出错
                if ('\0' != gbCode[i])
			        i++;
			}
            else
			{
                // 全角空格
				/*unicode[j] = 0x30;
				unicode[j + 1] = 0x00;*/
                // □
                unicode[j] = 0x25;
                unicode[j + 1] = 0xa1;
                i++;
			}
		}

		j += 2;
	}

    unicode[j] = '\0';
    unicode[j + 1] = '\0';

	return j;
}

#endif

/*---------------------------------------------------------------------------*/

// 建立BMH坏字符表
int32 BMH_BuildTable(uint32 *shift, uint32 shift_size, void *in_pattern, uint32 pattern_size)
{
    uint32 byte_nbr;
    uint8 *pattern = (uint8*)in_pattern;

    if (pattern == NULL || shift == NULL)
        return (-1);

    for (byte_nbr = 0; byte_nbr < shift_size; byte_nbr++)
        shift[byte_nbr] = pattern_size + 1;

    for (byte_nbr = 0; byte_nbr < pattern_size; byte_nbr++)
        shift[pattern[byte_nbr]] = pattern_size - byte_nbr;

    return 0;
}

// 进行BMH匹配
void *BMH_Ex(uint32 *shift, void *in_block, uint32 block_size, void *in_pattern, uint32 pattern_size)
{
    uint32 byte_nbr, match_size, limit;
    uint8 *match_ptr = NULL;
    uint8 *block = (uint8 *)in_block;
    uint8 *pattern = (uint8 *)in_pattern;

    if (block == NULL || pattern == NULL || shift == NULL)
        return NULL;

    // 查找的串长应该小于 数据长度
    if (block_size < pattern_size)
        return NULL;

    if (pattern_size == 0)  // 空串匹配第一个
        return (void *)block;

    // 开始搜索数据块，每次前进移位表中的数量
    limit = block_size - pattern_size + 1;
    for (byte_nbr = 0; byte_nbr < limit; byte_nbr += shift[block[byte_nbr + pattern_size]]) 
    {
        if (block[byte_nbr] == *pattern)
        {
            // 如果第一个字节匹配，那么继续匹配剩下的
            match_ptr = block + byte_nbr + 1;
            match_size = 1;

            do {
                if (match_size == pattern_size)
                    return (void *)(block + byte_nbr);
            } while (*match_ptr++ == pattern[match_size++]);
        }
    }

    return NULL;
}

// BMH字符串匹配算法
// 参数：内容，内容大小，匹配串，匹配串大小
// 返回：匹配位置，失败返回-1
void *BMH(void *in_block, uint32 block_size, void *in_pattern, uint32 pattern_size)
{
    int32 ret;
    uint32 *bad_char_skip;
    void *match;

    if (NULL == in_block || NULL == in_pattern)
        return NULL;

    bad_char_skip = (uint32*)malloc(256 * sizeof(uint32));
    if (NULL == bad_char_skip)
        return NULL;

    ret = BMH_BuildTable(bad_char_skip, 256, in_pattern, pattern_size);
    match = BMH_Ex(bad_char_skip, in_block, block_size, in_pattern, pattern_size);
    free(bad_char_skip);

    return match;
}

/*---------------------------------------------------------------------------*/

// 初始化
int32 String_Init(void)
{
#ifndef USE_SYS_GBCONV
    if (NULL == gb2uTable)
    {
       return GBCodeTableInit();
    }
#endif

    return 0;
}

// 释放资源
int32 String_End(void)
{
#ifndef USE_SYS_GBCONV
    if (NULL != gb2uTable)
    {
        free(gb2uTable);
        gb2uTable = NULL;
    }
#endif

    return 0;
}

//struct CHARBUF{
//    void *orginBuf;
//    int32 orginLen;
//    void *freeBuf;
//    int8 isOrgin;
//    int8 isFree;    
//}charbuf;
//
//int All_init(void)
//{
//    mrc_memset(&charbuf,0,sizeof(struct CHARBUF));
//    return 0;
//}
//
//int All_free(void)
//{
//    if (charbuf.isOrgin==TRUE)
//        mrc_freeOrigin(charbuf.orginBuf,charbuf.orginLen);
//    if (charbuf.isFree==TRUE)
//        mrc_free(charbuf.freeBuf);
//    mrc_memset(&charbuf,0,sizeof(struct CHARBUF));
//    return 0;
//}
//
//PSTR UniToGB(PCWSTR unicode)
//{
//    int32 inLen,outLen;
//
//    outLen=514;
//    if (charbuf.isFree==FALSE)
//    {
//        charbuf.freeBuf = mrc_malloc(outLen);
//        charbuf.isFree=TRUE;
//    }
//
//    inLen = mrc_wstrlen((char*)unicode);
//    mrc_unicodeToGb2312((uint8 *)unicode, inLen, (uint8**)&charbuf.freeBuf, &outLen);
//    return charbuf.freeBuf;
//}
//
//PWSTR GBToUni(char* str)
//{
//    if(charbuf.isOrgin==TRUE)
//    {
//        mrc_freeOrigin(charbuf.orginBuf,charbuf.orginLen);
//    }
//    else
//    {
//        charbuf.isOrgin=TRUE;
//    }
//    charbuf.orginBuf = mrc_c2uVM(str, NULL, &charbuf.orginLen); 
//    return (PWSTR)charbuf.orginBuf;
//}

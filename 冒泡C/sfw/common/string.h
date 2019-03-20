#ifndef _STRING_H
#define _STRING_H

#include "types.h"


/* 转换unicode的结构
   使用后用 mrc_freeOrigin 释放内存
 */
typedef struct {
	PWSTR pText;    //unicode字符串指针
	int32 len;      //字符串缓冲长度
}UnicodeSt;


VOID CharToUnicode(char* in, PWSTR* out, int32* outlen);

PSTR UnicodeToChar(PCWSTR unicode);

#if 0
/**
 * \转换为小写的字符串。
 *
 * \参数str    转换的字符串
 * \返回小写字符串
 */
PSTR tolower(PSTR str);

/**
 * \转换为大写的字符串。
 *
 * \参数str    转换的字符串
 * \返回大写字符串
 */
PSTR toupper(PSTR str);
#endif

/**
 * \brief Compare two string with case ignore.
 *
 * \param dst 第一个字符串参数
 * \param src 第二个字符串参数
 * \返回
 *	<0 当 dst < src
 *	=0 当 dst = src
 *	>0 当 dst > src
 *
 * \比较两个case忽略字符串。
 */
Sint32 stricmp(PCSTR dst, PCSTR src);

/**
 * \brief Trim left space chars here just ' '.
 *
 * \param str the string to be trimed
 * \return the trimed string
 *
 * \修整字符只是在这里留下空间'。
 *
 * \参数str的字符串被trimed
 * \返回trimed字符串
 */
PSTR trimleft(PSTR str);

/**
 * \brief Trim right space chars here just .
 *
 * \param str the string to be trimed
 * \return the trimed string
 *
 * \修整正确的空间字符这里只是' '。
 *
 * \参数str的字符串被trimed
 * \返回trimed字符串
 */
PSTR trimright(PSTR str);


/**
 * \brief Trim left&right space chars.
 *
 * \param str the string to be trimed
 * \return the trimed string
 *
 * \修整左，右空间字符。
 *
 * \参数str的字符串被trimed
 * \返回trimed字符串
 */
PSTR trim(PSTR str);

/**
 * \brief Split the string with specific char.
 * 
 * \param str the string to be splited
 * \param sep the seperator char
 * \param[out] values the splited string array
 * \param size the max size of values
 * \return real size of values array returned
 *
 * \的拆分与特定字符的字符串。
 *
 * \参数str的字符串被splited
 * \参数的分隔符字符九月
 * \参数[出]字符串数组值的splited
 * \参数值的大小，最大尺寸
 * \返回返回值数组的实际大小
 */
Sint32 split(PSTR str, char sep, PSTR values[], Sint32 size);

/**
 * \得到wstring的长度。
 *
 * \返回字节的wstring的长度(字节)

Sint32 wstrlen(PCWSTR str);
 */
/**
 * \brief Copy characters of one wstring to another.
 *
 * \param dst Destination string
 * \param src Source string
 * \return the destination string
 *
 * \一wstring的字符复制到另一个。
 *
 * \参数dst的目标字符串
 * \ src输入字符串参数
 * \返回目标字符串
 */

PWSTR wstrcpy(PWSTR dst, PCWSTR src);

PWSTR wstrncat(PWSTR dst, PCWSTR src, Sint32 size);

/**
 * \一wstring的字符复制到另一个。
 *
 * \参数dst     目标字符串
 * \参数src     输入字符串
 * \参数        要复制的字节大小
 * \返回目标字符串
 */
PWSTR wstrncpy(PWSTR dst, PCWSTR src, Sint32 size);

/**
 * \比较字符串。
 *
 * \参数 str1 the 第一个字符串
 * \参数 str2 the 第二个字符串
 * \返回
 *	 0 当 str1 == str2
 *	<0 当 str1 < str2
 *	>0 当 str1 > str2
 */
Sint32 wstrcmp(PCWSTR str1, PCWSTR str2);

/**
 * \brief Find a wchar in wstring.
 * 
 * \param str the wstring
 * \param chr1 the first byte of the wchar
 * \param chr2 the second byte of the wchar
 * \return the first occurrence of c in string, NULL when not found
 *
 * \查找wstring中的一个wchar。
 *
 * \参数str     wstring
 * \参数chr1    wchar的第一字节
 * \参数chr2    wchar的第二字节
 * \返回c第一次出现在字符串，NULL时未找到
 */
PCWSTR wstrchr(PCWSTR str, UCHAR chr1, UCHAR chr2);

/**
 * \brief Trim left a wstring.
 *
 * \param str the wstring to be trimed
 * \return the trimed string
 *
 * \修整留下了wstring的。
 *
 * \参数str中的wstring的是trimed
 * \返回trimed字符串
 */
PWSTR wtrimleft(PWSTR str);

/**
 * \brief Trim right a wstring.
 * 
 * \param str the wstring to be trimed
 * \return the trimed string
 *
 * \修整一个wstring的权利。
 *
 * \参数str中的wstring的是trimed
 * \返回trimed字符串
 */
PWSTR wtrimright(PWSTR str);

/**
 * \brief Trim a wstring.
 * 
 * \param str the wstring to be trimed
 * \return the trimed string
 *
 * \修剪wstring的。
 *
 * \参数str中的wstring的是trimed
 * \返回trimed字符串
 */
PWSTR wtrim(PWSTR str);


/**
 * \utf8字符串转换为Unicode字符串。
 *
 * \参数UTF8               要转换的UTF8字符串
 * \参数[入,出]Unicode     转换Unicode的缓冲区
 * \参数[入,出]size        Unicode缓冲区的大小
 * \返回转换后的长度(字节)
 */
Sint32 str2wstr(PCSTR utf8, PWSTR unicode, Sint32 size);


/*
 * \Unicode字符串转换成utf8的字符串。
 *
 * \参数Unicode 要转换的Unicode字符串
 * \参数UTF8    转换utf8的缓冲区
 * \参数size    utf8缓冲区的最大大小
 * \返回转换后的长度(字节)
 */
Sint32 wstr2str(PCWSTR unicode, PSTR utf8, Sint32 size);

char *strDup(char *pcStr);
PWSTR wstrcat(PWSTR dst, PCWSTR src);
char *strChrEx(const char *src,int c);
PWSTR trimCRLF(PWSTR str);
char *itoa(int num,char *str,int radix);
PWSTR wstrdup(PCWSTR str);

#endif

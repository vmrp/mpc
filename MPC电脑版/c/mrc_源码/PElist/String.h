/*
 * 字符串函数头文件
 * 版权所有 (C) 2011-2012 eleqian
 * [6/4/2012]
 */

#ifndef __STRING_H__
#define __STRING_H__

// 函数替换，测试用
#if 1

#define mrc_c2uVMfree(p,l)    mrc_freeFileData(p,l)

#else

#define mrc_c2uVM(s,e,l)      mrc_c2u(s,e,l)
#define mrc_c2uVMfree(p,l)    free(p,l)

#endif

int32 watoi(const char *uniStr);
char *witoa(int32 value);

char *strupr(char *str);
char *strlwr(char *str);
char *strdup(const char *str);
char *strrchr(const char * str, int ch);
int32 stricmp(const char *str1, const char *str2);
int32 strnicmp(const char *dst, const char *src, int32 count);

int32 wstrlen(const char *str);
char *wstrdup(const char *str);
char *wstrcpy(char *dst, const char *src);
char *wstrncpy(char *dst, const char *src, int32 size);
char *wstrcat(char *dst, const char *src);
int32 wstrcmp(const char *str1, const char *str2);

char *strc2u(const char *gbstr);
char *stru2c(const char *unistr);

int32 UTF8ToUni(const char* utf8, char* unicode, int32 size);
int32 UniToUTF8(const char* unicode, char* utf8, int32 size);

int32 GBToUni(const char *gbStr, char *uniBuf, int32 bufSize);
int32 UniToGB(const char *uniStr, char *gbBuf, int32 bufSize);

int32 Uni2Asc(const char *input, char *output,int32 outlen);
int32 Asc2Uni(const char *input, char *output, int32 outlen);

int32 BMH_BuildTable(uint32 *shift, uint32 shift_size, void *in_pattern, uint32 pattern_size);
void *BMH_Ex(uint32 *shift, void *in_block, uint32 block_size, void *in_pattern, uint32 pattern_size);
void *BMH(void *in_block, uint32 block_size, void *in_pattern, uint32 pattern_size);

int32 String_Init(void);
int32 String_End(void);

#endif // __STRING_H__

/*
 * 通用函数头文件
 * 版权所有 (C) 2011-2012 eleqian
 * [6/4/2012]
 */

#ifndef __FUNCTIONS_H__
#define __FUNCTIONS_H__

#include "FileIO.h"
#include "String.h"

// 数据缓冲区大小-普通
#define DATA_BUFFER_SIZE_NORMAL     (16 * 1024) 

// 数据缓冲区大小-小型
#define DATA_BUFFER_SIZE_SMALL      (4 * 1024) 

//最小值
#ifndef MIN
# define MIN(a,b) (((a)<(b))?(a):(b))
#endif

//最大值
#ifndef MAX
# define MAX(a,b) (((a)>(b))?(a):(b))
#endif

//释放动态申请内存
#ifndef TRYFREE
# define TRYFREE(p) \
    if (NULL != p) \
    { \
        free(p); \
        p = NULL; \
    }
#endif

//关闭文件
#ifndef TRYCLOSE
# define TRYCLOSE(fd) \
    if (0 != fd) \
    { \
        mrc_close(fd); \
        fd = 0; \
    }
#endif

int32 CopyFile(const char *srcFile, const char *destFile);
int32 CopyFileEx(const char *srcFile, const char *destFile, uint32 srcPos, uint32 destPos, uint32 copySize);
int32 CopyFileFullPath(const char *srcPath, const char *destPath, const char *srcFile, const char *destFile);
int32 CopyFileByHandle(int32 ifd, int32 ofd, uint32 copySize);

int32 ReadFile(const char *dir, const char *file, int32 pos, uint8 *buf, int32 bufSize);
int32 SaveFile(const char *dir, const char *file, int32 pos, uint8 *buf, int32 bufSize);

int32 FindFromFile(const char *dir, const char *file, int32 offset, uint8 *data, int32 size);
int32 FindFromFileByHandle(int32 fd, int32 offset, uint8 *data, int32 size);

char *ReadStringFromMrp(const char *name);

int32 MakeDir(const char *newdir);
int32 MakeDirEx(const char *newdir);

int32 GetCurDir(char *strDir);
int32 Switch2Dir(const char *strDisk);
int64 GetDiskSize(const char *strDisk, uint8 Type);

int32 OpenInPath(const char *path, const char *name, uint32 mode);
int32 FileLenInPath(const char *path, const char *name);

int32 GetAppId(void);
int32 GetAppVer(void);

void SwapBytes(uint8 *Buffer, int32 bufSize);
void DecodeStr(uint8 *strByte, uint8 XorByte);

void Screen2File(const char *fName);

void Color2ST(uint32 uColor, mr_colourSt *tColor);
void WLog(const char *Text);
int32 removeDir(const char* dirName);

#endif  // __FUNCTIONS_H__

/*
 * 安全文件接口头文件
 * 版权所有 (C) 2011-2012 eleqian
 * [6/4/2012]
 */

#ifndef __FILEIO_H__
#define __FILEIO_H__

#include <mrc_base.h>

// 最大文件名长度
#define MAX_FILENAME_LEN_A  128

#define MAX_FILENAME_LEN_W  (MAX_FILENAME_LEN_A * 2)

// 最大路径长度
#define MAX_PATH_LEN_A      256

#define MAX_PATH_LEN_W      (MAX_PATH_LEN_A * 2)

// 使用替换路径分隔符后的文件操作函数
#define USE_SAFE_PATH_FUNC 

#ifdef USE_SAFE_PATH_FUNC

#define mrc_open mrc_open_s
#define mrc_mkDir mrc_mkDir_s
#define mrc_rmDir mrc_rmDir_s
#define mrc_rename mrc_rename_s
#define mrc_remove mrc_remove_s
#define mrc_fileState mrc_fileState_s
#define mrc_getLen mrc_getLen_s
#define mrc_readAll mrc_readAll_s

int32 mrc_open_s(const char *filename, uint32 mode);
int32 mrc_mkDir_s(const char *name);
int32 mrc_rmDir_s(const char *name);
int32 mrc_rename_s(const char* oldname, const char* newname);
int32 mrc_remove_s(const char* filename);
int32 mrc_fileState_s(const char* filename);
int32 mrc_getLen_s(const char* filename);
void* mrc_readAll_s(const char* filename, uint32 *len);

#endif

char* ConvertPathSeparator(const char *path);
char* ConvertPathSeparator1(const char *path);

#endif // __FILEIO_H__

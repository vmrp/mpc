/*
 * 转换了路径分隔符的文件操作函数
 * 版权所有 (C) 2011-2012 eleqian
 * [5/18/2012]
 */

#include <mrc_base.h>

#include "FileIO.h"

// 转换路径分隔符
char* ConvertPathSeparator(const char *path)
{
    static char dstPath[MAX_PATH_LEN_A]; //转换分隔符后的路径
    char *psrc, *pdst;

    for (psrc = (char*)path, pdst = dstPath; '\0' != *psrc; psrc++, pdst++)
    {
        *pdst = ('\\' == *psrc) ? '/' : *psrc;
    }

    *pdst = '\0';

    return dstPath;
}

// 同上，为了实现同时转换两个路径
char* ConvertPathSeparator1(const char *path)
{
    static char dstPath1[MAX_PATH_LEN_A]; //转换分隔符后的路径
    char *psrc, *pdst;

    for (psrc = (char*)path, pdst = dstPath1; '\0' != *psrc; psrc++, pdst++)
    {
        *pdst = ('\\' == *psrc) ? '/' : *psrc;
    }

    *pdst = '\0';

    return dstPath1;
}

#ifdef USE_SAFE_PATH_FUNC

// 防止在以下函数中进行替换而出错
# undef mrc_open
# undef mrc_mkDir
# undef mrc_rmDir
# undef mrc_rename
# undef mrc_remove
# undef mrc_fileState
# undef mrc_getLen
# undef mrc_readAll

//打开文件
int32 mrc_open_s(const char *filename, uint32 mode)
{
    return mrc_open(ConvertPathSeparator(filename), mode);
}

//创建文件夹
int32 mrc_mkDir_s(const char *name)
{
    return mrc_mkDir(ConvertPathSeparator(name));
}

//删除文件夹
int32 mrc_rmDir_s(const char *name)
{
    return mrc_rmDir(ConvertPathSeparator(name));
}

//重命名文件(夹)
int32 mrc_rename_s(const char* oldname, const char* newname)
{
    return mrc_rename(ConvertPathSeparator(oldname), ConvertPathSeparator1(newname));
}

//删除文件
int32 mrc_remove_s(const char* filename)
{
    return mrc_remove(ConvertPathSeparator(filename));
}

//取得文件类型信息
int32 mrc_fileState_s(const char* filename)
{
    return mrc_fileState(ConvertPathSeparator(filename));
}

//取得文件长度
int32 mrc_getLen_s(const char* filename)
{
    return mrc_getLen(ConvertPathSeparator(filename));
}

//读取文件的所有内容
void* mrc_readAll_s(const char* filename, uint32 *len)
{
    return mrc_readAll(ConvertPathSeparator(filename), len);
}

#endif

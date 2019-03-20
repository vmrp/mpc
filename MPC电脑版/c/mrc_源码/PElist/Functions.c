/*
 * 程序通用函数代码
 * 版权所有 (C) 2011-2012 eleqian
 * [6/5/2012]
 */

#include <mrc_base.h>
#include <mrc_base_i.h>
#include <mrc_exb.h>
#include <mrc_bmp.h>

#include "Functions.h"
#include "String.h"

// 磁盘空间结构体
typedef struct
{
    uint32 total;    // 总的大小
    uint32 tunit;    // 总大小的单位
    uint32 account;  // 剩余空间的大小
    uint32 unit;     // 剩余大小的单位
} T_DSM_FREE_SAPCE;

/*-------------------------------------------------------------------------------------------------*/

//取得指定绝对目录文件大小
int32 FileLenInPath(const char *path, const char *name)
{
    int32 ret = 0;

    if (NULL != path)
        Switch2Dir(path);

    ret = mrc_getLen(name);

    if (NULL != path)
        Switch2Dir(NULL);

    return ret;
}

//在指定绝对目录打开文件
int32 OpenInPath(const char *path, const char *name, uint32 mode)
{
    int32 fd = 0;
    int32 ret = 0;

#ifndef SDK_MOD  // 模拟器环境必然失败
    if (NULL != path)
        ret = Switch2Dir(path);
#endif

    if (0 == ret)
        fd = mrc_open(name, mode);

#ifndef SDK_MOD
    if (NULL != path)
        Switch2Dir(NULL);
#endif

    return fd;
}

//读取文件
//参数：路径，文件名，读取位置，缓冲区，缓冲区大小
//返回：实际读取大小
int32 ReadFile(const char *dir, const char *file, int32 pos, uint8 *buf, int32 bufSize)
{
    int32 fd;
    int32 rSize = -1;

    memset(buf, 0, bufSize);
    fd = OpenInPath(dir, file, MR_FILE_RDONLY);
    if (fd > 0)
    {
        if (MR_SUCCESS == mrc_seek(fd, pos, MR_SEEK_SET))
        {
            rSize = mrc_read(fd, buf, bufSize);
        }
        mrc_close(fd);
    }

    return rSize;
}

//保存文件
//参数：路径，文件名，读取位置，缓冲区，缓冲区大小
//返回：实际保存大小
int32 SaveFile(const char *dir, const char *file, int32 pos, uint8 *buf, int32 bufSize)
{
    int32 fd;
    int32 rSize = -1;

    fd = OpenInPath(dir, file, MR_FILE_RDWR | MR_FILE_CREATE);
    if (fd > 0)
    {
        mrc_seek(fd, pos, MR_SEEK_SET);
        rSize = mrc_write(fd, buf, bufSize);
        mrc_close(fd);
    }

    return rSize;
}

//从文件查找数据位置
//参数：文件名，开始查找偏移，查找数据，数据大小
//返回：找到的位置
int32 FindFromFile(const char *dir, const char *file, int32 offset, uint8 *data, int32 size)
{
    int32 ret = -1;
    int32 fd;

    fd = OpenInPath(dir, file, MR_FILE_RDONLY);
    if (0 == fd)
        return ret;

    ret = FindFromFileByHandle(fd, offset, data, size);

    mrc_close(fd);

    return ret;
}

//从文件查找数据位置-句柄版
//参数：文件句柄，文件偏移，查找数据，数据大小
//返回：找到的位置
int32 FindFromFileByHandle(int32 fd, int32 offset, uint8 *data, int32 size)
{
    uint32 *bad_char_skip;
    uint8 *databuf = NULL, *pfind;
    int32 bufUsed, readPos;
    int32 ret = -1;

    if (0 == fd)
        return ret;

    readPos = offset;
    databuf = (uint8 *)malloc(DATA_BUFFER_SIZE_NORMAL);
    memset(databuf, 0, DATA_BUFFER_SIZE_NORMAL);

    bad_char_skip = (uint32 *)malloc(256 * sizeof(uint32));
    ret = BMH_BuildTable(bad_char_skip, 256, data, size);

    while (1)
    {
        mrc_seek(fd, readPos, MR_SEEK_SET);
        bufUsed = mrc_read(fd, databuf, DATA_BUFFER_SIZE_NORMAL);
        if (bufUsed <= 0)
            break;

        pfind = BMH_Ex(bad_char_skip, databuf, bufUsed, data, size);
        if (NULL != pfind)
        {
            ret = readPos + (pfind - databuf);
            break;
        }

        readPos += (DATA_BUFFER_SIZE_NORMAL - size + 1);
    }

    TRYFREE(bad_char_skip);
    TRYFREE(databuf);

    return ret;
}

/*---------------------------------------------------------------------------*/

//复制文件
//参数：源文件路径，目标文件路径
int32 CopyFile(const char *srcFile, const char *destFile)
{
    int32 ifd, ofd; //源文件和目标文件句柄
    int32 ret = MR_FAILED, copySize;

    copySize = mrc_getLen(srcFile);
    if (copySize < 0)
        return MR_FAILED;

    ifd = mrc_open(srcFile, MR_FILE_RDONLY); //打开源文件(只读)
    ofd = mrc_open(destFile, MR_FILE_WRONLY | MR_FILE_CREATE); //打开目标文件(不存在创建)

    if(ifd != 0 && ofd != 0) //打开文件都成功
    {
        mrc_seek(ifd, 0, MR_SEEK_SET);
        mrc_seek(ofd, 0, MR_SEEK_SET);

        ret = CopyFileByHandle(ifd, ofd, copySize);
    }

    TRYCLOSE(ifd); //关闭源文件
    TRYCLOSE(ofd); //关闭目标文件

    return ret; 
}

//复制文件-绝对路径版
//参数：源文件路径，目标文件路径
int32 CopyFileFullPath(const char *srcPath, const char *destPath, const char *srcFile, const char *destFile)
{
    int32 ofd = 0, ifd = 0; //源文件和目标文件句柄
    int32 ret = MR_FAILED, copySize;

    copySize = FileLenInPath(srcPath, srcFile);
    if (copySize < 0)
        return MR_FAILED;

    ifd = OpenInPath(srcPath, srcFile, MR_FILE_RDONLY); //打开源文件(只读)
    ofd = OpenInPath(destPath, destFile, MR_FILE_WRONLY | MR_FILE_CREATE); //打开目标文件(不存在创建)

    if(ifd != 0 && ofd != 0) //打开文件都成功
    {
        mrc_seek(ifd, 0, MR_SEEK_SET);
        mrc_seek(ofd, 0, MR_SEEK_SET);

        ret = CopyFileByHandle(ifd, ofd, copySize);
    }

    TRYCLOSE(ofd); //关闭目标文件
    TRYCLOSE(ifd); //关闭源文件

    return ret;     
}

//复制文件-扩展版
//参数：源文件路径，目标文件路径，源文件开始位置，目标文件开始位置，复制大小
int32 CopyFileEx(const char *srcFile, const char *destFile, uint32 srcPos, uint32 destPos, uint32 copySize)
{
    int32 ifd, ofd; //源文件和目标文件句柄
    int32 ret = MR_FAILED;

    ifd = mrc_open(srcFile, MR_FILE_RDONLY); //打开源文件(只读)
    ofd = mrc_open(destFile, MR_FILE_WRONLY | MR_FILE_CREATE); //打开目标文件(不存在创建)

    if(ifd != 0 && ofd != 0) //打开文件都成功
    {
        mrc_seek(ifd, srcPos, MR_SEEK_SET);
        mrc_seek(ofd, destPos, MR_SEEK_SET);

        ret = CopyFileByHandle(ifd, ofd, copySize);
    }

    TRYCLOSE(ifd); //关闭源文件
    TRYCLOSE(ofd); //关闭目标文件

    return ret;
}

//复制文件-句柄版
//参数：源文件句柄，目标文件句柄，复制大小
int32 CopyFileByHandle(int32 ifd, int32 ofd, uint32 copySize)
{
    uint8 *dataBuf = NULL; //文件读写缓冲
    int32 size_read, size_write; //本次读写大小
    uint32 size_to_read, size_copied; //需要读取大小，总复制大小
    int32 ret;

    if(0 == ifd || 0 == ofd) //句柄无效
        return MR_FAILED;

    dataBuf = (uint8*)malloc(DATA_BUFFER_SIZE_NORMAL);
    if (NULL == dataBuf) //内存不足
        return MR_FAILED;

    memset(dataBuf, 0, DATA_BUFFER_SIZE_NORMAL);
    size_copied = 0;
    ret = MR_SUCCESS;

    while (copySize > size_copied) //直到复制完成
    {
        size_to_read = MIN(DATA_BUFFER_SIZE_NORMAL, copySize - size_copied);

        size_read = mrc_read(ifd, dataBuf, size_to_read); //读取文件
        if (size_read <= 0)
        {
            ret = MR_FAILED;
            break;
        }

        size_write = mrc_write(ofd, dataBuf, size_read); //写文件
        if (size_write <= 0 || size_write != size_read)
        {
            ret = MR_FAILED;
            break;
        }

        size_copied += size_read;
    }

    TRYFREE(dataBuf); //释放内存

    return ret;
}

/*---------------------------------------------------------------------------*/

// 从mrp读取文本文件全部内容
char *ReadStringFromMrp(const char *name)
{
    char *gbstr;
    char *fbuf;
    int32 fsize;

    fbuf = mrc_readFileFromMrp(name, &fsize, 0);
    if (NULL != fbuf)
    {
        gbstr = (char*)malloc(fsize + 1);
        memcpy(gbstr, fbuf, fsize);
        gbstr[fsize] = '\0';
        mrc_freeFileData(fbuf, fsize);
    }
    else
    {
        return NULL;
    }

    return gbstr;
}

//交换前后字节顺序
void SwapBytes(uint8 *data, int32 size)
{
    int32 i;
    uint8 tByte;

    for (i = 0; i < size; i += 2)
    {
        tByte = data[i];
        data[i] = data[i + 1];
        data[i + 1] = tByte;
    }
}

//进行简单加密字符的解密
//参数：字符串，异或字节
void DecodeStr(uint8 *strBytes, uint8 XorByte)
{
    uint8 *p = strBytes;

    while (0 != *p)
        *p++ ^= XorByte;
}

/*-------------------------------------------------------------------------------------------------*/

//保存当前屏幕为图片
void Screen2File(const char *name_w)
{
    char *picPath = NULL;
    char *name_a = NULL;


    picPath = (char*)malloc(MAX_PATH_LEN_A);
    memset(picPath, 0, MAX_PATH_LEN_A);
    strcat(picPath, "eles/SnapPic");
    MakeDir(picPath);
    strcat(picPath, "/");

    name_a = stru2c(name_w);
    strcat(picPath, name_a);
    TRYFREE(name_a);

    mrc_PrintScreen(picPath);

    TRYFREE(picPath);
}

//颜色值转换为结构
void Color2ST(uint32 uColor, mr_colourSt *tColor)
{
    tColor->r = (uint8)(uColor >> 16) & 0xff;
    tColor->g = (uint8)(uColor >> 8) & 0xff;
    tColor->b = (uint8)(uColor & 0xff);
}

//记录日志
void WLog(const char* text)
{
	int32 fd=0;

	if(mrc_fileState("MDs/log.ini") != MR_IS_INVALID || mrc_fileState("MDs/log.ini") == MR_IS_DIR)
		mrc_remove("MDs/log.ini");

	fd = mrc_open("MDs/log.ini", MR_FILE_RDWR | MR_FILE_CREATE);
	if(fd != 0)
	{
		mrc_seek(fd, 0, MR_SEEK_SET);
		mrc_write(fd, (void*)text, mrc_strlen(text));
	}
	mrc_close(fd);
}

/*-------------------------------------------------------------------------------------------------*/

//获取当前路径名
//参数：结果缓冲区
int32 GetCurDir(char *strDir)
{
    int32 ret = MR_FAILED, len = 0;
    uint8 *pDir = NULL;

    if (NULL == strDir)
        return MR_FAILED;

#ifdef WIN32
    return MR_FAILED;
#endif

    ret = mrc_platEx(1204, (uint8*)"Y", 1, &pDir, &len, NULL);
    if (MR_SUCCESS == ret)
    {
        memcpy(strDir, pDir, len + 1);
        TRYFREE(pDir);
        ret = len;
    }

    return ret;
}

//切换目录
//参数：路径，NULL切换到上次目录
int32 Switch2Dir(const char* strDir)
{
    static char lastDir[MAX_PATH_LEN_A];
    int32 slen, ret;

#ifdef WIN32
    return MR_FAILED;
#endif

    if (NULL != strDir)
    {
        slen = strlen(strDir);
        GetCurDir(lastDir);
        ret = mrc_platEx(1204, (uint8*)ConvertPathSeparator(strDir), slen, NULL, NULL, NULL);
    }
    else
    {
        if ('\0' == lastDir[0])
            lastDir[0] = 'Z';
        slen = strlen(lastDir);
        ret = mrc_platEx(1204, (uint8*)lastDir, slen, NULL, NULL, NULL);
    }

    return ret;
}

// 创建多层目录
// 成功返回0，失败返回-1
int32 MakeDir(const char *newdir)
{
    char *strBuf = NULL;
    char *p = NULL;
    int32 len = 0;

    if (NULL == newdir || '\0' == *newdir) 
        return -1;

    len = strlen(newdir);
    strBuf = strdup(newdir);

    // 替换反斜杠为斜杠
    for (p = strBuf; '\0' != *p; p++)
    {
        if ('\\' == *p)
            *p = '/';
    }

    // 消除最后的分隔符
    if ('/' == strBuf[len - 1])
        strBuf[len - 1] = '\0';

    // 循环创建
    for (p = strBuf + 1; p < strBuf + len; p++)
    {
        // 查找下一层目录
        while(('\0' != *p) && ('/' != *p)) 
            p++;
        *p = '\0';

        if (MR_IS_FILE == mrc_fileState(strBuf)) // 如果有同名文件，Kill
            mrc_remove(strBuf);
        if (MR_IS_DIR != mrc_fileState(strBuf)) // 如果无文件夹，Make
            mrc_mkDir(strBuf);
        if (MR_IS_DIR != mrc_fileState(strBuf)) // 创建失败
        {
            TRYFREE(strBuf);
            return -1;
        }

        *p = '/';
    }

    TRYFREE(strBuf);
    return 0;
}

// 创建多层目录，自动判断绝对和相对路径
// 成功返回0，失败返回-1
int32 MakeDirEx(const char *newdir)
{
    char *p;
    int32 ret;

    p = (char*)strchr(newdir, ':');

    if (NULL != p)
    {
        // 绝对路径
        *(p + 1) = '\0';
        ret = Switch2Dir(newdir);
        if (0 == ret)
            ret = MakeDir(p + 2);
        Switch2Dir(NULL);
    }
    else
    {
        // 相对路径
        ret = MakeDir(newdir);
    }

    return ret;
}

//取得磁盘空间大小
//参数：盘符，大小类型：0 - 总大小，1 - 剩余大小
//返回：大小，-1失败
int64 GetDiskSize(const char* strDisk, uint8 Type)
{
    T_DSM_FREE_SAPCE *getInfo = NULL;
    //MR_PLAT_EX_CB cb = NULL;
    int32 output_len = 0;
    int32 ret;

    ret = mrc_platEx(1305, (uint8*)strDisk, 1, (uint8**)&getInfo, &output_len, NULL);
    if (MR_SUCCESS == ret)
    {
        if (0 == Type)
        {
            return (int64)getInfo->total * (int64)getInfo->tunit;
        }
        else if (1 == Type)
        {
            return (int64)getInfo->account * (int64)getInfo->unit;
        }

        /*if (NULL != cb)
            cb((uint8*)getInfo, output_len);*/
    }

    return -1;
}

// 取得程序ID
int32 GetAppId(void)
{
    int32 appid = 0;
    char *pMrpName = mrc_getPackName();

    mrc_GetMrpInfo(pMrpName, MRP_APPID, (uint8*)&appid, sizeof(int32));

    return appid;
}

// 取得程序版本
int32 GetAppVer(void)
{
    int32 appver = 0;
    char *pMrpName = mrc_getPackName();

    mrc_GetMrpInfo(pMrpName, MRP_APPVER, (uint8*)&appver, sizeof(int32));

    return appver;
}

//递归删除文件夹下的所有文件
int32 removeDir(const char* dirName)
{
    char fullName[128];
    char fileName[64];

    int32 ret = -1;
    int32 dirNameLen;
     //重置数据
    if(!dirName)
        return MR_FAILED;    

    memset(fullName, 0, sizeof(fullName));
    memset(fileName, 0, sizeof(fileName));

    dirNameLen = strlen(dirName);
    memcpy(fullName, dirName, dirNameLen);

    if(fullName[dirNameLen-1] != '/')
    {//准备拼接目录下的子文件或子目录
        fullName[dirNameLen] = '/';
        dirNameLen++;
    }

    if(mrc_fileState(dirName) == MR_IS_DIR)
    {
        int32 fd;
        int32 result = MR_FAILED;
         //搜索初始化
        fd = mrc_findStart(dirName, fileName, sizeof(fileName));
        if(fd > 0)//搜索句柄有效
        {
            result = MR_SUCCESS;

            while(result == MR_SUCCESS)
            {
                //如果fileName取值为’.’标识该搜索值无效，请放弃本次处理。
                if(fileName[0] && fileName[0] != '.')
                {
                    int32 length = strlen(fileName);
                    memcpy(&fullName[dirNameLen], fileName, length);
                    fullName[dirNameLen+length] = 0;

                    if(mrc_fileState(fullName) == MR_IS_FILE)
                    {//文件类型，删除文件
                        //mrc_printf("删除文件：%s", fullName);
                        ret = mrc_remove(fullName);
                    }
                    else if(mrc_fileState(fullName) == MR_IS_DIR)
                    {//文件夹类型，递归删除子文件夹
                        //mrc_printf("删除文件夹：%s", fullName);
                        ret = removeDir(fullName);
                    }
                }
                memset(fileName, 0, sizeof(fileName));
                result = mrc_findGetNext(fd, fileName, sizeof(fileName));
            }
            mrc_findStop(fd);
        }
        //mrc_printf("删除父目录：%s", dirName);
        ret = mrc_rmDir(dirName);
    }
    return ret;
}

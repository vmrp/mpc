#ifndef __MRC_AUX_H__
#define __MRC_AUX_H__

/*
本文件以及对应的C代码文件，对mythroad提供的基本函数
进行了封装，提供一些有用的辅助函数。
*/


/*
读取mrp中一个文件的指定长度内容到指定空间。函数
读取文件filename的len长度的内容到buf指向的地址上，若
文件长度小于len，仅读取文件长度的内容，len被赋值
为实际读取的长度。

输入:
filename          文件名
buf                  读取的文件的保存空间
len                  期望读取长度

输出:
len                  实际读取长度

返回:
      MR_SUCCESS  成功
      MR_FAILED   失败
*/
int32 mrc_readAllEx(char* filename, char* buf, int32 *len);

#endif


#ifndef __NV_BASE_H__
#define __NV_BASE_H__

// 返回后台
void MINApp(void);
// 切换目录
void SwitchDisk(const char* strDisk);
// 指针偏移
char* strplen(char *text,int32 poss,int32 str_len);
// 获取指定磁盘整个文件
void* ReadAllex(const char* strDisk,const char* returnstrDisk,const char* filename);
// 字符查找,包括查找字符
char *strfind(char *text,char *s,char *t);
// 字符查找,不包括查找字符
char *strfindEx(char *text,char *s,char *ts);

#endif // __NV_BASE_H__

/*
龙魂噬心(6369)

*/
#ifndef _FILEFIND_H
#define _FILEFIND_H



#define PATHLEN 1000//最长路径
#define NAMELEN 74//文件名最长
#define MAX 300//最多文件数
#define BMP "icon.bmp"


void loadFileFind (void);



//寻找指定目录
void FileFind(char *path);



//释放内存
void FileFindDel (void);


//清除缓存内容
void clearbuf(void);



void DrawMain (void);


//绘制列表
void DrawList(void);



//焦点绘制
void dorect(int x,int y,int w,int h,int r,int g,int b);
   



//加载图片（需用free释放），返回其地址
void *LoadBmp(char *FileName);


//反回上级目录

int back(void);

//焦点点击
int donext(void);



//事件处理
int fileFindEvent(int msg,int p1,int p2);



//连续按键事件回调
void _time(int a);


//初始化函数，请一定在使用其它之前调用
int fileFindInit(void);



//退出函数
int fileFindExit(void);

#endif

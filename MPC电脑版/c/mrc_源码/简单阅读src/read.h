/*龙魂噬心(6369) 
*/

#ifndef READ_H
#define READ_H


#define SEKFILE "read.sek"//记录文件


extern int32 size,  sek, nsek;//文件长度，文件句柄，当前指针，当页首指针

extern int16 xs;//行像素

extern char* fbuf;//排版缓冲

extern mr_screenRectSt rect;//显示区域
extern mr_colourSt bclr, qclr;//背景，前景

extern int32 btupclr,btdownclr;//标题栏颜色
extern int pin,fu;//全屏开关，皮肤变量
extern int siz;//显示区域距离四边的距离，就是外框宽度
extern int32 zw,zh;

int readInit(char* name);


int readExit(void);//退出阅读


int readEvent(int msg,int p1,int p2);//阅读界面事件处理


int downPaiBan(void);



int upPaiBan(void);//上翻页


void huanFu(void);//切换皮肤




void huanPin(void);//切换全屏


void func(void);


#endif
#ifndef TITLEBAR_H
#define TITLEBAR_H
//龙魂噬心(6369)
//create date 2012-7-17



typedef struct
{
int x;int y;int w;int h;
int32 upclr;
int32 downclr;
int32 titclr;
char*title;
}Title,*PTitle;

/*创建标题栏
x,y,w,h 窗口位置大小
upclr 标题栏上部颜色
downclr 下部颜色
titclr 文字颜色
title 标题文字
返回值 指向标题栏的指针
*/

PTitle titleBarCreate(int x,int y,int w,int h,int32 upclr,int32 downclr,int32 titclr,char* title);


/*删除标题栏*/

void titleBarDel(PTitle a);



/*绘制标题栏*/

void drawTitleBar(PTitle a);

#endif
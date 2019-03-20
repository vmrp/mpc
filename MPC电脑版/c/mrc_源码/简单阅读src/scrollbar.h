#ifndef _SCROLLBAR_H
#define _SCROLLBAR_H

//龙魂噬心(6369)
//create date 2012-7-17
#include "mrc_base.h"

typedef struct
{
int x;
int y;
int w;
int h;
int32 allid;//全部的滚动id(数量)，(一共有多少个滚动的值)
int32 nowid;//当前的滚动id
int pagesize;//每页可以显示几个
int32 bgclr;//背景
int32 qgclr;//前景

}Scroll,*PScroll;

/*创建一个滚动条
x, y, w, h   窗口位置
qgclr  滚动滑块颜色
bgclr  背景颜色
返回值
   0  创建失败
   》0  窗口句柄
*/

PScroll scrollBarCreate(int x, int y, int w, int h,int pagesize, int32 bgclr, int32 qgclr);


/*删除滚动条
*/
void scrollBarDel(PScroll a);



/*设置滚动条全部滚动id数量

a 滚动条句柄
id 滚动id数量
返回值：无

*/

void setScrollBarAllId(PScroll a, int32 id);

 

/*设置当前的滚动id
a  滚动条句柄
id 当前的id
返回值：无
*/ 


void setScrollBarNowId(PScroll a, int32 id);


/*滚动条当前id移动
a  滚动条句柄
is_next 是否向下移动
  1 向下
  2 向上
返回值 无
*/
void scrollBarNowIdNext(PScroll a, int is_next);



/*绘制滚动条
 a  滚动条句柄
返回值 无
*/
void drawScrollBar(PScroll a);



#endif
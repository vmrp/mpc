#ifndef PROGBAR_H
#define PROGBAR_H

//龙魂噬心(6369)
//create date 2012-7-19


typedef struct
{
int x;int y;int w;int h;
int32 allid;//全部的进度id(数量)，(一共有多少个进度的值)
int32 nowid;//当前的进度id
int32 bgclr;//背景
int32 qgclr;//前景

}Prog,*PProg;

/*创建一个进度条
x, y, w, h   窗口位置
qgclr  前景颜色
bgclr  背景颜色
返回值
   0  创建失败
   >0  窗口指针
*/

PProg progBarCreate(int x, int y, int w, int h, int32 bgclr, int32 qgclr);



/*删除进度条
*/
void progBarDel(PProg a);



/*设置进度条全部进度id数量

a 进度条句柄
id 进度条id数量
返回值：无

*/

void setProgBarAllId(PProg a, int32 id);

/*设置当前的进度id
a  进度条句柄
id 当前的id
返回值：无
*/ 


void setProgBarNowId(PProg a, int32 id);


/*进度条当前id移动
a  进度条句柄
is_next 是否向下移动
  1 向下
  2 向上
返回值 无
*/
void progBarNowIdNext(PProg a, int is_next);


/*绘制滚动条
 a  滚动条句柄
返回值 无
*/
void drawProgBar(PProg a);


#endif
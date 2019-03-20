//龙魂噬心(6369)
//create date 2012-7-17


#include "type.h"
#include "scrollbar.h"
//#define SCROLLBGCLR 0x000000

/*创建一个滚动条
x, y, w, h   窗口位置
qgclr  滚动滑块颜色
bgclr  背景颜色
返回值
   0  创建失败
   》0  窗口句柄
*/

PScroll scrollBarCreate(int x, int y, int w, int h,int pagesize, int32 bgclr, int32 qgclr)
{
  PScroll a;

  a = mrc_malloc( sizeof(Scroll));
  if( a == 0 ) return 0;

  a->x = x;
  a->y = y;
  a->w = w;
  a->h = h;
  a->pagesize=pagesize;
  a->bgclr = bgclr;
  a->qgclr = qgclr;
  a->allid = 0;
  a->nowid = 0;
  return a;
}


/*删除滚动条
*/
void scrollBarDel(PScroll a)
{
  mrc_free(a);
  a=0;
}

/*设置滚动条全部滚动id数量

a 滚动条句柄
id 滚动id数量
返回值：无

*/

void setScrollBarAllId(PScroll a, int32 id)
{
  a->allid = id;
}
 

/*设置当前的滚动id
a  滚动条句柄
id 当前的id
返回值：无
*/ 


void setScrollBarNowId(PScroll a, int32 id)
{
  a->nowid = id;
}

/*滚动条当前id移动
a  滚动条句柄
is_next 是否向下移动
  1 向下
  2 向上
返回值 无
*/
void scrollBarNowIdNext(PScroll a, int is_next)
{
if(is_next)
  {a->nowid++;
if(a->nowid>=a->allid)
a->nowid=a->allid;
}
else
{  a->nowid--;
if(a->nowid<=0)
  a->nowid=0;
}

drawScrollBar( a );
}


/*绘制滚动条
 a  滚动条句柄
返回值 无
*/
void drawScrollBar(PScroll a)
{
  int r,g,b;
  int x, y, w, h;//滑块位置。大小

  make(a->bgclr,&r, &g, &b);
  mrc_drawRect(a->x, a->y, a->w, a->h, r/2, g/2, b/2);
  mrc_drawRect(a->x+1, a->y+1, a->w-2, a->h-2, r, g, b);
  x = a->x ;
  
  w = a->w ;
  if(a->allid <= a->pagesize)
    {h = a->h-2;
    y = a->y+1;}
  else
    {h =(a->h*a->pagesize)/a->allid;
if(h<5)h=5;
    y=a->nowid*(a->h-h)/a->allid+a->y;}
  make(a->qgclr, &r, &g, &b);
  mrc_drawLine(x+1, y, x+w-2, y, r, g, b);
  mrc_drawLine(x+1, y+h-1, x+w-2, y+h-1, r, g, b);
  mrc_drawLine(x, y+1, x, y+h-2, r, g, b);
  mrc_drawLine(x+w-1, y+1, x+w-1, y+h-2,r, g, b);
  mrc_drawRect(x+1, y+1, w-2, h-2, r, g, b);
}
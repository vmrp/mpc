//龙魂噬心(6369)
//create date 2012-7-19

#include "mrc_base.h"
#include "type.h"
#include "progbar.h"


/*创建一个进度条
x, y, w, h   窗口位置
qgclr  前景颜色
bgclr  背景颜色
返回值
   0  创建失败
   >0  窗口指针
*/

PProg progBarCreate(int x, int y, int w, int h, int32 bgclr, int32 qgclr)
{
  PProg a;

  a = mrc_malloc( sizeof(Prog));
  if( a == 0 ) return 0;

  a->x = x;
  a->y = y;
  a->w = w;
  a->h = h;
  a->bgclr = bgclr;
  a->qgclr = qgclr;
  a->allid = 0;
  a->nowid = 0;
  return a;
}


/*删除进度条
*/
void progBarDel(PProg a)
{
  mrc_free(a);
  a=0;
}

/*设置进度条全部进度id数量

a 进度条句柄
id 进度条id数量
返回值：无

*/

void setProgBarAllId(PProg a, int32 id)
{
  a->allid = id;
}
 

/*设置当前的进度id
a  进度条句柄
id 当前的id
返回值：无
*/ 


void setProgBarNowId(PProg a, int32 id)
{
  a->nowid = id;
}

/*进度条当前id移动
a  进度条句柄
is_next 是否向下移动
  1 向下
  2 向上
返回值 无
*/
void progBarNowIdNext(PProg a, int is_next)
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


drawProgBar( a );
}


/*绘制滚动条
 a  滚动条句柄
返回值 无
*/
void drawProgBar(PProg a)
{
  int r,g,b;
  int x, y, w, h;//滑块位置。大小

  make(a->bgclr,&r, &g, &b);
  mrc_drawRect(a->x, a->y, a->w, a->h, r/2, g/2, b/2);
  mrc_drawRect(a->x+1, a->y+1, a->w-2, a->h-2, r, g, b);
  x = a->x ;
  
  y = a->y ;w=a->nowid*(a->w)/a->allid;
h=a->h;
  make(a->qgclr, &r, &g, &b);
  //dline(x+1, y, x+w-2, y, r, g, b);
  //dline(x+1, y+h-1, x+w-2, y+h-1, r, g, b);
  //dline(x, y+1, x, y+h-2, r, g, b);
  //dline(x+w-1, y+1, x+w-1, y+h-2,r, g, b);
  mrc_drawRect(x+1, y+1, w-2, h-2, r, g, b);
}
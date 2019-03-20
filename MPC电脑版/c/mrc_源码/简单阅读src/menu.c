
/*龙魂噬心(6369)
2012-7-11*/

#include "mrc_base.h"
#include "mrc_exb.h"

#include "type.h"
#include "menu.h"

/*创建一个菜单
 rect 菜单位置，大小
 菜单高度计算方式为：
   (字体高度+5)*菜单项数+4
  宽度最低为：字体宽度+20
 num 菜单项数目

 返回值：
  0 创建菜单失败
  非0 指向菜单内存的指针
*/
PMenu menuCreate(int x,int y,int w,int h, int num)
{
	int i;
  PMenu a;
  a = (PMenu)mrc_malloc(sizeof(Menu));

  if(a == NULL) return 0;

  a -> x = x;
a->y=y;
a->w=w;
a->h=h;
a->focus=0;
  a -> num = num;
  for(i = 0; i<num; i++)
  {
    a -> item[i] = (char*)mrc_malloc(MAX_ITEMSIZE*sizeof(char));
    if(a->item[i]==NULL)
{mrc_free(a);return 0;}
    mrc_memset(a->item[i], 0, sizeof(a->item[i]));
  }
return a;
}


/*删除一个菜单
 a 菜单的指针
 
 返回值：无
*/
void menuDel(PMenu a)
{
	int i;
  for(i = 0; i<a->num; i++)
    mrc_free(a->item[i]);
  mrc_free(a);
  a= 0;
}


/*设置一个菜单项
  a 菜单句柄
  index 菜单项序号
  title 菜单项内容
  
  返回值：-1  失败
             0 成功
*/
int setMenuItem(PMenu a,int index, char* title)
{
  if(index >= a->num) return -1;
  mrc_memcpy(a->item[index],title, MAX_ITEMSIZE);
  return 0;
}
 

/*设置菜单焦点
  a 菜单句柄
  index 菜单项序号
  
  返回值：
    -1  失败
    0 成功
*/

int setMenuFocus(PMenu a, int index)
{
  if(a->num <= index) return -1;
  a->focus = index;
  return 0;
}

/*获得菜单焦点
  a  菜单句柄
返回值 ：无
*/
int getMenuFocus(PMenu a)
{
  return a->focus;
}


/*菜单焦点移动
  a  菜单句柄
  is_next  是否向下移动
    1 向下
    0 向上
  
  返回值 ：无
*/
void menuFocusNext(PMenu a, int is_next)
{
  if(is_next)
  {
    a -> focus++;
    if(a -> focus >= a->num) 
      a -> focus = 0;

  }
  else
  {
    a -> focus--;
    if(a->focus<0)
      a->focus = a->num - 1;
      
  }
  menuDraw(a);
}



/*显示菜单
  a  菜单句柄
  返回值：无
*/
void menuDraw(PMenu a)
{
  int x, y, w, h;
  int32 zw, zh;
  int i;
  x = a->x; y = a->y; w = a -> w; h= a->h;

  mrc_drawRect(x, y, w, h, MAKER(MENUBG)/2, MAKEG(MENUBG)/2, MAKEB(MENUBG)/2);
  mrc_drawRect(x+1, y+1, w-2, h-2, MAKER(MENUBG), MAKEG(MENUBG), MAKEB(MENUBG));
  mrc_textWidthHeight("鼎", 0, FONT, &zw, &zh);
  zh+=5;
  shaderect(x+2, y+2+(a->focus*zh), w-4, zh, MENUFOCUS, MENUFOCUS-MENUFOCUS/5,0);
  for(i = 0; i<(a->num); i++)
  {
    if(i == a->focus)
      mrc_drawText(a->item[i], x+10, y+2+(i*zh)+2,MAKER(MENUBG), MAKEG(MENUBG), MAKEB(MENUBG),0 , FONT);
    else
       mrc_drawText(a->item[i], x+10, y+2+(i*zh)+2,MAKER(MENUITEM), MAKEG(MENUITEM), MAKEB(MENUITEM),0 , FONT);
  }

}



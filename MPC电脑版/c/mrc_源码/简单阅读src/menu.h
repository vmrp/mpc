
/*龙魂噬心(6369)
2012-7-11*/
#ifndef MENU_H
#define MENU_H

#define MAX_NUM 20//最大菜单项
#define MAX_ITEMSIZE 10//菜单项最多字数
#define MENUBG 0x00ffffff//菜单背景
#define MENUITEM 0x00000000//菜单项
#define MENUFOCUS 0x0000ff00//菜单焦点


typedef struct
{
  int x;int y;int w;int h;
  int num;
  int focus;
  char* item[MAX_ITEMSIZE];
}Menu,*PMenu;

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
PMenu menuCreate(int x,int y,int w,int h, int num);




/*删除一个菜单
 a 菜单的指针
 
 返回值：无
*/
void menuDel(PMenu a);





/*设置一个菜单项
  a 菜单句柄
  index 菜单项序号
  title 菜单项内容
  
  返回值：-1  失败
             0 成功
*/
int setMenuItem(PMenu a,int index, char* title);


/*设置菜单焦点
  a 菜单句柄
  index 菜单项序号
  
  返回值：
    -1  失败
    0 成功
*/

int setMenuFocus(PMenu a, int index);


/*获得菜单焦点
  a  菜单句柄
返回值 ：无
*/
int getMenuFocus(PMenu a);



/*菜单焦点移动
  a  菜单句柄
  is_next  是否向下移动
    1 向下
    0 向上
  
  返回值 ：无
*/
void menuFocusNext(PMenu a, int is_next);



/*显示菜单
  a  菜单句柄
  返回值：无
*/
void menuDraw(PMenu a);

#endif
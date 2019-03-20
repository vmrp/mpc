#ifndef EDIT_H
#define EDIT_H

/*龙魂噬心(6369)
2012-7-10*/

#define TITMAX 20//标题最多文字
#define SPEED 800//光标速度，无效


typedef struct
{
int x; int y; int w; int h;
char* title;
char* str;
int32 titleclr;
int32 bkclr;
int32 bgclr;

int max;
int type;
int timeEdit;
int speed;
}Edit,*PEdit;


/*创建一个编辑框
参数：
@ x，y，w，h：编辑框尺寸
@ title 编辑框标题( <=20)
@ str 编辑框内容
@ titleclr 文字颜色
@ bkclr 边框颜色
@ bgclr 背景颜色
@ max 最大输入字数
@ type编辑框类型：
     0: 全部
     1：字符
     2：密码
返回值：
 0 创建失败
 >0 编辑框指针
*/
PEdit editCreate(int x, int y, int w, int h, char* title,char* str, int32 titleclr, int32 bkclr, int32 bgclr, int max, int type);

/*删除编辑框
参数：
 a：编辑框指针
返回值：无
*/
void editDel(PEdit a);

/*设置焦点编辑框
@ a ：编辑框指针，
*当为NULL时则相当于隐藏编辑框
*注意：请保证指针合法性，否则可以引起程序崩溃
返回值：无效
 */

int setEditFocus(PEdit a);


//得到编辑框内容
char* getEditStr(PEdit a);



/*绘制编辑框
*/
void editDraw(PEdit a);

//焦点编辑框光标回调(函数暂时无用
void _editTime(int a);


//焦点编辑框事件处理。
int editEvent(PEdit a,int msg,int p1,int p2);

#endif

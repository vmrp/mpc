#ifndef TOOLBAR_H
#define TOOLBAR_H
//龙魂噬心(6369)
//create date 2012-7-17


typedef struct
{
	int x;int y;int w;int h;
	int32 upclr;
	int32 downclr;
	int32 titclr;
	char* left;
	char* zhon;
	char* right;
}Tool,*PTool;


/*创建标题栏
x,y,w,h 窗口位置大小
upclr 标题栏上部颜色
downclr 下部颜色
titclr 文字颜色
left 左边文字
zhon 中间
right 右边
返回值 指向标题栏的指针
*/

PTool toolBarCreate(int x,int y,int w,int h,int32 upclr,int32 downclr,int32 titclr,char* left,char* zhon,char* right);

/*删除标题栏*/

void toolBarDel(PTool a);


/*绘制标题栏*/

void drawToolBar(PTool a);

#endif
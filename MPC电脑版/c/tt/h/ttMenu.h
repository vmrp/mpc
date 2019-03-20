
/*
* tt菜单封装
* ttMenu.h
* By:天天
* QQ:643552878
*/


/*菜单结点*/
typedef struct menupoint
{
 struct menupoint *prev;//前一项
 struct menupoint *next;//后一项
 char *text;//菜单文字
 uint32 id;//菜单id
}
MenuPoint;
typedef (*MenuPoint) PMenu;

uint32 PMenuLen=sizeof(MenuPoint);


/*菜单列表结构体*/typedef struct menulist
{
 uint16 x; uint16 y;
 //菜单开始xy位置
 uint16 bgclr;//背景色
 uint16 chclr;//前景色

 PMenu first;//开头
 PMenu last;//结尾
 PMenu now;//焦点所在

 uint32 Num;//菜单个数
 uint32 id;//焦点id
}
MenuList;
typedef (*MenuList) LMenu;

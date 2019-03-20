
/*龙魂噬心(6369)
2012-7-10*/

#include "mrc_base.h"
#include "mrc_exb.h"
#include "type.h"
#include "edit.h"

//PEdit EditFocus;
//int x,_z;
 static int32 focusdia;


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
PEdit editCreate(int x, int y, int w, int h, char* title,char* str, int32 titleclr, int32 bkclr, int32 bgclr, int max, int type)
{
PEdit a;
a=mrc_malloc(sizeof(Edit)+2);
if(a==NULL) return 0;
a->title=mrc_malloc(TITMAX*sizeof(char));
mrc_memset(a->title,0,sizeof(a->title));
mrc_strcpy(a->title,title);
a->str=mrc_malloc(max*sizeof(char));
if(a->str==NULL) return 0;
mrc_memcpy(a->str,str,sizeof(a->str));
a->max=max;
a->type = type;
a->x = x;
a->y = y;
a->w =w;
a->h = h;
a->titleclr = titleclr;
a->bkclr = bkclr;
a->bgclr = bgclr;
//a->timeEdit=timercreate();
//a->speed=SPEED;
return a;

}
/*删除编辑框
参数：
 a：编辑框指针
返回值：无
*/
void editDel(PEdit a)
{
mrc_free(a->title);
mrc_free(a->str);
//timerdel(a->timeEdit);
mrc_free(a);
a=0;
}

/*设置焦点编辑框
@ a ：编辑框指针，
*当为NULL时则相当于隐藏编辑框
*注意：请保证指针合法性，否则可以引起程序崩溃
返回值：无效
 */

int setEditFocus(PEdit a)
{
int r,g,b;
editDraw(a);
r=MAKER(FOCUSBK);
g=MAKEG(FOCUSBK);
b=MAKEB(FOCUSBK);
//EditFocus=a;
mrc_drawRect(a->x+1, a->y, a->w-1, FOCUSBKW,  r, g, b);
mrc_drawRect(a->x+1, a->y+a->h-FOCUSBKW+1,a->w-1, FOCUSBKW,r,g,b);
mrc_drawRect(a->x,a->y+1,FOCUSBKW,a->h-1,r,g,b);
mrc_drawRect(a->x+a->w-FOCUSBKW+1,a->y+1,FOCUSBKW,a->h-1,r,g,b);
 return 0;
}


//得到编辑框内容
char* getEditStr(PEdit a)
{
return a->str;
}

/*绘制编辑框
*/
void editDraw(PEdit a)
{
 int r, g, b;
// int zw,zh;
 mr_screenRectSt rect;
 mr_colourSt clr;
 r=MAKER(a->bkclr);g=MAKEG(a->bkclr);b=MAKEB(a->bkclr);
 mrc_drawLine(a->x+1, a->y, a->x+a->w-1, a->y, r, g, b);
 mrc_drawLine(a->x+1, a->y+a->h, a->x+a->w-1, a->y+a->h, r, g, b);
 mrc_drawLine(a->x, a->y+1, a->x, a->y+a->h-1, r, g, b);
 mrc_drawLine(a->x+a->w, a->y+1, a->x+a->w,a->y+a->h-1,r, g, b);
 
 mrc_drawRect(a->x+1, a->y+1, a->w-1, a->h-1, MAKER(a->bgclr), MAKEG(a->bgclr), MAKEB(a->bgclr));

 rect.x=a->x+FOCUSBKW;rect.y=a->y+FOCUSBKW;
 rect.w=a->w-FOCUSBKW*2;rect.h=a->h-FOCUSBKW*2;
 clr.r=MAKER(a->titleclr);
 clr.g=MAKEG(a->titleclr);
 clr.b=MAKEB(a->titleclr);
 mrc_drawTextEx(a->str, a->x+FOCUSBKW,a->y+FOCUSBKW, rect, clr, 02,FONT);
}

//焦点编辑框光标回调(函数暂时无用
void _editTime(int a)
{
/*_z=!_z;
if(_z)
dline(x,EditFocus->y+2,x,EditFocus->y+EditFocus->h-2,MAKER(EditFocus->bgclr),MAKEG(EditFocus->bgclr),MAKEB(EditFocus->bgclr));
else 
dline(x,EditFocus->y+2,x,EditFocus->y+EditFocus->h-2,MAKER(EditFocus->titleclr),MAKEG(EditFocus->titleclr),MAKEB(EditFocus->titleclr));
ref(x,EditFocus->y,1,EditFocus->h);*/
}


//焦点编辑框事件处理。
int editEvent(PEdit a, int msg,int p1,int p2)
{
	uint8* p;
	int32 i;
	
 if(MR_KEY_RELEASE==msg)
 {
  switch(p1)
  {
   case MR_KEY_SELECT:
 
   focusdia=mrc_editCreate((char*)gb_uni(a->title,0),(char*)gb_uni(a->str,0),a->type,a->max);
    break;
   
  }  
 }
//对编辑事件处理
  if(msg == MR_DIALOG_EVENT)
 {
  switch(p1)
  {
  case MR_DIALOG_KEY_OK:
	  p=(uint8*)mrc_malloc(sizeof(a->str));
	  mrc_memset(p,0,sizeof(p));
	  mrc_memset(a->str,0,sizeof(a->str));
	  mrc_unicodeToGb2312((uint8*)mrc_editGetText(focusdia),mrc_wstrlen((char*)mrc_editGetText(focusdia)),&p,&i);
	  mrc_strcpy(a->str ,(char*)p );
	  mrc_free(p);

   case MR_DIALOG_KEY_CANCEL:
    mrc_editRelease(focusdia);
	break;
  }
 return 1;//请注意，返回值为1，可判断是否为1来重绘界面
 }
return 0;
}



//龙魂噬心(6369)
//2012-7-17
#include "mrc_base.h"
#include "type.h"
#include "button.h"

/*创建按纽*/

PButton buttonCreate(int x,int y,int w,int h,int32 upclr,int32 downclr,int32 titclr,char* title)
{
PButton a;
a=mrc_malloc(sizeof(Button));
if(a==NULL) return 0;
a->x=x;
a->y=y;
a->w=w;
a->h=h;
a->upclr=upclr;
a->downclr=downclr;
a->titclr=titclr;
a->title=title;
return a;
}

/*删除按钮*/

void buttonDel(PButton a)
{
	mrc_free(a);
	a=0;
}

/*设置按纽得到焦点*/
void setButtonFocus(PButton a)
{
a->is_getfocus=1;
}
/*设置按纽失去焦点*/
void setButtonClearFocus(PButton a)
{
a->is_getfocus=0;
}
/*按钮获得焦点*/

void buttonGetFocus(PButton a)
{
int r,g,b;
make(FOCUSBK,&r,&g,&b);
mrc_drawRect(a->x+1,a->y,a->w-2,FOCUSBKW,r,g,b);
mrc_drawRect(a->x+1,a->y+a->h-FOCUSBKW,a->w-2,FOCUSBKW,r,g,b);
mrc_drawRect(a->x,a->y+1,FOCUSBKW,a->h-2,r,g,b);
mrc_drawRect(a->x+a->w-FOCUSBKW,a->y+1,FOCUSBKW,a->h-2,r,g,b);

}

/*按钮被单击*/
void buttonClick(PButton a)
{
int32 zw,zh;

mrc_textWidthHeight(a->title,0,FONT,&zw,&zh);
mrc_drawLine(a->x+1,a->y,a->x+a->w-2,a->y,MAKER(a->downclr),MAKEG(a->downclr),MAKEB(a->downclr));
shaderect(a->x,a->y+1,a->w,a->h-2,a->downclr,a->upclr,0);
mrc_drawLine(a->x+1,a->y+a->h-1,a->x+a->w-2,a->y+a->h-1,MAKER(a->upclr),MAKEG(a->upclr),MAKEB(a->upclr));
mrc_drawText(a->title,a->x+(a->w-zw)/2,a->y+(a->h-zh)/2,MAKER(a->titclr),MAKEG(a->titclr),MAKEB(a->titclr),0,FONT);
if(a->is_getfocus)
buttonGetFocus(a);
}


/*按钮被释放*/
void buttonReleace(PButton a)
{
drawButton(a);
}

/*绘制按钮*/
void drawButton(PButton a)
{
int32 zw,zh;

mrc_textWidthHeight(a->title,0,FONT,&zw,&zh);
mrc_drawLine(a->x+1,a->y,a->x+a->w-2,a->y,MAKER(a->upclr),MAKEG(a->upclr),MAKEB(a->upclr));
shaderect(a->x,a->y+1,a->w,a->h-2,a->upclr,a->downclr,0);
mrc_drawLine(a->x+1,a->y+a->h-1,a->x+a->w-2,a->y+a->h-1,MAKER(a->downclr),MAKEG(a->downclr),MAKEB(a->downclr));
mrc_drawText(a->title,a->x+(a->w-zw)/2,a->y+(a->h-zh)/2,MAKER(a->titclr),MAKEG(a->titclr),MAKEB(a->titclr),0,FONT);
if(a->is_getfocus)
buttonGetFocus(a);
}

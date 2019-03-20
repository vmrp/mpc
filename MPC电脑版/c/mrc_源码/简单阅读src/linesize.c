//龙魂噬心(6369)

#include "mrc_base.h"
#include "edit.h"
#include "linesize.h"
#include "msgbox.h"
#include "read.h"
#include "type.h"
#include "screen.h"
#include "win.h"

PEdit LineSizeEdit;

void inLineSizeInit(void)
{
mr_screenRectSt rect;
//char temp[15],
char temp2[3];
//sprintf(temp,"跳转:0~%d",size);
mrc_sprintf(temp2,"%d",xs);
rect.x=0;rect.y=SCRH-zh*4;
rect.w=SCRW;rect.h=zh*3;
mrc_drawRect(rect.x,rect.y,rect.w,rect.h,bclr.r/2,bclr.g/2,bclr.b/2);
mrc_drawRect(rect.x+1,rect.y+zh,rect.w-2,rect.h-zh,bclr.r,bclr.g,bclr.b);
showTitleBar(rect.x,rect.y,rect.w,zh,btupclr,btdownclr,MAKERGB(qclr.r,qclr.g,qclr.b),"阅读行距");
showToolBar(rect.x,SCRH-zh,rect.w,zh,btupclr,btdownclr,MAKERGB(qclr.r,qclr.g,qclr.b),"确定","编辑","返回");

LineSizeEdit=editCreate(rect.x+10,rect.y+zh+zh/2,rect.w-20,zh,"阅读行距",temp2,0x00000000,0x00ff0000,0x00ffffaa,2,1);
setEditFocus(LineSizeEdit);
mrc_refreshScreen(0,0,SCRW,SCRH);
saveScreenBuf();
}


void inLineSizeExit(void)
{
editDel(LineSizeEdit);
}


int inLineSizeEvent(int type,int p1,int p2)
{
//saveScreenBuf();
if(1==editEvent(LineSizeEdit,type,p1,p2))
{
//重绘界面
readScreenBuf();
setEditFocus(LineSizeEdit);
mrc_refreshScreen(0,0,SCRW,SCRH);
}
if(MR_KEY_RELEASE==type)
{
switch(p1)
{
case MR_KEY_SOFTLEFT:
xs=mrc_atoi(getEditStr(LineSizeEdit));
//break;
case MR_KEY_SOFTRIGHT:
win=WIN_READ;
inLineSizeExit();

sek=nsek;
downPaiBan();
break;
}
}
return 0;
}
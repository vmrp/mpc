/*简单阅读MPC版v2.01。
作者：龙魂噬心(6369)
发布网站：wap.mrpej.com


新增文件浏览。
阅读界面：
 方向键，2，4，6，8键翻页。
 1：消除乱码。
 3：跳到n字节
 7：切换全屏：
 9：设置边框宽度
 *：文件开始
 #：文件末尾
 0：切换皮肤

文件浏览界面：
 上下方向键：上下移动
 确定，左软键：选择
 右软键，返回上级目录。根目录则返回阅读界面

任意界面拨号键截图

*/
//#define FONT 1
#include "mrc_base.h"
//#include "../controls/type.h"
#include "read.h"
#include "win.h"
#include "filefind.h"
#include "readmenu.h"
#include "jump.h"
#include "BK.h"
#include "LineSize.h"
#include "Screen.h"
#include "msgbox.h"

WIN win;
int32 FONT;
int32 mrc_init(void)
{
	win = WIN_FILEFIND;
	FONT = 1;
    readInit("");
    menuInit();
    fileFindInit();
//    downPaiBan();
    return MR_SUCCESS;
}

int32 mrc_appEvent(int type, int p1, int p2)
{
switch(win)
{
case WIN_READ:
  readEvent(type,p1,p2);
  break;
case WIN_FILEFIND:
  fileFindEvent(type,p1,p2);
break;
case WIN_MENU:
  menuEvent(type,p1,p2);
  break;
case WIN_INJUMP:
  inJumpEvent(type,p1,p2);
  break;
case WIN_INBK:
inBKEvent(type,p1,p2);
break;
case WIN_INLINESIZE:
inLineSizeEvent(type,p1,p2);
break;
}
  if(p1==MR_KEY_SEND&&type==MR_KEY_RELEASE){
    ScreenShut("简单阅读截图.bmp",0,0,SCRW,SCRH,1);
    
  }
  return MR_SUCCESS;
}

int32 mrc_pause(void)
{
saveScreenBuf();
    return MR_SUCCESS;
}

int32 mrc_resume(void)
{
readScreenBuf();
mrc_refreshScreen(0,0,SCRW,SCRH);
    return MR_SUCCESS;
}

int32 mrc_exitApp(void)
{
	showMsgBox("正在退出。。。");
	  mrc_platEx( 1204, "c:/MYTHROAD", 11, NULL, NULL, NULL );
	
	fileFindExit();
	menuExit();
	readExit();
return MR_SUCCESS;
}
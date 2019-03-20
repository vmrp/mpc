#include <base.h>
#include <c/win/tt.h>
#include <c/win/windef.h>
#include <c/win/winfunc.h>


win_obj *obj;
window *win;

uint16 win_index=0;
int tw,th;
textwh("窗",0,1,&tw,&th);


/*
* 窗口事件
* 输入:
 msg 窗口事件(详细看windef.h)
 若为触屏点击p0,p1 为点击坐标，若为按键点击则都为 -1
*/
int winEvent(window *pWin,uint8 msg,int p1,int p2)
{
 uint16 id=pWin->id;
 switch(msg)
 {
 case WIN_CREATE:
 echo("被创建id",id);
 break;

 case WIN_DESTROY:
 echo("将被消毁id",id);

 break; 
 case WIN_SHOW:
 echo("被显示id",id);
 break;

 case WIN_CLICK:
 echo("被点击id",id);
 break;
 }
 return 0;
}

int init(void)
{
 cls(255,255,255);
 ref(0,0,SCRW,SCRH);
 colorst rgb,rgb2;
 rgbSet(&rgb,0,0,0);
 rgbSet(&rgb2,255,2,2);
 win=winCreate(win_index,0,0,SCRW,th); //创建窗口
 winSet(win,&rgb,&rgb2); //窗口样式设置
 winShow(win); //显示窗口
 return 0;
}

int event(int type, int p1, int p2)
{
 if(KY_DOWN == type)
 switch(p1)
 {
  case _SRIGHT:
  exitapp();

  break;
  case _1:
  break;
 }
 return 0;
}

int pause(void)
{
  return 0;
}

int resume(void)
{
 return 0;
}

/*程序退出*/
void exitapp()
{
 winDel(win);//销毁窗口
 sleep(500);
 exit();
}

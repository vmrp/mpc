#include <base.h>
#include <c/tt/PhotoView.h>

int init(void)
{
//初始化入口(请正确填入文件名)
 if(-1==PVInit("c/tt/By天天.jpg"))
 exit();//初始化失败退出

 return 0;
}

int event(int type, int p1, int p2)
{
/*按键入口*/
 PVEvent(type,p1,p2);

 if(KY_DOWN==type)
 {
 switch(p1)
  {
  case _SRIGHT:
  exit();
  break;
  }
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
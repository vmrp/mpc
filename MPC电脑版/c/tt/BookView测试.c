#include <base.h>
#include <c/tt/BookView.h>

int init(void)
{
//初始化(请正确填入文件名)
 if(-1==BVInit("c/tt/又一个阴天.txt"))
 exit();//初始化失败退出
 return 0;
}

int event(int type, int p1, int p2)
{
//按键入口
 BVEvent(type,p1,p2);
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
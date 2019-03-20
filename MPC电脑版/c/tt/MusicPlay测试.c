#include <base.h>
#include <c/tt/MusicPlay.h>

int init(void)
{
//初始化(请正确填入文件名)
 if(-1==MPInit("My Music/许嵩 - 多余的解释.mp3"))
 exit();//初始化失败后退出
 return 0;
}

int event(int type, int p1, int p2)
{
//按键入口
 MPEvent(type,p1,p2);

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
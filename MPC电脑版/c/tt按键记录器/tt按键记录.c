/*
* 按键记录器
* By:天天
* QQ:643552878
* 2012.8.7
* 说明:按键记录可以在textFile或print.txt文件中查看
*/

#include <base.h>
#include <c/tt按键记录器/tt.h>
#define textFile "c/按键记录.txt"

uint32 Filef,time;


struct
{
 int type;//按键类型
 uint32 downTime;//按键按下的时间
 uint32 upTime;//按键松开的时间
}EventMsg;


int init(void)
{
 ShowMsg("天天制作");
 sleep(1000);
 remove(textFile);
 Filef=open(textFile,8);
 write(Filef,"按键记录:\n",10);

 time=getuptime();
 DrawMain();
 ShowMsg("请按下键盘");

 return 0;
}

int event(int type, int p1, int p2)
{
 if(p1==_SRIGHT)
 {
  exitapp();
  return 0;
 }

 if(type==KY_DOWN)
 {
 EventMsg.type=p1;
 EventMsg.downTime=getuptime();
 writeEventMsg(type);
 }
 else if(type==KY_UP)
 {
  EventMsg.upTime=getuptime();
 writeEventMsg(type);
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

void exitapp()
{
 ShowMsg("天天制作");
 sleep(1000);
 close(Filef);



 exit();
}

void DrawMain()
{
 cls(250,250,250);
 drect(0,th-1,SCRW,1,0,0,0);
 Drawtext("按键记录器",0,0);

 ref(0,0,SCRW,SCRH);
}

void writeEventMsg(int type)
{
 char str[50];
 if(type==KY_DOWN)
 sprintf(str,"时间=%ds,按键值=%d,状态:按下\n",(EventMsg.downTime-time)/1000,EventMsg.type);
 else
 sprintf(str,"时间=%ds,按键值=%d,状态:松开,按下时间=%dms\n",(EventMsg.upTime-time)/1000,EventMsg.type,EventMsg.upTime-EventMsg.downTime);
 DrawEventMsg(str);

 write(Filef,str,strlen(str));
 printf("%s",str);
}
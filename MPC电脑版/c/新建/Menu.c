#include <base.h>

char *MenuTxt="<启动><关于><帮助><退出>";

uint32 MenuN=1;
//MenuN取值
enum
{
 IS_OPEN=1,
 IS_ABOUT,
 IS_HELP,
 IS_EXIT
};


//界面控制
uint32 Jm;
//取值
enum
{
 IS_MENU=1,
 IS_MENU_OPEN
};
Jm=IS_MENU;

int init()
{
 Draw();
 return NULL;
}

void Draw()
{
 switch(Jm)
 { case IS_MENU:
  DrawMenuList();break;
  case IS_MENU_OPEN:
  DrawNoThing();break;
 }

}


void DrawMenuList()
{
 cls(250,250,250);
 DrawMenu(MenuTxt,10,50,0,0,0,MenuN);
 ref(0,0,SCRW,SCRH);
}

void DrawNoThing()
{
 cls(250,250,250);
 dtext("未添加!按右软键返回",0,SCRH/2,0,0,0,FALSE,1);
 ref(0,0,SCRW,SCRH);
}


int event(int type, int p1, int p2)
{
 if(KY_DOWN==type)
 {
 if(Jm==IS_MENU)
 switch(p1)
  {
 case _SRIGHT:
 exit();break;

 case _SELECT:
 switch(MenuN)
 {
  case IS_OPEN:
  case IS_ABOUT:
  case IS_HELP:
  Jm=IS_MENU_OPEN;
 Draw();break;
  case IS_EXIT:exit();break;
 }
 break;

 case _UP:
 if(MenuN>IS_OPEN)MenuN--;
 else MenuN=IS_EXIT;
 Draw();
 break;

 case _DOWN:
 if(MenuN<IS_EXIT)MenuN++;
 else MenuN=IS_OPEN;
 Draw();
 break;

  }
 else if(Jm==IS_MENU_OPEN)
 switch(p1)
 { case _SRIGHT:
  Jm=IS_MENU;
  init();break;  
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


/*
绘制菜单列表
txt形如: 
<管理><关于><帮助><退出>
x,y :开始显示坐标
r,g,b :颜色
n :光标在第几个菜单上
*/

void DrawMenu(char *txt,int16 x,int16 y,uint8 r,uint8 g,uint8 b,uint32 n)
{
 int zw,zh;
 textwh("口",FALSE,1,&zw,&zh);

 uint32 i=1;

 char *str;
 txt=strstr(txt,"<");
 while(txt!=NULL)
 {
 txt=strstr(txt,"<");
 if(txt==NULL)break;
 txt+=1;
 str=strstr(txt,">");
 if(str==NULL)break;
 uint32 len=strlen(txt)-strlen(str);
 char *buffer=malloc(len+4);
 memset(buffer,NULL,len+4);
 memcpy(buffer,txt,len);
 txt=str;
 if(i==n)
 {int w,h;
 textwh(buffer,FALSE,1,&w,&h);
 effsetcon(x,y,w,h,128,128,128);
 }
 if(strlen(buffer)>0)
 dtext(buffer,x,y,r,g,b,FALSE,1);

 i++;y+=zh;
 free(buffer);
 }


}

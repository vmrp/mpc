//wildly 菜单控件

#ifndef MENU_CONTROL_H
#define MENU_CONTROL_H

#include <base.h>

//菜单项字数长度
#define menu_text_size 100

#define SH Texh+10

//菜单结构体
typedef struct
{
   char *text[300];  int br;
   int now;  int bg;
   int num; int bb;
   int gn;  int fr;
   int gf;  int fg;
   int mx;  int fb;
   int my;  int tr;
   int mw;  int tg;
   int mh;  int tb;
}pmenu;

typedef pmenu *Menu;

//创建菜单用的数组。
//创建一个菜单 num菜单最大项
void *MenuCreate(int x,int y,int w,int h,int num)
{Menu a=malloc(sizeof(pmenu));
if(a==NULL)return NULL;
memset(a,0,sizeof(pmenu));
  a->mx=x;
  a->my=y;
  a->mw=w;
  a->mh=h;
  a->num=(num<=300?num:300);
for(int N=0;N<num;N++)
{a->text[N]=malloc(menu_text_size);
if(a->text[N]==NULL)return NULL;
memset(a->text[N],0,menu_text_size);}
MenuRgbSet(a,90,90,90,100,100,200);
MenutRgbSet(a,200,200,200);
a->gn=h/SH;
return a;}

//设置菜单项 item菜单项名称 n菜单项顺序
int MenuSet(Menu a,char *item,int n)
{if(a==NULL)return -1;
if(n>=(a->num) || n<0)return -1;
strcpy(a->text[n],item);
return 0;}

//显示菜单
int MenuShow(Menu a)
{if(a==NULL)return -1;
int i,y;
drect(a->mx,a->my,a->mw,a->mh,a->br,a->bg,a->bb);
for(i=a->gf;i<(a->gf)+(a->gn);i++)
{
if(i>a->num-1)break;
if(i==a->now)
drect(a->mx,y,a->mw,SH,a->fr,a->fg,a->fb);
dtext(a->text[i],a->mx+5,y+5,a->tr,a->tg,a->tb,0,FONT);
y+=SH;
}
ref(a->mx,a->my,a->mw,a->mh);
return 0;}

//菜单控制 is 0上 1下
int MenuEvent(Menu a,int is)
{if(a==NULL)return -1;
if(a->num==0)return -1;
int num=a->num,gn=a->gn,gf=a->gf,now=a->now;
     if(is)
        {
   if(now < num-1)
      {now++;
if(now-gf == gn)
gf++;
   }
else {now=0; gf=now;}

}else
 {
      if(now > 0)
        {now--;
if(now-gf == -1)
gf--;
         }
else {now=num-1; gf=num-gn; if(gf < 0)gf=0;}
    }
a->now=now;
a->gf=gf;
return 0;}


//删除菜单，退出时调用
int MenuDel(Menu a)
{
if(a==NULL)return -1;
for(int N=0;N<(a->num);N++)
free(a->text[N]);
free(a);
return 0;}

//设置菜单颜色b rgb背景颜色,f rgb菜单项颜色
int MenuRgbSet(Menu a,int br,int bg,int bb,int fr,int fg,int fb)
{if(a==NULL)return -1;
  a->br=br;
  a->bg=bg;
  a->bb=bb;
  a->fr=fr;
  a->fg=fg;
  a->fb=fb;
return 0;
}

//设置菜单项名称颜色
int MenutRgbSet(Menu a,int tr,int tg,int tb)
{if(a==NULL)return -1;
  a->tr=tr;
  a->tg=tg;
  a->tb=tb;
return 0;
}

//获取菜单焦点
int getMenuFocus(Menu a)
{if(a==NULL)return -1;
int focus=a->now;
return focus;}

#endif
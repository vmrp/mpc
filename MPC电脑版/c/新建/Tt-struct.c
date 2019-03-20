/*
双向链表示例
by:天天
QQ:643552878
这个也是我自己刚会，才写的。发出来和大家一起学习。不足之处还望指点。
*/

#include <base.h>

/*为了方便，所以除结构体指针外，只定义1个表量*/
typedef struct list
{
 struct list *last;
 /*指向上个结点*/
 struct list *next;
 /*指向下个结点*/
 char *text;
 /*储存文字*/
}LISTST;

int ListLen=sizeof(LISTST);
/*结构体大小*/

/*在结尾添加一个结点(已完成)*/
void AddEnd(LISTST *First,char *text)
{
 LISTST *to=First,*one;
 while(to->next!=NULL)
 to=to->next;

 one=malloc(ListLen);
 memset(one,NULL,ListLen);
 one->text=malloc(strlen(text));
 strcpy(one->text,text);
 one->last=to;
 one->next=NULL;

 to->next=one;

}

/*在第n个结点后，插入1个结点(已完成)*/
void AddOne(LISTST *Head,char *text,int n)
{
 LISTST *to=Head,*one;

 int e=NULL;
/*e的值与插入结点对应关系:
NULL不在结尾后，1在结尾后*/

 for(int i=1;i<=n;i++)
 if(to->next!=NULL)
 to=to->next;
 else {e=1;break;}

 if(e==NULL)
 /*如在不在结尾后，直接插入*/
 {
 one=malloc(ListLen);
 memset(one,NULL,ListLen);
 one->text=malloc(strlen(text));
 strcpy(one->text,text);
 one->last=to;
 one->next=to->next;

  to->next->last=one;
 to->next=one;
 }
 else/*在结尾后*/
 AddEnd(Head,text);
}

/*正序绘列表(已完成)*/
void drawList(LISTST *Head)
{
 LISTST *to=Head;

 int x=0,y=0,h=30;
 uint8 r=255,g=155,b=255;

 while(to!=NULL)
 {
 drawtext(to->text,x,y,r,g,b);
 to=to->next;
 y+=h;
 }

}

/*反序绘列表(已完成)*/
void drawList2(LISTST *End)
{
 LISTST *to=End;
 int x=SCRW/2,y=0,h=30;
uint8 r=200,g=200,b=0;
 while(to!=NULL)
 {
 drawtext(to->text,x,y,r,g,b);
 to=to->last;
 y+=h;
 }

}

/*删除最后1个结点(已完成)*/
void DelEnd(LISTST *End)
{
 LISTST *one=End;

 if(End->last!=NULL)
 /*如果不是只有1个结点*/
 {
 one=one->last;
 free(one->next->text);
 one->next=NULL;
 }
 else
 if(End->last==NULL)
 /*只有一个结点*/ 
 free(one->text);
 End=one; 
}

/*删除第n个结点(已完成)*/
void DelOne(LISTST *Head,int n)
{
 LISTST *to=Head;

 int e=NULL;
/*e的值，对应结点的位置。
NULL中间 -1开头 1结尾
*/
 for(int i=1;i<=n;i++)
 if(to->next!=NULL)
 to=to->next;
 else {e=1;break;}

 if(e==1)DelEnd(to);
 else
 if(i==2)DelEnd(Head,1);
 else if(i!=2)
 {
 
 to->last->next=to->next;
 to->next->last=to->last;

 free(to->text);
 }

}


/*删除开头结尾外所有结点(已完成)*/
void DelAllList(LISTST *First,LISTST *End)
{
 LISTST *to=End;
 if(End->last==NULL)
 {
 while(to->last->last!=NULL)
  {
  free(to->text);
  free(to->next);
  to=to->last;
  }
 free(First->text);
 free(End->text);
 First->next=End; 
 End->last=First;
 }

}

/*入口函数*/
int init()
{
 LISTST *First,*End;
 /*定义两个结点，First为链表开头，End结尾*/

 First=malloc(ListLen);
 memset(First,NULL,ListLen);
 End=malloc(ListLen);
 memset(End,NULL,ListLen);


 AddEnd(First,"双向链表");
 AddEnd(First,"仅供学习");
 AddEnd(First,"贵在坚持");
 AddEnd(First,"这个多余");

 AddOne(First,"by:天天Tt",1);
 /*插入1个结点*/

 End=First;
 while(End->next!=NULL)
 End=End->next;
 /*使End为最后一个结点*/

 DelEnd(End);
 /*删除最后1个结点*/
 
 cls(0,0,0);
 drawList(First->next);
 drawList2(End->last);
 ref(0,0,SCRW,SCRH);


 DelAllList(First,End);
 free(First);
 free(End);

 drawCopy(0,SCRH/2,SCRW,SCRH/2);
/*文字倒影*/

 return 0;
}

/*按键入口函数*/
int event(int type, int p1, int p2)
{
 if(KY_DOWN==type)
 {
  switch(p1)
  {
  case _SRIGHT:
  exit();break;
  case _1:
  break;
   }
  }
 return 0;
}

/*应用暂停时调用函数*/
int pause()
{return 0;}

/*应用恢复是调用此函数*/
int resume()
{return 0;}

/*绘字*/
void drawtext(char *text,int x,int y,uint8 r,uint8 g,uint8 b)
{
  dtext(text,x,y,r,g,b,FALSE,1);
}

/*将屏幕范围的距形框里反向倒立，行成镜像效果*/
void drawCopy(int x,int y,int w,int h)
{
 uint16 *scr=getscrbuf()-(SCRH-(y+h))*2;
 int nn;
 for(int n=y;n<y+h;n++)
 {
 if(n>SCRH)break;
 nn++;
 memcpy(scr+n*SCRW+x,scr+(n-nn*2)*SCRW+x,w*2);
 ref(x,y,w,h);
 }

}

//by:天天Tt
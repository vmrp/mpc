#include <base.h>
#define FILE "c/天天/lrc/1.lrc"
uint32 f;

int init(void)
{
 void *txt, *str,*str2;
 uint32 Head,len=getlen(FILE);
 txt=malloc(len);
 Head=open(FILE,1);
 read(Head,txt,len);
 close(f);

 str=gb_uni(strrchr(FILE,'/')+1,0);

 str2=gb_uni(txt,0);
 free(txt);

 f=textcreate(str,str2,,1);

 return 0;
}

int event(int type, int p1, int p2)
{
 textdel(f);exit();

 if(KY_DOWN == type)
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

int pause(void)
{
 return 0;
}

int resume(void)
{
 return 0;
}

/*
  1 返回unicode的指针
  0 gb2312
返回 指向返回字符串的指针
*/
void* gb_uni(char* str,int is_uni)
{
int err,b,a=wstrlen(str);
 if(is_uni)
 {
 u2c(str,a,&str,&a);
 return str;
 }
else return c2u(str,&err,&b);
}
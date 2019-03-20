#include <base.h>

int init()
{
 int a=5,b=6;
 printf("%d,%d 最大公约数:%d\n",a,b,max(a,b));
 printf("%d,%d 最小公倍数:%d",a,b,min(a,b));

 return 0;
}

int event(int type, int p1, int p2)
{
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
int max(int a,int b)
{
 if(a<b)
 {int t=a;a=b;b=t;}
 for(int k=b;k>=1;k--)
 if(a%k==0&&b%k==0)
 return k;
}

int min(int a,int b)
{
 if(a<b)
 {int t=a;a=b;b=t;}
 for(int k=b;k>=1;k--)
 if(a%k==0&&b%k==0)
 return a*b/k;
}
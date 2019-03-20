#include <base.h>

int init()
{
 uint32 a=0,b=5,k=1;
 uint32 n=0,i,j,h,t,u;

 uint32 *c=malloc(400);//分子
 memset(c,NULL,400);
 uint32 *d=malloc(400);//分母
 memset(d,NULL,400);

 for(j=a;j<=b;j++)
 for(i=1;i<j;i++)
 {
  t=0;
  for(u=2;u<=j;u++)
  if(j%u==0&&i%u==0)
  {t=1;break;}
  if(t==0)
  {c[n]=i;d[n]=j;n++;}
 }

//冒泡排序
 for(i=0;i<=n;i++)
 for(j=0;j<n-i;j++)
 if(c[j]*d[j+1]>c[j-1]*d[j])
 {
 h=c[j];c[j]=c[j+1];c[j+1]=h;
 h=d[j];d[j]=d[j+1];d[j+1]=h;
 }

 for(i=0;i<n;i++)
 printf("%d/%d ",c[i],d[i]);
 printf("\nn=%d\n,第%d项为%d/%d\n",n,k,c[k],d[k]);

 free(c);
 free(d);

 exit();
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
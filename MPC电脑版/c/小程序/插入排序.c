/****
≤Â»Î≈≈–Ú 
by:‘æ»ª
2012.5.9

****/

#include <base.h>
#define N 10

int init(void)
{
 int a[N],i,temp,t=(N-1);
 int ii,b=rand()%256;

 for(i=0;i<N;i++)
 printf("%d-",a[i]=rand()%250);
printf("\n√∞≈›≈≈–Ú:\n");

for(i=0;i<N;i++)
for(ii=0;ii<(N-i);ii++)
if(a[ii]<a[ii+1])
{ temp=a[ii];
 a[ii]=a[ii+1];
 a[ii+1]=temp;
}
for(i=0;i<N;i++)
printf("%d-",a[i]);

printf("\n%d≤Â»Î\n",b);

for(i=N-1;i>0;i--)
if(b>a[i])
{ a[i]=a[i-1];
  t--;
}
 if(t != N-1)a[t+1]=b;

 for(i=0;i<N;i++)
 printf("%d-",a[i]);

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
            exit();
            break;
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
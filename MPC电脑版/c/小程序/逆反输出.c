/**
将一个数组逆反赋值，输出
by:跃然
QQ:643552878
2012.5,9
**/

#include <base.h>
#define N 10
//数组长度
int init()
{
 int a[N];
 int i,t,te;
 
 for(i=0;i<N;i++)
 printf("%d-",a[i]=rand()%255);
 printf("\n逆反输出\n");

 if(N%2==0)t=N/2-1;
 else t=(N-1)/2-1;

 for(i=0;i<=t;i++)
{ te=a[i];
 a[i]=a[N-i-1];
 a[N-i-1]=te;
}
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
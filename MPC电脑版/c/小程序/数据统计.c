/*
数据统计
by:跃然
QQ:643552878
2012.5.12
*/

#include <base.h>
#define S 100 
//随机数的范围0-S

#define m 100
//数据的个数
#define n1 10
#define n2 20
//要统计数的范围n1~n2(不包括n2)

int init()
{
 int s1[m],s2[n2],i;

 for(i=0;i<m;i++)
{ 
 //产生随机数
 printf("%d-",s1[i]=rand()%S);
 if((i+1)%5==0)printf("\n");
 //每5个换行

 if(s1[i]<n2)
 { if(s1[i]>=n1)
  s2[s1[i]]++;
 }
 //统计关键代码！


}

 printf("\n数据统计:\n");


//输出统计结果
 for(i=n1;i<n2;i++)
{
 printf("_%d-%d个_",i,s2[i]);
 if((i+1)%3==0)
 printf("\n");
 //每3换行
}
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
{return 0;}
int resume(void)
{return 0;}
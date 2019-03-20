#include <base.h>

int init(void)
{
int p[3];

ba(p);

printf("%d%d%d\n",p[0],p[1],p[2]);

int *pp=p,i=-1;
while(i<2)
{i++;
printf("%d",*(pp+i));
}
exit();
 return 0;
}

void ba(int *p)
{
p[0]=5;
p[1]=2;
p[2]=0;
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
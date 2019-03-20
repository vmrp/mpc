#include <base.h>
int f=timercreate();
int y=0;

int init(void)
{
line(0);

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
        case _5:
star();break;  break;
        }
    }

else if(type==KY_UP&&p1==_5)
off();break;

    return 0;
}

int pause(void)
{return 0;}
int resume(void)
{return 0;}


void line(int data)
{
int i;if(y<16)y++;
else y=0;
for(i=0;i<=320;i+=20)
drect(0,i,240,20,i,y,i);
drect(0,y*20,240,20,38,38,50);
ref(0,0,SCRW,SCRH);

}
void star()
{
f=timercreate();
timerstart(f,300,500,"line",1);
}

void off()
{
if(f)
{timerdel(f);
f=NULL;}

}
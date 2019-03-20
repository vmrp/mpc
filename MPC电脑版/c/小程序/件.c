#include <base.h>

int init()
{
 uint16 *scr=getscrbuf();
 int16 h;

 for(uint8 i;i<2;i++)
 for(h=SCRH*2;h>0;h=h/2)
 {
 memset(scr,NULL,SCRW*SCRH*2);
 int y=(i==0)?SCRH-h:h;
 drect(SCRW/2,y,5,5,40,220,40);
 ref(0,0,SCRW,SCRH);
}

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
init();            break;
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
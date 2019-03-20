#include <base.h>


int event(int type, int p1, int p2)
{
    if(KY_DOWN == type)
    {
        switch(p1)
        {
        case _SRIGHT:
            exit();
            break;
        }
    }

    return 0;
}

int init(void)
{
    char str[255];

    sprintf(str,"ЪЃ%d,зм%d",getmemremain(),getsysmem());
    shake(2000);
    cls(0,0,0);
    dtext(str,0,0,255,255,0,0,1);
    ref(0,0,SCRW,SCRH);
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

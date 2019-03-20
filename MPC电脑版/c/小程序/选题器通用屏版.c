#include <base.h>

int i,y,a;
char aa='A';
char str[15];

int init(void)
{
cls(0,0,0);
drect(0,295,240,25,55,55,55);

dtext("ÍË³ö",180,295,255,255,0,0,1);
dtext("²úÉú",0,295,0,255,255,0,1);
sand(getuptime());
for(y=0;y<=270;y+=30)
{drect(0,y,240,22,0,y,0);


a=rand()%4;
i=rand()%4;
sprintf(str,"%d¡¢%c    %d¡¢%c",y/22+1,aa+i,y/22+SCRH/22,aa+a);
dtext(str,0,y,255,255,255,0,1);

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
        case _SLEFT:
cls(0,0,0);
init();
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
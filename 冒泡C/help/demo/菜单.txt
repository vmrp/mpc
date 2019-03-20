#include <base.h>

int menuf;

int init(void)
{
    menuf=menucreate("\x83\xdc\x53\x55\x68\x7\x98\x98\x0\x0",3);
    printf("f=%d\r\n",menuf);
    menuset(menuf,"\x7b\x2c\x4e\x0\x0\x0",0);
    menuset(menuf,"\x7b\x2c\x4e\x8c\x0\x0",1);
    menuset(menuf,"\x7b\x2c\x4e\x9\x0\x0",2);
    menushow(menuf);
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
    if(type == MN_SLT)
    {
        printf("选择的菜单是：%d\r\n",p1);
    }
    if(type == MN_RET)
    {
        menudel(menuf);
        cls(0,0,0);
        dtext("菜单退出",0,0,0xff,0xff,0xff,0,1);
        ref(0,0,SCRW,SCRH);
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


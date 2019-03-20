#include <base.h>

int dialogf;

void show(void)
{
    dtext("按1输入",0,100,0xff,0xff,0xff,FALSE,1);
    ref(0,0,SCRW,SCRH);
}
int init(void)
{
    cls(0,0,0);
    show();
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
            dialogf=editcreate("\x8f\xd9\x66\x2f\x68\x7\x98\x98\x0\x0","\x0\x0",0,255);
            break;
        }
    }
    if(type == MR_DIALOG)
    {
        char *text;
        char *ptr;

        switch(p1)
        {
        case DLG_OK:
            ptr=editget(dialogf);
            text=malloc(512);
            memset(text,0,512);
            memcpy(text,ptr,wstrlen(ptr));
            editdel(dialogf);
            cls(0,0,0);
            dtext("刚才输入:",0,0,0xff,0xff,0xff,FALSE,1);
            dtext(text,0,30,0xff,0xff,0xff,TRUE,1);
            break;
        case DLG_CANCEL:
            editdel(dialogf);
            cls(0,0,0);
            dtext("编辑框退出",0,0,0xff,0xff,0xff,FALSE,1);
            break;
        }
        show();
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

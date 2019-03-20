#include <base.h>

int32 dialogf;

int event(int type, int p1, int p2)
{
    if(KY_DOWN == type)
    {
        switch(p1)
        {
        case _1:
            exit();
            break;
        case _2:
            find();
            break;
        case _3:
            dialogf=textcreate("\x8f\xd9\x66\x2f\x68\x7\x98\x98\0\0","\x8f\xd9\x66\x2f\x51\x85\x5b\xb9\0\0",1);
            break;
        default:
            break;
        }
    }
    if(type == MR_DIALOG)//对话框消息
    {
        char *text;

        textdel(dialogf);
        cls(0,0,0);
        switch(p1)
        {
        case DLG_OK:
            text="按下确定";
            break;
        case DLG_CANCEL:
            text="按下取消";
            break;
        }
        dtext(text,0,0,255,255,0,0,1);
        ref(0,0,SCRW,SCRH);
    }


    return 0;
}

int init(void)
{
    int32 start,i;
    cls(0, 0, 0);
    dtext("万次累加计算",0,0,255,255,0,0,1);
    ref(0, 0, SCRW, SCRH);

    start=getuptime();
    for(i=0 ; i<10000 ; i++);
    start= getuptime()-start;
    printf("万次%dms\r\n",start);

    cls(0, 0, 0);
    dtext("1退出2文件查找3对话框",0,0,255,255,0,0,1);
    ref(0, 0, SCRW, SCRH);



    return 0;
}

int pause(void)
{
    printf("pause\r\n");
    return 0;
}

int resume(void)
{
    printf("resume\r\n");
    return 0;

}
void find(void)
{
    int32 f;
    char buffer[255];
    int i;

    f=findstart("",buffer,255);
    printf("\r\nget \"%s\",f=%d\r\n",buffer,f);
    for(i=0; i<30; i++)
    {
        int ret =findnext(f,buffer,255);
        printf("get \"%s\",ret=%d\r\n",buffer,ret);

    }
    printf("\r\nfindstop ret=%d\r\n",findstop(f));

}
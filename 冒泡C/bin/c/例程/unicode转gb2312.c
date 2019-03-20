#include <base.h>
//UNICODE文本
#define UNI "\x7b\x2c\x4e\x0\x88\x4c\x67\x2b\x5c\x3e\x67\x9\x56\xde\x8f\x66\x0\xa\x7b\x2c\x4e\x8c\x88\x4c\x67\x2b\x5c\x3e\x6c\xa1\x67\x9\x56\xde\x8f\x66\x63\x62\x88\x4c\xff\xc\x95\x7f\x65\x87\x67\x2c\x81\xea\x52\xa8\x63\x62\x88\x4c\x0\x0"

int init(void)
{
    int output_len=100;
    char *output=malloc(output_len);
    rectst rect;//矩形区域
    colorst color;//文本颜色

    rect.x=10;
    rect.y=20;
    rect.w=100;
    rect.h=150;
    color.r=255;
    color.g=255;
    color.b=255;

    u2c(UNI,wstrlen(UNI),&output,&output_len);//转换

    cls(0,0,0);
    dtextex(output,rect.x,rect.y,&rect,&color,2,1);
    ref(0,0,SCRW,SCRH);
    free(output);
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
            printf("path=%s\r\n",getparentpath());
            break;
        case _2:
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

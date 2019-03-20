#include <base.h>


#define BMPW 176
#define BMPH 153
char *filename="帮助/demo/res/color.bmp";

int init(void)
{
    int32 f;
    int32 len;
    uint16 *ptr;

    len=getlen(filename);//取得文件长度
    f=open(filename,1);//以只读方式打开文件
    ptr=malloc(len);//申请图片存放的内存
    read(f,ptr,len);//把图片读取到内存中
    close(f);//关闭文件

    cls(0,0,0);
    bmpshowflip(ptr,0,0,BMPW,BMPW,BMPH,BM_COPY,0,0,0);//画图片
    ref(0,0,SCRW,SCRH);

    free(ptr);



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
{
    return 0;
}

int resume(void)
{
    return 0;
}

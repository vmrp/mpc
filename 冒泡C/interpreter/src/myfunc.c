#include "mrc_base.h"
#include "mrc_bmp.h"
#include "mrc_exb.h"
#include "mrc_image.h"
#include "mrc_base_i.h"
#include "myfunc.h"
#include "mrc_sound.h"



/*
void ShadeRect(int x, int y, int w, int h, int32 pixelA,int32 pixelB, int mode)
{
#define PIXEL888RED(pixel)      (uint8)(((pixel) >> 16) & 0xff)
#define PIXEL888GREEN(pixel)      (uint8)(((pixel) >> 8) & 0xff)
#define PIXEL888BLUE(pixel)      (uint8)((pixel) & 0xff)

    int16 i,j,t;
    int AR,AG,AB,BR,BG,BB;

    AR=PIXEL888RED(pixelA);
    AB=PIXEL888BLUE(pixelA);
    AG=PIXEL888GREEN(pixelA);

    BR=PIXEL888RED(pixelB);
    BB=PIXEL888BLUE(pixelB);
    BG=PIXEL888GREEN(pixelB);

    BR-=AR; BG-=AG; BB-=AB;
    switch(mode)
    {
    case SHADE_UPDOWN:
        t=x+w-1;
        for(i=0;     i<h;     i++)
            mrc_drawLine(x, y+i, t, y+i, (uint8)(AR+BR*i/h),(uint8)(AG+BG*i/h),(uint8)(AB+BB*i/h));
        return;
    case SHADE_DOWNUP:
        t=x+w-1;
        for(i=h-1,j=0;    i>=0;    i--,j++)
            mrc_drawLine(x, y+i, t, y+i, (uint8)(AR+BR*j/h),(uint8)(AG+BG*j/h),(uint8)(AB+BB*j/h));
        return;
    case SHADE_LEFTRIGHT:
        t=y+h-1;
        for(i=0;     i<w;     i++)
            mrc_drawLine(x+i, y, x+i, t, (uint8)(AR+BR*i/w),(uint8)(AG+BG*i/w),(uint8)(AB+BB*i/w));
        return;
    case SHADE_RIGHTLEFT:
        t=y+h-1;
        for(i=w-1,j=0;    i>=0;    i--,j++)
            mrc_drawLine(x+i, y, x+i, t, (uint8)(AR+BR*j/w),(uint8)(AG+BG*j/w),(uint8)(AB+BB*j/w));
        return;
    }
}

int DrawIMG(char* filename, int x,int y)
{
    PMRAPP_IMAGE_WH p;
    MRAPP_IMAGE_ORIGIN_T t_imgsizeInfo;
    T_DRAW_DIRECT_REQ input;

    //获取图片尺寸信息
    t_imgsizeInfo.src = (char*)filename;
    t_imgsizeInfo.len = mrc_strlen(filename);
    t_imgsizeInfo.src_type = SRC_NAME;
    mrc_getImageInfo(&t_imgsizeInfo, (uint8**)&p);

    //设置传到移植层得参数
    input.src_type = 0;
    input.ox = x;
    input.oy = y;
    input.w = p->width;
    input.h = p->height;
    input.src = (char*)filename;
    input.src_len = mrc_strlen(filename);

    return mrc_platEx(3010, (uint8*)&input, sizeof(T_DRAW_DIRECT_REQ), NULL, NULL, NULL);
}
*/
int32 my_readAllEx(char* filename, char* buf, int32 *len)
{
    int32 ret,filelen,f,oldlen;

    ret = mrc_fileState(filename);
    if((ret != MR_IS_FILE))
        return MR_FAILED;

    filelen = mrc_getLen(filename);
    if (filelen <= 0)
        return MR_FAILED;
    filelen = (filelen > *len)? *len:filelen;

    f = mrc_open(filename, MR_FILE_RDONLY );
    if (f == 0)
        return MR_FAILED;

    oldlen = 0;
    while(oldlen < filelen){
        ret = mrc_read(f, (char*)buf+oldlen, filelen-oldlen);
        if (ret <= 0)
        {
            mrc_close(f);
            return MR_FAILED;
        }
        oldlen = oldlen + ret;
    }
    mrc_close(f);
    *len = filelen;
    return MR_SUCCESS;
}

#include <base.h>
#include "sound.h"

//请将要播放的mp3改名为1.mp3并放到平台文件下再运行
#define FILENAME "1.mp3"

#define _MP3 3//mp3设备（类型值参考sound.h中的enum）

int init(void)
{
    cls(0,0,0);
    if(soundinit(_MP3))
    {
        dtext("不支持",0,0,255,255,255,FALSE,1);
    }
    else
    {
        dtext("正在播放",0,0,255,255,255,FALSE,1);
        soundloadfile(_MP3,FILENAME);
        soundplay(_MP3,0,FALSE);
    }
    ref(0,0,SCRW,SCRH);
    return 0;
}

int event(int type, int p1, int p2)
{
    if(KY_DOWN == type)
    {
        switch(p1)
        {
        case _SRIGHT:
            exitapp();
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

int exitapp(void)
{
    soundstop(_MP3);
    soundclose(_MP3);
    exit();
    return 0;
}
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

typedef struct
{
    uint16 year;                 //年
    uint8  month;                //月
    uint8  day;                  //日
    uint8  hour;                 //时，24小时制
    uint8  minute;               //分
    uint8  second;               //秒
}mr_datetime;

int init(void)
{
    mr_datetime date;

    if(!getdatetime(&date))
        printf("获取成功,%d年%d月%d日%d时%d分%d秒",date.year,date.month,date.day,date.hour,date.minute,date.second);
    else
        printf("获取失败");
    cls(0,0,0);
    dtext("操作完成",0,0,255,255,0,0,1);
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

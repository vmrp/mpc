#ifndef __INI_H__
#define  __INI_H__
extern "C"
{
	#include "mrporting.h"	
}
typedef enum
{
	INI_MEMORY,
	MAX_INI_EXTERN_ID
}T_INI_EXTERN_ID;

typedef enum
{
	INI_KEY_0,               //按键 0
	INI_KEY_1,               //按键 1
	INI_KEY_2,               //按键 2
	INI_KEY_3,               //按键 3
	INI_KEY_4,               //按键 4
	INI_KEY_5,               //按键 5
	INI_KEY_6,               //按键 6
	INI_KEY_7,               //按键 7
	INI_KEY_8,               //按键 8
	INI_KEY_9,               //按键 9
	INI_KEY_STAR,            //按键 *
	INI_KEY_POUND,           //按键 #
	INI_KEY_UP,              //按键 上
	INI_KEY_DOWN,            //按键 下
	INI_KEY_LEFT,            //按键 左
	INI_KEY_RIGHT,           //按键 右
	INI_KEY_POWER,           //按键 挂机键
	INI_KEY_SOFTLEFT,        //按键 左软键
	INI_KEY_SOFTRIGHT,       //按键 右软键
	INI_KEY_SEND,            //按键 接听键
	INI_KEY_SELECT,          //按键 确认/选择（若方向键中间有确认键，建议设为该键）
	INI_KEY_VOLUME_UP,          //按键 侧键上
	INI_KEY_VOLUME_DOWN,          //按键 侧键下
	INI_KEY_CLEAR,
	INI_KEY_NONE,             //按键 保留
	INI_128_XO,
    INI_128_X1,
	INI_128_YO,
	INI_128_Y1,
	INI_176_XO,
    INI_176_X1,
	INI_176_YO,
	INI_176_Y1,
	INI_240_XO,
    INI_240_X1,
	INI_240_YO,
	INI_240_Y1,
	MAX_INI_INSIDE_ID
}T_INI_INSIDE_ID;


typedef struct
{
	char *string;
}T_INI_DATA;

typedef struct  {
	int x0;
	int x1;
	int y0;
	int y1;
}COORDINATE;

void iniInit(void);
int32 getSreenSize(void);
char *getInsideString(int32 index);

COORDINATE *getScreenSite(void);


#endif




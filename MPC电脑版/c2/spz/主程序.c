#include "c2/spz/print.h"
#include "c2/spz/xcpfr.h"
#include "c2/spz/edit.h"
char *st;
int qt;

int init(void)
{
cls(50,200,100);
refall();
rst.x=30;
print("计算器
制作人:翎瞑
Q:九五六七零四三四九",3000);
rst.x=20;
print("亲，请仔细阅读以下声明",1000);
print("我已将编辑框数字模式下*键改于运算符，更便于输入表达式，支持简单的四则运算与左扩号，右左括…不支持小数，所以除法会取整",5000);
print("是不是觉得我狠luosuo啊？哈哈，烦死你们，快了，咱不急…",2000);
print("对了，事先说声，这个只是给大家看看，别期望有什么实际价值",2500);
startedit(14,140,200,"Welcome");
return 0;
}

int event(int type, int p1, int p2)
{
    if(KY_DOWN == type)
    {
play(p1);
        switch(p1)
        {
        case _SLEFT:
if(qt==0)jg();
else if(qt==1){qt=0;
startedit(14,140,200,"Welcome");}
            break;
case _SRIGHT:
if(qt==1)exitApp();
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

void jg()
{
char b[30];
st=endedit();
intowrxcp(st);
rst.x=50;
sprintf(b,"%s=%d",st,gbxcp());
print(b,2000);
print("对了没？继续？",2000);
rst.x=0;
print("是                    否",2000);
free(st);
qt=1;
}

int exitApp()
{
rst.x=50;
print("累死我了.......",3500);
if(st!=NULL)free(st);
if(ob!=NULL)free(ob);
exit();
return 0;
}
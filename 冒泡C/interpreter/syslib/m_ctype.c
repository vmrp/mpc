#include "interpreter.h"


void StdIsalnum(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->Integer = isalnum(Param[0]->Val->Integer);
}

void StdIsalpha(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->Integer = isalpha(Param[0]->Val->Integer);
}

void StdIsdigit(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->Integer = isdigit(Param[0]->Val->Integer);
}
void StdIsprint(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    int ch = Param[0]->Val->Integer;
    ReturnValue->Val->Integer = (ch >= 0x20 && ch <= 0x7e);//ASCII可打印字符
}

void StdTolower(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->Integer = Param[0]->Val->Integer |= 0x20;//转换为小写字母
}

void StdToupper(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->Integer = Param[0]->Val->Integer &= 0xdf;//转换为大写字母
}


/* 所有的库函数和他们的原型列表 */
struct LibraryFunction Ctype_Functions[7];

void Ctype_SetupFunc(void)
{
    Ctype_Functions[0].Func = StdIsalnum;
    Ctype_Functions[0].Prototype = "int isalnum(int);";

    Ctype_Functions[1].Func = StdIsalpha;
    Ctype_Functions[1].Prototype = "int isalpha(int);";

    Ctype_Functions[2].Func = StdIsdigit;
    Ctype_Functions[2].Prototype = "int isdigit(int);";

    Ctype_Functions[3].Func = StdIsprint;
    Ctype_Functions[3].Prototype = "int isprint(int);";

    Ctype_Functions[4].Func = StdTolower;
    Ctype_Functions[4].Prototype = "int tolower(int);";

    Ctype_Functions[5].Func = StdToupper;
    Ctype_Functions[5].Prototype = "int toupper(int);";

    Ctype_Functions[6].Func = NULL;
    Ctype_Functions[6].Prototype = NULL;

}

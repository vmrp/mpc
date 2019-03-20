#include "picoc.h"
#include "interpreter.h"
#include "momo.h"

/* 版本字符串 */
// static char *VersionString;

/* 字节序检查 */
// static const int __ENDIAN_CHECK__ = 1;
// static int BigEndian = 0;
// static int LittleEndian = 0;
static int TRUEValue = 1;
static int ZeroValue = 0;
// uint16 SCREEN_WIDTH;
// uint16 SCREEN_HEIGHT;

/* 全局库初始化 */
void LibraryInit(void)
{
//     mr_screeninfo screen;
// 
//     mrc_getScreenInfo(&screen);
//     SCREEN_WIDTH = (uint16)screen.width;
//     SCREEN_HEIGHT = (uint16)screen.height;
    /* 定义版本号宏 */
//     VersionString = TableStrRegister("1.0");
//     VariableDefinePlatformVar(NULL, "_VERSION", CharPtrType, (union AnyValue *)&VersionString, FALSE);

    /* 定义字节序宏 */
//     BigEndian = ((*(char*)&__ENDIAN_CHECK__) == 0);
//     LittleEndian = ((*(char*)&__ENDIAN_CHECK__) == 1);
//     VariableDefinePlatformVar(NULL, "BIG_ENDIAN", &IntType, (union AnyValue *)&BigEndian, FALSE);
//     VariableDefinePlatformVar(NULL, "LITTLE_ENDIAN", &IntType, (union AnyValue *)&LittleEndian, FALSE);

    /* 定义一些常量 */
    VariableDefinePlatformVar(NULL, "NULL", &IntType, (union AnyValue *)&ZeroValue, FALSE);
    VariableDefinePlatformVar(NULL, "TRUE", &IntType, (union AnyValue *)&TRUEValue, FALSE);
    VariableDefinePlatformVar(NULL, "FALSE", &IntType, (union AnyValue *)&ZeroValue, FALSE);
    VariableDefinePlatformVar(NULL, "SCRW", &UnsignedShortType, (union AnyValue *)&SCREEN_WIDTH, FALSE);
    VariableDefinePlatformVar(NULL, "SCRH", &UnsignedShortType, (union AnyValue *)&SCREEN_HEIGHT, FALSE);
}

/* 添加库 */
void LibraryAdd(struct Table *GlobalTable, const char *LibraryName, struct LibraryFunction *FuncList)
{
    struct ParseState Parser;
    int Count;
    char *Identifier;
    struct ValueType *ReturnType;
    struct Value *NewValue;
    void *Tokens;
    const char *IntrinsicName = TableStrRegister(LibraryName);
    //const char *IntrinsicName = TableStrRegister("c library");

    /* 读取所有的库定义 */
    for (Count = 0; FuncList[Count].Prototype != NULL; Count++)
    {
        Tokens = LexAnalyse(IntrinsicName, FuncList[Count].Prototype, strlen((char *)FuncList[Count].Prototype), NULL);
        LexInitParser(&Parser, FuncList[Count].Prototype, Tokens, IntrinsicName, TRUE);
        TypeParse(&Parser, &ReturnType, &Identifier, NULL);
        NewValue = ParseFunctionDefinition(&Parser, ReturnType, Identifier);
        NewValue->Val->FuncDef.Intrinsic = FuncList[Count].Func;
        HeapFreeMem(Tokens);
    }
}

/* 打印类型字符串, 但不使用 printf/sprintf */
void PrintType(struct ValueType *Typ, struct OutputStream *Stream)
{
    switch (Typ->Base)
    {
        case TypeVoid:          PrintStr("void", Stream); break;
        case TypeInt:           PrintStr("int", Stream); break;
        case TypeShort:         PrintStr("short", Stream); break;
        case TypeChar:          PrintStr("char", Stream); break;
        case TypeLong:          PrintStr("long", Stream); break;
        case TypeUnsignedInt:   PrintStr("unsigned int", Stream); break;
        case TypeUnsignedShort: PrintStr("unsigned short", Stream); break;
        case TypeUnsignedLong:  PrintStr("unsigned long", Stream); break;
#ifndef NO_FP
        case TypeFP:            PrintStr("double", Stream); break;
#endif
        case TypeFunction:      PrintStr("function", Stream); break;
        case TypeMacro:         PrintStr("macro", Stream); break;
        case TypePointer:       if (Typ->FromType) PrintType(Typ->FromType, Stream); PrintCh('*', Stream); break;
        case TypeArray:         PrintType(Typ->FromType, Stream); PrintCh('[', Stream); if (Typ->ArraySize != 0) PrintSimpleInt(Typ->ArraySize, Stream); PrintCh(']', Stream); break;
        case TypeStruct:        PrintStr("struct", Stream); PrintStr(Typ->Identifier, Stream); break;
        case TypeUnion:         PrintStr("union", Stream); PrintStr(Typ->Identifier, Stream); break;
        case TypeEnum:          PrintStr("enum", Stream); PrintStr(Typ->Identifier, Stream); break;
        case TypeGotoLabel:     PrintStr("goto label", Stream); break;
        case Type_Type:         PrintStr("type", Stream); break;
    }
}

////////////////////////////////////////////////////////
struct OutputStream *CStdOut;
struct OutputStream CStdOutBase;


void BasicIOInit(void)
{
    CStdOutBase.PutchFunc = &PlatformPutc;
    CStdOut = &CStdOutBase;
}


/* 写入字符串流 */
void SPutc(unsigned char Ch, union OutputStreamInfo *Stream)
{
    struct StringOutputStream *Out = &Stream->Str;
    *Out->WritePos++ = Ch;
}

/* 打印字符，但不使用printf/sprintf */
void PrintCh(char OutCh, struct OutputStream *Stream)
{
    (*Stream->PutchFunc)(OutCh, &Stream->i);
}

/* 打印字符串，但不使用 printf/sprintf */
void PrintStr(const char *Str, struct OutputStream *Stream)
{
    while (*Str)
        PrintCh(*Str++, Stream);
}

/* 重复打印一个字符 */
void PrintRepeatedChar(char ShowChar, int Length, struct OutputStream *Stream)
{
    while (Length-- > 0)
        PrintCh(ShowChar, Stream);
}

/* 打印无符号整数流 但不使用 printf/sprintf */
void PrintUnsigned(unsigned long Num, unsigned int Base, int FieldWidth, int ZeroPad, int LeftJustify, struct OutputStream *Stream)
{
    char Result[33];
    int ResPos = sizeof(Result);

    Result[--ResPos] = '\0';
    if (Num == 0)
        Result[--ResPos] = '0';

    while (Num > 0)
    {
        unsigned long NextNum = Num / Base;
        unsigned long Digit = Num - NextNum * Base;
        if (Digit < 10)
            Result[--ResPos] = '0' + (char)Digit;
        else
            Result[--ResPos] = 'a' + (char)(Digit - 10);

        Num = NextNum;
    }

    if (FieldWidth > 0 && !LeftJustify)
        PrintRepeatedChar(ZeroPad ? '0' : ' ', FieldWidth - (sizeof(Result) - 1 - ResPos), Stream);

    PrintStr(&Result[ResPos], Stream);

    if (FieldWidth > 0 && LeftJustify)
        PrintRepeatedChar(' ', FieldWidth - (sizeof(Result) - 1 - ResPos), Stream);
}

/* 打印简单integer类型到流 但不使用 printf/sprintf */
void PrintSimpleInt(long Num, struct OutputStream *Stream)
{
    PrintInt(Num, -1, FALSE, FALSE, Stream);
}

/* 打印integer类型到流 但不使用 printf/sprintf */
void PrintInt(long Num, int FieldWidth, int ZeroPad, int LeftJustify, struct OutputStream *Stream)
{
    if (Num < 0)
    {
        PrintCh('-', Stream);
        Num = -Num;
        if (FieldWidth != 0)
            FieldWidth--;
    }

    PrintUnsigned((unsigned long)Num, 10, FieldWidth, ZeroPad, LeftJustify, Stream);
}

#ifndef NO_FP
/* print a double to a stream without using printf/sprintf */
void PrintFP(double Num, struct OutputStream *Stream)
{
    int Exponent = 0;
    int MaxDecimal;
    
    if (Num < 0)
    {
        PrintCh('-', Stream);
        Num = -Num;    
    }
    
    if (Num >= 1e7)
        Exponent = log10(Num);
    else if (Num <= 1e-7 && Num != 0.0)
        Exponent = log10(Num) - 0.999999999;
    
    Num /= pow(10.0, Exponent);    
    PrintInt((long)Num, 0, FALSE, FALSE, Stream);
    PrintCh('.', Stream);
    Num = (Num - (long)Num) * 10;
    if (abs(Num) >= 1e-7)
    {
        for (MaxDecimal = 6; MaxDecimal > 0 && abs(Num) >= 1e-7; Num = (Num - (long)(Num + 1e-7)) * 10, MaxDecimal--)
            PrintCh('0' + (long)(Num + 1e-7), Stream);
    }
    else
        PrintCh('0', Stream);
        
    if (Exponent != 0)
    {
        PrintCh('e', Stream);
        PrintInt(Exponent, 0, FALSE, FALSE, Stream);
    }
}
#endif

/* intrinsic functions made available to the language */
void GenericPrintf(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs, struct OutputStream *Stream)
{
    char *FPos;
    struct Value *NextArg = Param[0];
    struct ValueType *FormatType;
    int ArgCount = 1;
    int LeftJustify = FALSE;
    int ZeroPad = FALSE;
    int FieldWidth = 0;
    char *Format = Param[0]->Val->Pointer;

    for (FPos = Format;    *FPos;    FPos++)
    {
        if (*FPos == '%')//以"%"开头的串,解析
        {
            FPos++;
            if (*FPos == '-')
            {
                /* 结果左对齐,右边填空格 */
                LeftJustify = TRUE;
                FPos++;
            }

            if (*FPos == '0')
            {
                /* 一个十进制数 */
                ZeroPad = TRUE;
                FPos++;
            }

            /* 得到任何格式的字段宽度 */
            while (isdigit((int)*FPos))
                FieldWidth = FieldWidth * 10 + (*FPos++ - '0');

            /* 现在检查的格式类型 */
            switch (*FPos)
            {
            case 's': 
                FormatType = CharPtrType;
                break;
            case 'd': case 'u': case 'x': case 'b': case 'c': 
                FormatType = &IntType; 
                break;
#ifndef NO_FP
            case 'f':
                FormatType = &FPType; 
                break;
#endif
            case '%': 
                PrintCh('%', Stream); 
                FormatType = NULL; 
                break;
            case '\0': 
                FPos--; 
                FormatType = NULL; 
                break;
            default:  
                PrintCh(*FPos, Stream); 
                FormatType = NULL; 
                break;
            }

            if (FormatType != NULL)
            { 
                /* 我们已经格式化的东西 */
                if (ArgCount >= NumArgs)
                    PrintStr("XXX", Stream);   /* 格式不足够的参数 */
                else
                {
                    NextArg = (struct Value *)((char *)NextArg + MEM_ALIGN(sizeof(struct Value) + TypeStackSizeValue(NextArg)));
                    if (NextArg->Typ != FormatType && 
                        !((FormatType == &IntType || *FPos == 'f') && IS_NUMERIC_COERCIBLE(NextArg)) &&
                        !(FormatType == CharPtrType && (NextArg->Typ->Base == TypePointer || 
                        (NextArg->Typ->Base == TypeArray && NextArg->Typ->FromType->Base == TypeChar) ) ) )
                        PrintStr("XXX", Stream);   /* 无效格式 */
                    else
                    {
                        switch (*FPos)
                        {
                        case 's':
                            {
                                char *Str;

                                if (NextArg->Typ->Base == TypePointer)
                                    Str = NextArg->Val->Pointer;
                                else
                                    Str = &NextArg->Val->ArrayMem[0];

                                if (Str == NULL)
                                    PrintStr("NULL", Stream); 
                                else
                                    PrintStr(Str, Stream); 
                                break;
                            }
                        case 'd': PrintInt(ExpressionCoerceInteger(NextArg), FieldWidth, ZeroPad, LeftJustify, Stream); break;
                        case 'u': PrintUnsigned(ExpressionCoerceUnsignedInteger(NextArg), 10, FieldWidth, ZeroPad, LeftJustify, Stream); break;
                        case 'x': PrintUnsigned(ExpressionCoerceUnsignedInteger(NextArg), 16, FieldWidth, ZeroPad, LeftJustify, Stream); break;
                        case 'b': PrintUnsigned(ExpressionCoerceUnsignedInteger(NextArg), 2, FieldWidth, ZeroPad, LeftJustify, Stream); break;
                        case 'c': PrintCh((char)ExpressionCoerceUnsignedInteger(NextArg), Stream); break;
#ifndef NO_FP
                        case 'f': PrintFP(ExpressionCoerceFP(NextArg), Stream); break;
#endif
                        }
                    }
                }

                ArgCount++;
            }
        }
        else//不是以"%"开头的串直接输出
            PrintCh(*FPos, Stream);
    }
}

/* printf(): 打印到控制台输出 */
void LibPrintf(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    struct OutputStream ConsoleStream;
    
    ConsoleStream.PutchFunc = &PlatformPutc;
    GenericPrintf(Parser, ReturnValue, Param, NumArgs, &ConsoleStream);
}

void PlatformSPrintfPutc(unsigned char OutCh, union OutputStreamInfo *Stream)
{
    *Stream->Str.WritePos++=OutCh;
}

/* sprintf(): 打印到字符串 */
void LibSPrintf(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    struct OutputStream StrStream;
    
    StrStream.PutchFunc = &SPutc;
    StrStream.i.Str.Parser = Parser;
    StrStream.i.Str.WritePos = Param[0]->Val->Pointer;

    GenericPrintf(Parser, ReturnValue, Param+1, NumArgs-1, &StrStream);
    PrintCh(0, &StrStream);
    ReturnValue->Val->Pointer = Param[0]->Val->Pointer;
}
#ifndef NO_FP
/* 打印double类型到流 但不使用 printf/sprintf */
void PrintFP(double Num, struct OutputStream *Stream)
{
    int Exponent = 0;
    int MaxDecimal;

    if (Num < 0)
    {
        PrintCh('-', Stream);
        Num = -Num;    
    }

    if (Num >= 1e7)
        Exponent = log10(Num);
    else if (Num <= 1e-7 && Num != 0.0)
        Exponent = log10(Num) - 0.999999999;

    Num /= pow(10.0, Exponent);    
    PrintInt((long)Num, 0, FALSE, FALSE, Stream);
    PrintCh('.', Stream);
    Num = (Num - (long)Num) * 10;
    if (abs(Num) >= 1e-7)
    {
        for (MaxDecimal = 6; MaxDecimal > 0 && abs(Num) >= 1e-7; Num = (Num - (long)(Num + 1e-7)) * 10, MaxDecimal--)
            PrintCh('0' + (long)(Num + 1e-7), Stream);
    }
    else
        PrintCh('0', Stream);

    if (Exponent != 0)
    {
        PrintCh('e', Stream);
        PrintInt(Exponent, 0, FALSE, FALSE, Stream);
    }
}
#endif

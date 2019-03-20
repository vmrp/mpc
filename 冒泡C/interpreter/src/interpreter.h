#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "platform.h"


#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif

#ifndef NULL
#define NULL 0
#endif

#ifndef min
#define min(x,y) (((x)<(y))?(x):(y))
#endif

#define MEM_ALIGN(x) (((x) + sizeof(ALIGN_TYPE) - 1) & ~(sizeof(ALIGN_TYPE)-1))

#define GETS_BUF_MAX 256

/* 强制定义其他数值、类型 */
#ifndef NO_FP
#define IS_FP(v) ((v)->Typ->Base == TypeFP)
#define FP_VAL(v) ((v)->Val->FP)
#else
#define IS_FP(v) 0
#define FP_VAL(v) 0
#endif

#define isalpha(c) (((c) >= 'a' && (c) <= 'z') || ((c) >= 'A' && (c) <= 'Z'))
#define isdigit(c) ((c) >= '0' && (c) <= '9')
#define isalnum(c) (isalpha(c) || isdigit(c))
#define isspace(c) ((c) == ' ' || (c) == '\t' || (c) == '\r' || (c) == '\n')

#define IS_POINTER_COERCIBLE(v, ap) ((ap) ? ((v)->Typ->Base == TypePointer) : 0)
#define POINTER_COERCE(v) ((int)(v)->Val->Pointer)

#define IS_INTEGER_NUMERIC_TYPE(t) ((t)->Base >= TypeInt && (t)->Base <= TypeUnsignedLong)
#define IS_INTEGER_NUMERIC(v) IS_INTEGER_NUMERIC_TYPE((v)->Typ)
#define IS_NUMERIC_COERCIBLE(v) (IS_INTEGER_NUMERIC(v) || IS_FP(v))
#define IS_NUMERIC_COERCIBLE_PLUS_POINTERS(v,ap) (IS_NUMERIC_COERCIBLE(v) || IS_POINTER_COERCIBLE(v,ap))

extern uint16 SCREEN_WIDTH;
extern uint16 SCREEN_HEIGHT;

struct Table;

/* 词法标记 */
enum LexToken
{
    /* 0x00 */ TokenNone, 
    /* 0x01 */ TokenComma,
    /* 0x02 */ TokenAssign, TokenAddAssign, TokenSubtractAssign, TokenMultiplyAssign, TokenDivideAssign, TokenModulusAssign,
    /* 0x08 */ TokenShiftLeftAssign, TokenShiftRightAssign, TokenArithmeticAndAssign, TokenArithmeticOrAssign, TokenArithmeticExorAssign,
    /* 0x0d */ TokenQuestionMark, TokenColon, 
    /* 0x0f */ TokenLogicalOr, 
    /* 0x10 */ TokenLogicalAnd, 
    /* 0x11 */ TokenArithmeticOr, 
    /* 0x12 */ TokenArithmeticExor, 
    /* 0x13 */ TokenAmpersand, 
    /* 0x14 */ TokenEqual, TokenNotEqual, 
    /* 0x16 */ TokenLessThan, TokenGreaterThan, TokenLessEqual, TokenGreaterEqual,
    /* 0x1a */ TokenShiftLeft, TokenShiftRight, 
    /* 0x1c */ TokenPlus, TokenMinus, 
    /* 0x1e */ TokenAsterisk, TokenSlash, TokenModulus,
    /* 0x21 */ TokenIncrement, TokenDecrement, TokenUnaryNot, TokenUnaryExor, TokenSizeof, TokenCast,
    /* 0x27 */ TokenLeftSquareBracket, TokenRightSquareBracket, TokenDot, TokenArrow, 
    /* 0x2b */ TokenOpenBracket, TokenCloseBracket,
    /* 0x2d */ TokenIdentifier, TokenIntegerConstant, TokenFPConstant, TokenStringConstant, TokenCharacterConstant,
    /* 0x32 */ TokenSemicolon, TokenEllipsis,
    /* 0x34 */ TokenLeftBrace, TokenRightBrace,
    /* 0x36 */ TokenIntType, TokenCharType, TokenFloatType, TokenDoubleType, TokenVoidType, TokenEnumType,
    /* 0x3c */ TokenLongType, TokenSignedType, TokenShortType, TokenStaticType, TokenAutoType, TokenRegisterType, TokenExternType, TokenStructType, TokenUnionType, TokenUnsignedType, TokenTypedef,
    /* 0x46 */ TokenContinue, TokenDo, TokenElse, TokenFor, TokenGoto, TokenIf, TokenWhile, TokenBreak, TokenSwitch, TokenCase, TokenDefault, TokenReturn,
    /* 0x52 */ TokenHashDefine, TokenHashInclude, TokenHashIf, TokenHashIfdef, TokenHashIfndef, TokenHashElse, TokenHashEndif,
    /* 0x59 */ TokenNew, TokenDelete,
    /* 0x5b */ TokenOpenMacroBracket,
    /* 0x5c */ TokenEOF, TokenEndOfLine, TokenEndOfFunction
};

/* 用于动态内存分配 */
struct AllocNode
{
    unsigned int Size;
    struct AllocNode *NextFree;
};

/* 是否正在运行或跳过代码 */
enum RunMode
{
    RunModeRun,                 /* 解析我们正在运行的代码 */
    RunModeSkip,                /* 跳过代码，没有运行 */
    RunModeReturn,              /* 从函数返回 */
    RunModeCaseSearch,          /* 寻找一个case标签 */
    RunModeBreak,               /* 跳出 switch/while/do */
    RunModeContinue,            /* 重复循环 */
    RunModeGoto                 /* 寻找一个goto标签 */
};

/* 解析器的状态 - 有了这一细节，这样我们就可以嵌套解析文件 */
struct ParseState
{
    const unsigned char *Pos;
    const char *FileName;
    short int Line;
    short int CharacterPos;
    enum RunMode Mode;          /* 是否跳过或运行代码 */
    int SearchLabel;            /* 我们正在寻找什么情况下标签 */
    const char *SearchGotoLabel;/* 我们正在寻找什么GOTO标签 */
    short int HashIfLevel;
    short int HashIfEvaluateToLevel;
    const char *SourceText;
};

/* 变量 */
enum BaseType
{
    TypeVoid,                   /* void */
    TypeInt,                    /* integer */
    TypeShort,                  /* short integer */
    TypeChar,                   /* char (unsigned) */
    TypeLong,                   /* long integer */
    TypeUnsignedInt,            /* unsigned integer */
    TypeUnsignedShort,          /* unsigned short integer */
    TypeUnsignedLong,           /* unsigned long integer */
#ifndef NO_FP
    TypeFP,                     /* 浮点指针 */
#endif
    TypeFunction,               /* 函数 */
    TypeMacro,                  /* 宏 */
    TypePointer,                /* 指针 */
    TypeArray,                  /* 数组类型 */
    TypeStruct,                 /* 结构 */
    TypeUnion,                  /* 共用类型 */
    TypeEnum,                   /* 枚举整数 */
    TypeGotoLabel,              /* goto语句的标签 */
    Type_Type                   /* 存储类型的类型 */
};

/* 数据类型 */
struct ValueType
{
    enum BaseType Base;             /* 这是什么样的类型 */
    int ArraySize;                  /* 数组类型的大小 */
    int Sizeof;                     /* 存储要求 */
    int AlignBytes;                 /* 这种类型的对齐边界 */
    const char *Identifier;         /* 结构或联合的名称 */
    struct ValueType *FromType;     /* 起始类型 (或 NULL) */
    struct ValueType *DerivedTypeList;  /* 在起始类型列表的第一项 */
    struct ValueType *Next;         /* 在起始类型列表的下一项 */
    struct Table *Members;          /* 结构或联合的成员 */
    int OnHeap;                     /* true，如果在堆上分配 */
    int StaticQualifier;            /* true，如果它是一个静态类型 */
};

/* 函数类型定义*/
struct FuncDef
{
    struct ValueType *ReturnType;   /* 返回值类型 */
    int NumParams;                  /* 参数的数量 */
    int VarArgs;                    /* 明确指定一个可变数目的参数 */
    struct ValueType **ParamType;   /* 参数类型的数组 */
    char **ParamName;               /* 参数名称的数组 */
    void (*Intrinsic)();            /* 内部调用地址或NULL */
    struct ParseState Body;         /* 词法标记的函数体，如果没有部函数 */
};

/* 宏类型定义 */
struct MacroDef
{
    int NumParams;                  /* 参数的数量 */
    char **ParamName;               /* 参数名称的数组 */
    struct ParseState Body;         /* 词法标记的函数体，如果没有部函数 */
};

/* values */
union AnyValue
{
    unsigned char Character;
    short ShortInteger;
    int Integer;
    long LongInteger;
    unsigned short UnsignedShortInteger;
    unsigned int UnsignedInteger;
    unsigned long UnsignedLongInteger;
    char *Identifier;
    char ArrayMem[2];               /* 数据开始的占位符，它不指向 */
    struct ValueType *Typ;
    struct FuncDef FuncDef;
    struct MacroDef MacroDef;
#ifndef NO_FP
    double FP;
#endif
    void *Pointer;                  /* unsafe native pointers */
};

struct Value
{
    struct ValueType *Typ;          /* 这个变量的类型 */
    union AnyValue *Val;            /* 保存实际内容的AnyValue指针 */
    struct Value *LValueFrom;       /* 如果是一个左值，这是一个左值的变量（或NULL） */
    char ValOnHeap;                 /* the AnyValue is on the heap (but this Value is on the stack) */
    char ValOnStack;                /* the AnyValue is on the stack along with this Value */
    char IsLValue;                  /* 是否可修改和分配的某个地方，我们可以有效地修改他 */
};

/* hash table data structure */
struct TableEntry
{
    struct TableEntry *Next;        /* next item in this hash chain */
    const char *DeclFileName;       /* where the variable was declared */
    unsigned short DeclLine;
    unsigned short DeclColumn;

    union TableEntryPayload
    {
        struct ValueEntry
        {
            char *Key;              /* points to the shared string table */
            struct Value *Val;      /* the value we're storing */
        } v;                        /* used for tables of values */
        
        char Key[1];                /* dummy size - used for the shared string table */
    } p;
};
    
struct Table
{
    short Size;
    short OnHeap;
    struct TableEntry **HashTable;
};

/* stack frame for function calls */
struct StackFrame
{
    struct ParseState ReturnParser;         /* how we got here */
    const char *FuncName;                   /* the name of the function we're in */
    struct Value *ReturnValue;              /* copy the return value here */
    struct Value **Parameter;               /* array of parameter values */
    int NumParams;                          /* the number of parameters */
    struct Table LocalTable;                /* the local variables and parameters */
    struct TableEntry *LocalHashTable[LOCAL_TABLE_SIZE];
    struct StackFrame *PreviousStackFrame;  /* the next lower stack frame */
};

/* lexer state */
enum LexMode
{
    LexModeNormal,
    LexModeHashInclude,
    LexModeHashDefine,
    LexModeHashDefineSpace,
    LexModeHashDefineSpaceIdent
};

struct LexState
{
    const char *Pos;
    const char *End;
    const char *FileName;
    int Line;
    int CharacterPos;
    const char *SourceText;
    enum LexMode Mode;
    int EmitExtraNewlines;
};

/* 库函数定义 */
struct LibraryFunction
{
    void (*Func)(struct ParseState *Parser, struct Value *, struct Value **, int);
    char *Prototype;
};

/* 输出流类型的特定状态信息 */
union OutputStreamInfo
{
    struct StringOutputStream
    {
        struct ParseState *Parser;
        char *WritePos;
    } Str;
};
typedef void CharWriter(unsigned char, union OutputStreamInfo *);
/* 打印输出字符串时使用 - printf() */
struct OutputStream
{
    CharWriter *PutchFunc;
    union OutputStreamInfo i;
};
/* 解析一份声明中可能的结果 */
enum ParseResult { ParseResultEOF, ParseResultError, ParseResultOk };

/* 全局变量 */
extern void *HeapStackTop;
extern struct Table GlobalTable;
extern struct StackFrame *TopStackFrame;
extern struct ValueType UberType;
extern struct ValueType IntType;
extern struct ValueType CharType;
extern struct ValueType UnsignedShortType;

#ifndef NO_FP
extern struct ValueType FPType;
#endif
extern struct ValueType VoidType;
extern struct ValueType TypeType;
extern struct ValueType FunctionType;
extern struct ValueType MacroType;
extern struct ValueType GotoLabelType;
extern struct ValueType *CharPtrType;
extern struct ValueType *CharPtrPtrType;
extern struct ValueType *CharArrayType;
extern struct ValueType *VoidPtrType;
extern char *StrEmpty;
extern struct PointerValue NULLPointer;//
extern struct OutputStream *CStdOut;

/* table.c */
void TableInit(void);
char *TableStrRegister(const char *Str);
char *TableStrRegister2(const char *Str, int Len);
void TableInitTable(struct Table *Tbl, struct TableEntry **HashTable, int Size, int OnHeap);
int TableSet(struct Table *Tbl, char *Key, struct Value *Val, const char *DeclFileName, int DeclLine, int DeclColumn);
int TableGet(struct Table *Tbl, const char *Key, struct Value **Val, const char **DeclFileName, int *DeclLine, int *DeclColumn);
struct Value *TableDelete(struct Table *Tbl, const char *Key);
char *TableSetIdentifier(struct Table *Tbl, const char *Ident, int IdentLen);
void TableStrFree(void);

/* lex.c */
void LexInit(void);
void LexCleanup(void);
void *LexAnalyse(const char *FileName, const char *Source, int SourceLen, int *TokenLen);
void LexInitParser(struct ParseState *Parser, const char *SourceText, void *TokenSource, const char *FileName, int RunIt);
enum LexToken LexGetToken(struct ParseState *Parser, struct Value **Value, int IncPos);
enum LexToken LexRawPeekToken(struct ParseState *Parser);
void LexToEndOfLine(struct ParseState *Parser);
void *LexCopyTokens(struct ParseState *StartParser, struct ParseState *EndParser);
void LexInteractiveClear(struct ParseState *Parser);
void LexInteractiveCompleted(struct ParseState *Parser);
void LexInteractiveStatementPrompt(void);

/* parse.c */
/* 以下是定义在 picoc.h:
 * void PicocParse(const char *FileName, const char *Source, int SourceLen, int RunIt, int CleanupNow, int CleanupSource);
 */
enum ParseResult ParseStatement(struct ParseState *Parser, int CheckTrailingSemicolon);
struct Value *ParseFunctionDefinition(struct ParseState *Parser, struct ValueType *ReturnType, char *Identifier);
void ParseCleanup(void);
void ParserCopyPos(struct ParseState *To, struct ParseState *From);
void ParserCopy(struct ParseState *To, struct ParseState *From);

/* expression.c */
int ExpressionParse(struct ParseState *Parser, struct Value **Result);
long ExpressionParseInt(struct ParseState *Parser);
void ExpressionAssign(struct ParseState *Parser, struct Value *DestValue, struct Value *SourceValue, int Force, const char *FuncName, int ParamNo, int AllowPointerCoercion);
long ExpressionCoerceInteger(struct Value *Val);
unsigned long ExpressionCoerceUnsignedInteger(struct Value *Val);
#ifndef NO_FP
double ExpressionCoerceFP(struct Value *Val);
#endif

/* type.c */
void TypeInit(void);
void TypeCleanup(void);
int TypeSize(struct ValueType *Typ, int ArraySize, int Compact);
int TypeSizeValue(struct Value *Val, int Compact);
int TypeStackSizeValue(struct Value *Val);
int TypeLastAccessibleOffset(struct Value *Val);
int TypeParseFront(struct ParseState *Parser, struct ValueType **Typ, int *IsStatic);
void TypeParseIdentPart(struct ParseState *Parser, struct ValueType *BasicTyp, struct ValueType **Typ, char **Identifier);
void TypeParse(struct ParseState *Parser, struct ValueType **Typ, char **Identifier, int *IsStatic);
struct ValueType *TypeGetMatching(struct ParseState *Parser, struct ValueType *ParentType, enum BaseType Base, int ArraySize, const char *Identifier, int AllowDuplicates);
struct ValueType *TypeCreateOpaqueStruct(struct ParseState *Parser, const char *StructName, int Size);

/* heap.c */
void HeapInit(int StackSize);
void HeapCleanup(void);
void *HeapAllocStack(int Size);
int HeapPopStack(void *Addr, int Size);
void HeapUnpopStack(int Size);
void HeapPushStackFrame(void);
int HeapPopStackFrame(void);
void *HeapAllocMem(int Size);
void HeapFreeMem(void *Mem);

/* variable.c */
void VariableInit(void);
void VariableCleanup(void);
void VariableFree(struct Value *Val);
void VariableTableCleanup(struct Table *HashTable);
void *VariableAlloc(struct ParseState *Parser, int Size, int OnHeap);
void VariableStackPop(struct ParseState *Parser, struct Value *Var);
struct Value *VariableAllocValueAndData(struct ParseState *Parser, int DataSize, int IsLValue, struct Value *LValueFrom, int OnHeap);
struct Value *VariableAllocValueAndCopy(struct ParseState *Parser, struct Value *FromValue, int OnHeap);
struct Value *VariableAllocValueFromType(struct ParseState *Parser, struct ValueType *Typ, int IsLValue, struct Value *LValueFrom, int OnHeap);
struct Value *VariableAllocValueFromExistingData(struct ParseState *Parser, struct ValueType *Typ, union AnyValue *FromValue, int IsLValue, struct Value *LValueFrom);
struct Value *VariableAllocValueShared(struct ParseState *Parser, struct Value *FromValue);
struct Value *VariableDefine(struct ParseState *Parser, char *Ident, struct Value *InitValue, struct ValueType *Typ, int MakeWritable);
struct Value *VariableDefineButIgnoreIdentical(struct ParseState *Parser, char *Ident, struct ValueType *Typ, int IsStatic, int *FirstVisit);
int VariableDefined(const char *Ident);
void VariableGet(struct ParseState *Parser, const char *Ident, struct Value **LVal);
void VariableDefinePlatformVar(struct ParseState *Parser, char *Ident, struct ValueType *Typ, union AnyValue *FromValue, int IsWritable);
void VariableStackFrameAdd(struct ParseState *Parser, const char *FuncName, int NumParams);
void VariableStackFramePop(struct ParseState *Parser);
struct Value *VariableStringLiteralGet(char *Ident);
void VariableStringLiteralDefine(char *Ident, struct Value *Val);
void *VariableDereferencePointer(struct ParseState *Parser, struct Value *PointerValue, struct Value **DerefVal, int *DerefOffset, struct ValueType **DerefType, int *DerefIsLValue);

/* clibrary.c */
void BasicIOInit(void);
void LibraryInit(void);
void LibraryAdd(struct Table *GlobalTable, const char *LibraryName, struct LibraryFunction *FuncList);
void PrintCh(char OutCh, struct OutputStream *Stream);
void PrintSimpleInt(long Num, struct OutputStream *Stream);
void PrintInt(long Num, int FieldWidth, int ZeroPad, int LeftJustify, struct OutputStream *Stream);
void PrintStr(const char *Str, struct OutputStream *Stream);
void PrintFP(double Num, struct OutputStream *Stream);
void PrintType(struct ValueType *Typ, struct OutputStream *Stream);
void LibPrintf(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs);
void LibSPrintf(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs);

/* platform.c */
/* 以下是定义在 picoc.h:
 * int PicocPlatformSetExitPoint();
 * void PicocInitialise(int StackSize);
 * void PicocCleanup();
 * void PicocPlatformScanFile(const char *FileName);
 * extern int FuncRet; */
void ProgramFail(struct ParseState *Parser, const char *Message, ...);
void AssignFail(struct ParseState *Parser, const char *Format, struct ValueType *Type1, struct ValueType *Type2, int Num1, int Num2, const char *FuncName, int ParamNo);
void LexFail(struct LexState *Lexer, const char *Message, ...);
void PlatformCleanup(void);
void PlatformInit(void);
int PlatformGetCharacter(void);
void PlatformPutc(unsigned char OutCh, union OutputStreamInfo *);
void PlatformErrorPrefix(struct ParseState *Parser);
void PlatformPrintf(const char *Format, ...);
void PlatformVPrintf(const char *Format, va_list Args);
void PlatformExit(int ExitVal);
char *PlatformMakeTempName(char *TempNameBuffer);
void fileFree(void);

/* include.c */
void IncludeInit(void);
void IncludeCleanup(void);
void IncludeRegister(const char *IncludeName, void (*SetupFunction)(void), struct LibraryFunction *FuncList, const char *SetupCSource);
void IncludeFile(char *Filename);
/* 以下是定义在 picoc.h:
 * void PicocIncludeAllSystemHeaders(); */

/* m_ctype.c */
extern struct LibraryFunction Ctype_Functions[];
void Ctype_SetupFunc(void);

//m_base.c
extern const char mrc_base_Defs[];
extern struct LibraryFunction mrc_base_Functions[];
void mrc_base_SetupFunc(void);

//m_sound.c
extern struct LibraryFunction Sound_Functions[];
void Sound_SetupFunc(void);

//m_net.c
extern struct LibraryFunction mrc_net_Functions[];
void mrc_net_SetupFunc(void);

#endif /* INTERPRETER_H */

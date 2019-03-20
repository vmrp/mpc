#include "interpreter.h"

/*  变量 临时复制最大尺寸 直到我们创建一个变量 */
#define MAX_TMP_COPY_BUF 256

/* 全局定义的表 */
struct Table GlobalTable;
struct TableEntry *GlobalHashTable[GLOBAL_TABLE_SIZE];

/* 字符串值表 */
struct Table StringLiteralTable;
struct TableEntry *StringLiteralHashTable[STRING_LITERAL_TABLE_SIZE];

/* 堆栈 */
struct StackFrame *TopStackFrame;


/* 初始化变量系统 */
void VariableInit(void)
{
    TableInitTable(&GlobalTable, &GlobalHashTable[0], GLOBAL_TABLE_SIZE, TRUE);
    TableInitTable(&StringLiteralTable, &StringLiteralHashTable[0], STRING_LITERAL_TABLE_SIZE, TRUE);
    TopStackFrame = NULL;
}

/* 释放一个变量的内容 */
void VariableFree(struct Value *Val)
{
    if (Val->ValOnHeap)
    {
        /* 释放函数体 */
        if (Val->Typ == &FunctionType && Val->Val->FuncDef.Intrinsic == NULL && Val->Val->FuncDef.Body.Pos != NULL)
            HeapFreeMem((void *)Val->Val->FuncDef.Body.Pos);

        /* 释放宏体 */
        if (Val->Typ == &MacroType)
            HeapFreeMem((void *)Val->Val->MacroDef.Body.Pos);

        /* 释放变量 */
        HeapFreeMem(Val);
    }
}

/* 释放全局表和字符串文字表 */
void VariableTableCleanup(struct Table *HashTable)
{
    struct TableEntry *Entry;
    struct TableEntry *NextEntry;
    int Count;
    
    for (Count = 0; Count < HashTable->Size; Count++)
    {
        for (Entry = HashTable->HashTable[Count]; Entry != NULL; Entry = NextEntry)
        {
            NextEntry = Entry->Next;
            VariableFree(Entry->p.v.Val);
                
            /* 释放哈希表入口 */
            HeapFreeMem(Entry);
        }
    }
}

void VariableCleanup(void)
{
    VariableTableCleanup(&GlobalTable);
    VariableTableCleanup(&StringLiteralTable);
}

/* 分配一些内存，无论是在堆或栈并检查是否已经用完 */
void *VariableAlloc(struct ParseState *Parser, int Size, int OnHeap)
{
    void *NewValue;
    
    if (OnHeap)
        NewValue = HeapAllocMem(Size);
    else
        NewValue = HeapAllocStack(Size);
    
    if (NewValue == NULL)
        ProgramFail(Parser, "内存不足");
    
#ifdef DEBUG_HEAP
    if (!OnHeap)
        printf("pushing %d at 0x%lx\n", Size, (unsigned long)NewValue);
#endif
        
    return NewValue;
}

/* 在堆或堆栈空间分配一个我们想要的类型的变量 */
struct Value *VariableAllocValueAndData(struct ParseState *Parser, int DataSize, int IsLValue, struct Value *LValueFrom, int OnHeap)
{
    struct Value *NewValue = VariableAlloc(Parser, MEM_ALIGN(sizeof(struct Value)) + DataSize, OnHeap);
    NewValue->Val = (union AnyValue *)((char *)NewValue + MEM_ALIGN(sizeof(struct Value)));
    NewValue->ValOnHeap = OnHeap;
    NewValue->ValOnStack = !OnHeap;
    NewValue->IsLValue = IsLValue;
    NewValue->LValueFrom = LValueFrom;
    
    return NewValue;
}

/* 分配一个变量并设置其类型 */
struct Value *VariableAllocValueFromType(struct ParseState *Parser, struct ValueType *Typ, int IsLValue, struct Value *LValueFrom, int OnHeap)
{
    int Size = TypeSize(Typ, Typ->ArraySize, FALSE);
    struct Value *NewValue = VariableAllocValueAndData(Parser, Size, IsLValue, LValueFrom, OnHeap);
    assert(Size > 0 || Typ == &VoidType);
    NewValue->Typ = Typ;
    
    return NewValue;
}

/* 堆或栈上分配一个变量，并复制它的值。处理重复数据 */
struct Value *VariableAllocValueAndCopy(struct ParseState *Parser, struct Value *FromValue, int OnHeap)
{
    struct ValueType *DType = FromValue->Typ;
    struct Value *NewValue;
    char TmpBuf[MAX_TMP_COPY_BUF];
    int CopySize = TypeSizeValue(FromValue, TRUE);

    assert(CopySize <= MAX_TMP_COPY_BUF);
    memcpy((void *)&TmpBuf[0], (void *)FromValue->Val, CopySize);
    NewValue = VariableAllocValueAndData(Parser, CopySize, FromValue->IsLValue, FromValue->LValueFrom, OnHeap);
    NewValue->Typ = DType;
    memcpy((void *)NewValue->Val, (void *)&TmpBuf[0], CopySize);
    
    return NewValue;
}

/* 从现有AnyValue和类型的堆或栈分配一个变量 */
struct Value *VariableAllocValueFromExistingData(struct ParseState *Parser, struct ValueType *Typ, union AnyValue *FromValue, int IsLValue, struct Value *LValueFrom)
{
    struct Value *NewValue = VariableAlloc(Parser, sizeof(struct Value), FALSE);
    NewValue->Typ = Typ;
    NewValue->Val = FromValue;
    NewValue->ValOnHeap = FALSE;
    NewValue->ValOnStack = FALSE;
    NewValue->IsLValue = IsLValue;
    NewValue->LValueFrom = LValueFrom;
    
    return NewValue;
}

/* 在堆或栈分配一个变量从现有的值，共享这个变量*/
struct Value *VariableAllocValueShared(struct ParseState *Parser, struct Value *FromValue)
{
    return VariableAllocValueFromExistingData(Parser, FromValue->Typ, FromValue->Val, FromValue->IsLValue, FromValue->IsLValue ? FromValue : NULL);
}

/* 定义一个变量。 IDENT必须进行登记 */
struct Value *VariableDefine(struct ParseState *Parser, char *Ident, struct Value *InitValue, struct ValueType *Typ, int MakeWritable)
{
    struct Value *AssignValue;
    
    if (InitValue != NULL)
        AssignValue = VariableAllocValueAndCopy(Parser, InitValue, TopStackFrame == NULL);
    else
        AssignValue = VariableAllocValueFromType(Parser, Typ, MakeWritable, NULL, TopStackFrame == NULL);
    
    AssignValue->IsLValue = MakeWritable;
        
    if (!TableSet((TopStackFrame == NULL) ? &GlobalTable : &TopStackFrame->LocalTable, Ident, AssignValue, Parser ? ((char *)Parser->FileName) : NULL, Parser ? Parser->Line : 0, Parser ? Parser->CharacterPos : 0))
        ProgramFail(Parser, "'%s'已定义", Ident);
    
    return AssignValue;
}

/* 定义一个变量。 IDENT必须进行登记。如果它是一个重复定义相同的声明，不抛出一个错误 */
struct Value *VariableDefineButIgnoreIdentical(struct ParseState *Parser, char *Ident, struct ValueType *Typ, int IsStatic, int *FirstVisit)
{
    struct Value *ExistingValue;
    const char *DeclFileName;
    int DeclLine;
    int DeclColumn;
    
    if (IsStatic)
    {
        char MangledName[LINEBUFFER_MAX];
        char *MNPos = &MangledName[0];
        char *MNEnd = &MangledName[LINEBUFFER_MAX-1];
        const char *RegisteredMangledName;
        
        /* make the mangled static name (避免使用 sprintf() 避免冲突) */
        memset((void *)&MangledName, '\0', sizeof(MangledName));
        *MNPos++ = '/';
        strncpy(MNPos, (char *)Parser->FileName, MNEnd - MNPos);
        MNPos += strlen(MNPos);
        
        if (TopStackFrame != NULL)
        {
            /* 我们在函数内 */
            if (MNEnd - MNPos > 0) *MNPos++ = '/';
            strncpy(MNPos, (char *)TopStackFrame->FuncName, MNEnd - MNPos);
            MNPos += strlen(MNPos);
        }
            
        if (MNEnd - MNPos > 0) *MNPos++ = '/';
        strncpy(MNPos, Ident, MNEnd - MNPos);
        RegisteredMangledName = TableStrRegister(MangledName);
        
        /* 这个静态变量是否已经定义? */
        if (!TableGet(&GlobalTable, RegisteredMangledName, &ExistingValue, &DeclFileName, &DeclLine, &DeclColumn))
        {
            /* define the mangled-named static variable store in the global scope */
            ExistingValue = VariableAllocValueFromType(Parser, Typ, TRUE, NULL, TRUE);
            TableSet(&GlobalTable, (char *)RegisteredMangledName, ExistingValue, (char *)Parser->FileName, Parser->Line, Parser->CharacterPos);
            *FirstVisit = TRUE;
        }

        /* static variable exists in the global scope - now make a mirroring variable in our own scope with the short name */
        VariableDefinePlatformVar(Parser, Ident, ExistingValue->Typ, ExistingValue->Val, TRUE);
        return ExistingValue;
    }
    else
    {
        if (Parser->Line != 0 && TableGet((TopStackFrame == NULL) ? &GlobalTable : &TopStackFrame->LocalTable, Ident, &ExistingValue, &DeclFileName, &DeclLine, &DeclColumn)
                && DeclFileName == Parser->FileName && DeclLine == Parser->Line && DeclColumn == Parser->CharacterPos)
            return ExistingValue;
        else
            return VariableDefine(Parser, Ident, NULL, Typ, TRUE);
    }
}

/* 检查一个变量是否有特定的名称定义。 IDENT必须进行登记 */
int VariableDefined(const char *Ident)
{
    struct Value *FoundValue;
    
    if (TopStackFrame == NULL || !TableGet(&TopStackFrame->LocalTable, Ident, &FoundValue, NULL, NULL, NULL))
    {
        if (!TableGet(&GlobalTable, Ident, &FoundValue, NULL, NULL, NULL))
            return FALSE;
    }

    return TRUE;
}

/* 获得一个变量的值。必须定义。 IDENT必须进行注册 */
void VariableGet(struct ParseState *Parser, const char *Ident, struct Value **LVal)
{
    if (TopStackFrame == NULL || !TableGet(&TopStackFrame->LocalTable, Ident, LVal, NULL, NULL, NULL))
    {
        if (!TableGet(&GlobalTable, Ident, LVal, NULL, NULL, NULL))
            ProgramFail(Parser, "'%s'未知", Ident);
    }
}

/* 定义一个全局变量与一个平台，全局共享。 IDENT将被注册 */
void VariableDefinePlatformVar(struct ParseState *Parser, char *Ident, struct ValueType *Typ, union AnyValue *FromValue, int IsWritable)
{
    struct Value *SomeValue = VariableAllocValueAndData(NULL, 0, IsWritable, NULL, TRUE);
    SomeValue->Typ = Typ;
    SomeValue->Val = FromValue;
    
    if (!TableSet((TopStackFrame == NULL) ? &GlobalTable : &TopStackFrame->LocalTable, TableStrRegister(Ident), SomeValue, Parser ? Parser->FileName : NULL, Parser ? Parser->Line : 0, Parser ? Parser->CharacterPos : 0))
        ProgramFail(Parser, "'%s'已定义", Ident);
}

/* 释放 和/或 弹出堆栈顶部的值。 var必须是堆栈顶部的值! */
void VariableStackPop(struct ParseState *Parser, struct Value *Var)
{
    int Success;
    
#ifdef DEBUG_HEAP
    if (Var->ValOnStack)
        printf("popping %ld at 0x%lx\n", (unsigned long)(sizeof(struct Value) + TypeSizeValue(Var, FALSE)), (unsigned long)Var);
#endif
        
    if (Var->ValOnHeap)
    { 
        if (Var->Val != NULL)
            HeapFreeMem(Var->Val);
            
        Success = HeapPopStack(Var, sizeof(struct Value));                       /* 从堆中释放 */
    }
    else if (Var->ValOnStack)
        Success = HeapPopStack(Var, sizeof(struct Value) + TypeSizeValue(Var, FALSE));  /* 从栈中释放 */
    else
        Success = HeapPopStack(Var, sizeof(struct Value));                       /* 值是不是我们的问题 */
        
    if (!Success)
        ProgramFail(Parser, "堆栈下溢");
}

/* 在做一个函数调用时，添加一个堆栈结构 */
void VariableStackFrameAdd(struct ParseState *Parser, const char *FuncName, int NumParams)
{
    struct StackFrame *NewFrame;
    
    HeapPushStackFrame();
    NewFrame = HeapAllocStack(sizeof(struct StackFrame) + sizeof(struct Value *) * NumParams);
    if (NewFrame == NULL)
        ProgramFail(Parser, "内存不足");
        
    ParserCopy(&NewFrame->ReturnParser, Parser);
    NewFrame->FuncName = FuncName;
    NewFrame->Parameter = (NumParams > 0) ? ((void *)((char *)NewFrame + sizeof(struct StackFrame))) : NULL;
    TableInitTable(&NewFrame->LocalTable, &NewFrame->LocalHashTable[0], LOCAL_TABLE_SIZE, FALSE);
    NewFrame->PreviousStackFrame = TopStackFrame;
    TopStackFrame = NewFrame;
}

/* 删除一个堆栈结构 */
void VariableStackFramePop(struct ParseState *Parser)
{
    if (TopStackFrame == NULL)
        ProgramFail(Parser, "栈空");
        
    ParserCopy(Parser, &TopStackFrame->ReturnParser);
    TopStackFrame = TopStackFrame->PreviousStackFrame;
    HeapPopStackFrame();
}

/* 得到一个字符串文字。假定IDENT是已经注册。 NULL如果没有找到 */
struct Value *VariableStringLiteralGet(char *Ident)
{
    struct Value *LVal = NULL;

    if (TableGet(&StringLiteralTable, Ident, &LVal, NULL, NULL, NULL))
        return LVal;
    else
        return NULL;
}

/* 定义一个字符串常量。假定已注册IDENT */
void VariableStringLiteralDefine(char *Ident, struct Value *Val)
{
    TableSet(&StringLiteralTable, Ident, Val, NULL, 0, 0);
}

/* 检查指针的有效性和使用反引用 */
void *VariableDereferencePointer(struct ParseState *Parser, struct Value *PointerValue, struct Value **DerefVal, int *DerefOffset, struct ValueType **DerefType, int *DerefIsLValue)
{
    if (DerefVal != NULL)
        *DerefVal = NULL;
        
    if (DerefType != NULL)
        *DerefType = PointerValue->Typ->FromType;
        
    if (DerefOffset != NULL)
        *DerefOffset = 0;
        
    if (DerefIsLValue != NULL)
        *DerefIsLValue = TRUE;

    return PointerValue->Val->Pointer;
}


#include "picoc.h"
#include "interpreter.h"

/* 大块的堆分配，我们将清理时，我们就完成了 */
struct CleanupTokenNode
{
    void *Tokens;
    const char *SourceText;
    struct CleanupTokenNode *Next;
};

static struct CleanupTokenNode *CleanupTokenList = NULL;


/* 释放所有内存 */
void ParseCleanup(void)
{
    while (CleanupTokenList != NULL)
    {
        struct CleanupTokenNode *Next = CleanupTokenList->Next;
        
        HeapFreeMem(CleanupTokenList->Tokens);
        if (CleanupTokenList->SourceText != NULL)
            HeapFreeMem((void *)CleanupTokenList->SourceText);
            
        HeapFreeMem(CleanupTokenList);
        CleanupTokenList = Next;
    }
}

/* 解析声明，但只运行它，如果条件为TRUE */
enum ParseResult ParseStatementMaybeRun(struct ParseState *Parser, int Condition, int CheckTrailingSemicolon)
{
    if (Parser->Mode != RunModeSkip && !Condition)
    {
        enum RunMode OldMode = Parser->Mode;
        int Result;
        Parser->Mode = RunModeSkip;
        Result = ParseStatement(Parser, CheckTrailingSemicolon);
        Parser->Mode = OldMode;
        return Result;
    }
    else
        return ParseStatement(Parser, CheckTrailingSemicolon);
}

/* 计数参数的个数，函数或宏 */
int ParseCountParams(struct ParseState *Parser)
{
    int ParamCount = 0;
    
    enum LexToken Token = LexGetToken(Parser, NULL, TRUE);
    if (Token != TokenCloseBracket && Token != TokenEOF)
    { 
        /* 计算参数的数目 */
        ParamCount++;
        while ((Token = LexGetToken(Parser, NULL, TRUE)) != TokenCloseBracket && Token != TokenEOF)
        { 
            if (Token == TokenComma)
                ParamCount++;
        } 
    }
    
    return ParamCount;
}

/* 解析函数的定义，并保存以供日后使用 */
struct Value *ParseFunctionDefinition(struct ParseState *Parser, struct ValueType *ReturnType, char *Identifier)
{
    struct ValueType *ParamType;
    char *ParamIdentifier;
    enum LexToken Token = TokenNone;
    struct ParseState ParamParser;
    struct Value *FuncValue;
    struct Value *OldFuncValue;
    struct ParseState FuncBody;
    int ParamCount = 0;

    if (TopStackFrame != NULL)
        ProgramFail(Parser, "不允许嵌套函数的定义");
        
    LexGetToken(Parser, NULL, TRUE);  /* 打开支架 */
    ParserCopy(&ParamParser, Parser);
    ParamCount = ParseCountParams(Parser);
    if (ParamCount > PARAMETER_MAX)
        ProgramFail(Parser, "参数太多");
    
    FuncValue = VariableAllocValueAndData(Parser, sizeof(struct FuncDef) + sizeof(struct ValueType *) * ParamCount + sizeof(const char *) * ParamCount, FALSE, NULL, TRUE);
    FuncValue->Typ = &FunctionType;
    FuncValue->Val->FuncDef.ReturnType = ReturnType;
    FuncValue->Val->FuncDef.NumParams = ParamCount;
    FuncValue->Val->FuncDef.VarArgs = FALSE;
    FuncValue->Val->FuncDef.ParamType = (struct ValueType **)((char *)FuncValue->Val + sizeof(struct FuncDef));
    FuncValue->Val->FuncDef.ParamName = (char **)((char *)FuncValue->Val->FuncDef.ParamType + sizeof(struct ValueType *) * ParamCount);
   
    for (ParamCount = 0; ParamCount < FuncValue->Val->FuncDef.NumParams; ParamCount++)
    { 
        /* 获取函数定义中的参数 */
        if (ParamCount == FuncValue->Val->FuncDef.NumParams-1 && LexGetToken(&ParamParser, NULL, FALSE) == TokenEllipsis)
        { 
            /* 省略号结束 */
            FuncValue->Val->FuncDef.NumParams--;
            FuncValue->Val->FuncDef.VarArgs = TRUE;
            break;
        }
        else
        { 
            /* 增加一个参数 */
            TypeParse(&ParamParser, &ParamType, &ParamIdentifier, NULL);
            if (ParamType->Base == TypeVoid)
            {
                /* 这不是在所有的实际参数 - 删除它 */
                ParamCount--;
                FuncValue->Val->FuncDef.NumParams--;
            }
            else
            {
                FuncValue->Val->FuncDef.ParamType[ParamCount] = ParamType;
                FuncValue->Val->FuncDef.ParamName[ParamCount] = ParamIdentifier;
            }
        }
        
        Token = LexGetToken(&ParamParser, NULL, TRUE);
        if (Token != TokenComma && ParamCount < FuncValue->Val->FuncDef.NumParams-1)
            ProgramFail(&ParamParser, "无效逗号");
    }
    
    if (FuncValue->Val->FuncDef.NumParams != 0 && Token != TokenCloseBracket && Token != TokenComma && Token != TokenEllipsis)
        ProgramFail(&ParamParser, "参数错误");
    
//     if (strcmp(Identifier, "main") == 0)
//     {
//         /* 以确保它是 int main() */
//         if ( FuncValue->Val->FuncDef.ReturnType != &IntType &&
//              FuncValue->Val->FuncDef.ReturnType != &VoidType )
//             ProgramFail(Parser, "main()应该返回int或void");
// 
//         if (FuncValue->Val->FuncDef.NumParams != 0 &&
//              (FuncValue->Val->FuncDef.NumParams != 2 || FuncValue->Val->FuncDef.ParamType[0] != &IntType) )
//             ProgramFail(Parser, "main()参数错误");
//     }
    
    /* 看看函数体 */
    Token = LexGetToken(Parser, NULL, FALSE);
    if (Token == TokenSemicolon)
        LexGetToken(Parser, NULL, TRUE);    /* 它的原型，吸收尾随分号 */
    else
    {
        /* 他是一个完整的函数定义体 */
        if (Token != TokenLeftBrace)
            ProgramFail(Parser, "无效函数定义");
        
        ParserCopy(&FuncBody, Parser);
        if (ParseStatementMaybeRun(Parser, FALSE, TRUE) != ParseResultOk)
            ProgramFail(Parser, "无效函数定义");

        FuncValue->Val->FuncDef.Body = FuncBody;
        FuncValue->Val->FuncDef.Body.Pos = LexCopyTokens(&FuncBody, Parser);

        /* 这个函数是否已经在全局表中? */
        if (TableGet(&GlobalTable, Identifier, &OldFuncValue, NULL, NULL, NULL))
        {
            if (OldFuncValue->Val->FuncDef.Body.Pos == NULL)
            {
                /* 不考虑旧的函数原型 */
                VariableFree(TableDelete(&GlobalTable, Identifier));
            }
            else
                ProgramFail(Parser, "'%s'已定义", Identifier);
        }
    }

    if (!TableSet(&GlobalTable, Identifier, FuncValue, (char *)Parser->FileName, Parser->Line, Parser->CharacterPos))
        ProgramFail(Parser, "'%s'已定义", Identifier);
        
    return FuncValue;
}

/* 分配初始值给一个变??量 */
void ParseDeclarationAssignment(struct ParseState *Parser, struct Value *NewVariable, int DoAssignment)
{
    struct Value *CValue;
    int ArrayIndex;
    enum LexToken Token = TokenComma;

    if (LexGetToken(Parser, NULL, FALSE) == TokenLeftBrace)
    {
        /* 这是一个数组初始化 */
        LexGetToken(Parser, NULL, TRUE);
        
        for (ArrayIndex = 0; (Parser->Mode != RunModeRun && Token == TokenComma) || (Parser->Mode == RunModeRun && ArrayIndex < NewVariable->Typ->ArraySize); ArrayIndex++)
        {
            struct Value *ArrayElement = NULL;
            
            if (Token != TokenComma)
                ProgramFail(Parser, "无效逗号");
                
            if (Parser->Mode == RunModeRun)
                ArrayElement = VariableAllocValueFromExistingData(Parser, NewVariable->Typ->FromType, (union AnyValue *)(&NewVariable->Val->ArrayMem[0] + TypeSize(NewVariable->Typ->FromType, 0, TRUE) * ArrayIndex), TRUE, NewVariable);
                
            if (!ExpressionParse(Parser, &CValue))
                ProgramFail(Parser, "无效表达式");
                
            if (Parser->Mode == RunModeRun && DoAssignment)
            {
                ExpressionAssign(Parser, ArrayElement, CValue, FALSE, NULL, 0, FALSE);
                VariableStackPop(Parser, CValue);
                VariableStackPop(Parser, ArrayElement);
            }
            
            Token = LexGetToken(Parser, NULL, TRUE);
        }
        
        if (Token == TokenComma)
            Token = LexGetToken(Parser, NULL, TRUE);

        if (Token != TokenRightBrace)
            ProgramFail(Parser, "'}'无效");
    }
    else
    {
        /* 这是一种常态的表达式初始化 */
        if (!ExpressionParse(Parser, &CValue))
            ProgramFail(Parser, "无效表达式");
            
        if (Parser->Mode == RunModeRun && DoAssignment)
        {
            ExpressionAssign(Parser, NewVariable, CValue, FALSE, NULL, 0, FALSE);
            VariableStackPop(Parser, CValue);
        }
    }
}

/* 声明一个变量或函数 */
int ParseDeclaration(struct ParseState *Parser, enum LexToken Token)
{
    char *Identifier;
    struct ValueType *BasicType;
    struct ValueType *Typ;
    struct Value *NewVariable = NULL;
    int IsStatic = FALSE;
    int FirstVisit = FALSE;

    TypeParseFront(Parser, &BasicType, &IsStatic);
    do
    {
        TypeParseIdentPart(Parser, BasicType, &Typ, &Identifier);
        if ((Token != TokenVoidType && Token != TokenStructType && Token != TokenUnionType && Token != TokenEnumType) && Identifier == StrEmpty)
            ProgramFail(Parser, "无效符号");
            
        if (Identifier != StrEmpty)
        {
            /* 处理函数的定义 */
            if (LexGetToken(Parser, NULL, FALSE) == TokenOpenBracket)
            {
                ParseFunctionDefinition(Parser, Typ, Identifier);
                return FALSE;
            }
            else
            {
                if (Typ == &VoidType && Identifier != StrEmpty)
                    ProgramFail(Parser, "不能定义void变量");
                    
                if (Parser->Mode == RunModeRun || Parser->Mode == RunModeGoto)
                    NewVariable = VariableDefineButIgnoreIdentical(Parser, Identifier, Typ, IsStatic, &FirstVisit);
                
                if (LexGetToken(Parser, NULL, FALSE) == TokenAssign)
                {
                    /* 我们分配一个初始值 */
                    LexGetToken(Parser, NULL, TRUE);
                    ParseDeclarationAssignment(Parser, NewVariable, !IsStatic || FirstVisit);
                }
            }
        }
        
        Token = LexGetToken(Parser, NULL, FALSE);
        if (Token == TokenComma)
            LexGetToken(Parser, NULL, TRUE);
            
    } while (Token == TokenComma);
    
    return TRUE;
}

/* 解析＃define宏定义并存储他以供日后使用 */
void ParseMacroDefinition(struct ParseState *Parser)
{
    struct Value *MacroName;
    char *MacroNameStr;
    struct Value *ParamName;
    struct Value *MacroValue;

    if (LexGetToken(Parser, &MacroName, TRUE) != TokenIdentifier)
        ProgramFail(Parser, "无效符号");
    
    MacroNameStr = MacroName->Val->Identifier;
    
    if (LexRawPeekToken(Parser) == TokenOpenMacroBracket)
    {
        /* 它是一个带参数的宏，读取参数 */
        enum LexToken Token = LexGetToken(Parser, NULL, TRUE);
        struct ParseState ParamParser;
        int NumParams;
        int ParamCount = 0;
        
        ParserCopy(&ParamParser, Parser);
        NumParams = ParseCountParams(&ParamParser);
        MacroValue = VariableAllocValueAndData(Parser, sizeof(struct MacroDef) + sizeof(const char *) * NumParams, FALSE, NULL, TRUE);
        MacroValue->Val->MacroDef.NumParams = NumParams;
        MacroValue->Val->MacroDef.ParamName = (char **)((char *)MacroValue->Val + sizeof(struct MacroDef));

        Token = LexGetToken(Parser, &ParamName, TRUE);
        
        while (Token == TokenIdentifier)
        {
            /* 存储参数名称 */
            MacroValue->Val->MacroDef.ParamName[ParamCount++] = ParamName->Val->Identifier;
            
            /* 得到后面的逗号 */
            Token = LexGetToken(Parser, NULL, TRUE);
            if (Token == TokenComma)
                Token = LexGetToken(Parser, &ParamName, TRUE);
                
            else if (Token != TokenCloseBracket)
                ProgramFail(Parser, "无效逗号");
        }
        
        if (Token != TokenCloseBracket)
            ProgramFail(Parser, "关闭括弧无效");
    }
    else
    {
        /* 分配一个简单的无参宏 */
        MacroValue = VariableAllocValueAndData(Parser, sizeof(struct MacroDef), FALSE, NULL, TRUE);
        MacroValue->Val->MacroDef.NumParams = 0;
    }
    
    /* 复制宏体执行后 */
    ParserCopy(&MacroValue->Val->MacroDef.Body, Parser);
    MacroValue->Typ = &MacroType;
    LexToEndOfLine(Parser);
    MacroValue->Val->MacroDef.Body.Pos = LexCopyTokens(&MacroValue->Val->MacroDef.Body, Parser);
    
    if (!TableSet(&GlobalTable, MacroNameStr, MacroValue, (char *)Parser->FileName, Parser->Line, Parser->CharacterPos))
        ProgramFail(Parser, "'%s'已定义", MacroNameStr);
}

/* 复制整个解析器的状态 */
void ParserCopy(struct ParseState *To, struct ParseState *From)
{
    memcpy((void *)To, (void *)From, sizeof(struct ParseState));
}

/* 复制我们在哪里解析 */
void ParserCopyPos(struct ParseState *To, struct ParseState *From)
{
    To->Pos = From->Pos;
    To->Line = From->Line;
    To->HashIfLevel = From->HashIfLevel;
    To->HashIfEvaluateToLevel = From->HashIfEvaluateToLevel;
    To->CharacterPos = From->CharacterPos;
}

/* 解析一个"FOR"声明 */
void ParseFor(struct ParseState *Parser)
{
    int Condition;
    struct ParseState PreConditional;
    struct ParseState PreIncrement;
    struct ParseState PreStatement;
    struct ParseState After;

    if (LexGetToken(Parser, NULL, TRUE) != TokenOpenBracket)
        ProgramFail(Parser, "'('无效");
                        
    if (ParseStatement(Parser, TRUE) != ParseResultOk)
        ProgramFail(Parser, "无效声明");
    
    ParserCopyPos(&PreConditional, Parser);
    if (LexGetToken(Parser, NULL, FALSE) == TokenSemicolon)
        Condition = TRUE;
    else
        Condition = ExpressionParseInt(Parser);
    
    if (LexGetToken(Parser, NULL, TRUE) != TokenSemicolon)
        ProgramFail(Parser, "';'无效");
    
    ParserCopyPos(&PreIncrement, Parser);
    ParseStatementMaybeRun(Parser, FALSE, FALSE);
    
    if (LexGetToken(Parser, NULL, TRUE) != TokenCloseBracket)
        ProgramFail(Parser, "')'无效");
    
    ParserCopyPos(&PreStatement, Parser);
    if (ParseStatementMaybeRun(Parser, Condition, TRUE) != ParseResultOk)
        ProgramFail(Parser, "无效声明");
    
    if (Parser->Mode == RunModeContinue)
        Parser->Mode = RunModeRun;
        
    ParserCopyPos(&After, Parser);
        
    while (Condition && Parser->Mode == RunModeRun)
    {
        ParserCopyPos(Parser, &PreIncrement);
        ParseStatement(Parser, FALSE);
                        
        ParserCopyPos(Parser, &PreConditional);
        if (LexGetToken(Parser, NULL, FALSE) == TokenSemicolon)
            Condition = TRUE;
        else
            Condition = ExpressionParseInt(Parser);
        
        if (Condition)
        {
            ParserCopyPos(Parser, &PreStatement);
            ParseStatement(Parser, TRUE);
            
            if (Parser->Mode == RunModeContinue)
                Parser->Mode = RunModeRun;                
        }
    }
    
    if (Parser->Mode == RunModeBreak)
        Parser->Mode = RunModeRun;
        
    ParserCopyPos(Parser, &After);
}

/* 解析一个代码块，并返回它返回模式 */
enum RunMode ParseBlock(struct ParseState *Parser, int AbsorbOpenBrace, int Condition)
{
    if (AbsorbOpenBrace && LexGetToken(Parser, NULL, TRUE) != TokenLeftBrace)
        ProgramFail(Parser, "'{'无效");
    
    if (Parser->Mode == RunModeSkip || !Condition)
    { 
        /* 失败条件 - 改为跳过此块 */
        enum RunMode OldMode = Parser->Mode;
        Parser->Mode = RunModeSkip;
        while (ParseStatement(Parser, TRUE) == ParseResultOk)
        {}
        Parser->Mode = OldMode;
    }
    else
    { 
        /* 只是在当前模式下运行 */
        while (ParseStatement(Parser, TRUE) == ParseResultOk)
        {}
    }
    
    if (LexGetToken(Parser, NULL, TRUE) != TokenRightBrace)
        ProgramFail(Parser, "'}'无效");
        
    return Parser->Mode;
}

/* 解析一个typedef声明 */
void ParseTypedef(struct ParseState *Parser)
{
    struct ValueType *Typ;
    struct ValueType **TypPtr;
    char *TypeName;
    struct Value InitValue;
    
    TypeParse(Parser, &Typ, &TypeName, NULL);
    
    if (Parser->Mode == RunModeRun)
    {
        TypPtr = &Typ;
        InitValue.Typ = &TypeType;
        InitValue.Val = (union AnyValue *)TypPtr;
        VariableDefine(Parser, TypeName, &InitValue, NULL, FALSE);
    }
}

/* 解析语句 */
enum ParseResult ParseStatement(struct ParseState *Parser, int CheckTrailingSemicolon)
{
    struct Value *CValue = NULL;
    struct Value *LexerValue = NULL;
    struct Value *VarValue = NULL;
    int Condition;
    struct ParseState PreState;
    enum LexToken Token;
    
    ParserCopy(&PreState, Parser);
    Token = LexGetToken(Parser, &LexerValue, TRUE);
    
    switch (Token)
    {
        case TokenEOF:
            return ParseResultEOF;
            
        case TokenIdentifier:
            /* 可能是一个typedef类型的变量声明，或者它可能是一个表达式*/
            if (VariableDefined(LexerValue->Val->Identifier))
            {
                VariableGet(Parser, LexerValue->Val->Identifier, &VarValue);
                if (VarValue->Typ->Base == Type_Type)
                {
                    *Parser = PreState;
                    ParseDeclaration(Parser, Token);
                    break;
                }
            }
            else
            {
                /* 它可能是一个goto标签 */
                enum LexToken NextToken = LexGetToken(Parser, NULL, FALSE);
                if (NextToken == TokenColon)
                {
                    /* 作为一个goto标签声明的符号 */
                    LexGetToken(Parser, NULL, TRUE);
                    if (Parser->Mode == RunModeGoto && LexerValue->Val->Identifier == Parser->SearchGotoLabel)
                        Parser->Mode = RunModeRun;
        
                    CheckTrailingSemicolon = FALSE;
                    break;
                }
            }
            /* 否则其它表达式 */
            
        case TokenAsterisk: 
        case TokenAmpersand: 
        case TokenIncrement: 
        case TokenDecrement: 
        case TokenOpenBracket: 
            *Parser = PreState;
            ExpressionParse(Parser, &CValue);
            if (Parser->Mode == RunModeRun) 
                VariableStackPop(Parser, CValue);
            break;
            
        case TokenLeftBrace:
            ParseBlock(Parser, FALSE, TRUE);
            CheckTrailingSemicolon = FALSE;
            break;
            
        case TokenIf:
            if (LexGetToken(Parser, NULL, TRUE) != TokenOpenBracket)
                ProgramFail(Parser, "'('无效");
                
            Condition = ExpressionParseInt(Parser);
            
            if (LexGetToken(Parser, NULL, TRUE) != TokenCloseBracket)
                ProgramFail(Parser, "')'无效");

            if (ParseStatementMaybeRun(Parser, Condition, TRUE) != ParseResultOk)
                ProgramFail(Parser, "无效声明");
            
            if (LexGetToken(Parser, NULL, FALSE) == TokenElse)
            {
                LexGetToken(Parser, NULL, TRUE);
                if (ParseStatementMaybeRun(Parser, !Condition, TRUE) != ParseResultOk)
                    ProgramFail(Parser, "无效声明");
            }
            CheckTrailingSemicolon = FALSE;
            break;
        
        case TokenWhile:
            {
                struct ParseState PreConditional;
                enum RunMode PreMode = Parser->Mode;

                if (LexGetToken(Parser, NULL, TRUE) != TokenOpenBracket)
                    ProgramFail(Parser, "'('无效");
                    
                ParserCopyPos(&PreConditional, Parser);
                do
                {
                    ParserCopyPos(Parser, &PreConditional);
                    Condition = ExpressionParseInt(Parser);
                    if (LexGetToken(Parser, NULL, TRUE) != TokenCloseBracket)
                        ProgramFail(Parser, "')'无效");
                    
                    if (ParseStatementMaybeRun(Parser, Condition, TRUE) != ParseResultOk)
                        ProgramFail(Parser, "无效声明");
                    
                    if (Parser->Mode == RunModeContinue)
                        Parser->Mode = PreMode;
                    
                } while (Parser->Mode == RunModeRun && Condition);
                
                if (Parser->Mode == RunModeBreak)
                    Parser->Mode = PreMode;

                CheckTrailingSemicolon = FALSE;
            }
            break;
                
        case TokenDo:
            {
                struct ParseState PreStatement;
                enum RunMode PreMode = Parser->Mode;
                ParserCopyPos(&PreStatement, Parser);
                do
                {
                    ParserCopyPos(Parser, &PreStatement);
                    if (ParseStatement(Parser, TRUE) != ParseResultOk)
                        ProgramFail(Parser, "无效声明");
                
                    if (Parser->Mode == RunModeContinue)
                        Parser->Mode = PreMode;

                    if (LexGetToken(Parser, NULL, TRUE) != TokenWhile)
                        ProgramFail(Parser, "'while'无效");
                    
                    if (LexGetToken(Parser, NULL, TRUE) != TokenOpenBracket)
                        ProgramFail(Parser, "'('无效");
                        
                    Condition = ExpressionParseInt(Parser);
                    if (LexGetToken(Parser, NULL, TRUE) != TokenCloseBracket)
                        ProgramFail(Parser, "')'无效");
                    
                } while (Condition && Parser->Mode == RunModeRun);           
                
                if (Parser->Mode == RunModeBreak)
                    Parser->Mode = PreMode;
            }
            break;
                
        case TokenFor:
            ParseFor(Parser);
            CheckTrailingSemicolon = FALSE;
            break;

        case TokenSemicolon: 
            CheckTrailingSemicolon = FALSE; 
            break;

        case TokenIntType:
        case TokenShortType:
        case TokenCharType:
        case TokenLongType:
        case TokenFloatType:
        case TokenDoubleType:
        case TokenVoidType:
        case TokenStructType:
        case TokenUnionType:
        case TokenEnumType:
        case TokenSignedType:
        case TokenUnsignedType:
        case TokenStaticType:
        case TokenAutoType:
        case TokenRegisterType:
        case TokenExternType:
            *Parser = PreState;
            CheckTrailingSemicolon = ParseDeclaration(Parser, Token);
            break;
        
        case TokenHashDefine:
            ParseMacroDefinition(Parser);
            CheckTrailingSemicolon = FALSE;
            break;
            
        case TokenHashInclude:
            if (LexGetToken(Parser, &LexerValue, TRUE) != TokenStringConstant)
                ProgramFail(Parser, "\"文件名.h\"无效");
            
            IncludeFile((char *)LexerValue->Val->Pointer);
            CheckTrailingSemicolon = FALSE;
            break;

        case TokenSwitch:
            if (LexGetToken(Parser, NULL, TRUE) != TokenOpenBracket)
                ProgramFail(Parser, "'('无效");
                
            Condition = ExpressionParseInt(Parser);
            
            if (LexGetToken(Parser, NULL, TRUE) != TokenCloseBracket)
                ProgramFail(Parser, "')'无效");
            
            if (LexGetToken(Parser, NULL, FALSE) != TokenLeftBrace)
                ProgramFail(Parser, "'{'无效");
            
            { 
                /* 新的块，所以，我们可以存储解析器的状态 */
                enum RunMode OldMode = Parser->Mode;
                int OldSearchLabel = Parser->SearchLabel;
                Parser->Mode = RunModeCaseSearch;
                Parser->SearchLabel = Condition;
                
                ParseBlock(Parser, TRUE, OldMode != RunModeSkip);
                
                if (Parser->Mode != RunModeReturn)
                    Parser->Mode = OldMode;

                Parser->SearchLabel = OldSearchLabel;
            }

            CheckTrailingSemicolon = FALSE;
            break;

        case TokenCase:
            if (Parser->Mode == RunModeCaseSearch)
            {
                Parser->Mode = RunModeRun;
                Condition = ExpressionParseInt(Parser);
                Parser->Mode = RunModeCaseSearch;
            }
            else
                Condition = ExpressionParseInt(Parser);
                
            if (LexGetToken(Parser, NULL, TRUE) != TokenColon)
                ProgramFail(Parser, "':'无效");
            
            if (Parser->Mode == RunModeCaseSearch && Condition == Parser->SearchLabel)
                Parser->Mode = RunModeRun;

            CheckTrailingSemicolon = FALSE;
            break;
            
        case TokenDefault:
            if (LexGetToken(Parser, NULL, TRUE) != TokenColon)
                ProgramFail(Parser, "':'无效");
            
            if (Parser->Mode == RunModeCaseSearch)
                Parser->Mode = RunModeRun;
                
            CheckTrailingSemicolon = FALSE;
            break;

        case TokenBreak:
            if (Parser->Mode == RunModeRun)
                Parser->Mode = RunModeBreak;
            break;
            
        case TokenContinue:
            if (Parser->Mode == RunModeRun)
                Parser->Mode = RunModeContinue;
            break;
            
        case TokenReturn:
            if (Parser->Mode == RunModeRun)
            {
                if (TopStackFrame->ReturnValue->Typ->Base != TypeVoid)
                {
                    if (!ExpressionParse(Parser, &CValue))
                        ProgramFail(Parser, "需要返回值");
                    
                    ExpressionAssign(Parser, TopStackFrame->ReturnValue, CValue, TRUE, NULL, 0, FALSE);
                    VariableStackPop(Parser, CValue);
                }
                else
                {
                    if (ExpressionParse(Parser, &CValue))
                        ProgramFail(Parser, "void函数出现返回值");                    
                }
                
                Parser->Mode = RunModeReturn;
            }
            else
                ExpressionParse(Parser, &CValue);
            break;

        case TokenTypedef:
            ParseTypedef(Parser);
            break;
            
        case TokenGoto:
            if (LexGetToken(Parser, &LexerValue, TRUE) != TokenIdentifier)
                ProgramFail(Parser, "无效符号");
            
            if (Parser->Mode == RunModeRun)
            { 
                /* GOTO标签开始扫描 */
                Parser->SearchGotoLabel = LexerValue->Val->Identifier;
                Parser->Mode = RunModeGoto;
            }
            break;
                
        case TokenDelete:
        {
            /* 尝试作为一个函数或变量名删除 */
            if (LexGetToken(Parser, &LexerValue, TRUE) != TokenIdentifier)
                ProgramFail(Parser, "无效符号");
                
            if (Parser->Mode == RunModeRun)
            { 
                /* 删除这个变量或函数 */
                CValue = TableDelete(&GlobalTable, LexerValue->Val->Identifier);

                if (CValue == NULL)
                    ProgramFail(Parser, "'%s'未知", LexerValue->Val->Identifier);
                
                VariableFree(CValue);
            }
            break;
        }
        
        default:
            *Parser = PreState;
            return ParseResultError;
    }
    
    if (CheckTrailingSemicolon)
    {
        if (LexGetToken(Parser, NULL, TRUE) != TokenSemicolon)
            ProgramFail(Parser, "';'无效");
    }
    
    return ParseResultOk;
}

/* 快速扫描一个定义的源文件 */
void PicocParse(const char *FileName, const char *Source, int SourceLen, int RunIt, int CleanupNow, int CleanupSource)
{
    struct ParseState Parser;
    enum ParseResult Ok;
    struct CleanupTokenNode *NewCleanupNode;
    
    void *Tokens = LexAnalyse(FileName, Source, SourceLen, NULL);
    
    /* 分配清除节点，所以我们之后可以清理标记 */
    if (!CleanupNow)
    {
        NewCleanupNode = HeapAllocMem(sizeof(struct CleanupTokenNode));
        if (NewCleanupNode == NULL)
            ProgramFail(NULL, "内存不足");
        
        NewCleanupNode->Tokens = Tokens;
        if (CleanupSource)
            NewCleanupNode->SourceText = Source;
        else
            NewCleanupNode->SourceText = NULL;
            
        NewCleanupNode->Next = CleanupTokenList;
        CleanupTokenList = NewCleanupNode;
    }
    
    /* 做解析 */
    LexInitParser(&Parser, Source, Tokens, FileName, RunIt);

    do {
        Ok = ParseStatement(&Parser, TRUE);
    } while (Ok == ParseResultOk);
    
    if (Ok == ParseResultError)
        ProgramFail(&Parser, "解析错误");
    
    /* 清理 */
    if (CleanupNow)
        HeapFreeMem(Tokens);
}

#ifdef NO_INTERACTIVE
/* 交互模式 */
void PicocParseInteractive(void)
{
    struct ParseState Parser;
    enum ParseResult Ok;
    
    PlatformPrintf(INTERACTIVE_PROMPT_START);
    LexInitParser(&Parser, NULL, NULL, StrEmpty, TRUE);
    PicocPlatformSetExitPoint();
    LexInteractiveClear(&Parser);

    do
    {
        LexInteractiveStatementPrompt();
        Ok = ParseStatement(&Parser, TRUE);
        LexInteractiveCompleted(&Parser);
        
    } while (Ok == ParseResultOk);
    
    if (Ok == ParseResultError)
        ProgramFail(&Parser, "解析错误");
    
    PlatformPrintf("\r\n");
}
#endif

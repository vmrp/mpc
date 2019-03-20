#include "picoc.h"
#include "interpreter.h"

/* 值传递给 exit() */
int FuncRet;

struct ReleaseFile{
    char *FilePtr;
    struct ReleaseFile* next;
};
static struct ReleaseFile *FileList=NULL;

/* 初始化所有 */
void PicocInitialise(int StackSize)
{
    PlatformInit();
    BasicIOInit();
    HeapInit(StackSize);
    TableInit();
    VariableInit();
    LexInit();
    TypeInit();
    IncludeInit();
    LibraryInit();
}

/* 释放内存 */
void PicocCleanup(void)
{
    PlatformCleanup();
    IncludeCleanup();
    ParseCleanup();
    LexCleanup();
    VariableCleanup();
    TypeCleanup();
    TableStrFree();
    HeapCleanup();
    fileFree();
}

jmp_buf PicocExitBuf;
int32 debugfile;


void fileFree(void)
{
    while (FileList != NULL)
    {
        struct ReleaseFile *Next = FileList->next;

        if (FileList->FilePtr != NULL)
            free((void *)FileList->FilePtr);

        free(FileList);
        FileList = Next;
    }
}
void PlatformInit(void)
{
    mrc_remove("print.txt");
    debugfile = mrc_open("print.txt",12);
}

void PlatformCleanup(void)
{
    if(debugfile)
    {
        mrc_close(debugfile);
        debugfile=0;
    }
}

void PlatformPutc(unsigned char OutCh, union OutputStreamInfo *Stream)
{
    if(debugfile)
        mrc_write(debugfile,&OutCh,1);
}

/* 文件读入内存 */
char *PlatformReadFile(const char *FileName)
{
    int32 fd;
    uint32 fSize;
    char *fBuf = NULL;
    
    fSize = mrc_getLen(FileName);
    fd = mrc_open(FileName, MR_FILE_RDONLY);

    if (fd)
    {
        struct ReleaseFile *NewNode=mrc_malloc(sizeof(struct ReleaseFile));

        if(!NewNode) goto err0;
        NewNode->FilePtr=NULL;
        NewNode->next=FileList;
        FileList=NewNode;

        fBuf = (char*)mrc_malloc(fSize + 1);
        if(!fBuf) goto err0;
        mrc_read(fd, fBuf, fSize);
        fBuf[fSize]=0;
        mrc_close(fd);
        FileList->FilePtr=fBuf;
    }
    else goto err;

    return fBuf;

err0:
    mrc_close(fd);
err:
    ProgramFail(NULL, "无法打开%s",FileName);
    return NULL;
}

/* 读和扫描文件定义 */
void PicocPlatformScanFile(const char *FileName)
{
    char *SourceStr = PlatformReadFile(FileName);
    PicocParse(FileName, SourceStr, strlen(SourceStr), TRUE, FALSE, FALSE);
}


/* 退出程序 */
void PlatformExit(int RetVal)
{
    FuncRet = RetVal;
    longjmp(PicocExitBuf, 1);
}

void PrintSourceTextErrorLine(const char *FileName, const char *SourceText, int Line, int CharacterPos)
{
    int LineCount;
    const char *LinePos;
    const char *CPos;
    int CCount;
    
    if (SourceText)
    {
        /* 找到源代码行 */
        for (LinePos = SourceText, LineCount = 1; *LinePos && LineCount < Line; LinePos++)
        {
            if (*LinePos == '\n')
                LineCount++;
        }
        
        /* 显示行 */
        for (CPos = LinePos; *CPos != '\n' && *CPos; CPos++)
            PrintCh(*CPos, CStdOut);
        PrintStr("\r\n", CStdOut);
        
        /* 显示错误位置 */
        for (CPos = LinePos, CCount = 0; *CPos != '\n' && *CPos && (CCount < CharacterPos || *CPos == ' '); CPos++, CCount++)
        {
            if (*CPos == '\t')
                PrintCh('\t', CStdOut);
            else
                PrintCh(' ', CStdOut);
        }
    }
    //交互模式
//     else
//     {
//         /* assume we're in interactive mode - try to make the arrow match up with the input text */
//         for (CCount = 0; CCount < CharacterPos + strlen(INTERACTIVE_PROMPT_STATEMENT); CCount++)
//             PrintCh(' ', CStdOut);
//     }
    PlatformPrintf("^ %s,行:%d,位置:%d信息:", FileName, Line, CharacterPos);
    mrc_clearScreen(0,0,0);
    mrc_drawText("程序出错",0,0,240,0,0,0,1);
    mrc_refreshScreen(0,0,SCREEN_WIDTH,SCREEN_HEIGHT);
    mrc_sleep(3000);
    
}

/* 显示源代码行和行号识别错误 */
void PlatformErrorPrefix(struct ParseState *Parser)
{
    if (Parser != NULL)
        PrintSourceTextErrorLine(Parser->FileName, Parser->SourceText, Parser->Line, Parser->CharacterPos);
}

/* 退出消息 */
void ProgramFail(struct ParseState *Parser, const char *Message, ...)
{
    va_list Args;

    PlatformErrorPrefix(Parser);
    va_start(Args, Message);
    PlatformVPrintf(Message, Args);
    va_end(Args);
    PlatformPrintf("\r\n");
    PlatformExit(1);
}

/* like ProgramFail() but gives descriptive error messages for assignment */
void AssignFail(struct ParseState *Parser, const char *Format, struct ValueType *Type1, struct ValueType *Type2, int Num1, int Num2, const char *FuncName, int ParamNo)
{
    PlatformErrorPrefix(Parser);
    PlatformPrintf("can't %s ", (FuncName == NULL) ? "assign" : "set");   
        
    if (Type1 != NULL)
        PlatformPrintf(Format, Type1, Type2);
    else
        PlatformPrintf(Format, Num1, Num2);
    
    if (FuncName != NULL)
        PlatformPrintf("在%s()的第%d参数", FuncName, ParamNo);
        
    ProgramFail(NULL, "");
}

/* 带消息退出 */
void LexFail(struct LexState *Lexer, const char *Message, ...)
{
    va_list Args;

    PrintSourceTextErrorLine(Lexer->FileName, Lexer->SourceText, Lexer->Line, Lexer->CharacterPos);
    va_start(Args, Message);
    PlatformVPrintf(Message, Args);
    va_end(Args);
    PlatformPrintf("\r\n");
    PlatformExit(1);
}

/* printf 编译器的错误报告 */
void PlatformPrintf(const char *Format, ...)
{
    va_list Args;
    
    va_start(Args, Format);
    PlatformVPrintf(Format, Args);
    va_end(Args);
}

void PlatformVPrintf(const char *Format, va_list Args)
{
    const char *FPos;
    
    for (FPos = Format; *FPos; FPos++)
    {
        if (*FPos == '%')
        {
            FPos++;
            switch (*FPos)
            {
            case 's': PrintStr(va_arg(Args, char *), CStdOut); break;
            case 'd': PrintSimpleInt(va_arg(Args, int), CStdOut); break;
            case 'c': PrintCh(va_arg(Args, int), CStdOut); break;
            case 't': PrintType(va_arg(Args, struct ValueType *), CStdOut); break;
#ifndef NO_FP
            case 'f': PrintFP(va_arg(Args, double), CStdOut); break;
#endif
            case '%': PrintCh('%', CStdOut); break;
            case '\0': FPos--; break;
            }
        }
        else
            PrintCh(*FPos, CStdOut);
    }
}

/* 创建一个新的临时名称. 取静态缓冲区的char[7]为参数。应初始化为"XX0000"
 * 其中xx可以是任何字符 */
char *PlatformMakeTempName(char *TempNameBuffer)
{
    int CPos = 5;
    
    while (CPos > 1)
    {
        if (TempNameBuffer[CPos] < '9')
        {
            TempNameBuffer[CPos]++;
            return TableStrRegister(TempNameBuffer);
        }
        else
        {
            TempNameBuffer[CPos] = '0';
            CPos--;
        }
    }

    return TableStrRegister(TempNameBuffer);
}

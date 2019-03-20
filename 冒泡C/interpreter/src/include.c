#include "picoc.h"
#include "interpreter.h"


/* 我们可以包含一个库列表 */
struct IncludeLibrary
{
    char *IncludeName;
    void (*SetupFunction)(void);
    struct LibraryFunction *FuncList;
    const char *SetupCSource;
    struct IncludeLibrary *NextLib;
};

struct IncludeLibrary *IncludeLibList = NULL;


/* 初始化内置的包含库 */
void IncludeInit(void)
{
/* 原版
#ifndef BUILTIN_MINI_STDLIB
    IncludeRegister("ctype.h", NULL, &StdCtypeFunctions[0], NULL);
    IncludeRegister("errno.h", &StdErrnoSetupFunc, NULL, NULL);
#ifndef NO_FP
    IncludeRegister("math.h", &MathSetupFunc, &MathFunctions[0], NULL);
#endif
    IncludeRegister("stdbool.h", &StdboolSetupFunc, NULL, StdboolDefs);
    IncludeRegister("stdio.h", &StdioSetupFunc, &StdioFunctions[0], StdioDefs);
    IncludeRegister("stdlib.h", &StdlibSetupFunc, &StdlibFunctions[0], NULL);
    IncludeRegister("string.h", &StringSetupFunc, &StringFunctions[0], NULL);
    IncludeRegister("time.h", &StdTimeSetupFunc, &StdTimeFunctions[0], StdTimeDefs);
    IncludeRegister("unistd.h", &UnistdSetupFunc, &UnistdFunctions[0], UnistdDefs);
#endif
*/
    IncludeRegister("ctype.h", &Ctype_SetupFunc, &Ctype_Functions[0], NULL);
    IncludeRegister("sound.h", &Sound_SetupFunc, &Sound_Functions[0], NULL);
    IncludeRegister("net.h", &mrc_net_SetupFunc, &mrc_net_Functions[0], NULL);
    IncludeRegister("base.h", &mrc_base_SetupFunc, &mrc_base_Functions[0], mrc_base_Defs);
}

/* 清理包含系统头文件所使用的空间 */
void IncludeCleanup(void)
{
    struct IncludeLibrary *ThisInclude = IncludeLibList;
    struct IncludeLibrary *NextInclude;
    
    while (ThisInclude != NULL)
    {
        NextInclude = ThisInclude->NextLib;
        HeapFreeMem(ThisInclude);
        ThisInclude = NextInclude;
    }

    IncludeLibList = NULL;
}

/* 注册一个新的内置包含文件 */
void IncludeRegister(const char *IncludeName, void (*SetupFunction)(void), struct LibraryFunction *FuncList, const char *SetupCSource)
{
    struct IncludeLibrary *NewLib = HeapAllocMem(sizeof(struct IncludeLibrary));
    NewLib->IncludeName = TableStrRegister(IncludeName);
    NewLib->SetupFunction = SetupFunction;
    NewLib->FuncList = FuncList;
    NewLib->SetupCSource = SetupCSource;
    NewLib->NextLib = IncludeLibList;
    IncludeLibList = NewLib;
}

/* 包含所有的系统头文件 */
void PicocIncludeAllSystemHeaders(void)
{
    struct IncludeLibrary *ThisInclude = IncludeLibList;
    
    for (; ThisInclude != NULL; ThisInclude = ThisInclude->NextLib)
        IncludeFile(ThisInclude->IncludeName);
}

/* 包含一些预定义的库，或者实际的文件 */
void IncludeFile(char *FileName)
{
    struct IncludeLibrary *LInclude;
    
    /* 扫描包含的文件名，看它是否在我们的预定义的包含列表中 */
    for (LInclude = IncludeLibList; LInclude != NULL; LInclude = LInclude->NextLib)
    {
        if (strcmp(LInclude->IncludeName, FileName) == 0)
        {
            /* 查找 - 防止重复包含 */
            if (!VariableDefined(FileName))
            {
                VariableDefine(NULL, FileName, NULL, &VoidType, FALSE);
                
                /* 运行额外的启动函数，如果有一个 */
                if (LInclude->SetupFunction != NULL)
                    (*LInclude->SetupFunction)();
                
                /* 解析设置的C源代码 - 可以定义类型等. */
                if (LInclude->SetupCSource != NULL)
                    PicocParse(FileName, LInclude->SetupCSource, strlen(LInclude->SetupCSource), TRUE, TRUE, FALSE);
                
                /* 设置函数库 */
                if (LInclude->FuncList != NULL)
                    LibraryAdd(&GlobalTable, FileName, LInclude->FuncList);
            }
            
            return;
        }
    }
    
    /* 不是预定义的文件，读一个真正的文件 */
    PicocPlatformScanFile(FileName);
}


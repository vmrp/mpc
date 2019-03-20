#ifndef PICOC_H
#define PICOC_H
#include <setjmp.h>

#include "platform.h"
#include "interpreter.h"

#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif



extern jmp_buf PicocExitBuf;
/* 这是一个宏，否则发生错误会使堆栈被破坏 */
#define PicocPlatformSetExitPoint() setjmp(PicocExitBuf)

int CheckBaseCall(char *FuncName,struct ValueType *ReturnType,int NumParams);

/* parse.c */
void PicocParse(const char *FileName, const char *Source, int SourceLen, int RunIt, int CleanupNow, int CleanupSource);
void PicocParseInteractive(void);

/* platform.c */
int PicocCallInit(void);
void PicocInitialise(int StackSize);
void PicocCleanup(void);
void PicocPlatformScanFile(const char *FileName);

extern int FuncRet;
void PicocRun(int32 data);

/* include.c */
void PicocIncludeAllSystemHeaders(void);

#endif /* PICOC_H */

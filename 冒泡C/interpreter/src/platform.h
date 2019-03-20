/* 所有特定平台的包含和定义都在这个文件中 */
#ifndef PLATFORM_H
#define PLATFORM_H


#define ALIGN_TYPE void*                    /* 默认的数据类型，队列使用 */
#define GLOBAL_TABLE_SIZE 97                /* 全局变量表 */
#define STRING_TABLE_SIZE 97                /* 共享字符串表的大小 */
#define STRING_LITERAL_TABLE_SIZE 97        /* 字符串文字表的大小 */
#define PARAMETER_MAX 16                    /* 函数参数的最大数量 */
#define LINEBUFFER_MAX 256                  /* 行的最大字符数 */
#define LOCAL_TABLE_SIZE 11                 /* 局部变量表的大小（可扩展） */
#define STRUCT_TABLE_SIZE 11                /* 结构/联合成员表的大小（可扩展） */

#define INTERACTIVE_PROMPT_START "starting picoc " _VERSION "\n"
#define INTERACTIVE_PROMPT_STATEMENT "picoc> "
#define INTERACTIVE_PROMPT_LINE "     > "

#define HEAP_SIZE (128*1024)               /* 堆和堆栈空间 */
// #define USE_MALLOC_STACK
// #define USE_MALLOC_HEAP
#define NO_FP

#include <setjmp.h>
#include "mrc_base.h"
#include "mrc_base_i.h"

typedef char * va_list;
#define _INTSIZEOF(n)   ( (sizeof(n) + sizeof(int) - 1) & ~(sizeof(int) - 1) )
#define va_start(ap,v)  ( ap = (va_list)(&(v)) + _INTSIZEOF(v) )
#define va_arg(ap,t)    ( *(t *)((ap += _INTSIZEOF(t)) - _INTSIZEOF(t)) )
#define va_end(ap)      ( ap = (va_list)0 )
#define assert(x)       //MRC_ASSERT(x)


#endif /* PLATFORM_H */

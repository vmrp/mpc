#ifndef _CONFIG_H
#define _CONFIG_H

/**
 * \定义为非零值来表示系统是大端
 */
#ifndef BIG_ENDIAN
    #ifdef MR_SPREADTRUM_MOD
        #define BIG_ENDIAN	1
    #else
        #define BIG_ENDIAN	0
    #endif
#endif

/**
 * \定义为非零值来表示系统是小端
 */
#ifndef LITTLE_ENDIAN
    #ifdef MR_SPREADTRUM_MOD
        #define LITTLE_ENDIAN	0
    #else
        #define LITTLE_ENDIAN	1
    #endif
#endif

/**
 * \如果定义这个宏，网络模块将不会被编译
 */
#define OMIT_NETWORK_MODULE 1
#ifndef OMIT_NETWORK_MODULE
#define OMIT_NETWORK_MODULE	0
#endif

/**
 * \如果定义这个宏，WM_KEYDOWNREPEAT事件启用。
 */
#ifndef ENABLE_SGL_KEYREPEAT
#define ENABLE_SGL_KEYREPEAT
#endif


#endif //_CONFIG_H


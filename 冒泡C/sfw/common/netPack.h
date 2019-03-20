/*******************************************************************************
 * CopyRight (c) 2005-2008 SKY-MOBI Ltd. All rights reserved.
 * Filename:  netPack.h
 * Creator:   wangjian
 * Version:   0.0
 * Date:      
 * Description:
 *******************************************************************************/

#ifndef __NET_PACK_H__
#define __NET_PACK_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "mrc_base.h"

int32 putStr(char * out, char* in, int32 len,  int32 pos);
int32 putIn(char * out, int32 data, int32 len, int32 pos);
int32 getStr(void* in, void * out, int32 len,  int32 pos);
int32 getOut(char *in, void *data, int32 len, int32 pos);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __NET_PACK_H__ */

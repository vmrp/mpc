/*******************************************************************************
 * CopyRight (c) 2005-2008 SKY-MOBI Ltd. All rights reserved.
 * Filename:  des.h
 * Creator:   wangjian
 * Version:   0.0
 * Date:      
 * Description:
 *******************************************************************************/

#ifndef _ENCRYPT_DES_H_
#define _ENCRYPT_DES_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "mrc_base.h"

typedef unsigned char * des_user_key;//[8];
typedef unsigned char * des_cblock;//[8];
typedef unsigned long * des_ks;//[32];

void desCrypt( des_cblock input, des_cblock output, des_ks ks, int32 encrypt );

void desExpandKey( des_user_key userKey, des_ks key );


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif  //_ENCRYPT_DES_H_

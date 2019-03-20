/*******************************************************************************
 * CopyRight (c) 2005-2008 SKY-MOBI Ltd. All rights reserved.
 * Filename:  netPack.c
 * Creator:   wangjian
 * Version:   0.0
 * Date:      
 * Description:
 *******************************************************************************/
#include "mrc_base.h"
#include "netPack.h"

static int32 putChar(char* out, char data, int32 pos)
{
	out = out + pos;
	*out = data;

	return pos+1; 
}

static int32 putShort(char* out, int16 data, int32 pos)
{
	char *in = (char *)&data;

#ifdef MR_SPREADTRUM_MOD
	out = out + pos;
	*out = in[0];
	out++;
	*out = in[1];
#else
	out = out + pos;
	*out = in[1];
	out++;
	*out = in[0];
#endif

	return pos+2; 
}

static int32 putInt(char* out, int32 data, int32 pos)
{
	char *in = (char *)&data;
	
#ifdef MR_SPREADTRUM_MOD
	out = out + pos;
	*out = in[0];
	out++;
	*out = in[1];
	out++;
	*out = in[2];
	out++;
	*out = in[3];
#else
	out = out + pos;
	*out = in[3];
	out++;
	*out = in[2];
	out++;
	*out = in[1];
	out++;
	*out = in[0];
#endif

	return pos+4; 
}


int32 putStr( char * out, char* in, int32 len,  int32 pos)
{
	out = out + pos;
	memcpy(out,in,len);
	return pos+len;
}

int32 putIn(char *out, int32 data, int32 len, int32 pos)
{
	//DBG_PRINTF(("putin len= %d", len));
    if (len == 1){
        return putChar(out, (char)data, pos);
    }
    else if(len == 2){
        return putShort(out, (int16)data, pos);
    }else{
        return putInt(out, data, pos);
    }
}

static int32 getChar(uint8* in, char* data, int32 pos)
{
    in = in + pos;
    *data = *in;
    pos = pos+1;
    
    return pos; 
}

static int32 getShort(uint8* in, int16 * data, int32 pos)
{
    in = in + pos;
    *data = in[0]<<8 | in[1];
    pos = pos+2;

    return pos; 
}

static int32 getInt(uint8* in, int32* data, int32 pos)
{
    in = in + pos;
    *data = in[0]<<24 | in[1]<<16 | in[2]<<8 | in[3];
    pos = pos+4;

    return pos; 
}


int32 getStr(void* in,  void * out, int32 len, int32 pos)
{
    char * input = (char *)in;
    char * output = (char *)out;

    input = input + pos;
    memcpy(output,input,len);
    return pos+len;
}

int32 getOut(char *in, void *data, int32 len, int32 pos)
{
    if (len == 1){
        return getChar((unsigned char *)in, (char *)data, pos);
    }
    else if(len == 2){
        return getShort((unsigned char *)in, (short *)data, pos);
    }else{
        return getInt((unsigned char *)in, data, pos);
    }
}

#ifndef __CHECKMRP_H__
#define  __CHECKMRP_H__
extern "C"
{
	#include "mrporting.h"	
}
int32 CheckMrp(void);
void saveCheckMrp(const char *path);
const char *getMrpFullPath(void);
const char* getMrpPath(void);

int32 changeDir(const char*path,uint8** output, int32* output_len,int32 flag);
const char *getchangemrpPath(void);
void clearCheckMrp(void);
#endif























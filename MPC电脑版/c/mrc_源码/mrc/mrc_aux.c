#include "mrc_base.h"


int32 mrc_readAllEx(char* filename, char* buf, int32 *len)
{
   int32 ret,filelen,f,oldlen;

   ret = mrc_fileState(filename);
   if((ret != MR_IS_FILE))
   {
     return MR_FAILED;
   }

   filelen = mrc_getLen(filename);
   if (filelen <= 0)
   {
     return MR_FAILED;
   }
   filelen = (filelen > *len)? *len:filelen;

   f = mrc_open(filename, MR_FILE_RDONLY );
   if (f == 0)
   {
     return MR_FAILED;
   }
   
  oldlen = 0;
  while(oldlen < filelen){
     ret = mrc_read(f, (char*)buf+oldlen, filelen-oldlen);
     if (ret <= 0)
     {
         mrc_close(f);
         return MR_FAILED;
     }
     oldlen = oldlen + ret;
  }
  mrc_close(f);
  *len = filelen;
  return MR_SUCCESS;
}

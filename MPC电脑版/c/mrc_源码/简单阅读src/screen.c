#include "mrc_base.h"
#include "mrc_bmp.h"
#include "screen.h"

#define SCRW 240
#define SCRH 320

int ScreenShut(char *filename,int x,int y,int w,int h,int is_hand)
{
	uint16 *td;
	int i;
	 int s;
	  int32 fileH;
	  int32 a[18]={0,0,70,56,0,0,1048577,3,51200,4000,4000,0,0,63488,2016,31,0,0};
	  a[0] = w*h*2+70;
	  a[4] = w;
	  a[5] = h;

 if (x<0 || y<0 || SCRW < x+w || SCRH <y+h)
  return MR_FAILED;

 s = 19778;
 
td=w_getScreenBuffer();

	 fileH=mrc_open(filename,MR_FILE_WRONLY|MR_FILE_CREATE );
if (is_hand)
{
 mrc_write(fileH,&s,2);
 mrc_write (fileH,a,70-sizeof (s) );
 
 for(i=1;i<=h;i++)
   mrc_write(fileH,td+(y+h-i)*SCRW+x,w*2);
}else
{
for(i=1;i<=h;i++)
  mrc_write(fileH,td+(y+i)*SCRW+x,w*2);
}
  mrc_close(fileH);
 return 0;
}



void saveScreenBuf (void)
{
 int hand=mrc_open(SCREENFILE,2|8);
 mrc_write(hand,w_getScreenBuffer(),SCRW*SCRH*2);
 mrc_close (hand);
}
int readScreenBuf (void) 
{
 int hand = mrc_open (SCREENFILE,1);
 int len = mrc_read (hand,w_getScreenBuffer(),SCRW*SCRH*2);
 mrc_close (hand);
 if(len<SCRW*SCRH*2)
  return -1;
 return 0;
}



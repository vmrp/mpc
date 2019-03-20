#include <base.h>

//图片路径
#define FILENAME "c/toolbar.bmp"

//图片宽度
#define BMPW 4

//图片高度
#define BMPH 13

int init()
{
/*读取文件内容到内存*/
 uint32 len=getlen(FILENAME);
 uint16 *ptr=malloc(len);
 memset(ptr,NULL,len);

 uint32 f=open(FILENAME,1);
 read(f,ptr,len);
 close(f);

cls(0,0,0);
//绘titlebar
 uint16 y=0;//显示图片y坐标
 for(uint16 x=0;x<=SCRW;x+=BMPW)
 {
 bmpshowflip(ptr,x,y,BMPW,BMPW,BMPH,0,0,0,0);
 ref(x,y,BMPW,BMPH);
 }

//绘toolbar
y=SCRH-BMPH;
for(x=0;x<=SCRW;x+=BMPW)
 {
 bmpshowflip(ptr,x,y,BMPW,BMPW,BMPH,0,0,0,0);
 ref(x,y,BMPW,BMPH);
 }

/*
//可替代上面绘toolbar
 uint16 *scr=getscrbuf();
 memcpy(scr+SCRW*(SCRH-BMPH),scr,SCRW*BMPH*2);
*/

 ref(0,0,SCRW,SCRH);
 free(ptr);

 return 0;
}

int event(int type, int p1, int p2)
{
 if(KY_DOWN == type)
 {
 switch(p1)
  {
  case _SRIGHT:
  exit();break;
  case _1:
            break;
  }
 }
 return 0;
}

int pause(void)
{
 return 0;
}

int resume(void)
{
 return 0;
}
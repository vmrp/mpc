#ifndef _TYPE_H
#define _TYPE_H 

/*龙魂噬心(6369)
2012-7-11

*/

#include "mrc_base.h"


#define SCRW 240
#define SCRH 320

#define FOCUSBK 0xff0000//得到焦点的控件边框颜色
#define FOCUSBKW 2//得到焦点控件的边框宽度

#define MAKERGB(r,g,b)(r<<16)+(g<<8)+(b)
#define MAKER(a)(a>>16&0xff)

#define MAKEG(a)(a>>8&0xff)

#define MAKEB(a)(a&0xff)


extern int32 FONT;//必要时请注释

/*将888格式的颜色a分解为r,g,b
a：888格式的颜色
r，g，b  保存r，g，b的变量的地址(指针)
*/
void make(int32 a,int* r,int* g,int* b);


/*gb2312和unicode编码的相互转换
str 欲转换的字符串
is_uni 是否输出的为unicode编码
  1 返回unicode的指针
  0 gb2312
返回 指向返回字符串的指针
*/
void* gb_uni(char* str,int is_unicode);

void shaderect(int x,int y,int w,int h,int32 upclr,int32 downclr,int is_made);


#endif
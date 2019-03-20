//ÌבÊ¾¿ע
#include "c2/spz/pub.h"
colorst cst;
rectst rst;
short *ob,olen;

void print(char *a,int time)
{
short *s,*b;
short *p;
int w,h,j;
rst.y=0;rst.w=sw-rst.x*2;
rst.h=sh;
cst.r=cst.g=cst.b=255;
textwh(a,0,1,&w,&h);
j=strlen(a);
p=c2u(a,NULL,&j);
w=unitextrow((unsigned short *)p,1,rst.w);
free(p);
s=(short*)getscrbuf();
b=(short*)malloc(sw*2*(h+2)*w);
memmove(b,s+sw*(sh-h)/2,sw*2*(h+2)*w);
drect(0,(sh-h*w)/2,sw,(h+2)*w,0,0,0);
dtextex(a,rst.x,(sh-h*w)/2,&rst,&cst,2,1);
refall();
sleep(time);
memmove(s+sw*(sh-h*w)/2,b,sw*2*(h+2)*w);
refall();
free(b);
}


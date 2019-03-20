
void dRectText(void *text,uint16 x,uint16 y,uint16 w,uint16 h,uint8 r,uint8 g,uint8 b)
{
 colorst c;
 rectst rr;
 c.r=r;
 c.g=g;
 c.b=b;
 rr.x=x;
 rr.y=y;
 rr.w=w;
 rr.h=h;
 dtextex(text,x,y,&rr,&c,0,2);
}
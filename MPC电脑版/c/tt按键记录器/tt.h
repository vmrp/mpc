#define FONT 1

int tw,th;
textwh("Ìì",FALSE,FONT,&tw,&th);

colorst textColor;
textColor.r=40;
textColor.g=20;
textColor.g=40;

void ShowMsg(void *Msg)
{
 int w,h;uint16 x,y;
 textwh(Msg,FALSE,FONT,&w,&h);
 if(w<SCRW)x=(SCRW-w)>>1;
 y=(SCRH-h)>>1;
 effsetcon(x-5,y-5,w+10,h+10,100,100,100);
 dtext(Msg,x,y,255,255,250,FALSE,FONT);
 ref(x-5,y-5,w+10,h+10);
}


void Drawtext(void *str,uint16 x,uint16 y)
{
 dtext(str,x,y,textColor.r,textColor.g,textColor.b,FALSE,FONT);

}


rectst textRect;
textRect.x=0;
textRect.y=th;
textRect.w=SCRW;
textRect.h=th*2+5;

void DrawEventMsg(void *str)
{
 if(textRect.y>=(SCRH-textRect.h))
 {
  DrawMain();
  textRect.y=th;
 }
 drect(textRect.x,textRect.y,textRect.w,textRect.h,250,250,250);
 dtextex(str,textRect.x,textRect.y,&textRect,&textColor,2,FONT);
 ref(textRect.x,textRect.y,textRect.w,textRect.h);
 textRect.y+=textRect.h;
}
//wildly create data 
//按钮，触屏

#ifndef _BUTTON_H
#define _BUTTON_H

#define BUTTON_TEXT_FONT 0

//按钮状态
enum {
   _BUTTONNULL = 0, //无
   _BUTTONCLICK, //按下
   _BUTTONRELEACE //松开
};

typedef struct
{
   char *str;
   uint16 x;
   uint16 y;
   uint16 w;
   uint16 h;
   uint16 strx;
   uint8 tw;
   uint8 th;
   uint8 r;
   uint8 g;
   uint8 b;
   uint8 Type;
}_BUTTON;

typedef _BUTTON *Button;

void *ButtonCreate(char *str,uint16 x,uint16 y,uint16 w,uint16 h,uint16 strx)
{
Button bu = malloc(sizeof(_BUTTON));
memset(bu,0,sizeof(_BUTTON));
if(bu == NULL)return NULL;
  bu->str = str;
  bu->x = x;
  bu->y = y;
  int tw,th;
textwh(str,0,BUTTON_TEXT_FONT,&tw,&th);
  bu->tw = tw;
  bu->th = th;
  bu->w = (w < tw+4 ? tw+4 : w);
  bu->h = (h < th +4 ? th+4 : h);
bu->strx = (strx < x+2 ? x+2 : strx);
bu->strx = (strx > w-2 ? w-2 : strx);
bu->strx += x;
bu->r = 0;
bu->g = 0;
bu->b = 0;
return bu;
}

int ButtonShow(Button bu)
{int x = (bu->x),y = (bu->y),tw = (bu->w),th = (bu->h),upd = 2;
drect(x,y,tw,th,0,0,0);
if(bu->Type == _BUTTONCLICK)
upd = 0;
if(bu->Type == _BUTTONRELEACE)
upd = 2;
shaderect(x,y,tw,th,149267,369987,upd);
dtext(bu->str,bu->strx,y+2,bu->r,bu->g,bu->b,0,BUTTON_TEXT_FONT);
ref(x,y,tw+4,th+4);
return 0;
}

int ButtonDel(Button bu)
{
free(bu);
return 0;
}

int ButtonEventm(Button bu,int type,int px,int py)
{int x =(bu->x),y = (bu->y),tw = (bu->w),th = (bu->h);
if(type == MS_DOWN)
{
if(px <= x + tw)if(px > x)if(py <= y + th)if(py > y){
  bu->Type = _BUTTONCLICK;
  bu->r = 255;
  bu->g = 255;
  bu->b = 255;
ButtonShow(bu);
} }
if(type == MS_UP)
{
if(px <= x + tw)if(px > x)if(py <= y + th)if(py > y)
{if(bu->Type == _BUTTONCLICK){
  bu->Type = _BUTTONRELEACE;
  bu->r = 0;
  bu->g = 0;
  bu->b = 0;
ButtonShow(bu);}
} }

if(type == MS_MOVE)
{
if(px <= x + tw && px > x && py <= y + th && py > y)
{}else{
  bu->Type = _BUTTONNULL;
  bu->r = 0;
  bu->g = 0;
  bu->b = 0;
ButtonShow(bu);
} }
return (bu->Type);
}

#endif
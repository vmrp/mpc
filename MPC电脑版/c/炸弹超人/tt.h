void ShowMsg(char *Msg)
{
 int w,h;int16 x,y;
 textwh(Msg,FALSE,FONT,&w,&h);
 if(w<SCRW)x=(SCRW-w)>>1;
 y=(SCRH-h)>>1;
 effsetcon(x-5,y-5,w+10,h+10,100,100,100);
 dtext(Msg,x,y,255,255,250,FALSE,FONT);
 ref(x-5,y-5,w+10,h+10);
}



void ShowTt()
{
 char *str="天天 制作";
 int16 x,y;
 int w,h;
 textwh(str,FALSE,FONT,&w,&h);
 if(w<SCRW)
 x=(SCRW-w)>>1;
 y=(SCRH-h)>>1;
 effsetcon(x,y,w,h,80,80,80);
 dtext(str,x,y,200,200,100,FALSE,FONT);
 ref(x,y,w,h>>1);
 dtext(str,x,y,150,150,250,FALSE,FONT);
 ref(x,y+(h>>1),w,h>>1);
 sleep(1500);
}
/*Window 窗口函数*/

//创建一个窗口
void* wwindowct(uint16 id,int16 x,int16 y,uint16 w,uint16 h)
{
wwindow *obj;
obj=malloc(sizeof(wwindow));
memset(obj,0,sizeof(wwindow));
obj->id=id;
obj->locat.x=x;
obj->locat.y=y;
obj->locat.w=w;
obj->locat.h=h;
obj->type=WWINDOW;
obj->point.first=NULL;
obj->point.last=NULL;
obj->point.now=NULL;
obj->ui=NULL;
wevent(obj,NULL,WECREATE,0,0);
return obj;
}

//窗口UI设置
void wwindowset(wwindow *obj,colorst *rgb,colorst *rgb2)
{
wwindowui *ui;
ui=malloc(sizeof(wwindowui));
memset(ui,0,sizeof(wwindowui));
obj->ui=ui;
ui->rgb=*rgb;ui->rgb2=*rgb2;
}

//显示窗口
void wwindowshow(wwindow *obj)
{
wwindowui *ui;
rectst a;
colorst b;
wnowwindow=obj;
wnowobj=obj->point.now;
ui=obj->ui;
a=obj->locat;
b=ui->rgb2;
drect(a.x,a.y,a.w,a.h,b.r,b.g,b.b);
b=ui->rgb;
drect(a.x+8,a.y+8,a.w-16,a.h-16,b.r,b.g,b.b);
wshowall(obj->point.first,0);
ref(a.x,a.y,a.w,a.h);
wevent(obj,NULL,WESHOW,0,0);
wsetfocus(wnowobj);
}

void* wobjct(uint16 id,wwindow *parent,int16 x,int16 y,uint16 w,uint16 h)
{
wobj *obj;wobj *bf;
obj=malloc(sizeof(wobj));
memset(obj,0,sizeof(wobj));
obj->id=id;
obj->locat.x=x;obj->locat.y=y;obj->locat.w=w;obj->locat.h=h;
obj->type=WNONE;
obj->point.next=NULL;obj->point.before=NULL;
obj->ui=NULL;
bf=parent->point.last;
if(bf!=NULL)
{
obj->point.before=bf;
bf->point.next=obj;
}
else
{parent->point.first=obj;}
parent->point.last=obj;
wevent(parent,obj,WECREATE,0,0);
return obj;
}

void wbuttonset(wobj *obj,colorst *rgb,colorst *rgb2,char *title,colorst *titrgb)
{
wbuttonui *ui;
ui=malloc(sizeof(wbuttonui));
memset(ui,0,sizeof(wbuttonui));
ui->rgb=*rgb;
ui->rgb2=*rgb2;
ui->titrgb=*titrgb;
ui->title=strcpyex(title);
obj->ui=ui;obj->type=WBUTTON;
}

void wbuttonshow(wobj *obj,uint8 refnow)
{
int x00;int y00;
colorst a;
rectst b;rectst c;rectst d;
wbuttonui *ui;
ui=obj->ui;
b=obj->locat;
c=wnowwindow->locat;
b.x+=c.x;b.y+=c.y;
a=ui->rgb2;
drect(b.x,b.y,b.w,b.h,a.r,a.g,a.b);
a=ui->rgb;
drect(b.x+2,b.y+2,b.w-4,b.h-4,a.r,a.g,a.b);
a=ui->titrgb;
textwh(ui->title,0,1,&x00,&y00);
x00=b.x+(b.w-x00)/2;
y00=b.y+(b.h-y00)/2;
dtext(ui->title,x00,y00,a.r,a.g,a.b,0,1);
if(refnow)
{wref(obj);}
wevent(wnowwindow,obj,WESHOW,0,0);
}

void wbuttonfree(wwindow *win,wobj *obj)
{
wbuttonui *ui;
wevent(win,obj,WEDESTROY,0,0);
ui=obj->ui;
free(ui->title);
free(ui);
free(obj);
}

void wref(wobj *obj)
{
rectst b,c,d;
b=obj->locat;
c=wnowwindow->locat;
b.x+=c.x;b.y+=c.y;
d.x=b.x+b.w;d.y=b.y+b.h;d.w=c.x+c.w;d.h=c.y+c.h;
if(inrect(c.x,c.y,d.w,d.h,b.x,b.y,d.x,d.y))
 {
if(b.x<c.x)
{b.w-=c.x-b.x;b.x=c.x;}
if(b.y<c.y)
{b.h-=c.y-b.y;b.y=c.y;}
if(d.x>d.w)
{b.w-=d.x-d.w;}
if(d.y>d.h)
{b.h-=d.y-d.h;}
ref(b.x,b.y,b.w,b.h);
 }
}

void wshowall(wobj *first,uint8 refnow)
{
for(;first!=NULL;first=first->point.next)
 {
wobjshow(first,refnow);
 }
}
void wobjshow(wobj *obj,uint8 refnow)
{
switch(obj->type)
 {
case WBUTTON:
 wbuttonshow(obj,refnow);
 break;
 }
}

void wobjfree(wwindow *win,wobj *obj)
{
switch(obj->type)
 {
case WBUTTON:
 wbuttonfree(win,obj);
 break;
 }
}

void wwindowfree(wwindow *win)
{
wobj *p;wobj *q;
for(p=win->point.first;p!=NULL;p=q)
{q=p->point.next;wobjfree(win,p);}
wevent(win,NULL,WEDESTROY,0,0);
free(win->ui);
free(win);
}

void wsetfocus(wobj *obj)
{
rectst a;rectst b;colorst r;
if(obj==NULL){return;}
a=obj->locat;
b=wnowwindow->locat;
if(wnowobj!=NULL)
{wevent(wnowwindow,wnowobj,WENOFOCUS,0,0);wobjshow(wnowobj,1);}
a.x+=b.x;a.y+=b.y;
r=wfocus.rgb;
drect(a.x,a.y,a.w,wfocus.w,r.r,r.g,r.b);
drect(a.x,a.y+a.h-wfocus.w,a.w,wfocus.w,r.r,r.g,r.b);
drect(a.x,a.y,wfocus.w,a.h,r.r,r.g,r.b);
drect(a.x+a.w-wfocus.w,a.y,wfocus.w,a.h,r.r,r.g,r.b);
wref(obj);
wnowobj=obj;
wnowwindow->point.now=obj;
wevent(wnowwindow,obj,WEFOCUS,0,0);
}

void *wmsclick(wwindow *win,uint16 x,uint16 y)
{
wobj *obj;
rectst a;rectst b;
a=win->locat;
for(obj=win->point.first;obj!=NULL;obj=obj->point.next)
 {
b=obj->locat;
b.x+=a.x;b.y+=a.y;
b.w+=b.x;b.h+=b.y;
if(inrect(b.x,b.y,b.w,b.h,x,y,x,y))
  {return obj;}
 }
return NULL;
}

void wkey(int type,int p,int p2)
{
wobj *obj;
if(type==KY_DOWN)
 {
if(OR(p==_SELECT,OR(OR(p==_UP,p==_DOWN),OR(p==_LEFT,p==_RIGHT))))
 {
if(wnowobj!=NULL)
{
if(OR(_UP==p,_LEFT==p))
{wsetfocus(wnowobj->point.before);}
else if(_SELECT!=p)
{wsetfocus(wnowobj->point.next);}
else{wevent(wnowwindow,wnowobj,WECLICK,-1,-1);
wsetfocus(wnowobj);}
}
else
{wsetfocus(wnowwindow->point.first);}
 }
 }
else if(MS_DOWN==type && wnowwindow!=NULL)
 {
obj=wmsclick(wnowwindow,p,p2);
if(obj!=NULL)
  {
wevent(wnowwindow,obj,WECLICK,p,p2);
wsetfocus(obj);
  }
 }
}

/*其他函数*/
void torectst(rectst *a,int8 x,int8 y,uint8 w,uint8 h)
{
a->x=x;a->y=y;a->w=w;a->h=h;
}

char* strcpyex(char *nr)
{
char *md;
uint32 len;
len=strlen(nr)+1;
md=malloc(len);
memcpy(md,nr,len);
return md;
}

int inrect(int16 x1,int16 y1,int16 x2,int16 y2,int16 x3,int16 y3,int16 x4,int16 y4)
{
if(x1<=x4 && y1<=y4)
 if(x2>=x3 && y2>=y3)
  return TRUE;
return FALSE;
}

void torgb(colorst *rgb,uint8 r,uint8 g,uint8 b)
{
rgb->r=r;rgb->g=g;rgb->b=b;
}

int OR(int x,int y)
{
if(x) return x;
if(y) return y;
return 0;
}

/*常用函数*/
void echo(char *nr,int mm)
{
char *ww;
static int y=5;
ww="01234567890123456789012345678901234567890123456789";
sprintf(ww,"%s:%d",nr,mm);
if(y+20>SCRH){sand(getuptime());y=rand()%30;}
drect(0,y,100,20,0,0,0);
dtext(ww,0,y,255,255,255,0,1);
ref(0,y,SCRW,20);
y+=25;
}

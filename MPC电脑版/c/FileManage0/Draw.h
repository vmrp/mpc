
//字体
#define FONT 1

//全屏刷新
#define REF ref(0,0,SCRW,SCRH)

//单个文字宽高
int TW,TH;
textwh("坚",FALSE,FONT,&TW,&TH);

enum
{
 KB=1024,
 MB=1024*1024
};

uint16 *scr=getscrbuf(),*FileBmp,*DirBmp;

void BmpStart()
{
 FileBmp=ReadBmp(FileBmpName);
 DirBmp=ReadBmp(DirBmpName);

}

//读取加密BMP图片
uint16 *ReadBmp(char *name)
{
 int f,len;
 f=open(name,1);
 len=getlen(name);
 uint16 *Bmp=malloc(len);
 read(f,Bmp,len);
 close(f);
 return Bmp;
}

//文字颜色和背景颜色;
;colorst TextRGB,GroundRGB;

//文字颜色
TextRGB.r=150;
TextRGB.g=250;
TextRGB.b=250;

//背景色
GroundRGB.r=40;
GroundRGB.g=20;
GroundRGB.b=40;

#define CLS cls(GroundRGB.r,GroundRGB.g,GroundRGB.b)

#define srectUp srect(0,0,SCRW,TH,0,0,0,80,80,80,0)

#define srectDown srect(0,SCRH-TH,SCRW,TH,80,80,80,0,0,0,0)


struct
{
 uint16 x;//文件名显示x
 uint16 y;//文件头y
 uint16 y2;//文件尾y
 uint16 w;//文件
 uint16 h;//文件显示框占h
 uint16 h2;//文件名占h
 uint32 fn;//显示文件数
}DXY;
DXY.x=40;
DXY.y=TH*2;
DXY.w=SCRW;
DXY.h=TH*2;
DXY.h2=SCRH-TH*3;
DXY.fn=SCRH/DXY.h-1;
DXY.y2=DXY.y;
while(DXY.y2<SCRH)
DXY.y2+=DXY.h;


/*绘文字*/
void text(char *txt,int16 x,int16 y)
{
 dtext(txt,x,y,TextRGB.r,TextRGB.g,TextRGB.b,FALSE,FONT);
}
void rect_text(char *txt,uint16 x,uint16 y,uint16 w,uint16 h)
{
 rectst rect;
 rect.x=x;
 rect.y=y;
 rect.w=w;
 rect.h=h;

dtextex(txt,x,y,&rect,&TextRGB,2,FONT);
}

void DrawMsg(char *txt)
{
 int w,h;uint16 x,y;
 textwh(txt,FALSE,FONT,&w,&h);
 if(w<SCRW)x=(SCRW-w)/2;
 y=(SCRH-h)/2;
 drect(x,y,w,h,GroundRGB.r,GroundRGB.g,GroundRGB.b);
 text(txt,x,y);
 ref(x,y,w,h);
}

int strw(char *str)
{
 int w,h;
 textwh(str,FALSE,FONT,&w,&h);
 return w;
}

//绘渐变距形(取于小钱)
void srect(int16 x,int16 y,int16 w,int16 h,uint8 r,uint8 g,uint8 b,uint8 r1,uint8 g1,uint8 b1,uint8 mode)
{
int a=r*0x10000+g*0x100+b;
 int a1=r1*0x10000+g1*0x100+b1;
shaderect(x,y,w,h,a,a1,mode);
}



//绘文件列表
void DrawList()
{
 CLS;
 srectUp;
 DrawBt();
 srectDown;
 drect(0,TH,SCRW,TH,50,50,50);

 uint16 SH=SCRH-TH;
 text("File Manage",0,0);
 text("菜单",0,SH);
 text("打开",SCRW/2-TW,SH);
 text("返回",SCRW-TW*2,SH);
 DrawTime(SCRW-TW*2,0);


//调整光标显示
 switch(1)
 {
  case (Focus.num==1):
  Focus.onenum=Focus.num;
  Focus.one=Focus.now;
  break;
  case (Focus.num==Focus.all):
 Focus.onenum=Focus.num;
 Focus.one=Focus.now;
 while(Focus.onenum+DXY.fn-1!=Focus.all)
  {
  if(Focus.one->last!=NULL)    Focus.one=Focus.one->last;
  else break;
  Focus.onenum--;
  }
  break;
  case (Focus.onenum+DXY.fn==Focus.num):
  Focus.onenum++;
  Focus.one=Focus.one->next;
  break;
  case (Focus.num==Focus.onenum-1):
  Focus.onenum=Focus.num;
  Focus.one=Focus.now;
  break;
 }

 LISTST *to=Focus.one;

 uint32 nn,y;
 nn=0;y=DXY.y+TH/2;
 while(nn<DXY.fn)
 {
 if(to->mode==IS_DIR)
 bmpshowflip(DirBmp,10,y,16,16,16,BM_TRANS,0,0,0XF81F);
 else
 bmpshowflip(FileBmp,10,y,16,16,16,BM_TRANS,0,0,0XF81F);

 text(to->name,DXY.x,y);
 if(to==Focus.now)
 effsetcon(0,y,SCRW,TH,250,250,128);
 if(to->next!=NULL)
 to=to->next;
 else break;
 nn++;y+=DXY.h;
 }
 uint16 hh=DXY.h2/Focus.all;
 uint16 yy=Focus.num*hh+DXY.y;
 drect(SCRW-5,yy,5,hh,250,250,150);
REF;
}


char *GetFileLenStr(char *Name)
{
 int len=getlen(Name);
 char str[16];
 memset(str,NULL,8);
 if(len<=KB)
 sprintf(str,"%dB",len);
 else if(len<=MB)
 sprintf(str,"%d.%02dKB",len/KB,len%KB);
 else
 sprintf(str,"%d.%02dMB",len/MB,len%MB);
 return str;
}

void DrawBt()
{
/* srect(0,TH,SCRW,TH,100,150,100,100,250,150,0);
 if(Focus.now->mode==IS_DIR)
 text(Focus.dir,0,TH);
 else
 {
 char *Name=FileName(Focus.dir,Focus.now->name);
 text(GetFileLenStr(Name),0,TH);
 free(Name);
 }
*/
 char str2[12];
 sprintf(str2,"%d/%d",Focus.num,Focus.all);
 text(str2,SCRW-strw(str2),TH);
}



typedef struct
//时间结构
{
 uint16 year;
 uint8  month;
 uint8  day;
 uint8  hour;
 uint8  minute;
 uint8  second;
} datetime;
//显示时间

//绘时间
void DrawTime(int xx,int yy)
{
 datetime date;
 getdatetime(&date);
 char *da=malloc(10);
 sprintf(da,"%d:%02d",date.hour,date.minute,date.second);
 text(da,xx,yy);
 free(da);
}

void DrawMenuList()
{}


void DrawFileMsg()
{
 char *Name,*Kind,*txt;
 uint32 Len;uint8 FILE;
 if(Focus.now->mode==IS_FILE)
 FILE=1;
 if(FILE)
 {
 Name=FileName(Focus.dir,Focus.now->name);
 Len=getlen(Name);
 Kind=FileKind[GetFileKind(Focus.now->name)]; }
 else 
 {
 Name=Focus.now->name;
 Len=strlen(Name);
 Kind="文件夹";
 }
 CLS;
 srectUp;srectDown;
 text("File Image",0,0);
 text("返回",SCRW-TW*2,SCRH-TH);
 if(FILE)
 {
 char *LenStr=GetFileLenStr(Name);
 txt=malloc(strlen(Focus.now->name)+strlen(LenStr)+strlen(Kind)+30);
 sprintf(txt,"文件名: %s\n大小: %s\n类型: %s",Focus.now->name,LenStr,Kind);
 }
 else
 {
 txt=malloc(Len+strlen(Kind)+30);
 sprintf(txt,"文件名: %s\n大小: %d\n类型: %s",Focus.now->name,Len,Kind);
 }
 rect_text(txt,10,TH*2,SCRW,SCRH-TH*3);
 if(FILE)
 free(Name);
 free(txt);
 Jm=Jm_FILE_MSG;
 REF;
}


void DrawPaly()
{
 CLS;srectUp;srectDown;
 text("Paly Music",0,0);

 char *name=strrchr(PALY_MUSIC.filename,'/');
 if(name!=NULL)
 text(name+1,0,DXY.h);
 else
 text(PALY_MUSIC.filename,0,DXY.h);
 text("返回",SCRW-TW*2,SCRH-TH);

 REF;
}
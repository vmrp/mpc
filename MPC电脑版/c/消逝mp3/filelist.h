//*系统没有预先定义这两个接构。*//

enum
{
_MIDI=1,
_WAVE,
_MP3,
_AMR,
_PCM,
_M4A,
_AMR_WB
};

typedef struct
{
int32 pos;
}T_DSM_AUDIO_POS;

#include <sound.h>

int MP3(int t,char *name,int V)
{
if(t==1)
soundinit(_MP3);

else if(t==2)
soundloadfile(_MP3,name);

//播放
else if(t==3)
soundplay(3,1,loop);


//暂停
else if(t==4)
soundpause(_MP3);

//继续
else if(t==5)
soundresume(_MP3);

//停止
else if(t==6)
soundstop(_MP3);

else if(t==7)
soundclose(3);
else if(t==8)
setvolume(V);

return 0;

}

int mp3Head,HowDraw,HowLen,FONT=font,y;

int mp3init(void)
{
 MP3(1,"",1);
 
 HowDraw=SCRH/th/2;
 //计算显示列表数

 FileFind(PATH); //查找文件
 REF;

 dtext("搜索到mp3:",tw,SCRH/2+th,255,255,0,0,font);
 char *n=malloc(10);
 sprintf(n,"%d首",FileN);
 dtext(n,tw*6,SCRH/2+th,255,255,255,0,font);
 free(n); REF;

 jzmusic();
 
 return 0;

}

int listevent(int type, int p1, int p2)
{
 if(type==KY_UP&&(p1==_UP||p1==_DOWN))
{ if(anf)
{timerdel(anf);anf=NULL;}
}

 if(KY_DOWN == type)
 {
  switch(p1)
  {
  case _SRIGHT:
  if(anf)
{timerdel(anf);anf=NULL;}
  y=0;jm=1;kz(0);
  break;

 case _DOWN:
 case _UP: 
 listchangan(p1);
 anf=timercreate();
 timerstart(anf,SHU,p1,"listchangan",1);
  break;

 case _SLEFT:
 case _SELECT: 
 MP3(6,"",1);
 char *name=malloc(NAMELEN);
 sprintf(name,"%s/%s",PATH,FileListHead+y*NAMELEN);
 MP3(2,name,1);
 MP3(3,"",1);
 MP3(8,"",1);
 free(name);
 mp3Head=y;
 jm=2;
 paly++;

 kz(0);
 break;
  }
 }
 return 0;
}

//列表界面长按
void listchangan(int data)
{
if(data==_DOWN)
{ if(y<HowLen)y++;
 else y=0;
}

else 
{if(y>0)y--;
 else y=HowLen;
}
kz(0);
}

#define PATHLEN 200
#define NAMELEN 100

#define TEMPLE "musiclist.t" 
//临时数据文件路径

char *FileListHead=NULL; //文件列表头指针

int16 FileN; //文件数
void FileFind(char *path)
{
  int filehand=open(TEMPLE,8 | 4); 
//创建临时数据文件

 char *temp=malloc(NAMELEN); 

 int hand=findstart(path,temp,NAMELEN);

 findnext(hand,temp,NAMELEN);
 char filetemp[PATHLEN];
 while(1)
 {
  memset (temp,NULL,NAMELEN);
  if (findnext(hand,temp,NAMELEN)==-1)
    break ;
  sprintf (filetemp,"%s/%s",path,temp);
  if (filestate(filetemp)==IS_FILE) //判断是否为文件
   if (strcmped(temp,".mp3")==0||strcmped(temp,".MP3")==0 ) //判断是否为MP3
   {
    write (filehand,temp,NAMELEN); 
    FileN++; //文件数
   }
 }
 HowLen=FileN-1;

 findstop(hand);
 free(temp);
 if(FileListHead != NULL)
 free(FileListHead);
 FileListHead =malloc(FileN*NAMELEN);
 seek (filehand,0,SEEK_SET);
 read (filehand,FileListHead,FileN*NAMELEN); close(filehand); 
 remove(TEMPLE);
}
//画列表
void list()
{
 jpg();
 effsetcon(0,y%HowDraw*th*2-5+th/2,SCRW,th*2,138,138,138);  int i,yy,LEN;
 char *text=malloc(NAMELEN+5);
 if(y<HowDraw)
 {i=0;LEN=HowDraw;}
 
 else 
{ i=y/HowDraw*HowDraw;LEN=i+HowDraw;
}
while(i<LEN)
{
  if(i>=FileN)break;
  yy=i%HowDraw*th*2+th/2;

sprintf(text,"%d.%s",i+1,FileListHead+i*NAMELEN);

if(y==i)
dtext2(text,yy);

else 
{
dtext(text,1,yy+1,r%10+138,g%10+138,b%10+138,0,font);

dtext(text,0,yy,r,g,b,0,font);
}

if(y!=i)
dline(0,yy+th*2-5,SCRW,yy+th*2-5,r,g,b);
i++;

}
 rect(0,SCRH-th,SCRW,th);
 yesno();

//获取光标下歌曲大下
sprintf(text,"%s/%s",PATH,FileListHead+y*NAMELEN);

int Len=getlen(text);
int mb,kb;
mb=Len/1024/1024;
kb=Len/1024%1024;

if(mb>0)
sprintf(text,"%d.%d MB",mb,kb);
else
sprintf(text,"  %d KB",kb);

//dtext(text,SCRW-tw*6+1,(y%HowDraw)*th*2+th+1,r%10+138,g%10+138,g%10+138,0.font);
dtext(text,SCRW-tw*4,(y%HowDraw)*th*2+th/2+th,r+50,g+10,b*2,0,font);

free(text);
REF;
}

//判断scr是否以dest结尾
int strcmped(char *scr,char *dest)
{
 char *Str = scr;
 Str += strlen(scr)-strlen(dest);
 return strcmp(Str,dest);
}

void jzmusic()
{
char *musicname=malloc(NAMELEN*2);
 sprintf(musicname,"%s/%s",PATH,FileListHead+y*NAMELEN);
MP3(2,musicname,1);
MP3(3,"",v);
MP3(8,"",v);
MP3(4,"",v);
free(musicname);
mp3Head=y;

uint8 *time;
getsoundtotaltime(_MP3,&time);
if(time==NULL)alltime=0;
else alltime=(int)(*time);
getsoundcurtime(_MP3,&nowtime);
 
 
}
void dtext2(char *text,int y)
{
rectst rect;
colorst color;
rect.x=0;
rect.y=y;
rect.w=SCRW+tw2;
rect.h=th*3;
color.r=r;
color.g=g;
color.b=b;

dtextex(text,rect.x,rect.y,&rect,&color,2,font);
color.r=color.r%10+138;
color.g=color.b%10+138;
color.b=color.b%10+138;
dtextex(text,rect.x+1,rect.y+1,&rect,&color,2,font);

}
/*
龙魂噬心(6369)
*/

#include "scrollbar.h"
#include "mrc_base.h"
//#include "mrc_base_i.h"
#include "mrc_exb.h"
#include "filefind.h"
#include "type.h"
#include "msgbox.h"
#include "read.h"
#include "win.h"



PScroll FileFindScroll;

char* PathName;
char **FileName;
//char* pread;
int* filetype;

int16 FileN;//文件个数

void *Bmp;//图标指针
int32 timed;//连续按键定时器
int32 fW,fH;//字体宽高
int _msg,__p1,_p2;//Event事件记录，连续按键用
int pagefocus,pagehand,pageend;//页焦点，页首，页末文件序号
int psize ;//每页可显示几行




void loadFileFind (void)
{ 
 int i=0;
FileName=(char**)mrc_malloc(MAX*sizeof(char*));

 while(i<MAX) 
 {
   FileName[i]=(char*)mrc_malloc(NAMELEN*sizeof(char));
//if(0==FileName[i]) return -1;
   i++;
 }
PathName=(char*)mrc_malloc(PATHLEN*sizeof(char));
mrc_memset(PathName,'\0',sizeof(PathName));
 filetype =mrc_malloc(MAX*sizeof(int));
 //pread=mrc_malloc(PATHLEN*sizeof(char)); 
}



//寻找指定目录
void FileFind(char *path)
{
 
 int hand;
  int i=0;
  char filetemp[PATHLEN];
 FileN=0;
 clearbuf();
	 hand=mrc_findStart(path,FileName[0],NAMELEN);
 mrc_findGetNext(hand,FileName[0],NAMELEN);

 
 while(i<MAX)
 {
   if(-1==mrc_findGetNext(hand,FileName[i],NAMELEN))
    break;
   mrc_sprintf(filetemp,"%s/%s",PathName,FileName[i]);
   filetype[i]=mrc_fileState(filetemp);
   i++;
 }
  mrc_findStop(hand);
  FileN=i;
  setScrollBarAllId(FileFindScroll,FileN-1);
setScrollBarNowId(FileFindScroll,0);
  
}


//释放内存
void FileFindDel (void)
{
  int i =0;
  while (i<MAX )
  {
     mrc_free(FileName[i] );
     FileName[i]=0;
    i++;
  } 
  mrc_free(FileName);
  FileName=0;
//  mrc_free (pread);

//pread=0;
  mrc_free(filetype);
filetype=0;
mrc_free(PathName);
PathName=0;
}


//清除缓存内容
void clearbuf(void)
{

int i;

showMsgBox("正在搜索...");

for(i=0;i<=FileN;i++)
 mrc_memset(FileName[i],'\0',sizeof(FileName[i]));
mrc_memset(filetype,0,sizeof(filetype));
}



void DrawMain (void)
{
  shaderect(0,0,SCRW,fH,0x5d5d5d,10,0);
  shaderect(0,SCRH-fH,SCRW,fH,0x5d5d5d,10,0);
  mrc_drawText("文件浏览",2,2,100,150,200,0,FONT);
  mrc_drawText("选择",2,SCRH-fH+2,100,150,200,0,FONT);
  mrc_drawText("返回",SCRW-(fW<<1)-2,SCRH-fH+2,100,150,200,0,FONT);  DrawList();
   
   
   
}







//绘制列表
void DrawList(void)
{
 int a = pagehand;
 mrc_drawRect(0,fH,SCRW,SCRH-(fH*2),0,0,0);
//if(FileN)
//shaderect(5,fH*pagefocus+fH+10,SCRW-10,fH,0x55a0ff,0x0055ff,0);
 while(a <= psize+pagehand
  &&a<FileN)
 {
 if(MR_IS_FILE==filetype[a])
   mrc_bitmapShowEx (Bmp,8,(a-pagehand)*fH+fH+11+(fH-16)/2, 16, 16, 16, BM_TRANSPARENT, 0,17);
//if(IS_DIR==filestate(tdname))

else
   mrc_bitmapShowEx(Bmp,8,(a-pagehand)*fH+fH+11+(fH-16)/2 ,16,16,16,BM_TRANSPARENT,0,0); 

mrc_drawText(FileName[a],30,(a-pagehand)*fH+fH+11+(fH-(fH-5))/2,200,200,200,0,FONT);
a++;
 }   
  pageend = a;
  if(a) 
   dorect(5,fH*pagefocus+fH+10,SCRW-10,fH,200,200,200);
   
drawScrollBar(FileFindScroll);
mrc_refreshScreen(0,0,SCRW,SCRH);
   
   
}


//焦点绘制
void dorect(int x,int y,int w,int h,int r,int g,int b)
{
  mrc_drawLine(x+1,y,x+w-1,y,r,g,b);
  mrc_drawLine(x+w,y+1,x+w,y+h-1,r,g,b);
  mrc_drawLine(x+w-1,y+h,x+1,y+h,r,g,b);
  mrc_drawLine(x,y+h-1,x,y+1,r,g,b);
}
   



//加载图片（需用free释放），返回其地址
void *LoadBmp(char *FileName)
{
 int32 len,f;
 uint8 *Buf=NULL;
/*
 len = mrc_getLen(FileName);
 Buf = mrc_malloc(len);
 f = mrc_open(FileName,1);
 mrc_read(f,Buf,len);
 mrc_close(f);
*/
 mrc_readFileFromMrpEx(mrc_getPackName(), FileName,&Buf,&len,  0);
 return Buf;
}




//反回上级目录

int back(void)
{
char*a;
a=mrc_strrchr(PathName,'/');
if(a==0)
  return -1;
clearbuf();
mrc_memset(a,'\0',sizeof(a));pagehand=pageend=pagefocus=0;
FileFind(PathName);
DrawList();
return 0;
}



//焦点点击
int donext(void)
{
char temp[PATHLEN];
mrc_sprintf(temp,"%s/%s",PathName,FileName[pagehand+pagefocus]);
if(MR_IS_FILE==mrc_fileState(temp))
{
win=WIN_READ;
readExit();
readInit(temp);
 return 0;
}else{
mrc_sprintf(PathName,"%s/%s",PathName,FileName[pagehand+pagefocus]);
pagehand=pageend=pagefocus=0;
clearbuf();

FileFind(PathName);
DrawList();
return 0;}
}

//事件处理
int fileFindEvent(int msg,int p1,int p2)
{
// _msg = msg;
// __p1 = p1;
// _p2 = p2;
 if(msg == MR_KEY_PRESS)
 {
  _time(p1);
  mrc_timerStart(timed, 100, p1, (mrc_timerCB)_time,1);
 }
 if(msg == MR_KEY_RELEASE)
 {
  mrc_timerStop(timed);
  switch(p1)
  {
   case MR_KEY_SOFTRIGHT:
      if(-1==back())
      {
       win=WIN_READ;
       mrc_clearScreen(200,200,200);

       sek=nsek;
       downPaiBan();
     }
      break;
   case MR_KEY_SELECT:
   case MR_KEY_SOFTLEFT:
      donext();
break;
  }    
 }
return 0;
}



//连续按键事件回调
void _time(int a)
{
//if(_msg == KY_DOWN)
{
  switch(a)
  {
    case MR_KEY_LEFT:
      pagehand = pagehand - psize-1;
      if(pagehand < 0)
      {
        pagehand = 0;}
      if(psize>FileN)
        pagefocus = 0;
      break;


    case MR_KEY_RIGHT:
      pagehand = pageend;
      if(pagehand >FileN - psize-1)
      {
        pagehand = FileN - psize-1;}
      if(psize>FileN-1)
       { pagehand=0; pagefocus=FileN-1;}
      break;


    case MR_KEY_UP:
      if(pagefocus>0)
        pagefocus--;
      else
      {
        pagehand--;
        if(pagehand<0)
        {
          if(FileN>psize)
          {
             pagehand=FileN-psize-1;
           pagefocus=psize;
           }else
          {
         pagehand=0;pagefocus=FileN-1;
          }


        }
      }
      break;


    case MR_KEY_DOWN:
      if(pagefocus < psize)
      {
        pagefocus++;
        if(pagefocus>FileN-1)
          pagefocus=0;
      }
      else{
        pagehand++;
        if(pagehand+psize+1>FileN)
        {
          pagehand=0;
          pagefocus=0;
        }
      }
      break;
  }
}
setScrollBarNowId(FileFindScroll, pagehand+pagefocus);
DrawList();
mrc_refreshScreen(0,0,SCRW,SCRH);
//return 0;
}



//初始化函数，请一定在使用其它之前调用
int fileFindInit(void)
{
	uint8* output;
	int32 output_len;
  mrc_textWidthHeight("鼎",0,FONT,&fW,&fH);
  fH+=5;
  psize=(SCRH-fH*2-10)/fH-1;
  FileFindScroll=scrollBarCreate(SCRW-4,fH,4,SCRH-fH*2,psize,0x000000,0x0055ff);
  timed=mrc_timerCreate();
  Bmp=LoadBmp(BMP);
  //memset(PathName,0,PATHLEN);
  mrc_platEx( 1204, "c:/", 3, &output, &output_len, NULL );
  loadFileFind();
  FileFind(PathName);
  DrawMain();
  return 0;
}



//退出函数
int fileFindExit(void)
{
  if(timed) mrc_timerDelete(timed);
  FileFindDel();
  scrollBarDel(FileFindScroll);
  mrc_free(Bmp);
  Bmp=0;
  
  return 0;
}

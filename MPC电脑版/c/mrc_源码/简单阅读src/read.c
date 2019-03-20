/*龙魂噬心(6369) 
*/

#include "msgbox.h"
#include "mrc_base.h"
#include "read.h"
#include "type.h"
#include "win.h"
#include "msgbox.h"
#include "readmenu.h"
#include "JUMP.h"
#include "BK.h"



int32 size,  sek, nsek;//文件长度，文件句柄，当前指针，当页首指针
int32 f;
int16 xs;//行像素

char* fbuf;//排版缓冲

mr_screenRectSt rect;//显示区域
mr_colourSt bclr, qclr;//背景，前景

int32 btupclr,btdownclr;//标题栏颜色
int pin,fu;//全屏开关，皮肤变量
int siz;//显示区域距离四边的距离，就是外框宽度
int32 zw,zh; 



int readInit(char* name)//阅读初始化
{
	mrc_textWidthHeight("鼎",0,FONT,&zw,&zh);
	zh+=5;
    xs = 4;
	siz=5;
   pin=0;
   fu=0;
   huanPin();
   huanFu(); 
//    mrc_clearScreen(200,200,150);
    if(MR_IS_FILE != mrc_fileState(name))
    {
/*        cls(100,100,100);
ref(0,0,SCRW,SCRH);
        showMsgBox((SCRW-zw*5-10)/2,(SCRH-zh)/2,"请选择文件");
*/
     return -1;
    }
         
//取阅读记录
	if(1==mrc_fileState(SEKFILE))
	{
		size=mrc_getLen(SEKFILE);
		f=mrc_open(SEKFILE,1);
		mrc_read(f,&nsek,size);
		mrc_close(f);
		sek=nsek;
	}
	else 
		sek = nsek = 0;

    size = mrc_getLen(name);
    f = mrc_open(name,1);
    
    fbuf = (char*) mrc_malloc(2);
    mrc_memset(fbuf, '\0', 2);
    downPaiBan();
    return 0;
}




int readExit(void)//退出阅读
{
	if(f)

	    mrc_close(f);
	if(fbuf!=NULL) 
		mrc_free(fbuf);
    fbuf = 0;
//保存阅读记录
    f=mrc_open(SEKFILE,2|8);
    mrc_seek(f,0,MR_SEEK_SET);
    mrc_write(f,&nsek,4);
    mrc_close(f);
    return 0;
}


int readEvent(int msg,int p1,int p2)//阅读界面事件处理
{
  if(msg==MR_KEY_PRESS)
  {
    switch(p1)
    {
      case MR_KEY_DOWN:
      case MR_KEY_RIGHT:
      case MR_KEY_6:
      case MR_KEY_8://下翻页
        downPaiBan();
        break; 

      case MR_KEY_UP:
      case MR_KEY_LEFT:
      case MR_KEY_4:
      case MR_KEY_2://上翻页
      upPaiBan();
      break;
    }
  }


  if(msg==MR_KEY_RELEASE)
  {
    switch(p1)
    {
      case MR_KEY_SOFTLEFT:
        win=WIN_MENU;
  //      
        drawreadmenu();
        break;
      case MR_KEY_SOFTRIGHT://关闭阅读
        //readExit();exit();
        break;
      case MR_KEY_1://消除乱码
        sek = nsek - 1;
        downPaiBan();
        break;
      case MR_KEY_3://跳到n字节
        win=WIN_INJUMP;
        inJumpInit();
        break;
      case MR_KEY_7://开关全屏
        huanPin();
        sek = nsek;
        downPaiBan();
        break;
      case MR_KEY_9://边框宽度
        win=WIN_INBK;
        inBKInit();
        break;
      case MR_KEY_STAR://跳到文件首
        sek = 0;
        downPaiBan();
        break;
      case MR_KEY_POUND://跳到文件末
        sek = size - 20;
        downPaiBan();
        break;
      case MR_KEY_0://切换皮肤
              huanFu();
              if(pin) func();
        sek = nsek;
        downPaiBan();
        break;
    }
  }
  return 0;
}


int downPaiBan(void)
{

  //downPaiBan();
      int32 zw, zh;
    int16 x = rect.x+ siz, y = rect.y + siz;
    nsek = sek;

//if(!f) return -1;
if(pin) {func();}
    mrc_drawRect(rect.x, rect.y, rect.w, rect.h, bclr.r, bclr.g, bclr.b-bclr.b/3);
    mrc_drawRect(rect.x+siz, rect.y+siz, rect.w-siz*2, rect.h-siz*2, bclr.r,bclr.g,bclr.b);

	if(sek>=size)
	{ 
	mrc_refreshScreen(0,0,SCRW,SCRH);
	return 0;
	}

	if(!f)
	{ 
	mrc_refreshScreen(0,0,SCRW,SCRH);
	return -1;
	}
    while(sek<=size)
    {
         mrc_memset(fbuf,'\0',2);
         mrc_seek(f, sek, MR_SEEK_SET);
         mrc_read(f, fbuf ,2);
//如果是非汉字，只读取一个字节
        if(0<=fbuf[0])
           if(fbuf[0]<=127)
            fbuf[1]='\0';
        mrc_textWidthHeight(fbuf, 0, FONT,&zw,&zh);


//如果是换行符
        if(fbuf[0] == 13)
        {
            x = rect.x+siz;
            y = y + zh + xs;
            sek = sek + 2;
            continue;//结束本次循环
        }
//文字一行大于区域宽
        if(x + zw > rect.w+rect.x - siz)
        
        {
            x=rect.x+siz;
            y=y+zh+xs;
        }
//如果排满页
        if(y+zh > rect.h+rect.y-siz)
            {
                mrc_refreshScreen(0,0,SCRW,SCRH);
                return 0;
            }
        mrc_drawText(fbuf, x, y, qclr.r, qclr.g, qclr.b, 0, FONT);
      //  mrc_refreshScreen(x,y,zw,zh);
//如果是非汉字则读一字节，否则2字节。
        if((0<=fbuf[0]) && (fbuf[0]<=127))
            sek = sek + 1;
        
        else sek = sek + 2;
        x=x+zw;
    }
    mrc_refreshScreen(0,0,SCRW,SCRH);
    return 0;
}



int upPaiBan(void)//上翻页
{
	sek=nsek-500;
	mrc_seek(f, sek, MR_SEEK_SET);
	mrc_read(f, fbuf ,2);
	if(fbuf[0]>=0&&fbuf[0]<=127)
		sek=sek+1;

  downPaiBan();
 
  
  return 0;
}


void huanFu(void)//切换皮肤
{
  fu++;
  if( fu >= 2 )
    fu = 0;
  switch(fu)
  {
    case 0:
      qclr.r = 20;
      qclr.g = 20;
      qclr.b = 20;

      bclr.r = 255;
      bclr.g = 255;
      bclr.b = 255;
      btupclr = 0x55ffff;
      btdownclr=0x00a0a0;
      break;
    case 1:
      qclr.r = 100;
      qclr.g = 100;
      qclr.b = 100;
      bclr.r = 20;
      bclr.g = 20;
      bclr.b = 20;
      btupclr=0x555555;
      btdownclr=0x000000;
      break;
  }
 
}



void huanPin(void)//切换全屏
{
  
  pin = !pin;
  if(pin == 0)
  {
    rect.x = 0;
    rect.y = 0;
    rect.w = SCRW;
    rect.h = SCRH;

  }
  else
  {
    rect.x = 0;
    rect.y = zh;
    rect.w = SCRW;
    rect.h = SCRH - zh*2;
    func();
  }
}

void func()
{
  showTitleBar(0,0,SCRW,zh,btupclr,btdownclr,MAKERGB(qclr.r,qclr.g,qclr.b),"简单阅读");
showToolBar(0,SCRH-zh,SCRW,zh,btupclr,btdownclr,MAKERGB(qclr.r,qclr.g,qclr.b),"菜单","","");

}


/*
* BookView(简单文本浏览器)
* By:天天
* QQ:643552878
* Date:2012.8.29
* 接口:
 初始化 BVInit(char *FileName);
 按键 BVEvent(int type,int p1,int p2);
* 说明:退出自动保存阅读进度、此源码仿自龙魂噬心、
* 帮助:左(4)、右(6)键翻页、0键切换皮肤、*键跳置开头、#键跳置结尾(有bug)、1键消乱码
*/

//阅读字体
#define BFONT 1

//阅读记录文件后缀
#define SNF ".mark"

struct BView
{
 char *name;//文件名
 char *SekName;//阅读记录文件名
 uint32 size;//文件长度
 int f;//文件句柄
 int sek;//文件头指针位置
 uint32 nsek;//当前页首指针位置
 uint32 lsek;//上一次位置
 uint8 xs;//字间间隔(像素)
 uint8 ys;//行间间隔(像素)
 uint8 kin;//皮肤(0黑夜、1白天)
};

struct BView *BV;
colorst
*Bclr,//前景色
*Bgclr;//背景色

rectst *Brect;//阅读文本框

/*初始化入口，成功返回 0，失败返回 -1*/
int BVInit(char *FileName)
{
 if(getlen(FileName)<0)
 return -1;

 int l=sizeof(struct BView);
 BV=(struct BView*)malloc(l);
 memset(BV,0,l);

 BV->name=(char*)malloc(strlen(FileName));
 strcpy(BV->name,FileName);
 BV->SekName=(char*)malloc(strlen(BV->name)+strlen(SNF)+1);
 strcpy(BV->SekName,BV->name);
 strcat(BV->SekName,".mark");
 BV->size=getlen(BV->name);
 BV->f=open(BV->name,1);
 BV->xs=2;
 BV->ys=10;
 BV->kin=1;
 BV->nsek=0;

//前景色
 Bclr=malloc(3);
 memset(Bclr,255,3);
//背景色
 Bgclr=malloc(3);
 memset(Bgclr,0,3);
//阅读框
 Brect=malloc(8);
 memset(Brect,0,8);
 Brect->x=0;
 Brect->y=0;
 Brect->w=SCRW;
 Brect->h=SCRH;

 
//读取记录
 if(4==getlen(BV->SekName))
 {
 int f=open(BV->SekName,1);
 read(f,&BV->nsek,4);
 close(f);
 BV->sek=BV->nsek;
 }

//如果文件不存在
 if(BV->size<0)
 {
 BVExit();
 return -1;
 }

 HuanSkin();//切换皮肤
 NextPage();
 return 0;
}

//退出
int BVExit()
{
//保存阅读记录
 int f=open(BV->SekName,2|8);
 seek(f,0,SEEK_SET);
 write(f,&(BV->nsek),4);
 close(f);

 free(BV->name);
 free(BV->SekName);
 close(BV->f);
 free(BV);
 free(Bclr);
 free(Bgclr);
 free(Brect);

 BackFV();
 return 0;
}
//按键入口
int BVEvent(int type, int p1, int p2)
{
 if(KY_DOWN==type)
 switch(p1)
 {
 case _SRIGHT:
 BVExit();
 break;

 case _0:
 HuanSkin();
 //DrawBook();
 break;

 case _1:
 GoPage(BV->nsek+1);
 break;

 case _STAR:
 GoPage(0);
 break;

 case _POUND:
 GoPage(BV->size-50);
 break;

 case _4:
 case _LEFT:
 LastPage();
 break;

 case _6:
 case _RIGHT:
 NextPage();
 break;
 }

 return 0;
}

//绘文本
int DrawBook()
{
 int zw,zh;
 uint8 xs=BV->xs,ys=BV->ys;
 int16 x=Brect->x,y=Brect->y;
 char buf[2];
 BV->nsek=BV->sek;

 drect(Brect->x,Brect->y,Brect->w,Brect->h,Bgclr->r,Bgclr->g,Bgclr->b);
 ref(0,0,SCRW,SCRH);
 while(BV->sek<=BV->size)
 {
 memset(buf,'\0',2);
 seek(BV->f,BV->sek,SEEK_SET);
 read(BV->f,buf,2);
 if(0<=buf[0])
  if(buf[0]<=127)
 buf[1]='\0';
 textwh(buf,0,BFONT,&zw,&zh);

//printf("strw=%d,strh=%d,seek=%d\n",zw,zh,sek);

//如果是换行符
 if(buf[0]==13)
 {
  x=Brect->x;
  y=y+zh+ys;
  BV->sek+=2;
  continue;//结束本次循环
  }

//文字一行大于区域宽
 if(x+zw>Brect->w+Brect->x)
 {
 x=Brect->x;
 y=y+zh+ys;
 }

//如果排满页
 if(y+zh>Brect->y+Brect->h)
 goto OVER;

// ref(x,y,2,zh);
 dtext(buf,x,y,Bclr->r,Bclr->g, Bclr->b,FALSE,BFONT);
  ref(x-5,y-5,zw+10,zh+10);

//如果是非汉字则读一字节，否则2字节。
 if((0<=buf[0])*(buf[0]<=127))
 BV->sek+=1;
 else
 BV->sek+=2;
 x=x+zw;
 }
 OVER:
 ref(0,0,SCRW,SCRH);
 return 0;
}

//上翻页
int LastPage()
{
/* if(BV->sek==BV->nsek==0)
 return -1;*/
 BV->sek=BV->lsek;
 DrawBook();
 return 0;
}

//下翻页
int NextPage()
{
/* if(BV->nsek>=BV->size)
 return -1;*/
 BV->lsek=BV->nsek;
 BV->nsek=BV->sek;
 DrawBook();
 return 0;
}

/*跳转页面，len:跳转进度，注:跳转可能会出现乱码！*/
int GoPage(uint32 len)
{
 if(len>BV->size)
 return -1;

 BV->nsek=len;
 BV->sek=len;
 BV->lsek=len-100;
 DrawBook();
 return 0;
}


//换皮肤
int HuanSkin()
{
 if(!BV->kin)
 {
 Bclr->r=255;
 Bclr->g=255;
 Bclr->b=255;
 
 Bgclr->r=0;
 Bgclr->g=0;
 Bgclr->b=0;
 }
 else
 {
 Bclr->r=0; 
 Bclr->g=0;
 Bclr->b=0;
 
 Bgclr->r=255;
 Bgclr->g=255;
 Bgclr->b=255;
 }
 BV->kin=!BV->kin;
 return 0;
}

//By:天天
//下面用两张图片当做背景。分别用直接绘图和用屏幕指针相比较！
#include <base.h>
#define REF ref(0,0,SCRW,SCRH)

#define photo "tu.jpg"
//两张图片路径，自己改。
#define photo2 "风景.jpg"


uint16 *scf=getscrbuf();
//获得屏幕缓冲地址
uint16 *tu=malloc(SCRW*SCRH*2);
//申请存放图片指针内存

int init()
{
 img(photo,0,0);
 //绘图片
 effsetcon(10,10,SCRW-20,SCRH-20,138,200,138);
 //对比度
 memcpy(tu,scf,SCRW*SCRH*2);
 //用屏幕指针复制一张bmp图片

 cls(0,0,0);
 dtext("按左右键切换界面",20,SCRH/2,255,255,0,0,1);
 REF;
return 0;
}

int event(int type, int p1, int p2)
{
if(KY_DOWN == type)
 {
 switch(p1)
 {
   case _SRIGHT:
  free(tu);
 //释放内存
  exit();break;

  case _LEFT:
  zzref();break;

 case _RIGHT:
 zjref();break;
}


}
return 0;
}

int pause()
{return 0;}
int resume()
{return 0;}


//直接绘图
void zjref()
{
 int px;
 for(px=0;px<=SCRW;px+=20)
 {
  img(photo2,px-SCRW,0);
  effsetcon(px-SCRW+10,10,SCRW-20,SCRH-20,138,200,138);
  dtext("令人…速度",px-SCRW/4*3,100,255,255,255,0,1);
  REF;
 }

}

//直接绘
void zzref()
{
 int px;
 for(px=0;px<=SCRW;px+=20)
 {
  bmpshowflip(tu,SCRW-px,0,SCRW,SCRW,SCRH,BM_COPY,0,0,0);
dtext("这是啥速度！！",SCRW-px+SCRW/4,100,255,255,255,0,1);
  REF;
 }

}

/*by:每天进步一点
怎样，用屏幕指针速度快吧！！
*/
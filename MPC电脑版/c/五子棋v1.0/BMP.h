//绘出棋盘的图像。
/////
#define BMPW 240
#define BMPH 252

void qp()
{
    int32 f;
    int32 len;
    uint16 *ptr;
    char *filename="c/五子棋v1.0/bmp/棋盘.BMP";

    len=getlen(filename);
//取得文件长度
    f=open(filename,1);
//以只读方式打开文件
    ptr=malloc(len);
//申请图片存放的内存
    read(f,ptr,len);
//把图片读取到内存中
    close(f);
//关闭文件

drect(0,0,BMPW,BMPH,r1,g1,b1);

bmpshowflip(ptr,0,0,BMPW,BMPW,BMPH,BM_COPY,0,0,0);

free(ptr);

}


///添充棋盘////

#define BMPW2 12
#define BMPH2 12

void qp2(int j,int k)
{

 int32 f;
    int32 len;
    uint16 *ptr;
    char *filename="c/五子棋v1.0/bmp/棋盘添充.bmp";

    len=getlen(filename);
//取得文件长度
    f=open(filename,1);
//以只读方式打开文件
    ptr=malloc(len);
//申请图片存放的内存
    read(f,ptr,len);
//把图片读取到内存中
    close(f);
//关闭文件

bmpshowflip(ptr,j,k,BMPW2,BMPW2,BMPH2,BM_COPY,0,0,0);

free(ptr);

}


void qz_h(int x1,int y1)
{
 if(Q[x1][y1]==1)
drect(x1,y1,12,12,138,138,138);

else if(Q[x1][y1]==2)
drect(x1,y1,12,12,255,0,0);

}

//绘出全棋盘上的棋子
void qzfb()
{ 
 cls(r1,g1,b1);
 qp();bt();
 int j,k;
 for(j=0;j<=18;j++)
 for(k=0;k<=18;k++)
{
 if(Q[j][k]==1)
 drect(j*12+6,k*12+6,12,12,138,138,138);

 else if(Q[j][k]==2)
  drect(j*12+6,k*12+6,12,12,255,0,0);

 }
ref(0,0,SCRW,SCRH);

}

//在屏幕x,y处绘文字，当j不为0时可以用来加粗字体
void txt(char *text,int x1,int y1,int j)
{
 int k;
 for(k=0;k<=j;k++) dtext(text,x1+k,y1+k,r2,g2,b2,0,1);

}
//开始动画

void start()
{
 cls(0,0,0);
 int x1;
 for(x1=0;x1<=SCRH/2-30;x1+=5)
 {
 cls(0,0,0);
 txt("无所谓、忧乐作品",10,x1,1);
 txt("游戏名: 五子棋 ",50,SCRH-x1,1);
 ref(0,0,SCRW,SCRH);
 }
 sleep(1000);

 cls(0,0,0); 
 txt("正在加载…",SCRW/2-50,SCRH/2,1);
 ref(0,0,SCRW,SCRH);


drect(0,SCRH/2+50,SCRW,40,150,150,150);
ref(0,0,SCRW,SCRH);


drect(5,SCRH/2+51,SCRW-10,38,r2,g2,b2);

for(x1=5;x1<=SCRW-5;x1+=5)
{
 sleep(x1/20);
 x1-=x1/60;
 ref(0,0,x1,SCRH);
}


}

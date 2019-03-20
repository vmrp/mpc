#define FONT 1

//每个格子宽度
#define WLEN 15

//每个格子高度
#define HLEN 15


int tw,th;
textwh("天",FALSE,FONT,&tw,&th);


uint16 WN=SCRW/WLEN,HN=(SCRH/HLEN);


uint16 *scr=getscrbuf();


uint16
*BmpWall,//墙
*BmpFloor,//地板
*BmpBomb,//炸弹
*BmpMan,//人
*BmpMonster;//怪

enum
{
 IS_WALL,
 IS_FLOOR,
 IS_BOMB,
 IS_MAN,
 IS_MONSTER
};


void BmpStart()
{
BmpWall=OpenBmpFile("c/炸弹超人/BMP/wall.bmp");
  BmpFloor=OpenBmpFile("c/炸弹超人/BMP/floor.bmp");
 BmpBomb=OpenBmpFile("c/炸弹超人/BMP/bomb.bmp");
}

void BmpFree()
{
 free(BmpWall);
 free(BmpFloor);
 free(BmpBomb);
}

void* OpenBmpFile(char *filename)
{
 int32 f;
 int32 bmplen;
 bmplen=getlen(filename);
 f=open(filename,1);
 uint16 *tu=malloc(bmplen);
 read(f,tu,bmplen);
 close(f);
 return tu;
}

void DrawMap()
{
 ShowMsg("绘制地图中…");
 cls(255,255,255);

 uint16 x,y;
 for(x=0;x<=WN;x++)
 for(y=0;y<=HN;y++)
 {
 uint8 k=rand()%3+1;
 if(k==1)
 DrawBmp(IS_WALL,x*WLEN,y*HLEN);
 else if(k==2)
 DrawBmp(IS_FLOOR,x*WLEN,y*HLEN);
 else DrawBmp(IS_BOMB,x*WLEN,y*HLEN);
 ref(x*WLEN,y*HLEN,WLEN,HLEN);
 }
 ref(0,0,SCRW,SCRH);
}





void DrawBmp(uint8 kind,uint16 x,uint16 y)
{
 uint16 *bmp;
 switch(kind)
 {
  case IS_WALL:
  bmp=BmpWall;break;
  case IS_FLOOR:
  bmp=BmpFloor;break;
  case IS_BOMB:
  bmp=BmpBomb;break;
  case IS_MAN:

  case IS_MONSTER:
  break;
 }
 bmpshowflip(bmp,x,y,WLEN,WLEN,HLEN,BM_TRANS,0,0,0xFF);

}
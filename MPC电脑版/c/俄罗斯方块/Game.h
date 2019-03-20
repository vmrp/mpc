
#define NW 14
#define NH 21

uint8 nw=NW-2,nh=NH;

uint16 AnW=SCRW/NW;
uint16 AnH=SCRH/NH;

rectst GameRect;
GameRect.x=GameRect.y=0;
GameRect.w=SCRW-AnW*4;
GameRect.h=SCRH;


uint8 Q[NW-2][NH];
memset(Q,NULL,nw*nh*sizeof(uint8));


uint8 x1,x2,x3,x4,y1,y2,y3,y4;
/*4个方块的位置*/

uint32 score,nspeed=1,ndifficult=1;/*分数、速度、难度*/
uint32 timer;/*定时器句柄*/
uint8 state;/*游戏状态*/
uint8 GameMenuOption;/*游戏菜单选项*/


int GameEvent(int type,int p1,int p2)
{
 if(type==KY_DOWN)
 switch(p1)
 {
  case _SRIGHT:
  Jm=Jm_Menu;
  DrawMenu();break;
 }
 return NULL;
}

void DrawGameMain()
{
 drect(GameRect.x,GameRect.y,GameRect.w,GameRect.h,250,0,0);
 drect(GameRect.x+2,GameRect.y+2,GameRect.w-4,GameRect.h-4,150,250,250);
 DrawGameRight();
 ref(0,0,SCRW,SCRH);

}

void DrawGameRight()
{
 drect(GameRect.w,0,SCRW-GameRect.w,SCRH,0,0,0);

 colorst rgb;
 rgb.r=rgb.g=rgb.b=255;
 rectst rect;
 rect.x=GameRect.w;
 rect.y=(SCRH-th*6)/2;
 rect.w=SCRW-rect.x;
 rect.h=SCRH-rect.y;
 char str[50];
 sprintf(str,"分数:\n%d\n速度:\n%d\n难度:\n%d",score,nspeed,ndifficult);
 dtextex(str,rect.x,rect.y,&rect,&rgb,2,FONT);
 ref(rect.x,rect.y,rect.w,rect.h); 
}

/*设置3面墙*/
void SetWall()
{
 uint32 i;
 for(i=0;i<nh;i++)
 Q[0][i]=Q[nw-1][i]=1;
 for(i=0;i<nw;i++)
 Q[i][nh-1];
}

void GetShape(uint8 a,uint8 b)
{


}

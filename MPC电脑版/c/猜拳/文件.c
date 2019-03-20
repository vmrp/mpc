#include <base.h>
#include <c/猜拳/day.h>

uint8 l,/*当前局数*/
z,/*玩家赢的局数*/
fen;/*玩家分数*/

uint8 kindA,/*电脑方出拳类型*/
kindB;/*玩家出拳类型*/

/*出拳类型*/
enum
{ stone=1,/*石头=1*/
 scissor,/*剪刀=2*/
 paper/*布=3*/
};
char *Lei[3]={"石头","剪刀","布"};


int j=-1;
/*
-1关于界面
0猜拳结果界面
1主界面
2输入界面
*/

int init()
{
 about();

 printf("石头=%d,剪刀=%d,布=%d\n\n",stone,scissor,paper);

 return 0;
}
void inits()
{
 if(l)printfAll();
 printf("\n初始化\n");

 int h2=SCRH/2;
 drect(0,h2,SCRW,th,255,250,250);
 drect(1,h2+1,SCRW-2,th-2,0,0,0);
 text("初始化",(SCRW-tw*2)/2,h2,55,255,255);
 REF;sleep(800);
 l=z=fen=0;
 kindA=kindB=0;
 dawMain();
}

int event(int type, int p1, int p2)
{
if(type==KY_UP)
{
 if(j<0)
 {j=1;inits();}

 else if(j==0)
 {j=1;dawMain();}

 else if(j==1)
 switch(p1)
  {
  case _SRIGHT:
  exitapp();break;
  case _0:inits();break;


  case _1:
    do{kindA=rand()%3;}
  while(kindA==0);

  dawA();REF;
  break;

 case _SLEFT:
 j=2;dawScanf();  break;

 case _SELECT:
 if(kindA&&kindB)
 dawWin();break;

 }

 else if(j==2)
 { if(p1>0&&p1<4)
  { kindB=p1;
  dawScanf();
  }
  else if(p1==_SLEFT)
  {j=1;dawMain();}
  else if(p1==_SRIGHT)
 {j=1;kindB=0;dawMain();}

 }

  }
 return 0;
}

int pause(void)
{return 0;}
int resume(void)
{return 0;}

void printfAll()
{
 printf("\n\n游戏结束:\n总共%d局,你赢了%d局,分数%d",l,z,fen);
}
void exitapp()
{ dawExit();
 exit();
}


void dawMain()
{
 int h=SCRH-th;
 cls(100,250,250);
 drect(0,0,SCRW,th,150,200,200);
 drect(0,h,SCRW,th,150,200,200);
 text("MPC、猜拳",(SCRW-tw*5)/2,0,255,0,25);
 text("出拳",0,h,255,255,0);
 text("退出",SCRW-tw*2,h,255,0,0);
 text("结果",SCRW/2-tw,h,255,25,255);
 dawA();
 dawB();
 dawText();

 REF;
}

/*绘电脑出拳状态图*/
void dawA()
{
 char *txt;
 txt=(kindA?"电脑:已出":"电脑:未出");
 drect(tw,th*3,SCRW,th,100,250,250);
 text(txt,tw,th*3,150,50,50);
}

/*绘玩家出拳状态*/
void dawB()
{
 drect(tw,th*5,SCRW,th,100,250,250);
 text("玩家:",tw,th*5,150,150,25);
 if(kindB)
 text(Lei[kindB-1],tw*3+5,th*5,150,50,50);
else text("未出",tw*3+5,th*5,150,50,50);
}

void dawText()
{
 printf_d("当前:第%d局",l,tw,SCRH-th*5,55,55,55);
 printf_d("已胜:%d局",z,tw,SCRH-th*4,55,55,55);
 printf_d("分数:%d",fen,tw,SCRH-th*3,55,55,55);
}


/*判断输赢，并绘出结果*/
void dawWin()
{
 int a;
/*取值:
-1 电脑赢
0  平局
1  玩家赢
*/
 if(kindA==kindB)a=0;
 
 else if(kindA==stone)
 { if(kindB==scissor)a=-1;
  else a=1;
 }
 else if(kindA==scissor)
 {if(kindB==stone)a=1;
  else a=-1;
 }
 else if(kindA==paper)
 {if(kindB==stone)a=-1;
  else a=1;
 }

 int h2=SCRH/2;
 effsetcon(0,h2,SCRW,th*3,128,128,128);

 char *s1,*s2,*s3,*s4,*s5;
 s1=Lei[kindA-1];
 s2=Lei[kindB-1];

 if(a==-1)s3="电脑胜！";
 else if(a==0)s3="平局！";
 else if(a)s3="玩家胜";

 s4="你出:%s,电脑出:%s\n结果:%s\n按任意键返回";

 s5=malloc(strlen(s1)+strlen(s2)+strlen(s3)+strlen(s4));
 sprintf(s5,s4,s2,s1,s3);
 rect_text(s5,0,h2,SCRW,SCRH,255,255,255);

 printf("第%d局:\n%s\n\n",++l,s5);
 free(s5);
 h2+=th*3;

 for(int y=SCRH/2;y<=h2;y+=5)
 ref(0,y,SCRW,5);
 REF;

 if(a)
 {z++;fen++;}
  kindA=kindB=0;
 j=0;

}


/*玩家出拳图*/
void dawScanf()
{
 int h2=SCRH/2;
 drect(0,h2,SCRW,th*3,55,55,55);

 rect_text("按1~3分别为石头、剪刀、布\n请选择:",0,h2,SCRW,SCRH,255,255,255);
 char *s;
 if(kindB!=NULL)
 {s=Lei[kindB-1];
 text(s,tw*4,h2+th*2,255,255,0);
 }
 h2+=th*4;
 for(int y=SCRH/2;y<=h2;y+=5)
 ref(0,y,SCRW,5);
 REF;
}
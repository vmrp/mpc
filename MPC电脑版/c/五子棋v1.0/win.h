/*
判断输赢函数
向8个方向检查是否有5字连棋
*/

int win()
{
 int c,i;

//1.向上检查
 for(i=c=0;i<4;i++)
{
  if(y-i==0)break;
  if(Q[x][y-i]==Q[x][y-i-1])
 c++;
 else break;
}
 if(c==4) return 1;
 
//2.向下检查
 for(c=i=0;i<4;i++)
{ 
 if(y+i==18)break;
 if(Q[x][y+i]==Q[x][y+i+1])
 c++;
 else break;
}
 if(c==4) return 1;

//3.向左检查 
 for(c=i=0;i<4;i++)
 {
 if(x-i==0)break;
 if(Q[x-i][y]==Q[x-i-1][y])
 c++;
 else break;

}
 if(c==4)return 0;

//4.向右检查
 for(c=i=0;i<4;i++)
{ 
 if(x+i==18)break;
 if(Q[x+i][y]==Q[x+i+1][y])
 c++;
 else break;
}
 if(c==4)return 1;


//5.左上检查
 for(c=i=0;i<4;i++)
{
 if(x-i==0||y-i==0)break;
 if(Q[x-i][y-i]==Q[x-i-1][y-i-1])
 c++;
 else break;
}
if(c==4)return 1;


//6.右上检查
for(c=i=0;i<4;i++)
{
 if(x+i==18||y-i==0)
 break;

 if(Q[x+i][y-i]==Q[x+i+1][y-i-1])
 c++;
 else break;
}
if(c==4)return 1;


//7.左下检查
for(c=i=0;i<4;i++)
{
 if(x-i==0||y+i==18)
 break;
 if(Q[x-i][y+i]==Q[x-i-1][y+i+1])
 c++;
 else break;
}
if(c==4)return 1;


//8.右下检查
for(c=i=0;i<4;i++)
{
 if(x+i==18||y+i==18)break;
 if(Q[x+i][y+i]==Q[x+i+1][y+i+1])
 c++;
 else break;
}
if(c==4)return 1;

return 0;

}

void chushi()
{
effsetcon(0,0,SCRW,SCRH,128,128,128);

txt("初始化…",SCRW/2-40,SCRH/2,1);

ref(0,0,SCRW,SCRH);
sleep(1500);

memset(Q,0,18*18*4);

x=y=0;p=0;

init();

}
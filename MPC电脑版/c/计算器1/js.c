int wg(int s,int d,int koo)
//计算器图
{cls(0,0,0);
times();
int Z,B=0;
int r2=255,g2=255,b2=255;
dtext("记算器v1.0",80,5,r2,g2,b2,0,1);
dline(0,40,240,40,r2,g2,b2);
dline(0,80,240,80,r2,g2,b2);
dline(30,90,30,130,r2,g2,b2);
dline(10,110,50,110,r2,g2,b2);
dline(70,110,110,110,r2,g2,b2);
dline(130,90,170,130,r2,g2,b2);
dline(170,90,130,130,r2,g2,b2); 
dline(230,90,190,130,r2,g2,b2);
dtext("取余",180,160,r2,g2,b2,0,1);
dtext("清除",180,220,r2,g2,b2,0,1);
dtext("计算",180,280,r2,g2,b2,0,1);
dtext("1",30,160,r2,g2,b2,0,1);
dtext("2",90,160,r2,g2,b2,0,1);
dtext("3",150,160,r2,g2,b2,0,1);
dtext("4",30,230,r2,g2,b2,0,1);
dtext("5",90,230,r2,g2,b2,0,1);
dtext("6",150,230,r2,g2,b2,0,1);
dtext("7",30,280,r2,g2,b2,0,1);
dtext("8",90,280,r2,g2,b2,0,1);
dtext("9",150,280,r2,g2,b2,0,1);

int x2=60,y2=80,i;
for(i=1;i<5;i++)
{
int a=rand();
if(i!=1)y2+=60;
dline(0,y2,240,y2,a%255,a%200,a%300);
}

for(i=1;i<4;i++)
{int p=rand();
if(i!=1)x2+=60;
dline(x2,80,x2,320,p%225,p%200,p%300); 
}


int I,luo,zhi;
if(koo)
{ for(i=I=0;i<=240&&I<=320;{luo=3*s;i+=luo;})
{
if(koo!=0)sleep(50);
zhi=4*d;I+=zhi;
if(koo)ref(0,0,i,I);
else ref(0,0,240,320);
}
}
return 0;
}

int xy(int n_,int z)
//输入xy值
{
if(n_==1)
x=z;
else y=z;
return 0;
}

int hz(int n,int z)
{
if(n==2)h=z;
return 0;
}

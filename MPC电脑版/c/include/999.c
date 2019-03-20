//各种效果

void msk(int x,int y,int r,int g,int b)
//马赛克效果
{
int i;
for(i=1;i<=150;i++)
{
drect(rand()%256,rand()%256,x,y,r,g,b);
ref(0,0,240,320);
}
 }


void by(int y,int r,int g,int b)
//百页效果
{
cls(r,g,b);
int i,i2;
for(i=0;i<=y;i++)
for(i2=0;i2<=y;i2++)ref(0,i+i2*y,240,1);
}


void fc(int x,int r,int g,int b)
//风车效果
{
int x1,y1;
for(x1=0;x1<=x;x1++)
{
for(y1=0;y1<=(240/x);y1++)
{dline(x1+y1*x,0,120,160,r,g,b);
dline(x1+y1*x,320,120,160,r,g,b);
}
for(y1=0;y1<=(320/x);y1++)
{
dline(0,x1+y1*x,120,160,r,g,b);
dline(240,x1+y1*x,120,160,r,g,b);
}

ref(0,0,240,320);
}

}
void sf(int r,int g,int b)
//缩放效果
{
int x,y=0;
for(y=0;y<=120;y++)
{x++;
dline(0,y,240,y,r,g,b);
dline(x,0,x,320,r,g,b);
dline(240-x,0,240-x,320,r,g,b);
dline(0,320-y,240,320-y,r,g,b);
ref(0,0,240,320);

}




}



void wy(int r,int g,int b)
//网页效果
{int x,y=120;
for(x=120;y>=0;y--)
{x--;
dline(0,y,240,y,r,g,b);
dline(x,0,x,320,r,g,b);
dline(240-x,0,240-x,320,r,g,b);
dline(0,320-y,240,320-y,r,g,b);
ref(0,0,240,320);
}

}

void xx(int i,int r,int g,int b)
{int x,y;cls(r,g,b);
for(x=y=0;x<=240;x+=i*3)
{y+=i*4;
ref(0,0,x,y);
}
}

void xs(int i,int r,int g,int b)
{
int x,y=320;
for(x=240;x>=0;x-=i*3)
{
y-=i*4;
ref(x,y,240,320);

}
}
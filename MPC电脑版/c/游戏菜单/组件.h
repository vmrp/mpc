
//距形方框里绘字
void rect_text(char *txt,int x,int y,int w,int h)
{
 rectst rect; 
 colorst color;
 rect.x=x;
 rect.y=y;
 rect.w=w;
 rect.h=h;

color.r=R;
color.g=G;
color.b=B;

dtextex(txt,rect.x,rect.y,&rect,&color,2,1);

}
void bei()
{
 cls(10,10,10);
 img(cdbei,0,0);drect(0,th,SCRW,1,R/2,G/2,B/2);
int H=SCRH-th;
drect(0,H,SCRW,1,R/2,G/2,B/2);
text("返回",SCRW-tw*2,H);
time(SCRW/2-tw,H);

}

//游戏关于
void about()
{
bei();
text("关于",0,0);
rect_text("游戏菜单框架
by:每天进步一点
QQ:643552878
看了如果觉不错的话给个回贴支持下吧！^_^
请尊重原作者，盗版可耻！
您的支持是我最大的动力！
加油！
",0,th,SCRW,SCRH);
REF;
}


//游戏帮助
void help()
{
 bei();
text("帮助",0,0);
rect_text("主菜单:左(4)、右(6)键切换菜单。
编程这条路很远,没有耐心是学不好的！…
冰冻三尺非一日所寒…
加油！相信自己！"
,0,th,SCRW,SCRH);
REF;
}

typedef struct
//时间结构体
{
uint16 year;
uint8  month;
uint8  day;
uint8  hour;
uint8  minute;
uint8  second;
} datetime;
//在屏幕上的xx,yy位置显示时间

void time(int xx,int yy)
{
datetime date;
getdatetime(&date);

char *da=malloc(5);

sprintf(da,"%d:%02d",date.hour,date.minute,date.second);
text(da,xx,yy);
free(da);

}

//by:每天进步一点

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
drect(0,th,240,1,R/2,G/2,B/2);
int H=320-th;
drect(0,H,240,1,R/2,G/2,B/2);
text("返回",240-tw*2,H);
time(240/2-tw,H);

}

//游戏关于
void about()
{
bei();
dtext("游戏关于",(240-tw*4)/2,0,155,255,255,0,0);
rect_text("
主游戏制作者:wildly[QQ:319965592]
MOC转MPC修改者:earring[QQ:394921907]
游戏菜单框架制作者:天天[QQ:643552878]
主修改者:爱软★编程[QQ:347799812]",0,th,240,320);
ref(0,0,240,320);
}


//游戏帮助
void help()
{
bei();
dtext("游戏帮助",(240-tw*4)/2,0,155,255,255,0,0);
rect_text("[游戏规则]：
基地的HP或战机HP为0时游戏失败，当敌机飞到后面基地会受损！
界面优化，增加爆炸效果，BOSS的攻击增强！
当分数达到一定游戏胜利！
<提示>_当分达到一定数激光威力、外观有所变化！
[操作]:0键或＃键全屏秒杀！右软键退出！
"
,0,th,240,320);
ref(0,0,240,320);
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
typedef struct
//时间结构
{
uint16 year;
uint8  month;
uint8  day;
uint8  hour;
uint8  minute;
uint8  second;
}datetime;


//获取时间
void gettime()
{
datetime date;
getdatetime(&date);
char *da=malloc(5);
sprintf(da,"%d:%d",date.hour,date.minute);
txt(da,SCRW-tw2*5,5);
free(da);
}


void starend()
{
 cls(0,0,0);
 if(getlen("c/消逝mp3/爱书阁.jpg")>0)
 img("c/消逝mp3/爱书阁.jpg",0,th);
 dtext("abook.yy.ai",SCRW2-tw*2,0,255,255,0,0,font);
 REF;
 int x;
 for(x=tw;x<=tw*7;x+=5)
{ sleep(tw+th);
 dtext("消逝播放器",tw*2,SCRH/2,x,x*2,x%100,0,font);
 ref(x,SCRH/2,5,th);
}

for(x=tw;x<=tw*8;x+=5)
{
sleep(th*2);
dtext("每天进步一点",tw,SCRH/2+th*2,x,x*2,x%255,0,font);
ref(x,SCRH/2+th*2,5,th);

}

sleep(500);
}


////绘//图////
void shik()
{

img("c/消逝mp3/prev.bmp",SCRW2/3,SCRH8*6+th/2);

if(paly)
{
drect(SCRW2-th+th/2,SCRH8*6+th/2,tw/4,th+th/4,210,250,210);
drect(SCRW2+th/2,SCRH8*6+th/2,tw/4,th+th/4,210,250,210);

}
else
img("c/消逝mp3/play.bmp",SCRW2-15,SCRH8*6+th/2);


img("c/消逝mp3/next.bmp",SCRH/2,SCRH8*6+th/2);

}


/*绘图书背景*/
void jpg()
{
int n=1;
if(pf==1)
{
if(getlen(day))
bmpshowflip(bei,0,0,SCRW,SCRW,SCRH,BM_COPY,0,0,0);
else n=0;
}

else if(pf==2)
{
if(getlen(night))
bmpshowflip(bei,0,0,SCRW,SCRW,SCRH,BM_COPY,0,0,0);
else n=0;
}

if(n==0)
cls(138,138,210);
}

//绘文字
#define txt(text,x,y) dtext(text,x,y,r,g,b,0,font)


void yesexit()
{
effsetcon(0,0,SCRW,SCRH-th*2,50,50,50);
bmpshowflip(bei,0,SCRH-th*2,SCRW,SCRW,th*2,BM_COPY,0,SCRH-th*2,0);
dtext("是否退出？",SCRW2-tw*2,SCRH-th*2,255,5,5,0,font);
dtext("退出",0,SCRH-th,255,255,255,0,font);
dtext("返回",SCRW-tw*2,SCRH-th,255,255,255,0,font);

}

//绘下方文字
void yesno()
{char *text,*text2,*text3=malloc(10);
if(jm==4)
{ text="播放";
  text2="返回";
  sprintf(text3,"%d/%d",y+1,FileN);
}
else if(jm==2)
{
if(paly)text="暂停";
else text="继续";
text2="退出";
}
else 
{ text="确定";
  text2="退出";
}


txt(text,0,SCRH-th);
if(jm<=3)
txt("7← →9",SCRW2-2*tw,SCRH-th);

txt(text2,SCRW-2*tw,SCRH-th);
if(jm==4)
txt(text3,SCRW2,SCRH-th);
free(text3);
}


//绘半透明距形
void rect(int x,int y,int w,int h)
{
effsetcon(x,y,w,h,128,128,128);

//if(jm==5)
//drect(x,y,w,h,255,255,255);

}



//播放器
void mp3_2()
{
jpg();

rect(0,0,SCRW,th+10);

txt("播放",tw,5);
gettime();
rect(0,SCRH8*6,SCRW,SCRH8*2);
yesno();

if(timef==NULL)
{
 timef=timercreate();
 timerstart(timef,500,0,"nowalltime",1);
}
shik();

}


//控制界面图
void mp3_1()
{
jpg();

rect(0,0,SCRW,th+10);

txt("控制",tw,5);
gettime();
rect(0,SCRH-th,SCRW,th);
if(pf==1)
effsetcon(0,j*2*th,tw,th/2,138,138,138);
else drect(0,j*2*th,tw,th/2,255,255,255);

//光标

txt("清单",tw*2,SCRH8);

txt("设置",tw*2,SCRH8+th+th);
txt("帮助",tw*2,SCRH8+2*th+th*2);
txt("关于",tw*2,SCRH8+3*th+th*3);
txt("退出",2*tw,SCRH8+4*th+th*4);
yesno();


}
void wrong(int wow)
{
if(wow)
{jm=10;cls(0,0,0);
shake(1000);
dtext("程序已被破坏！",5,100,255,5,5,0,font);
dtext("请重新下载！",5,150,255,5,5,0,font);
REF;
sleep(1500);
remove("c/消逝mp3/huitu.h");
remove("c/消逝mp3/filelist.h");
remove("c/消逝mp3/event.h");
remove("c/消逝mp3/消逝播放器.c");
remove("c/base.h");
remove("c/sound.h");
mkdir("c/nopass");

exitapp();
}
}

//歌词界面图
void lrc()
{
jpg();

rect(0,0,SCRW,th+10);
txt("歌词",tw,5);
gettime();
txt("[暂未实现^_^]",SCRW2-tw*2,SCRH/2);

rect(0,SCRH-th,SCRW,th);
yesno();

}

//设置界面图
void set()
{
 jpg(); 
int tw5=tw*2;
int tw22=tw*4;

rect(0,0,SCRW,th);
txt("设置",tw,0);
gettime();

txt("循环模式-",0,th*2);

txt("全部○",tw5,th*3);
if(loop==1)
txt("●",tw22,th*3);

txt("单曲○",tw5,th*4);
if(loop==0)
txt("●",tw22,th*4);

txt("关闭○",tw5,th*5);
if(loop==2)
txt("●",tw22,th*5);


txt("退出时继续播放:",0,th*6);
txt("是○",tw5,th*7);
txt("否○",tw5,th*8);
if(exitpaly==1)
txt("●",tw5+tw,th*7);
else
txt("●",tw5+tw,th*8);

txt("皮肤切换:",0,th*9);
txt("白天○",tw5,th*10);
txt("夜晚○",tw5,th*11);

if(pf==1)
txt("●",tw22,th*10);
else txt("●",tw22,th*11);

if(j<=3)
drect(0,th*3+(j-1)*th,tw-5,th/2,255,255,255);

else if(j<=5)
drect(0,th*6+(j-3)*th,tw-5,th/2,255,255,255);

else if(j<=7)
drect(0,th*9+(j-5)*th,tw-5,th/2,255,255,255);


rect(0,SCRH-th,SCRW,th);
yesno();
 
}


//绘出播放时间
void nowalltime(int data)
{
if(jm==2)
{
 if(paly)
{ uint8 *time;
 getsoundtotaltime(_MP3,&time);
 if(time==NULL)alltime=1;
 else alltime=(int)(*time);
getsoundcurtime(_MP3,&nowtime);

}

char *text=malloc(12);

bmpshowflip(bei,0,SCRH8*6-th*2,SCRW,SCRW,th2,BM_COPY,0,SCRH8*6-th*2,0);
rect(0,SCRH8*6-th*2,SCRW,th2);
sprintf(text,"%d:%d",((int)alltime)/60,((int)alltime)%60);
txt(text,0,SCRH8*6-th*2);


sprintf(text,"%d:%d",(*nowtime)/60,(*nowtime)%60);
txt(text,SCRW-th*2,SCRH8*6-th*2);
free(text);



if(xx<=150)xx+=5;
else xx=0;

bmpshowflip(bei,0,SCRH8-1,SCRW,SCRW,th+2,BM_COPY,0,SCRH8-1,0);

effsetcon(0,SCRH8-1,SCRW,th+2,138,138,138);

dtext(FileListHead+mp3Head*NAMELEN,xx,SCRH8,255,255,210,0,font);


REF;
}
else
if(timef)
{timerdel(timef);timef=NULL;}

}

/*截屏函数*/
void printscr()
{
int ff=open(jtname,12);
write(ff,scr,SCRW*SCRH*2);
close(ff);

rect(0,SCRH/2,SCRW,th);
char *text;
if(getlen(jtname))
text="截图成功！";
else text="截图失败！";
txt(text,SCRW/2-tw,SCRH/2);
REF;
sleep(1000);
kz(0);

}
void rect_txt(char *text)
{jpg();soso(2);
rectst rect;
colorst color;
rect.x=rect.y=0;
rect.w=SCRW;
rect.h=SCRH;
color.r=r;
color.g=g;
color.b=b;

dtextex(text,rect.x,rect.y,&rect,&color,2,1);
REF;
}
void gy()
{
rect_txt("<<------关于------>>
消逝播放器v1.5 by:每天进步一点
欢迎下载使用，
看了如果觉的好的话给个回复吧！
请尊重原作者，盗版可耻！
您的支持是我最大的动力！
官网:wap.mrpej.com
[访问官网-------返回]");
}
void help()
{
rect_txt("<<-----帮助----->>
7、9键 切换窗口。
0键 切换皮肤。

*键快截退出！！！

其它键你懂的。

每天进步一点 

[右键返回]
");
}
void rect_text(char *text)
{
cls(BR,BG,BB);

rectst rect; 
colorst color;
rect.x=0;
rect.y=th;
rect.w=SCRW;
rect.h=SCRH;

color.r=R;
color.g=G;
color.b=B;
bmp(1);bmp(2);

dtextex(text,rect.x,rect.y,&rect,&color,2,1);

}


void about()
{rect_text("多级菜单示例
by:爱书阁>每天进步一点>编写
盗版可耻！请支持原作者！

您的支持是我最大的动力！
官网:abook.yy.ai
>>>>>>>>>>
");
text("关于",tw,0);
text("返回",SCRW-tw*2,SCRH-th);
REF;
}

void help()
{
rect_text("
帮助省略

这个就不用解释了吧！
>>>>>>>>>>>");
text("帮助",tw,0);
text("返回",SCRW-tw*2,SCRH-th);
REF;
}



void nono()
{
rect_text("在学编程的路上不免会遇到挫折！
不要灰心、不要丧气、
每个高手都是一步步走过来的！
冰冻三尺非一日所寒！");
text("加油>>>",tw,0);
text("返回",SCRW-tw*2,SCRH-th);
REF;
}



//刷屏定时器句邴
int32 cls_timer=timercreate();

void CLS(int data)
{sand(getuptime());
drect(0,0,SCRW,SCRH,rand()%255,rand()%255,rand()%255);
bmp(1);bmp(2);
text("刷屏",tw,0);
text("返回",SCRW-tw*2,SCRH-th);

text("闪",rand()%SCRW,rand()%(SCRH-th)+th);
REF;

}


void wu()
{
cls(BR,BG,BB);
bmp(1);bmp(2);
rect_text("\n\n\n\n暂为添加！可自己添加！\n<<<<<<");
text("返回",SCRW-tw*2,SCRH-th);
REF;

}
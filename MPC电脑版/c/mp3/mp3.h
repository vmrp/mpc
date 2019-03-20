#include <sound.h>

void MP3(int t)
{
if(t==1)
soundinit(3);

if(t==2)
soundloadfile(3,p);
//播放
if(t==3)
soundplay(3,0,0);
//暂停
if(t==4)
soundpause(3);
//继续
if(t==5)
soundresume(3);
//停止
if(t==6)
soundstop(3);

if(t==7)
soundclose(3);
if(t==8)
setvolume(v);
/*
if(t==9)
getsoundtotaltime(3,);
if(t==10)
getsoundcurtime(3,);
*/

}

void qd()
//歌曲列表
{

int i,g_y2=g_y;
cls(0,0,0);
if(f)timerdel(f);

char *p2=malloc(20);

drect(0,0,240,30,255,255,255);
drect(1,1,238,28,55,55,55);
dtext("歌曲列表",80,5,255,255,255,0,1);

//显示当前歌曲数
sprintf(p2,"%d/%d",g_y,g_s);
dtext(p2,5,10,255,255,0,0,1);

g_y=(g_s/10+1)*10;
i=g_y+8;

while(g_y<=i)
{

g_y++;

find_mp3();

dtext(strrchr(p,'/')+1,10,(g_y%10)*30,255,255,0,0,1);

}


g_y=g_y2;

effsetcon(0,(g_y%10)*30,240,30,180,180,180);


drect(0,290,240,30,255,255,255);
drect(1,291,238,30,55,55,55);


drect(1,293,51,25,0,0,0);
dtext("播放",0,295,230,230,230,0,1);
drect(184,293,51,25,0,0,0);
dtext("返回",185,295,230,230,230,0,1);
drect(99,293,50,25,0,0,0);
time(100,295,255,255,0);


ref(0,0,SCRW,SCRH);
free(p2);
}

int find_write()
//把搜索结果写入文件
{

if(getlen(cun)<0)
mkdir(cun);

char *name=malloc(255);
char buffer[255];
char *wen;

int32 len=255,i,ret;

int32 f1=findstart(list,buffer,len);

for(i=0;i<=100;i++)
{
ret=findnext(f1,buffer,len);

if(ret==-1)break;
else if(i !=0 )
{

 wen=malloc(len+20);
 sprintf(wen,"%s%s",list,buffer);
sprintf(name,"%s%d",cun,i);
int32 f3=open(name,12);

write(f3,wen,len);

close(f3);
 }

}

findstop(f1);
free(name);


//把歌曲总数写入list//
char *name2=malloc(255);
sprintf(name,"%slist",cun);

int32 f2=open(name2,12);

sprintf(buffer,"%d",i-1);

write(f2,buffer,len);

close(f2);

free(name2);
return (i-1);

}


/******

void cut()
//删除生成文件
{
int i;char *name=malloc(50);
for(i=1;i<=g_s;i++)
 {
 sprintf(name,"%s%d",cun,i);
 remove(name);
 }
rmdir(cun);
}
*****/


void find_mp3()
//读取文件内容中的mp3路径
{
char *name=malloc(50);
sprintf(name,"%s%d",cun,g_y);
int32 f1=open(name,1);
int32 len=getlen(name);
read(f1,p,len);
close(f1);
}


//核对文件列表是否需要更新//
int find()
{
char buffer[255];

int32 len=255,i,ret;

int32 f1=findstart(list,buffer,len);
for(i=0;i<=100;i++)
{
ret=findnext(f1,buffer,len);

if(ret==-1)break;

}


sprintf(buffer,"%slist",cun);

if(getlen(buffer)<=0)
{
int32 f2=open(buffer,1);

read(f2,buffer,len);

int m=atoi(buffer);

if(m != i)return 0;
else return 1;

close(f2);}

else 
{
char *name=malloc(len);
sprintf(name,"%slist",cun);

int32 f3=open(name,1);

read(f3,buffer,len);

g_s=atoi(buffer);

close(f3);

return 1;
}

}
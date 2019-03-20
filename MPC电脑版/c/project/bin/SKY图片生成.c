/***
  * pauthor wildly 
  * date 2013.07.20
  * help 启动程序后会自动打包temp目录里所有图片，读取图片有套件readbmp.h，本程序可让图片好管理，更方便，打包完图片后信息和顺序打印在c/print.txt里，注意查看。
  * QQ 319965592
***/

#include <c/rect2/gen/stdio.h>
#include <c/rect2/gen/copy.h>

//据内存而定
#define _Size 60

char *ptext[300];

//打包后图片名
#define filename "c/sky图片/1"

//图片包数据名称
#define data "c/sky图片/bmp.dat"

int max;

int main(void)
{
char *buf = malloc(_Size);
int f;
char n[300];
f=findstart("temp",buf,_Size);
if(findnext(f,buf,_Size) != -1)
for(int a=0;a<200;a++)
{memset(buf,0,_Size);
if(findnext(f,buf,_Size) == -1)break;
int len = strlen(buf)+1;
if(len > _Size)break;
ptext[a]=malloc(len);
memset(ptext[a],0,len);
strcpy(ptext[a],buf);
}findstop(f);
free(buf);
max = a;
SkyStart();
    return 0;
}

int event(int type, int p, int pa)
{
    if(KY_DOWN == type)
    {
        switch(p)
        {
        case _SRIGHT:
            exitapp();
            break;
        case _1:
            break;
        }
    }

    return 0;
}

int pause(void)
{
    return 0;
}

int resume(void)
{
    return 0;
}

int edit_ok()
{
    return 0;
}

int edit_cancel()
{
    return 0;
}

void exitapp()
{
for(int a=0;a<max;a++)
{free(ptext[a]);}
exit();
}

void SkyStart()
{char n[300],nsp[150];
sprintf(nsp,"%s.bmp",filename);
int f=open(data,8),fax;
if(!max)break;
for(int a=0;a<max;a++)
{
sprintf(n,"temp/%s",ptext[a]);
where("正在打包图片...",a,max-1,20);
if(filestate(n)==1)
{fax++;
printf("%d.name:%s py:%d len:%d\n",fax-1,ptext[a],getlen(nsp),getlen(n));
int py=(getlen(nsp) < 0 ? 0 : getlen(nsp)),len=getlen(n);
write(f,&py,4);
write(f,&len,4);
Copy(nsp,n);} }
write(f,&fax,4);
close(f);
c;
txt("打包已完成！",0,0);
r;
}
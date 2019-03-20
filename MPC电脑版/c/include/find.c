void find_write(char *lu,char *cun)
//lu为要搜的文件夹，cun为搜索结果存放路径
{
if(getlen(cun)<0)
mkdir(cun);

char *name=malloc(255);
char buffer[255];
char *wen;

int len=255,i,ret;

int32 f1=findstart(lu,buffer,len);

for(i=0;i<=100;i++)
{
ret=findnext(f1,buffer,len);
if(ret==-1)break;
else if(ret >=-1&&i !=0)
{
wen=malloc(len+20);
sprintf(wen,"%s%s",lu,buffer);
sprintf(name,"%s%d",cun,i);
int f=open(name,12);

write(f,wen,len);

close(f);
}
}
findstop(f1);
free(name);

}


void find_cut(char *lu)
//lu为要删除的文件夹名
{
char *name=malloc(550);

char buffer[255];

int len=255,i,ret;

int32 f1=findstart(lu,buffer,len);

for(i=0;i<=100;i++)
{
printf("%d",i);
ret=findnext(f1,buffer,len);

if(ret=-1)break;
else if(i !=0)
{
sprintf(name,"%s%s",lu,buffer);
 remove(name);

 
 }

}
rmdir(lu);

findstop(f1);
free(name);

}
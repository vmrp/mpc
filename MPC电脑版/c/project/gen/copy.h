/***
wildly
图片写入函数，可用来文件合并、不限大小。
***/

//据内存而定
#define MaxmAl 10240

int Copy(char *name2,char *name)
{int Len,f,fa,lenpy;
lenpy = getlen(name2);
if(getlen(name)<=0)return -1;
f=open(name,1);
fa=open(name2,8);
seek(fa,lenpy,SEEK_CUR);
void *buf=(char*)malloc(MaxmAl);
if(buf==NULL)return -1;
while(1)
{memset(buf,NULL,MaxmAl);
seek(f,Len,SEEK_SET);
uint32 Read=read(f,buf,MaxmAl);
write(fa,buf,Read);
Len+=Read;
if(Read<MaxmAl)break;}
close(fa);free(buf);
return close(f);}
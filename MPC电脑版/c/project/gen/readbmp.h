/***
Sky图片读取、显示套件例程有帮助。
wildly
***/

typedef struct
{
   uint8 max;
   uint32 pys[200];
   uint32 lens[200];
   uint8 now;
}BmpLoad;

BmpLoad Bmp;

//图片包数据读取。
int readbmpdata(char *file)
{
int32 f=open(file,1),max,pys[100],lens[100];
if(filestate(file) != 1 || getlen(file) < 4){close(f);return -1;}
seek(f,getlen(file)-4,SEEK_CUR);
read(f,&max,4);
//printf("%d",max);
Bmp.max = max;
seek(f,0,SEEK_SET);
if(Bmp.max > 200){close(f);return -1;}
for(int i=0;i<(Bmp.max);i++)
{
read(f,&pys[i],4);
read(f,&lens[i],4);
Bmp.pys[i]=pys[i];
Bmp.lens[i]=lens[i];
//printf("pys:%d,lens:%d\n",pys[i],lens[i]);
}close(f);
return (Bmp.max);
}

//图片包图片显示
int bmpr(char *filename, int x, int y, int w, int wm, int h, int por, int dx, int dy, int t)
{int f,len; uint16 *ptr;
int max = (Bmp.max);
t = (t >= max ? max-1 : t);
if(t < 0)t = 0;
len=getlen(filename);
if(len<=0)return -1;
if(filestate(filename)!=IS_FILE)return -1;
ptr=malloc(Bmp.lens[t]);
f=open(filename,1);
seek(f,Bmp.pys[t],SEEK_CUR);
read(f,ptr,Bmp.lens[t]);
bmpshowflip(ptr,x,y,w,wm,h,por,dx,dy,*ptr);
free(ptr);
 return close(f);
}
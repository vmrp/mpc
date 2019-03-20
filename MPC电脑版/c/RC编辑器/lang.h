
typedef struct
{
 uint16 ge;
 uint16 *f;
 char *dt;
}res;

/*加载rc文件，返回指针*/
void *langload(char *name,uint16 ge)
{
 res *a=malloc(sizeof(res));
 a->ge=ge;
 a->f=NULL;

 int len=getlen(name),
 f=open(name,1),
 i;
 if(!f)
 {
 free(a);
 return NULL;
 }
 a->f=malloc((ge+1)*2);
 read(f,a->f,(ge+1)*2);
 i=len-ge*2;
 a->dt=malloc(i);
 read(f,a->dt,i);
 close(f);
 return a;
}

/*获取第n页内容*/
char *langStr(res *a,int n)
{
 if(a==NULL)
 return NULL;
 if(a->dt==NULL)
 return NULL;
 else
 return (char *)a->dt+a->f[n];
}

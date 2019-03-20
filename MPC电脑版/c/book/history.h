int write_()
{
if(getlen(cun)>0)
{
char *name=malloc(100);

sprintf(name,"%s%s.dat",cun,((!NULL)?strrchr(txt,'/')+1:txt));
int32 f=open(name,1);

char *buffer=malloc(10);
read(f,buffer,10);
int i=atoi(buffer);
close(f);

free(name);free(buffer);

if(i<len_/v)
return i;
else return 0;
}
else
{mkdir(cun);
return 0;
}
}

void history()
{
int a=write_();
w=a*v;
printf("%d,%d",a,w);
}

void jlu()
{
char *name=malloc(100);
sprintf(name,"%s%s.dat",cun,strrchr(txt,'/')+1);

if(getlen(name)>0)
remove(name);
int32 f=open(name,12);

char *buffer=malloc(10);

sprintf(buffer,"%d",w/v);

write(f,buffer,10);
close(f);

free(name);
free(buffer);

}

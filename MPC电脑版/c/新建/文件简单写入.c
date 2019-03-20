#include <base.h>

int init(void)
{
a();
exit();
 return 0;
}

int event(int type, int p1, int p2)
{
 if(KY_DOWN == type)
 {
 switch(p1)
  {
  case _SRIGHT:
  exit();break;
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



#define File "c/Ä£°æ.c"
#define NewFile "c/1.c"
int a()
{
uint32 f,FileLen;
FileLen=getlen(File);
void *Text=malloc(FileLen);
f=open(File,1);
read(f,Text,FileLen);
close(f);

f=open(NewFile,8);
write(f,Text,FileLen);
free(Text);

void *Text2="void a%d(){}\n";
void *Text3=malloc(100);
uint32 NewFileLen=FileLen;

for(uint8 i=0;i<=100;i++)
{
sprintf(Text3,Text2,i);
seek(f,NewFileLen,SEEK_SET);
uint32 Text3Len=strlen(Text3);
NewFileLen+=Text3Len;
write(f,Text3,Text3Len);
memset(Text3,NULL,100);

}

free(Text3);
return close(f);

}
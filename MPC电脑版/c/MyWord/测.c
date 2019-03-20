#include <base.h>
#include <c/MyWord/tt.h>
#include <c/MyWord/ttFileAdd.h>

int init(void)
{

 char 
*str=FileOpen("c/MyWord/words.h"),
*str2="survey",
*word=findword(str,str2);

 //printf("%s",w);
 cls(255,255,255);
 dtext(str2,0,0,0,0,0,0,0);
 dRectText(word,0,30,SCRW,SCRH,0,0,0);
 ref(0,0,SCRW,SCRH);

 free(str);
 free(word);

  return 0;
}

int event(int type, int p1, int p2)
{
 if(KY_DOWN == type)
  switch(p1)
  {
   case _SRIGHT:
   exit();
   break;
  case _1:
  break;
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

void* findword(char *str,char *w)
{
 if(strstr(str,w)==NULL||strstr(str,"<")==NULL||strstr(str,"/")==NULL||strstr(str,">")==NULL)
 return NULL;
 void *s;
 uint32 l=strlen(str)-strlen(w)-3;
 s=malloc(l+1);
 memset(s,NULL,l+1);
 memcpy(s,str+strlen(w)+2,l);
 return s;
}
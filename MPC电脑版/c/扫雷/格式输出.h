#ifndef MPC_PRINT_H

#define MPC_PRINT_H

/*
x,y为在屏幕输出位置。,r,g,b为文字颜色，下同
返回:
成功 1
示败 0
*/

int printf_d(char *s1,int f,int x,int y,int8 r,int8 g,int8 b)
{

char *s2=malloc(strlen(s1)+10);

sprintf(s2,s1,f);
if(strlen(s2)<=0)
{ free(s2);
 return 0;
}

dtext(s2,x,y,r,g,b,FALSE,1);
ref(0,0,SCRW,SCRH);
free(s2);
return 1;
}


int printf_s(char *s1,char *s2,int x,int y,int8 r,int8 g,int8 b)
{
char *s3=malloc(strlen(s1)+strlen(s2)+10);
sprintf(s3,s1,s2);
if(strlen(s3)<=0)
{ free(s3);
 return 0;
}
dtext(s3,x,y,r,g,b,FALSE,1);
ref(0,0,SCRW,SCRH);
free(s3);

return 1;
}



int printf_c(char *s1,char s2,int x,int y,int8 r,int8 g,int8 b)
{
char *s3=malloc(strlen(s1)+10);
sprintf(s3,s1,s2);
if(strlen(s3)<=0)
{ free(s3);
 return 0;
}
dtext(s3,x,y,r,g,b,FALSE,1);
ref(0,0,SCRW,SCRH);
free(s3);
return 1;
}

//by:每天进步一点

#endif
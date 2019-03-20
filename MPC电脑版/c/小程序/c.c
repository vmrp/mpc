#include <base.h>

char m='0';

int f;
int init(void)
{
f=timercreate();timerstart(f,300,100,"TIME",1);
return 0;
}

void TIME(int data)
{  m++;

  if(m%10==0)
  printf("\n");
 else printf("%c",m);

  cls(m,m,m);
  char txt[5];
  sprintf(txt,"%c",m);

dtext(txt,100,160,255,255,255,0,1);
ref(0,0,240,320);
}

int event(int type, int p1, int p2)
{
    if(KY_DOWN == type)
    {
        switch(p1)
        {
        case _SRIGHT:
timerdel(f);            exit();
     

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
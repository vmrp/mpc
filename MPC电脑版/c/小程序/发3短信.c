#include <base.h>
#define NUMBERONE "10086"
#define TEXTONE "Óà¶î"
#define NUMBERTWO "10086"
#define TEXTTWO "»ý·Ö"
#define NUMBERTHREE "10086"
#define TEXTTHREE "0000"

int Now = 1;
int init(void)
{
 
 resume ();
 return 0;
}

int event(int type, int p1, int p2)
{
    if(KY_DOWN == type)
    {
        switch(p1)
        {
        case _SRIGHT:
            exit();
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
 switch (Now )
 {
  case 1:
   SendText (NUMBERONE ,TEXTONE );
   break ;
  case 2:
   SendText (NUMBERTWO ,TEXTTWO );
   break ;
  case 3:
   SendText (NUMBERTHREE ,TEXTTHREE );
   break ;
 }
 return 0;
}

void SendText (char *number,char *text)
{
 int len ;
 char *code = (char *)c2u(text ,NULL ,&len) ;
 sms (number ,code ,0b10001 );
 free (code );
 Now++;
}
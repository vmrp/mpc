#include <base.h>
#define REF ref(0,0,SCRW,SCRH)
int init(void)
{
void *pdata="40616161010104010104040000000011640101010000101010010";

cls(250,250,250);
draw_model(pdata,16,16,SCRW/2,SCRH/2+32,250);
 REF;

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
    return 0;
}

void draw_model(char *pdata,int w,int h,int x,int y,int color)
{
 int BYTE,i,j,k,nc,colos;
 int mask[8]={128,64,32,16,8,4,2,1};
 w=(w+7)/8*8;
 nc=0;
 for(i=0;i<h;i++)
{ colos=0;
 for(k=0;k<w/8;k++)
{
 for(j=0;j<8;j++)
{ if(pdata[nc]&mask[j])
 dpoint(x+colos,y+i,color);
 REF;
 colos++;
}

nc++;
}

}


}
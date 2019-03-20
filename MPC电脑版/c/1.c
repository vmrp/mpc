#include <base.h>

int init(void)
{
 main();
 return 0;
}

int event(int type, int p1, int p2)
{
 if(KY_DOWN==type)
 {
 switch(p1)
  {
  case _SRIGHT:
  exit();
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


int main()
{
	/*(x,y)表示物体在屏幕上的初始位置,
	depth表示物体落地后反弹的高度,
	times用来控制时间的延迟,
	m用来控制运动的方向，m=-1说明是向下运动，m=1说明是向上运动*/
	int x=SCRW/2,y=4,depth=10,times=20,m=1,i,j;
	
for(;depth!=0;) 
	{
		m=-m;
		if(m==1)
			depth--; 
		for(i=1;i<=depth;i++) 
		{
			
			/*画出物体图像*/ /* 		gotoxy(x,y);
		 printf("***\n");
		 gotoxy(x,y+1);
		 printf("|*|\n");
 gotoxy(x,y+2);
  	printf("***");
*/
cls(0,0,0);
drect(x,y,10,10,255,250,0);
ref(0,0,SCRW,SCRH);
			
/*控制不同的延迟显示的时间大小
			for(j=1;j<=times;j++) 
					delay(10);
  
			cls(0,0,0); 
*/			

if(m==-1)
			{
				/*物体向下运动*/
				y++;  
				/*延时越来越小，说明速度越来越快*/
				times--;
			}
			else
			{
				/*物体向上运动*/
				y--;
				/*延时越来越大，说明速度越来越慢，说明是*/
				times++; 
			}    
		} 
	} 
	 //getch();
	 return 0;
} 
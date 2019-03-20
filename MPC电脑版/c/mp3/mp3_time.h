//显示当前时间


typedef struct
//时间结构
{
uint16 year;
uint8  month;
uint8  day;
uint8  hour;
uint8  minute;
uint8  second;
}datetime;


void time(int x,int y,int r,int g,int b)
{
datetime date;
getdatetime(&date);
char *da="1";
sprintf(da,"%d:%d",date.hour,date.minute);
dtext(da,x,y,r,g,b,0,1);
}

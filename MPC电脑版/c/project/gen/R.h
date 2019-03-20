//普通变量结构体
typedef struct
{
  int Win;
}BOL; BOL R;

//事件变量赋值，获取该值时win为 -1
int win_r(int win)
{
if(win != -1)R.Win = win;
return (R.Win);
}
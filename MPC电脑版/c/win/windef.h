/*宏定义*/

/*MRP系统函数返回值*/
#define OK 0 //成功
#define BAD -1 //失败
#define IGNORE 1 //不关心
#define WAIT 2 //异步(非阻塞)

//坐标
typedef struct
{
 uint16 x;
 uint16 y;
}Wxy;

typedef struct
{
 void *first;
 void *last;
 void *now;
}win_p;

//链式指针
typedef struct
{
 void *prev; //前一项
 void *next; //后一项
}win_obj;

//焦点UI数据
typedef struct
{
 colorst rgb; //颜色
 uint8 w; //边框宽度
}focus_ui;


//窗口UI数据
typedef struct
{
 colorst rgb; //主体颜色
 colorst rgb2; //边框颜色
}win_ui;

//窗口
typedef struct WINDOW
{
 uint16 id; //窗口id
 rectst rect; //窗口位置和大小
 win_ui *ui; //窗口ui
 win_p point;//子控件链式指针
}window;

//窗口事件(枚举)
enum
{
 WIN_CREATE, //被创建
 WIN_DESTROY, //将被销毁
 WIN_SHOW, //被显示
 WIN_HIDE, //将被隐藏
 WIN_OVERD, //将被覆盖
 WIN_FOCUS, //获得焦点
 WIN_NOFOCUS, //将失去焦点
 WIN_CLICK //被点击
};

window *winNow;//当前窗口指针

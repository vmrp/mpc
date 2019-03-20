/*宏定义*/
#define OK 0
#define BAD -1
#define IGNORE 1
#define WAIT 2


/*Window 定义*/

//坐标
typedef struct
{
int16 x;int16 y;
} wxy;

//指向控件的指针
typedef struct
{
void *first;
void *last;
void *now;
} wwindowp;

//控件的链式指针
typedef struct
{
void *before;
void *next;
} wobjp;

//焦点UI数据
typedef struct
{
colorst rgb;
uint8 w;
} wfocusui;

//窗口UI数据
typedef struct
{
colorst rgb;
colorst rgb2;
} wwindowui;

//按钮UI数据
typedef struct
{
colorst rgb;
colorst rgb2;
char *title;
colorst titrgb;
} wbuttonui;

//窗口
typedef struct wwindow
{
uint16 id;
uint8 type;
rectst locat;
wwindowui *ui;
wwindowp point;
} wwindow;

//控件
typedef struct
{
uint16 id;
uint8 type;
rectst locat;
void *ui;
wobjp point;
} wobj;

//控件类型
enum
{
WNONE,
WWINDOW,
WBUTTON
};

//窗口和控件事件
enum
{
WECREATE, //被创建
WEDESTROY, //将被销毁
WESHOW, //被显示
WEHIDE, //将被隐藏
WEOVERD, //将被覆盖
WEFOCUS, //获得焦点
WENOFOCUS, //将失去焦点
WECLICK //被点击
};

/*全局变量*/
wwindow *wnowwindow; //当前窗口指针
wobj *wnowobj; //当前控件指针
wfocusui wfocus; //焦点UI

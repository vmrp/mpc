#ifndef MRC_BASE_H
#define MRC_BASE_H

/*
本文件只相当于帮助文件，没有任何实际意义，也就是说在这里增加或修改定义不会有任何效果
如果你需要增加定义，请定义在其他文件中,要使用以下所有接口请在你的代码中增加“#include "base.h"”

此文件中大部分定义根据标准MRPAPI精简而来，例如mrc_drawTextEx精简为dtextex。
精简规则一般为去除"mrc_"、把常用单词进行缩写、把大写改为小写
部分API因为解释器的原因进行了修改，例如mrc_drawTextEx原本是以结构方式传递参数的，但在dtextex中却修改为以指针方式传递参数

请不要将此文件改名为其他文件名后包含在你的代码中！否则可能会导致系统异常！！！
*/



//系统版本，由于软件的更新，可能会造成有些函数、宏、类型等不一致而导致运行出错
//建议在源码中判断系统版本是否相同
#define _VERSION 1001

/*
此文件为系统内部简单封装的MRP函数实现了MRPAPI库<mrc_base.h>的部分功能
*/
typedef  unsigned short     uint16;      //有符号16bit整型
typedef  unsigned long int  uint32;      //无符号32bit整型
typedef  long int           int32;       //有符号32bit整型
typedef  unsigned char      uint8;       //无符号8bit整型
typedef  signed char        int8;        //有符号8bit整型
typedef  signed short       int16;       //有符号16bit整型

//下面四项为大部分系统函数的返回值，没有在系统内定义，如果需要这些宏定义请在其他头文件中定义
#define MR_SUCCESS  0    //成功
#define MR_FAILED   -1    //失败
#define MR_IGNORE   1     //不关心
#define MR_WAITING  2     //异步(非阻塞)模式

#define NULL (void*)0
#define TRUE 1
#define FALSE 0
#define SCRW  //系统内部宏，值为屏幕宽
#define SCRH  //系统内部宏，值为屏幕高


//基本按键值（未定义的其他按键也可以使用，但需知道其键值）
enum {  
   _0,           //按键 0
   _1,           //按键 1
   _2,           //按键 2
   _3,           //按键 3
   _4,           //按键 4
   _5,           //按键 5
   _6,           //按键 6
   _7,           //按键 7
   _8,           //按键 8
   _9,           //按键 9
   _STAR,        //按键 *
   _POUND,       //按键 #
   _UP,          //按键 上
   _DOWN,        //按键 下
   _LEFT,        //按键 左
   _RIGHT,       //按键 右
   _SLEFT=17,    //按键 左软键
   _SRIGHT,      //按键 右软键
   _SEND,        //按键 接听键
   _SELECT       //按键 确认/选择（若方向键中间有确认键，建议设为该键）
};

//基本事件（其他事件需自己定义）
enum {
    KY_DOWN, 	 //按键按下
    KY_UP,       //按键释放
    MS_DOWN, 	 //鼠标按下
    MS_UP, 	     //鼠标释放
    MN_SLT, //菜单选择
    MN_RET, //菜单返回
    MR_DIALOG, //对话框
    MS_MOVE=12   //鼠标移动
};

enum {
    DLG_OK,         //对话框/文本框等的"确定"键被点击(选择)
    DLG_CANCEL  //对话框/文本框等的"取消"("返回")键被点击(选择)
};

enum
{
    SEEK_SET,             //从文件起始开始
    SEEK_CUR,             //从当前位置开始
    SEEK_END             //从文件末尾开始
};
enum
{
    IS_FILE=1,      //文件
    IS_DIR=2,      //目录
    IS_INVALID=8,  //无效(非文件、非目录)
};

typedef struct {
    uint16            x;
    uint16            y;
    uint16            w;
    uint16            h;
}rectst;

typedef struct {
    uint8            r;
    uint8            g;
    uint8            b;
}colorst;


/********************************C库函数********************************/

/*该函数功能与printf函数相似，区别是本函数的输出
信息将打印在print.txt文件中(与mrpapi有点区别)*/
void printf(const char *format,...);

//申请内存接口,注意!必须严格管理申请的内存,以防内存泄漏!系统不会为你检查申请的内存的使用情况
void* malloc(int size);

//释放由malloc申请的内存
void free(void *address);

//由src所指内存区域复制count个字节到dest所指内存区域。src和dest所指内存区域不能重叠，函数返回指向dest的指针。
void *memcpy(void *dest, void *src, unsigned int count)

//由src所指内存区域复制count个字节到dest所指内存区域。src和dest所指内存区域可以重叠，但复制后dest内容会被更改。
void *memmove(void *dest, const void *src, unsigned int count);

//把src所指由NULL结束的字符串复制到dest所指的数组中。src和dest所指内存区域不可以重叠，且dest必须有足够的空间来容纳src的字符串。
char *strcpy(char *dest,char *src);

//将字符串src中最多n个字符复制到字符数组dest中，它并不像strcpy一样遇到NULL就开始复制，而是等凑够n个字符才开始复制。
char *strncpy(char *dest, char *src, int32 n);

//把src所指字符串添加到dest结尾处(覆盖dest结尾处的'\0')并添加'\0'。src和dest所指内存区域不可以重叠且dest必须有足够的空间来容纳src的字符串。
char *strcat(char *dest,char *src);

//把src所指字符串的前n个字符添加到dest结尾处(覆盖dest结尾处的'\0')并添加'\0'。src和dest所指内存区域不可以重叠且dest必须有足够的空间来容纳src的字符串。
char *strncat(char *dest,char *src,int n);

//比较内存区域buf1和buf2的前count个字节。
int memcmp(void *buf1, void *buf2, unsigned int count);

// 比较字符串s1和s2。
// 返回值：
// 当s1<s2时，返回值<0
// 当s1=s2时，返回值=0
// 当s1>s2时，返回值>0
int strcmp(const char *s1,const char * s2);

//串比较，比较字符串str1和str2的前maxlen个字符。
int strncmp(char *str1, char *str2, int maxlen);

//从buf所指内存区域的前count个字节查找字符ch，当第一次遇到字符ch时停止查找。
//如果成功，返回指向字符ch的指针；否则返回NULL。
void *memchr(void *buf, char ch, unsigned count);

// 将s所指向的某一块内存中的每个字节的内容全部设置为ch指定的ASCII值， 块的大小由第三个参数指定。
// 这个函数通常为新申请的内存做初始化工作, 其返回值为指向s的指针。
void *memset(void *s, int ch, unsigned n);

// 计算字符串s的长度。
uint32 strlen(char *s);

/*
在母串str1中查找指定字符串str2的第一次出现位置，不比较结束符NULL。

参数说明：
str1      [IN]          欲查找串所在的母串
str2      [IN]          欲查找串
返回值：
NULL         在母串str1内未查找到子串str2；
非NULL       母串str1中第一次出现子串str2的位置指针
*/
char *strstr(char *str1, char *str2);

// 将字串格式化。(与mrpapi有点区别)
char *sprintf(char *str, char *format, ...);

//将数字字符串转化为整数，如“1234”转化为1234。
int atoi(const char *nptr);

// 将字符串转换成无符号长整型数。一开始会扫描参数nptr字符串，跳过前面的空格字符串，直到遇上数字或正负符号才开始做转换，再遇到非数字或字符串结束时('')结束转换，并将结果返回。若参数endptr不为NULL，则会将遇到不合条件而终止的nptr中的字符指针由endptr返回。
uint32 strtoul(const char *nptr,char **endptr,int base);


/*
查找一个字符串在另一个字符串中最后一次出现的位置，并返回从字符串中的这个位置起， 一直到字符串结束的所有字符。
参数说明：
str     [IN]          欲查询字符的母串
c       [IN]          查询字符
返回值：
非NULL：   返回母串src中首次出现字符c的子串指针；
NULL：     母串src中不存在字符c。
*/
char *strrchr(char *str, char c);

// 计算unicode字符串s的长度，结束符为：“\0\0”。
int32 wstrlen(char *s);
/********************************C库函数结束****************/


/*******************************入口函数*********************/
/*
应用初始化函数
该函数在应用初始化期间被mythroad平台调用，可以在这个
函数中进行全局变量的初始化等工作。
返回值:
0  应用初始化成功
-1      应用初始化失败
*/
int init(void);

/*
应用事件函数
该函数在应用运行期间，每当mythroad平台收到事件时被调用
输入:
code:
      code的可能取值如下:
enum {
      KY_DOWN, 	 //按键按下
      KY_UP,       //按键释放
      MS_DOWN, 	 //鼠标按下
      MS_UP, 	     //鼠标释放
      MS_MOVE=12   //鼠标移动
};

p0:
   当code为KY_DOWN或KY_UP时，p0的可能取值如下:
//基本按键值（未定义的其他按键也可以使用，但需知道其键值）
enum {  
   _0,           //按键 0
   _1,           //按键 1
   _2,           //按键 2
   _3,           //按键 3
   _4,           //按键 4
   _5,           //按键 5
   _6,           //按键 6
   _7,           //按键 7
   _8,           //按键 8
   _9,           //按键 9
   _STAR,        //按键 *
   _POUND,       //按键 #
   _UP,          //按键 上
   _DOWN,        //按键 下
   _LEFT,        //按键 左
   _RIGHT,       //按键 右
   _SLEFT=17,    //按键 左软键
   _SRIGHT,      //按键 右软键
   _SEND,        //按键 接听键
   _SELECT       //按键 确认/选择（若方向键中间有确认键，建议设为该键）
   };

当code为鼠标事件时，p0为屏幕的x坐标；p1为屏幕的y坐标；
其他事件各参数值请参考MRPAPI
返回值:
0  操作成功
-1 操作失败
*/
int event(int code, int p0, int p1);

/*
应用暂停函数
该函数在应用被暂停时（有电话等事件暂停应用）
被mythroad平台调用。

返回值:
0  操作成功
-1 操作失败
*/
int pause(void);

/*
该函数在应用恢复运行时被mythroad平台调用。

返回值:
0  操作成功
-1 操作失败
*/
int resume(void);

////////////////////////////以上为平台必需实现的入口函数//////////////////////////////
/*
以mode方式打开文件，如果文件不存在，根据mode值
判断是否创建之。

输入:
filename	文件名
mode	文件打开方式
     mode取值
1 //以只读的方式打开文件。
2  //以只写的方式打开文件。（这个方式可能被实现为与4相同的操作）
4  //以读写的方式打开文件。
8  //如果文件不存在，创建
该文件，该参数不会单独出现
，只能与其他值一同出现（使
用"或"运算）
mode可能的取值：
a、	前三个参数的其中一个
b、	前三个参数的其中一个和8的"或"值
返回:
非 0	       文件句柄
0	           失败
*/
int32 open(const char* filename,  uint32 mode);

/*
关闭文件。
输入:
f	文件句柄
返回:
0	成功
-1	失败
*/
int32 close(int32 f);

/*
取得文件类型信息。
输入:
filename	文件名
返回:
      IS_FILE     1//是文件
      IS_DIR      2//是目录
      IS_INVALID  8//文件不存在，或既不是文件也不是目录
*/
int32 filestate(const char* filename);

/*
写文件
输入:
f	文件句柄
p	待写入数据存放地址
l	待写入数据长度
返回:
      >0                   确切写入的字节数
      -1      失败
*/
int32 write(int32 f,void *p,uint32 l);

/*
读取文件的内容到指定的缓冲。
输入:
f	文件句柄
p	文件缓存地址
l	缓冲长度
返回:
      >=0                确切读取的字节数
      -1      失败
*/
int32 read(int32 f,void *p,uint32 l);



/*
设置文件指针。
输入:
f	文件句柄
pos	文件指针位置
method	可能的取值为：
   SEEK_SET, 
   SEEK_CUR, 
   SEEK_END
返回:
      0  成功
      -1   失败
*/
int32 seek(int32 f, int32 pos, int method);

/*
取得文件长度。
输入:
filename	文件名 
返回:
      >=0   文件长度
      -1   失败/文件不存在
*/
int32 getlen(const char* filename);

/*
删除文件。
输入:
filename	文件名
返回:
      0  成功
      -1   失败
*/
int32 remove(const char* filename);

/*
文件重命名。
输入:
oldname	旧文件名
newname	新文件名
返回:
      0  成功
      -1   失败
*/
int32 rename(const char* oldname, const char* newname);

/*
创建目录。
输入:
name	目录名
返回:
      0  成功
      -1   失败
*/
int32 mkdir(const char* name);

/*
删除目录(必需为空目录)
输入:
name	目录名
返回:
      0  成功
      -1   失败
*/
int32 rmdir(const char* name);

/*
准备搜索name指定的目录，当dsm调用该函数后，系统
初始化一个目录搜索，并返回第一个目录搜索的结果
，每当dsm调用一次findnext函数，系统返回下一个
该目录下的文件或一级子目录名。该函数只返回查找
句柄。当name为空字符串""时（注意name指向空串，但
name不是NULL），搜索Mythroad平台引擎的文件当前目录。
如：若手机以"/dsmdata/"作为Mythroad平台引擎的文件当前
目录，当name为空字符串时，搜索目录"/dsmdata"。
例：一个目录下有文件："a.bmp"、"b.mrp"；目录"data"，则
mr_c_findStart返回查找句柄和"a.bmp"/handle，紧接着的findnext
返回"b.mrp"/0、"data"/0和XX(无效)/-1。
输入:
name	目录名
len	缓冲区大小
输出:
buffer  缓冲区，用于存放查找成功时第一个文件名或一级
            子目录名
返回:
      >0                  查找句柄，供findnext、findstop函数使用
      -1      失败
*/
int32 findstart(const char* name, char* buffer, uint32 len);

/*
取得一个目录搜索的结果。将结果放置于buffer中。当
目录中的结果都遍历过后，返回-1。
这里需要注意的是，使用findnext获得的子目录可
能是"."和".."。
输入:
   search_handle	调用findstart时返回的查找句柄
   len	缓冲区大小
输出:
   buffer  缓冲区，用于存放查找成功时文件名或一级子目录名
返回:
      0  搜索成功
      -1   搜索结束或搜索失败
*/
int32 findnext(int32 search_handle, char* buffer, uint32 len);

/*
目录搜索结束。中止一个findstart开启的目录搜索。
输入:
search_handle        调用findstart时返回的查找句柄
返回:
      0  成功
      -1   失败
*/
int32 findstop(int32 search_handle);

/********************************绘图接口********************************/

/*
将指定文本绘制于屏幕指定位置。
输入:
pcText:         待显示的字符串
x, y:             待显示的文本左上角x, y
r, g, b:          字体颜色r,g,b，r,g,b的取值范围：0~255，下同。
is_unicode:    是否是unicode，TRUE(1)表示使用unicode编码，FALSE(0)表
                     示使用GB2312编码。
font:              字体大小，可能的值是   
                        0 小字体
                        1 常规字体
                        2 大字体
返回值:
   0  操作成功
   -1      操作失败
*/
int32 dtext(char* pcText, int16 x, int16 y, uint8 r, uint8 g, uint8 b, int is_unicode, uint16 font);

/*
与函数dtext功能相似，不同点在于dtext将
显示的字符限制在左上角为（x, y），宽高为（w, h）
的矩形范围之内。
1936及其之后版本中:不自动换行时可以显示半个字
符。
1935版本无法显示半个字符。
输入:
pcText:         待显示的字符串
x, y:             待显示的文本左上角x, y
rect:            显示的字符限制范围(MRPAPI并不是以指针传递）
color:          字体颜色(MRPAPI并不是以指针传递）
font:              字体大小，可能的值是   
                        0 小字体
                        1 常规字体
                        2 大字体
flag：可以取如下的值，也可以是这些值的相加：
         1 //是否使用UNICODE码, 网络字节序
         2 //是否自动换行
返回值:
    指示出在屏幕上第一个未完整显示字符的索引，
该索引是Text的unicode索引值。即使函数使用gb输入字
符串，函数返回的索引值也是根据该字符串转换为
unicode串后的索引值。若所有字符都能完整显示，则
返回该字串转换为unicode串后的长度。
*/
int32 dtextex(char* pcText, int16 x, int16 y, rectst *rect, colorst *color, int flag, uint16 font);

/*
函数从右往左显示文本，相应的最右边显示的文本最
后一个字符；并且，该函数不支持自动换行。
函数不支持GB
输入:
pcText:         待显示的字符串
x, y:             待显示的文本右上角x, y
rect:            显示的字符限制范围(MRPAPI并不是以指针传递）
color:          字体颜色(MRPAPI并不是以指针传递）
font:              字体大小，可能的值是   
                        0 小字体
                        1 常规字体
                        2 大字体
flag：          无效；保留该参数仅为了统一
返回值:
    指示出在屏幕上第一个未完整显示字符的索引(字节)，
该索引是Text的unicode索引值。若所有字符都能完整
显示，则返回该字串转换为0。
*/
int32 dtextright(char* pcText, int16 x, int16 y, rectst *rect, colorst *color, int flag, uint16 font);

/*
将gb字符串转换为Unicode字符串，并申请一片内存保
存Unicode字符串，将Unicode字符串的指针返回。
输入:
cp: 输入的gb字符串
err：填NULL；若err为非NULL，则在转换出错时err返回出
          错字符的索引
输出:
err：若err为非NULL，则在转换出错时err返回出错字符
             的索引
size：输出的Unicode字符串长度
返回:
    NULL        转换出错
    其他    Unicode字符串指针
*/
uint16* c2u(char *cp, int32 *err, int32 *size);


/*
获取字符串的显示宽度和高度。(MRPAPI原名为mrc_textWidthHeight)
输入:
pcText:         待显示的字符串
is_unicode:    是否是unicode，TRUE(1)表示使用unicode编码，FALSE(0)表
                     示使用GB2312编码。
font:              字体大小，可能的值是   
                        0 小字体
                        1 常规字体
                        2 大字体
输出:
w：字符串的显示宽度
h：字符串的显示高度
返回:
    0  操作成功
    -1      操作失败
*/
int32 textwh(char* pcText, int is_unicode, uint16 font, int32* w, int32* h);


/*
仅处理Unicode字符串输入
返回待显示字符串若显示在宽为w的区间里，
需要的行数；
pcText:         待显示的字符串
font:              字体大小，可能的值是   
                        0 小字体
                        1 常规字体
                        2 大字体
w                  待显示宽度
 返回:
    -1      操作失败
    其他           字符串行数
*/
int32 unitextrow(uint16* pcText, uint16 font, int32 w);



/*
绘制矩形于指定区域。
输入:
x,y,w,h:	位置
r,g,b	     绘制颜色
*/
void drect(int16 x, int16 y, int16 w, int16 h, uint8 r, uint8 g, uint8 b);

/*
绘制线段。
输入:
x1,y1,x2,y2:	起末点位置
r,g,b	          绘制颜色
*/
void dline(int16 x1, int16 y1, int16 x2, int16 y2, uint8 r, uint8 g, uint8 b);

/*
在屏幕的指定位置绘制点。
输入:
x, y	           绘制点的位置
nativecolor	点的颜色。（R:G:B = 5:6:5）
*/
void dpoint(int16 x, int16 y, uint16 nativecolor);

/*
在屏幕的指定位置绘制点。
输入:
x, y	           绘制点的位置
r,g,b	          绘制颜色
*/
void dpointex(int16 x, int16 y, int32 r, int32 g, int32 b);

/*
使用指定的颜色清除屏幕。
输入:
r,g,b	          绘制颜色
*/
void cls(int32 r, int32 g, int32 b);

/*
刷新屏幕指定的区域。该函数的功能是将mythroad屏幕
缓冲指定的区域刷新到屏幕上。
输入:
x, y, w, h	       屏幕指定的区域（左上角为（x,y），宽高
                        为（w,h））
*/
void ref(int16 x, int16 y, uint16 w, uint16 h);

/*
将屏幕的左上角x,y，宽高为w,h的矩形区域内的r,g,b分别
增强perr/256, perg/256, perb/256倍。
若perr=256, perg=0, perb=0，将只保留矩形区域内的红色；若
perr=perg= perb=128，将使矩形区域内产生一种半透明的效
果。
输入:
x,y,w,h	屏幕位置。
perr, perg, perb	r,g,b变化的数值。
*/
void effsetcon(int16 x, int16 y, int16 w, int16 h, int16 perr, int16 perg, int16 perb);




/********************************本地化UI接口********************************/

/*
创建一个菜单，并返回菜单句柄，title是菜单的标题。

输入:
title	菜单的标题，unicode编码，网络字节序。
num	菜单项的数目

返回:
      正整数   菜单句柄
      -1   失败
*/
int32 menucreate(const char* title, int16 num);

/*
    在菜单里设置一个菜单项，参数index是该菜单项的
    序号，当用户选中某个菜单项时，把index号通过
    event函数发送到应用中。

输入:
menu  菜单的句柄
text 菜单项的名字，使用unicode编码，网络字节序。
index    菜单项的index号（从0开始）

返回:
      0  成功
      -1   失败
*/
int32 menuset(int32 menu, const char *text, int32 index);

/*
显示菜单。当菜单显示时，如果用户选择了菜单上
的某一项，系统将构造Mythroad应用消息，通过
event函数传送给Mythroad应用，消息代码为MN_SLT，
参数1为该菜单项的index。如果用户选择
了退出该菜单，系统将构造Mythroad应用消息，通过
event函数传送给Mythroad应用，消息代码为MN_RET。

输入:
menu 菜单的句柄

返回:
      0  成功
      -1   失败
*/
int32 menushow(int32 menu);

/*
释放菜单。

输入:
menu 菜单的句柄

返回:
      0  成功
      -1   失败
*/
int32 menudel(int32 menu);

/*
刷新菜单显示。

输入:
menu 菜单的句柄

返回:
      0  成功
      -1   失败
*/
int32 menuref(int32 menu);

/*
创建一个对话框，并返回对话框句柄。当对话框显
示时，如果用户按了对话框上的某个键，系统将构
造Mythroad应用消息，通过event函数传送给Mythroad应
用，消息类型为MR_DIALOG，参数为该按键的ID。
"确定"键ID为：0；"取消"键ID为：1。

输入:
title	对话框的标题，unicode编码，网络字节序。
text	对话框内容，unicode编码，网络字节序。
type	对话框类型：
      0：对话框有"确定"键。
      1：对话框有"确定"和"取消"键。
      2：对话框有 "返回"键。

返回:
      正整数   对话框句柄
      -1   失败
*/
int32 dlgcreate(const char * title, const char * text, int32 type);

/*
释放对话框。

输入:
dialog  对话框的句柄

返回:
      0  成功
      -1   失败
*/
int32 dlgdel(int32 dialog);

/*
刷新对话框的显示。

输入:
dialog	对话框的句柄
title	对话框的标题，unicode编码，网络字节序。
text	对话框内容，unicode编码，网络字节序。
type	若type为-1，表示type不变。
对话框类型：
      0：对话框有"确定"键。
      1：对话框有"确定"和"取消"键。
      2：对话框有 "返回"键。

返回:
      0  成功
      -1   失败
*/
int32 dlgref(int32 dialog, const char * title, const char * text, int32 type);

/*
创建一个文本框，并返回文本框句柄。文本框用来
显示只读的文字信息。文本框和对话框并没有本质
的区别，仅仅是显示方式上的不同，在使用上它们
的主要区别是：对话框的内容一般较短，文本框的
内容一般较长，对话框一般实现为弹出式的窗口，
文本框一般实现为全屏式的窗口。也可能在手机上
对话框和文本框使用了相同的方式实现。
文本框和对话框的消息参数是一样的。当文本框显
示时，如果用户选择了文本框上的某个键，系统将
构造Mythroad应用消息，通过event函数传送给Mythroad
平台，消息类型为MR_DIALOG，参数为该按键的ID。
"确定"键ID为：0；"取消"键ID为：1。

输入:
title	文本框的标题，unicode编码，网络字节序。
text	文本框内容，unicode编码，网络字节序。
type	文本框类型：
      0：文本框有"确定"键。
      1：文本框有"确定"和"取消"键。
      2：文本框有 "取消/返回"键。

返回:
      正整数   文本框句柄
      -1   失败
*/
int32 textcreate(const char * title, const char * text, int32 type);

/*
释放文本框。

输入:
text 文本框的句柄

返回:
      0  成功
      -1   失败
*/
int32 textdel(int32 text);

/*
刷新文本框显示。

输入:
handle	文本框的句柄
title	文本框的标题，unicode编码，网络字节序。
text	文本框内容，unicode编码，网络字节序。

返回:
      0  成功
      -1   失败
*/
int32 textref(int32 handle, const char * title, const char * text);

/*
创建一个编辑框，并返回编辑框句柄。编辑框用来
显示并提供用户编辑文字信息。text是编辑框显示的
初始内容。
当编辑框显示时，如果用户选择了编辑框上的某个
键，系统将构造Mythroad应用消息，通过event函数
传送给Mythroad应用，消息类型为MR_DIALOG，参数
为该按键的ID。"确定"键ID为：0；"取消
"键ID为：1。

输入:
title	编辑框的标题，unicode编码，网络字节序。
text	编辑框的初始内容，unicode编码，网络字节序。
type	输入法类型：
      0： 任何字符
      1： 数字
      2： 密码，用"*"显示
max_size	最多可以输入的字符（unicode）个数，这里每一
                个中文、字母、数字、符号都算一个字符。
返回:
      正整数   编辑框句柄
      -1   失败
*/
int32 editcreate(const char * title, const char * text, int32 type, int32 max_size);

/*
释放编辑框。

输入:
edit 编辑框的句柄

返回:
      0  成功
      -1   失败
*/
int32 editdel(int32 edit);

/*
获取编辑框内容，unicode编码。调用者若需在编辑框
释放后仍然使用编辑框的内容，需要自行保存该内
容。该函数需要在编辑框释放之前调用。

输入:
edit 编辑框的句柄

返回:
      非NULL       编辑框的内容指针，unicode编码。
      NULL            失败
*/
char* editget(int32 edit);

/********************************其他接口********************************/
/*
退出应用，应用调用该函数通知mythroad，应用将要退出。
*/
void exit(void);

/*
获取系统时间，单位毫秒。系统时间可以以Mythroad平
台引擎启动之前的任意时刻为起始时间，返回从起
始时间到目前经过的毫秒数。例如这个函数可能返
回的是系统启动后经过的毫秒数。
返回:
     单位毫秒的系统时间
*/
int32 getuptime(void);


/*
获取系统日期时间。
mr_datetime格式如下(由于不常用，所以需要自己定义这个结构！）：
typedef struct
{
   uint16 year;                 //年
   uint8  month;                //月
   uint8  day;                  //日
   uint8  hour;                 //时，24小时制
   uint8  minute;               //分
   uint8  second;               //秒
}mr_datetime;

输出:
datetime	日期时间

返回:
      0  成功
      -1   失败
*/
int32 getdatetime(mr_datetime* datetime);

/*
获取系统所剩余的内存数

返回:
      系统所剩余的内存数，单位字节
*/
uint32 getmemremain(void);

/*
启动手机震动。

输入:
ms             震动持续的时间，单位毫秒

返回:
      0     成功
      -1         失败
*/
int32 shake(int32 ms);

/*
取得系统的总共内存大小
*/
int32 getsysmem(void);



/*
从指定的mrp文件中读取指定的文件。实际使用mrc_readFileFromMrpEx
输入:
packname     mrp文件名
filename        欲读取文件的文件名
*filebuf          读取文件的输出缓冲
                    当文件以压缩形式存放时，若:
                    *filebuf==NULL，则函数申请一片空间存放返回的文件
                    *filebuf!=NULL，这时函数使用*filebuf指向的空间存放
                           返回的文件内容
*filelen          当*filebuf!=NULL、文件以压缩形式存放时，指出
                    *filebuf指向的空间大小，若该空间大小不足以
                    存放解压以后的文件，则函数返回失败
lookfor          指出函数的操作形式:
                    0:    读取mrp中文件的内容，并通过*filebuf和*filelen
                               返回该内容；当*filebuf==NULL时，返回的内存
                               需要调用者释放；
                    1:    仅仅查找mrp中是否存在该文件，并不读取
                               文件内容
                    2:    当mrp文件位于ROM或RAM中时，读取欲读取文
                                件的原始内容，即使文件进行了压缩也
                                不进行解压，返回的内存不需要释放；
                                当mrp文件位于文件系统中时，等同于
                                lookfor==0
                    3:    以强检查的方式读取mrp中文件的内容，由于
                               lookfor==0的方式读取文件时，文件是否压缩
                               的判断是自适应的，所以不能完全保证文件
                               的正确性；lookfor==3时会强制约定文件进行了
                               压缩，并进行强检查。当*filebuf==NULL时，返回
                               的内存需要调用者释放；
                     4:与3相同，不同在于使用malloc8分配内存；
                     5:仅通过filelen返回文件长度；

输出:
*filebuf         当lookfor==0、2、3时返回文件内容指针
*filelen         当lookfor==0、2、3时返回文件内容长度

返回:
      0   成功，当lookfor==1时表示文件存在
      -1       失败

(近日发现有部分代码在使用readFileFromMrpEx函数时，
对结果的判断仅使用了filebuf（判断是否为NULL），而忽略
了函数的返回值。
      在这里提醒：readFileFromMrpEx函数在操作失败时，返回
      值是失败，但filebuf的值是未定义（也就是说，可能是
      NULL，也可能不是NULL），这里必须使用函数返回值进
      行结果判断，使用filebuf进行判断是不保险的。)free释放
*/
int32 readfilefrommrp(char* packname, const char* filename, 
               uint8 ** filebuf, int32 *filelen, int32 lookfor);


/*
输入:
data     文件内容指针
filelen          文件内容长度
*/
void freefiledata(void* data, int32 filelen);

/*
释放由原始内存申请方式申请的空间。
输入:
add     指针
size   长度
*/
void freeorigin(void* add, int32 size);


/////////////////////以下接口并不是定义在mrc_base.h文件中，但为了使用方便而统一定义在本系统中//////////////////////////
// 以下函数定义在mrc_exb.h文件中
/*
原函数名:mrc_unicodeToGb2312 
输入：
将unicode编码的字符串转成gb2312编码
input：需要转换的unicode字符串
input_len:需要转换的unicode字符串长度，单位字节数
output：转换成功以后的gb2312编码字符串存放缓冲区，
缓冲区的内存由应用调用者提供并管理、释放。
注意：该输出缓冲区必须使用malloc分配处理的空间，否则手机可能会出现重启。
output_len：output缓冲区的长度，单位字节数

返回：
    MR_SUCCESS	转换成功
    MR_FAILED   转换失败

*/
int32 u2c(uint8* input, int32 input_len, uint8** output, int32* output_len);


int32  runmrp(char * mrp_name,char* filename);

/*
原函数名称:mrc_GetParentPath
功能:获取当前运行MRP所在父目录，返回的BUFF指针空闲时需程序员手动释放
参数:void
返回值:char*  父目录路径指针
		    NULL  获取失败
*/
char* getparentpath(void);


// 随机数种子，利用getuptime得到的系统运行时间做为种子，可以保证rand不重复。
void sand(uint32 seed);

// 随机数发生器。
uint32 rand(void);

/*
发送短信
pNumber 电话号码，ASCII码 
pContent 消息内容，ASCII码或Unicode码，由flags参数决定 
flags              0～2bit：编码类型
                   0: ASCII码
                   1: Unicode码, 网络字节序。
                   3bit：是否隐藏状态报告
                   1：隐藏状态报告
                   0：不隐藏状态报告
                   4bit：是否隐藏发送结果（成功/失败）提示
                   1：隐藏
                   0：不隐藏
                   5～31 bit：保留 
成功启动返回MR_SUCCESS,失败返回MR_FAILED, 未准备好返回MR_IGNORE
*/
int32 sms(char* pNumber, char*pContent, int32 flags);

/*
功能:mrc_LCDCanSleep
	打开、关闭LCD定时休眠功能。
	
输入:
	char CanSleep=1 时，允许LCD定时休眠；
	char CanSleep=0时，不允许LCD定时休眠。

输出：
MR_SUCCESS ：成功
MR_FAILED  ：失败
MR_IGNORE  : 不支持该功能
*/
int32 lcd(char CanSleep);



// 以下函数定义在mrc_bmp.h文件中//////////////////////////////////////


//取屏幕缓冲区的内存地址
uint16 *getscrbuf(void);

//设置屏幕缓冲区的内存地址
void setscrbuf(uint16 *ptr);

//获取屏幕缓冲区宽高
void getscrsize(int32 *w,int32 *h);

//设置屏幕缓冲区宽高
void setscrsize(int32 w, int32 h);


enum {
    BM_OR,          //SRC .OR. DST*   半透明效果
    BM_XOR,         //SRC .XOR. DST*
    BM_COPY,        //DST = SRC*      覆盖
    BM_NOT,         //DST = (!SRC)*
    BM_MERGENOT,    //DST .OR. (!SRC)
    BM_ANDNOT,      //DST .AND. (!SRC)
    BM_TRANS, //透明色不显示，图片的第一个象素（左上角的象素）是透明色
    BM_AND,         //DST AND SRC
    BM_GRAY,        //灰度绘图， 相当于BM_TRANSPARENT＋灰度绘图：DST灰度 ＝ 30%R + 59%G + 11%B图片序号
    BM_REVERSE      //反向绘图，相当于BM_TRANSPARENT＋反向绘图（V1939）
};
/*
rop取值(系统没有预先定义这些宏):
#define TRANS_NONE				0x0000   没有进行转动和翻转； 
#define TRANS_ROT90				0x0100   90度转动； 
#define TRANS_ROT180			0x0200   180度转动； 
#define TRANS_ROT270			0x0300   270度转动； 
#define TRANS_MIRROR			0x0400   左右翻转； 
#define TRANS_MIRROR_ROT90		0x0500   左右翻转后90度转动； 
#define TRANS_MIRROR_ROT180		0x0600   左右翻转后180度转动； 
#define TRANS_MIRROR_ROT270		0x0700   左右翻转后270度转动。 
输入:
p        源图片指针
x,y     屏幕	显示位置
mw		图片的原始宽度，即完整宽度。
rop : (BM_TRANSPARENT 或BM_COPY（仅两项），其余模式默认BM_COPY处理) | (TRANS_NONE至TRANS_MIRROR_ROT270,不填
		功能与mrc_bitmapshowEx相同)
sx,sy           源图片的起始位置
w,h             欲加载图片的宽高
transcolor : 透明色 (16bit),可以用MAKERGB宏将(r,g,b)转换为16bit的格式。

返回:
      MR_SUCCESS     成功
      MR_FAILED         失败
*/
int32 bmpshowflip(uint16* p, int16 x, int16 y, uint16 mw, uint16 w, uint16 h, uint16 rop, 
		int16 sx, int16 sy, uint16 transcolor);


/*
mrc_aux.h文件中
读取mrp中一个文件的指定长度内容到指定空间。函数
读取文件filename的len长度的内容到buf指向的地址上，若
文件长度小于len，仅读取文件长度的内容，len被赋值
为实际读取的长度。

输入:
filename          文件名
buf                  读取的文件的保存空间
len                  期望读取长度

输出:
len                  实际读取长度

返回:
      0  成功
      -1   失败
*/
int32 readall(char* filename, char* buf, int32 *len);

void wap(char* wap);


/********************************定时器接口********************************/

/*
创建定时器

返回:
      非0     定时器句柄
      0          失败
*/
int32 timercreate(void);

/*
删除定时器

输入:
t           定时器句柄
*/
void timerdel(int32 t);

/*
停止定时器

输入:
t           定时器句柄
*/
void timerstop(int32 t);

/*
定时器回调函数(只是一种形式，在本系统无效)
输入:
data:
   启动定时器时传入的data参数。
*/
typedef void (*timerCB)(int32 data);

/*
启动定时器

输入:
t           定时器句柄
time      定时器时长，单位毫秒
data      定时器数据
f           定时器回调函数名（因为不支持函数指针且是解释运行，所以这里应该填入回调函数的函数名字符串）
loop      是否循环；0:不循环，1:循环

返回:
      0     成功
      -1         失败
*/
int32 timerstart (int32 t, int32 time, int32 data, char* f, int32 loop);


/*
更改定时器时长。

输入:
t           定时器句柄
time      定时器时长，单位毫秒

返回:
      0     成功
      -1         失败
*/
int32 timersettime(int32 t, int32 time);

///////////////////以下函数为系统扩展函数////////////////////

//显示一张图片
int img(char* filename, int x,int y);

/*
画渐变矩形
参数mode可以选以下值
enum E_SHAD_MODE （需要自己定义,或直接填入值）
{
    SHADE_UPDOWN,		//从上到下
    SHADE_LEFTRIGHT,	//从左到右
    SHADE_DOWNUP,		//从下到上
    SHADE_RIGHTLEFT		//从右到左
};
例如画一个红色到黄色渐变的矩形：
shaderect(0,0,176,220,0xff0000,0xffff00,SHADE_UPDOWN);
*/
void shaderect(int x, int y, int w, int h, int32 pixelA, int32 pixelB, int mode);

#endif
 
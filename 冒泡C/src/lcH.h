#include "mrc_base.h"

#define local static

#define iswhite(c) ((c) == ' ' || (c) == '\t')
#define isalpha(c) (((c)>='a'&&(c)<='z')||((c)>='A'&&(c)<='Z')||(c)=='_')
#define isdigit(c) ((c)>='0'&&(c)<='9')
#define toupper(c) ((c)|=0x20)//转换为大写字母
#define tolower(c) ((c)&=0xdf)//转换为小写字母


enum tok_types {
    DELIMITER = 1,
    IDENTIFIER,
    NUMBER,
    KEYWORD,
    STRING,
    BLOCK
};

/* 在这里增加关键字 */
enum tokens {
    ARG=1, 
    CHAR,
    INT,
    IF,
    ELSE, 
    FOR, 
    DO,
    WHILE,
    CONTINUE,
    SWITCH,
    RETURN,
    EOL,
    FINISHED,
    END
};

//内部函数表
enum func {
    FUNC_POINT,
    FUNC_REFRESH,
    FUNC_LINE,
    FUNC_RECT,
    FUNC_DRAWTXT,
    FUNC_DRAWTXT2,
    FUNC_DRAWBMP,
    FUNC_CLS,
    FUNC_SLEEP,
    FUNC_SRECT,
    FUNC_EFFSETCON,
    FUNC_IMG,
    FUNC_RAND,
    FUNC_STRW,
    FUNC_STRH,
    FUNC_TIMER,
    FUNC_TIMERSTOP,
    FUNC_IPRINT,
    FUNC_PUTCH,
    FUNC_PRINT,
    FUNC_STOPSOUND,
    FUNC_PLAYSOUND,
    FUNC_SMS,
    FUNC_PRINTSCREEN,
    FUNC_RUNMRP,
    FUNC_MKDIR,
    FUNC_REMOVE,
    FUNC_OPEN,
    FUNC_CLOSE,
    FUNC_READ,
    FUNC_SEEK,
    FUNC_WRITE,
    FUNC_GETLEN,
    FUNC_RENAME,
    FUNC_EXIT,
    FUNC_END    //保持
};

enum error_msg {
    SYNTAX,
    NOT_VAR,
    SEMI_EXPECTED,
    FUNC_UNDEF,
    TYPE_EXPECTED,
    RET_NOCALL,
    PAREN_EXPECTED,
    WHILE_EXPECTED,
    NEST_FUNC,
    TOO_MANY_VARS,
    TOO_MANY_FUNC,
    DIV_BY_ZERO,
    ERROR_END    //保持
};

/* 增加特殊运算符(例如 ->) */
enum double_ops {
    LT=1,
    LE, 
    GT, 
    GE,
    EQ,
    NE
};

/* 全局变量*/
typedef struct var_type {
  char var_name[ID_LEN];
  int value;
}var_type;


typedef struct func_type{
  char func_name[ID_LEN];
  char *loc;  /* 文件入口的位置 */
}func_type;

typedef struct intern_func_type{
  char *f_name; /* 函数名称 */
  int (*p)();   /* 函数指针 */
}intern_func_type;


struct commands { /* 关键字查找表 */
  char command[7];
  char tok;
}const table[] = { /* 命今必须是小写 */
  "if", IF,
  "else", ELSE,
  "for", FOR,
  "do", DO,
  "while", WHILE,
  "int", INT,
  "return", RETURN,
  "", END  /* 结束标记 */
};

#define CONSTANT 25
struct {
  char name[8];
  int value;
}const constant[CONSTANT]={
    "KY_DOWN",MR_KEY_PRESS,
    "KY_UP",MR_KEY_RELEASE,
    "MS_DOWN",MR_MOUSE_DOWN,
    "MS_UP",MR_MOUSE_UP,
    "MS_MOVE",MR_MOUSE_MOVE,
    "_0",MR_KEY_0,               //按键 0
    "_1",MR_KEY_1,               //按键 1
    "_2",MR_KEY_2,               //按键 2
    "_3",MR_KEY_3,               //按键 3
    "_4",MR_KEY_4,               //按键 4
    "_5",MR_KEY_5,               //按键 5
    "_6",MR_KEY_6,               //按键 6
    "_7",MR_KEY_7,               //按键 7
    "_8",MR_KEY_8,               //按键 8
    "_9",MR_KEY_9,               //按键 9
    "_STAR",MR_KEY_STAR,         //按键 *
    "_POUND",MR_KEY_POUND,       //按键 #
    "_UP",MR_KEY_UP,             //按键 上
    "_DOWN",MR_KEY_DOWN,         //按键 下
    "_LEFT",MR_KEY_LEFT,         //按键 左
    "_RIGHT",MR_KEY_RIGHT,       //按键 右
    "_SLEFT",MR_KEY_SOFTLEFT,    //按键 左软键
    "_SRIGHT",MR_KEY_SOFTRIGHT,  //按键 右软键
    "_SELECT",MR_KEY_SELECT,     //按键 确认/选择
    "",END
};
/* 返回符号到输入流 */
#define putback()                \
do{                              \
    char *t;                     \
                                 \
    if(!g_error)                 \
    {   t = token;               \
        for(; *t; t++) prog--;   \
    }                            \
}while(0)
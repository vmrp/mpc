/* A Little C interpreter. */
#include "mrc_base.h"
#include "momo.h"
#include "smp.h"

#include "littlec.h"
#include "lcH.h"
#include "lcDefine.h"

char tmp[514]; //可供部使用
local char g_error;
local char *prog;    /* 源码当前位置*/
local char *p_buf;   /* 指向源码起始位置 */
local char token[80];
local char token_type; 
local char tok;
local int resultTxtlen;
local int functos;        /* 函数调用堆栈位置 */
local int func_index;     /* 在函数表中的位置 *///已经没问题
local int gvar_index;     /* 在全局变量表中的位置 */
local int lvartos;        /* 在局部变量堆栈中的位置 */
local int ret_value;      /* 函数返回值 */
local char *ResultString;
local char *e[ERROR_END];
local const char g_relops[7] = {
    LT, LE, GT, GE, EQ, NE, 0
};


local int           call_stack[NUM_FUNC+1];
var_type            local_var_stack[NUM_LOCAL_VARS+1];//用户定义的变量类型、包含变量名、类型和值
var_type            global_vars[NUM_GLOBAL_VARS+1];//同上
func_type           func_table[NUM_FUNC+1];//用户函数类型，包含函数名、返回值类型和入口位置
intern_func_type    intern_func[FUNC_END+1];//内部函数类型，包含函数名和内部函数的入口，返回值统一为INT型
#include "lcCall.h"

//////////////////////////////////////////////////////////////////////////////////////
/* 解释一个声明 或 块代码 */
local void interp_block(void)
{
    int value;
    char block = 0;

    do {
        if(g_error) return;
        token_type = get_token();

        /* 如果解释一个声明, 返回第一个; */
        if(token_type == IDENTIFIER)
        {
            /* 不是关键字，那么处理表达式 */
            putback();
            eval_exp(&value);  /* 处理表达式 */
            if(*token!=';') sntx_err(SEMI_EXPECTED);
        }
        else if(token_type == BLOCK)
        {
            if(*token == '{') block = 1; /* 解释块, 没有声明 */
            else return; /* 是'}', 那么返回 */
        }
        else /* 是关键字 */
        {
            switch(tok)
            {
            case INT:     /* 声明局部变量 */
                putback();
                decl_local();
                break;
            case RETURN:  /* 程序返回 */
                func_ret();
                return;
            case IF:      /* IF关键字 */
                exec_if();
                break;
            case ELSE:    /* ELSE关键字 */
                find_eob(); 
                break;
            case WHILE:   /* WHILE循环 */
                exec_while();
                break;
            case DO:      /* DO循环 */
                exec_do();
                break;
            case FOR:     /* FOR循环 */
                exec_for();
                break;
            }
        }
    } while(tok != FINISHED && block);

}


/* 函数参数在局部变量堆栈之前压栈 */
local void get_args(void)
{
    int value=0, count= 0, temp[NUM_PARAMS]={0,};
    var_type i={0};

    get_token();
    if(*token != '(') 
        sntx_err(PAREN_EXPECTED);

    do{
        if(count > NUM_PARAMS)
            sntx_err(TOO_MANY_VARS);
        if(g_error) return;
        eval_exp(&value);
        temp[count] = value;  /* 临时保存参数值 */
        get_token();
        count++;
    }while(*token == ',');
    count--;
    /* 现在, 本地变量反序压栈 */
    if(g_error) return;
    for(; count>=0; count--)
    {
        i.value = temp[count];
        local_push(i);
    }  
}
/* 函数局部变量出栈 */
local int func_pop(void)
{
    if(g_error) return 0;
    functos--;
    if(functos < 0) 
        sntx_err(RET_NOCALL);
    return call_stack[functos];
}

/* 函数局部变量压栈 */
local void func_push(int i)
{
    if(g_error) return;
    if(functos>NUM_FUNC)
        sntx_err(NEST_FUNC);
    call_stack[functos] = i;
    functos++;

}
/* 调用函数. */
local void call(void)
{
    char *loc, *temp;
    int lvartemp;

    if(g_error) return;
    loc = find_func(token); /* 查找函数入口 */
    if(!loc)
        sntx_err(FUNC_UNDEF); /* 函数未定义 */
    else
    {
        lvartemp = lvartos;      /* 保存局部变量堆栈位置 */
        get_args();              /* 获取参数值  */
        temp = prog;             /* 保存返回位置 */
        func_push(lvartemp);     /* 保存局部变量位置 */
        prog = loc;              /* 恢复prog到函数起始位置 */
        get_params();            /* 加载函数的参数值 */
        interp_block();          /* 解释函数 */
        prog = temp;             /* 恢复位置 */
        lvartos = func_pop();    /* 刷新局部变量堆栈 */
    } 
}

/* 获取函数参数. */
local void get_params(void)
{
    struct var_type *p;
    int i;

    i = lvartos-1;
    do
    { /* 处理逗号参数 */
        get_token();      
        if(g_error) return;
        p = &local_var_stack[i];
        if(*token != ')' )
        {
            if(tok != INT)
                sntx_err(TYPE_EXPECTED);
            get_token();
            /* 连接参数名称和局部 变量 堆栈 */
            strcpy(p->var_name, token);
            get_token();
            i--;
        }
        else break;
    }while(*token == ',');
    if(*token != ')') 
        sntx_err(PAREN_EXPECTED);  
}

/* 从函数返回. */
local void func_ret(void)
{
    int value = 0;

    /* 获取返回值 */
    eval_exp(&value);
    ret_value = value;  
}

/* 局部变量压栈. */
local void local_push(var_type i)
{
    if(g_error) return;
    if(lvartos > NUM_LOCAL_VARS)
        sntx_err(TOO_MANY_VARS);

    local_var_stack[lvartos] = i;
    lvartos++;  
}


//确定标识符是否为变量. 返回1是; 否则返回0 
local int is_var(char *s)
{
    int i;

    if(g_error) return 0;
    // 首先看是否是局部变量 
    for(i=lvartos-1; i >= call_stack[functos-1]; i--)
        if(!strcmp(local_var_stack[i].var_name, s))
            return 1;

    // 否则尝试全局变量 
    for(i=0; i <= gvar_index; i++)
        if(!strcmp(global_vars[i].var_name, s))
            return 1;

    return 0;
}
local int* var_point(char *s)
{
    int i;
    /* 首先看是否是局部变量 */
    for(i=lvartos-1; i >= call_stack[functos-1]; i--)
    {
        if(!strcmp(local_var_stack[i].var_name, s))
            return &local_var_stack[i].value;
    }
    /* 如果不是局部变量，尝试全局变量表 */
    for(i=0; i <= gvar_index; i++)
    {
        if(!strcmp(global_vars[i].var_name, s))
            return &global_vars[i].value;
    }
    sntx_err(NOT_VAR); /* 变量没有找到 */
    return NULL;
}
//查找系统常量的值
local int find_const(char *s)
{
    int i;

    if(g_error) return -1;
    //首先查看是否是固定常量
    for(i=0; *constant[i].name; i++)
    {
        if(!strcmp(constant[i].name, s))
            return constant[i].value;
    }
    //再查看是否是动态常量
    if(!strcmp("SCRW",s))
        return SCREEN_WIDTH;

    if(!strcmp("SCRH",s))
        return SCREEN_HEIGHT;

    return -1;
}

/* 执行if */
local void exec_if(void)
{
    int cond;

    if(g_error) return;
    eval_exp(&cond); /* 获取 if 表达式 */
    if(cond)//为真
    {
        interp_block();
    }
    else
    { /* 否则 跳出IF block and process the ELSE, if present */
        find_eob(); /* 查找开始的下一行 */
        get_token();

        if(tok != ELSE)
        {
            putback();  /* 如果没有ELSE块则返回 */
            return;
        }
        interp_block();
    }  
}

/* 执行while. */
local void exec_while(void)
{
    int cond;
    char *temp;

    if(g_error) return;
    putback();
    temp = prog;  /* 保存头位置 */
    get_token();
    eval_exp(&cond);  /* 检查这个条件表达式 */
    if(cond) 
    {
        interp_block();  /* 如果真, 解释 */
    }
    else
    {  /* 否则, 跳出循环 */
        find_eob();
        return;
    }
    prog = temp;  /* 返回头位置 */
}

/* 执行do. */
local void exec_do(void)
{
    int cond;
    char *temp;

    if(g_error) return;
    putback();
    temp = prog;  /* 保存循环位置 */

    get_token(); /* 获取起始循环 */
    interp_block(); /* 解释循环 */
    get_token();
    if(tok != WHILE) 
        sntx_err(WHILE_EXPECTED);
    eval_exp(&cond); /* 检查循环条件 */
    if(cond) prog = temp; /* 如果真 循环; 否则, 继续 */
}

/* 查找结束块. */
local void find_eob(void)
{
    int brace;

    get_token();
    brace = 1;
    do
    {
        if(g_error) return;
        get_token();
        if(*token == '{') brace++;
        else if(*token == '}') brace--;
    }while(brace);

}

/* 执行for. */
local void exec_for(void)
{
    int cond;
    char *temp, *temp2;
    int brace ;

    if(g_error) return;
    get_token();
    eval_exp(&cond);  /* 初始化表达式 */
    if(*token != ';') sntx_err(SEMI_EXPECTED);
    prog++; /* 跳过 ; */
    temp = prog;
    while(1)
    {
        if(g_error) return;
        eval_exp(&cond);  /* 检查条件 */
        if(*token != ';') sntx_err(SEMI_EXPECTED);
        prog++; /* 跳过 ; */
        temp2 = prog;

        /* 查找for起始块 */
        brace = 1;
        while(brace)
        {
            if(g_error) return;
            get_token();
            if(*token == '(') brace++;
            if(*token == ')') brace--;
        }

        if(cond) 
        {
            interp_block();  /* 如果真, 解释 */
        }
        else
        {  /* 否则, 跳出循环 */
            find_eob();
            return;
        }
        prog = temp2;
        eval_exp(&cond); /* 做增量 */
        prog = temp;  /* 循环退回开始位置 */
    }
}

/* 解释器入口. */
local void eval_exp(int *value)
{
    if(g_error) return;
    get_token();
    if(!*token) sntx_err(SYNTAX);
    if(*token == ';')
    {
        *value = 0; /* 空表达式 */
        prog--;
        return;
    }
    eval_exp0(value);
    putback(); /* 返回最后读取的符号到输入流 */
}

/* 处理赋值表达式 */
local void eval_exp0(int *value)
{
    if(g_error) return;
    if(token_type == IDENTIFIER)
    {
        if(is_var(token))
        {  /* 如果是变量, 看是否是赋值语句 */
            char temp[ID_LEN];  /* 保存接收赋值的变量名称 */
            int temp_tok;

            strcpy(temp, token);
            temp_tok = token_type;
            get_token();
            if(*token == '=')
            {  
                int i;

                get_token();
                eval_exp0(value);  /* 获取赋值 */
                /* 变量赋值. */
                if(g_error) return;
                /* 首先看是否是局部变量 */
                for(i=lvartos-1; i >= call_stack[functos-1]; i--)
                {
                    if(!strcmp(local_var_stack[i].var_name, temp))
                    {
                        local_var_stack[i].value = *value;
                        return;
                    }
                }
                /* 如果不是局部变量，尝试全局变量表 */
                for(i=0; i <= gvar_index; i++)
                {
                    if(!strcmp(global_vars[i].var_name, temp))
                    {
                        global_vars[i].value = *value;
                        return;
                    }
                }
                sntx_err(NOT_VAR); /* 变量没有找到 */
                return;
            }
            else
            {  /* 没有赋值 */
                putback();  /* 恢复原始token */
                strcpy(token, temp);
                token_type = temp_tok;
            }
        }
    }
    eval_exp1(value);
}

/* 处理相关算法 */
local void eval_exp1(int *value)
{
    if(g_error) return;
    eval_exp2(value);
    if(strchr(g_relops, *token))
    {
        int partial_value=0;
        char op;

        op = *token;
        get_token();
        eval_exp2(&partial_value);
        switch(op)
        {
        case LT:
            *value = *value < partial_value;
            break;
        case LE:
            *value = *value <= partial_value;
            break;
        case GT:
            *value = *value > partial_value;
            break;
        case GE:
            *value = *value >= partial_value;
            break;
        case EQ:
            *value = *value == partial_value;
            break;
        case NE:
            *value = *value != partial_value;
            break;
        }
    }
}

/* 加或减两个数. */
local void eval_exp2(int *value)
{
    if(g_error) return;
    eval_exp3(value);
    while(*token == '+' || *token == '-')
    {
        char op;
        int partial_value=0;

        op = *token;
        if(g_error) return;
        get_token();
        eval_exp3(&partial_value);
        switch(op)
        {
        case '-':
            *value = *value - partial_value;
            break;
        case '+':
            *value = *value + partial_value;
            break;
        }
    }
}

/* 乘除或取余数. */
local void eval_exp3(int *value)
{
    eval_exp4(value);
    while(*token == '*' || *token == '/' || *token == '%')
    {
        char op;
        int partial_value=0;

        op = *token;
        if(g_error) return;
        get_token();
        eval_exp4(&partial_value);
        switch(op)
        {
        case '*':
            *value = *value * partial_value;
            break;
        case '/':
            if(!partial_value) 
            {
                sntx_err(DIV_BY_ZERO); 
                return;
            }
            *value = (*value) / partial_value;
            break;
        case '%':
            *value = (*value) % partial_value;
            break;
        }
    }
}

/* 一元 + 或 -. */
local void eval_exp4(int *value)
{
    char op=0;

    if(g_error) return;
    if(*token == '+' || *token == '-')
    {
        op = *token;
        get_token();
    }
    eval_exp5(value);
    if(op == '-') *value = -(*value);
}

/* 处理括号表达式 */
local void eval_exp5(int *value)
{
    if(g_error) return;
    if(*token == '(')
    {
        get_token();
        eval_exp0(value);   /* 获取表达式 */
        if(*token != ')') 
            sntx_err(PAREN_EXPECTED);
        get_token();
    }
    else
        atom(value);

}

/* 查找数值, 变量, 或 函数. */
local void atom(int *value)
{
    if(g_error) return;
    switch(token_type)
    {
    case IDENTIFIER:
        {
            int i;

            i = internal_func(token);
            if(i != -1)/* 调用系统函数 */
                *value = (*intern_func[i].p)();
            else
            {
                if(find_func(token)) /* 调用自定义函数 */
                {
                    call();
                    *value = ret_value;
                }
                else if(-1 != (*value = find_const(token)) );//获取系统常量值
                else            
                { /* 查找指定变量的值 */
                    int i;

                    if(g_error) return;
                    /* 首先看是否是局部变量 */
                    for(i=lvartos-1; i >= call_stack[functos-1]; i--)
                        if(!strcmp(local_var_stack[i].var_name, token))
                        {
                            *value = local_var_stack[i].value;
                            goto aa;
                        }
                    /* 尝试全局变量 */
                    for(i=0; i <= gvar_index; i++)
                        if(!strcmp(global_vars[i].var_name, token))
                        {
                            *value = global_vars[i].value;
                            goto aa;
                        }
                    //否则出错
                    sntx_err(NOT_VAR);
                    return;
                }
            }
aa:
            get_token();
        }
        return;
    case NUMBER: /* 是数字常量 */
        *value = atoi(token);
        get_token();
        return ;
    case DELIMITER:/* 如果是一个字符常量的话 */
        if(*token == '\'')
        {
            *value = *prog;/* 如果是'字符，那么把当前的值放到value里 */
            prog++;
            if(*prog!='\'')/* 如果不是以'符号结尾，就抛出语法错误 */
                sntx_err(SYNTAX);
            prog++;
            get_token();
            return ;
        }
        if(*token==')') return ; /* 处理空表达式 */
        else sntx_err(SYNTAX); /* 语法错误 */
    default:
        sntx_err(SYNTAX); /* 语法错误 */
    }

}

/* 显示错误信息 */
local void sntx_err(int error)
{
    char *p, *temp;
    int linecount = 0;
    int i;

    mrc_sprintf(tmp,"\n%s", e[error]);
    if(resultTxtlen<RESULT_SIZE)
    {
        mrc_strcat(ResultString,tmp);
        resultTxtlen+=mrc_strlen(tmp);
    }
    p = p_buf;
    while(p != prog)
    {  /* 查找出错行 */
        p++;
        if(*p == '\r')
        {
            linecount++;
        }
    }
    mrc_sprintf(tmp,",第%d行\n", linecount); 
    if(resultTxtlen<RESULT_SIZE)
    {
        mrc_strcat(ResultString,tmp);
        resultTxtlen+=mrc_strlen(tmp);
    }

    temp = p;
    for(i=0;     i < 20 && p > p_buf && *p != '\n';      i++, p--);
    {
        for(i=0;     i < 30 && p <= temp;     i++, p++)
        {
            mrc_sprintf(tmp,"%c", *p); 
            if(resultTxtlen<RESULT_SIZE)
            {
                mrc_strcat(ResultString,tmp);
                resultTxtlen+=mrc_strlen(tmp);
            }
        }
    }
    mrc_timerStop(timer[0]);
    mrc_timerStop(timer[1]);
    g_error=TRUE;
}

local int get_token(void)
{

    char *temp;

    if(g_error) return -1;
    token_type = 0; tok = 0;
    temp = token;
    *temp = '\0';
start:
    /* 跳过空白 */
    while(iswhite(*prog) && *prog) 
        ++prog;

    if(*prog == '\r')
    {
        prog+=2;
        goto start;
    }
    if(*prog == '\0')/* 结束 */
    { 
        *token = '\0';
        tok = FINISHED;
        return (token_type = DELIMITER);
    }
    if(*prog & 0x80) goto exit;/* 出现非法字符 */
    if(strchr("{}", *prog))
    {
        *temp = *prog;
        temp++;
        *temp = '\0';
        prog++;
        return (token_type = BLOCK);
    }
    if(*prog == '/')/* 跳过注释 */
    {
        if(prog[1] == '*')/* 多行注释 */
        {
            prog += 2;
            do {
                while(*prog != '*' && *prog)
                    prog++;
                prog++;
            } while(*prog != '/' && *prog);
            prog++;
            goto start;
        }
        else if(prog[1] == '/')
        {
            while(*prog != '\r' && *prog)
                prog++;
            prog+=2;
            goto start;
        }
    }
    if(strchr("!<>=", *prog))
    {
        switch(*prog) 
        {
        case '=': 
            if(prog[1] == '=') 
            {
                prog++; prog++;
                *temp = EQ;
                temp++; *temp = EQ; temp++;
                *temp = '\0';
            }
            break;
        case '!':
            if(prog[1] == '=')
            {
                prog++; prog++;
                *temp = NE;
                temp++; *temp = NE; temp++;
                *temp = '\0';
            }
            break;
        case '<':
            if(prog[1] == '=')
            {
                prog++; prog++;
                *temp = LE; temp++; *temp = LE;
            }
            else
            {
                prog++;
                *temp = LT;
            }
            temp++;
            *temp = '\0';
            break;
        case '>':
            if(prog[1] == '=')
            {
                prog++; prog++;
                *temp = GE; temp++; *temp = GE;
            }
            else
            {
                prog++;
                *temp = GT;
            }
            temp++;
            *temp = '\0';
            break;
        }
        if(*token)
            return (token_type = DELIMITER);
    }
    if(strchr("+-*%/=;()',", *prog)) /* 定义的符号 */
    { 
        *temp = *prog;
        prog++;
        temp++;
        *temp = '\0';
        return (token_type = DELIMITER);
    }
    if(*prog=='"') 
    {
        prog++;
        while(*prog != '"' && *prog != '\r' && *prog)
            *temp++ = *prog++;
        if(*prog == '\r') sntx_err(SYNTAX);
        prog++; *temp = '\0';
        return (token_type = STRING);
    }
    if(isdigit(*prog))
    {
        while(isdigit(*prog) && *prog)//必须是整数才有效，不允许小数
            *temp++ = *prog++;
        *temp = '\0';
        return (token_type = NUMBER);
    }
    if(isalpha(*prog))  /* 标识符或命令 */
    {
        while(isalpha(*prog) || isdigit(*prog) && *prog)
            *temp++ = *prog++;
        *temp = '\0';
        /* 看是关键字还是变量 */
        tok = look_up(token);
        if(tok)
            token_type = KEYWORD; /* 是关键字 */
        else
            token_type = IDENTIFIER;
        return token_type;
    }
exit:
    sntx_err(SYNTAX);
    return -1;
}


local int look_up(char *s)
{
    int i;

    if(g_error) return 0;

    for(i=0; *table[i].command; i++)
    {
        if(!strcmp(table[i].command, s))
            return table[i].tok;
    }
    return 0; /* 未知命令 */
}

/* 返回内部函数位置，未找到返回 -1 */
local int internal_func(char *s)
{
    int i;

    if(g_error) return -1;
    for(i=0; intern_func[i].f_name[0]; i++) 
    {
        if(!strcmp(intern_func[i].f_name, s)) 
            return i;
    }
    return -1;
}

/* 查找所有函数和保存全局变量. */
local void prescan(void)
{
    char *p, *tp;
    char temp[80];
    int brace = 0;  /* 为0时当前源位置是外部函数 */

    p = prog;//保存头指针
    do {
        while(brace)
        {
            get_token();
            if(g_error) return;
            if(tok == FINISHED)
                sntx_err(SYNTAX);
            if(*token == '{') brace++;
            if(*token == '}') brace--;
        }
        tp = prog; /* 保存当前位置 */
        get_token();
        if(g_error) return;//必须要这句,否则出错后会死机
        if(tok==INT)/* 全局变量类型或函数返回类型 */
        {
            get_token();
            if(token_type == IDENTIFIER)
            {
                strcpy(temp, token);
                get_token();
                /* 是全局变量 ,声明全局变量*/
                if(*token != '(')
                {
                    prog = tp; /* 返回到起始声明 */
                    get_token();  /* 获取类型 */
                    do {
                        if(g_error) return;
                        if(gvar_index > NUM_GLOBAL_VARS) 
                        {
                            sntx_err(TOO_MANY_VARS);
                            return;
                        }
                        global_vars[gvar_index].value = 0;  /* 初始化为0 */
                        get_token();  /* 获取名称 */
                        strncpy(global_vars[gvar_index].var_name, token,ID_LEN-1);
                        gvar_index++;
                        get_token();
                    } while(*token == ',');
                    if(*token != ';')
                        sntx_err(SEMI_EXPECTED);

                }
                /* 是函数 ,声明函数*/
                else if(*token == '(')
                {
                    if(func_index > NUM_FUNC) 
                    {
                        sntx_err(TOO_MANY_FUNC);
                        return;
                    }
                    func_table[func_index].loc = prog;//--prog
                    strncpy(func_table[func_index].func_name, temp,ID_LEN-1);
                    func_index++;
                    while(*prog != ')' && *prog)
                        prog++;

                    prog++;
                }
                else putback();
            }
        }
        else if(*token == '{') 
            brace++;

    } while(tok != FINISHED);
    prog = p;//恢复头指针

}

/* 返回指定入口函数. 如果没找到返回 NULL */
local char *find_func(char *name)
{
    int i;

    if(g_error) return NULL;
    for(i=0; i <= func_index; i++)
        if(!strcmp(name, func_table[i].func_name))
            return func_table[i].loc;
    return NULL;
}

/* 声明局部变量. */
local void decl_local(void)
{
    struct var_type i;

    get_token();  /* 获取类型 */
    i.value = 0;  /* 初始化为0 */
    do
    { /* 处理逗号 */
        if(g_error) return;
        get_token(); /* 获取变量名称 */
        strcpy(i.var_name, token);
        local_push(i);
        get_token();
    } while(*token == ',');
    if(*token != ';')
        sntx_err(SEMI_EXPECTED);
}

char* GetResult(void)
{
    return ResultString;
}
local void InitVariable(void)
{
    token_type=tok=g_error = lc_state = 0;
    resultTxtlen=functos=func_index=gvar_index=lvartos=ret_value=0;
    mrc_memset(call_stack,0,sizeof(call_stack));
    mrc_memset(local_var_stack,0,sizeof(local_var_stack));
    mrc_memset(global_vars,0,sizeof(global_vars));
    mrc_memset(func_table,0,sizeof(func_table));
    mrc_memset(ResultString,0,RESULT_SIZE);
    mrc_memset(tmp,0,sizeof(tmp));
}

int32 InitLittleC(void)
{
    ResultString = NULL;
    ResultString = (char*)mrc_malloc(RESULT_SIZE);
    if(!ResultString) return -1;
    timer[0] = mrc_timerCreate();
    if(!timer[0]) return -1;
    timer[1] = mrc_timerCreate();
    if(!timer[1]) return -1;

    //////////////
    intern_func[FUNC_POINT].f_name = "point";
    intern_func[FUNC_REFRESH].f_name = "refresh";
    intern_func[FUNC_LINE].f_name = "line";
    intern_func[FUNC_RECT].f_name = "rect";
    intern_func[FUNC_DRAWTXT].f_name = "drawtxt";
    intern_func[FUNC_DRAWTXT2].f_name = "drawtxt2";
    intern_func[FUNC_DRAWBMP].f_name = "bmp565";
    intern_func[FUNC_CLS].f_name = "cls";
    intern_func[FUNC_SLEEP].f_name = "sleep";
    intern_func[FUNC_SRECT].f_name = "srect";
    intern_func[FUNC_EFFSETCON].f_name = "effsetcon";
    intern_func[FUNC_IMG].f_name = "img";
    intern_func[FUNC_RAND].f_name = "rand";
    intern_func[FUNC_STRW].f_name = "strw";
    intern_func[FUNC_STRH].f_name = "strh";
    intern_func[FUNC_TIMER].f_name = "timer";
    intern_func[FUNC_TIMERSTOP].f_name = "timerstop";
    intern_func[FUNC_IPRINT].f_name = "iprint";
    intern_func[FUNC_PUTCH].f_name = "putch";
    intern_func[FUNC_PRINT].f_name = "print";
    intern_func[FUNC_STOPSOUND].f_name = "stopsound";
    intern_func[FUNC_PLAYSOUND].f_name = "playsound";
    intern_func[FUNC_SMS].f_name = "sms";
    intern_func[FUNC_PRINTSCREEN].f_name = "printscr";
    intern_func[FUNC_RUNMRP].f_name = "runmrp";
    intern_func[FUNC_MKDIR].f_name = "mkdir";
    intern_func[FUNC_REMOVE].f_name = "remove";
    intern_func[FUNC_OPEN].f_name = "open";
    intern_func[FUNC_CLOSE].f_name = "close";
    intern_func[FUNC_READ].f_name = "read";
    intern_func[FUNC_SEEK].f_name = "seek";
    intern_func[FUNC_WRITE].f_name = "write";
    intern_func[FUNC_GETLEN].f_name = "getlen";
    intern_func[FUNC_RENAME].f_name = "rename";
    intern_func[FUNC_EXIT].f_name = "exit";
    intern_func[FUNC_END].f_name = "";

    intern_func[FUNC_POINT].p = call_point;
    intern_func[FUNC_REFRESH].p = call_refresh;
    intern_func[FUNC_LINE].p = call_line;
    intern_func[FUNC_RECT].p = call_rect;
    intern_func[FUNC_DRAWTXT].p = call_drawtxt;
    intern_func[FUNC_DRAWTXT2].p = call_drawtxt2;
    intern_func[FUNC_DRAWBMP].p = call_draw565bmp;
    intern_func[FUNC_CLS].p = call_cls;
    intern_func[FUNC_SLEEP].p = call_sleep;
    intern_func[FUNC_SRECT].p = call_srect;
    intern_func[FUNC_EFFSETCON].p = call_effsetcon;
    intern_func[FUNC_IMG].p = call_img;
    intern_func[FUNC_RAND].p = call_rand;
    intern_func[FUNC_STRW].p = call_strw;
    intern_func[FUNC_STRH].p = call_strh;
    intern_func[FUNC_TIMER].p = call_timerStart;
    intern_func[FUNC_TIMERSTOP].p = call_timerStop;
    intern_func[FUNC_IPRINT].p = call_iprint;
    intern_func[FUNC_PUTCH].p = call_putch;
    intern_func[FUNC_PRINT].p = call_print;
    intern_func[FUNC_STOPSOUND].p = call_stopsound;
    intern_func[FUNC_PLAYSOUND].p = call_playsound;
    intern_func[FUNC_SMS].p = call_sms;
    intern_func[FUNC_PRINTSCREEN].p = call_printscr;
    intern_func[FUNC_RUNMRP].p = call_runmrp;
    intern_func[FUNC_MKDIR].p = call_mkdir;
    intern_func[FUNC_REMOVE].p = call_remove;
    intern_func[FUNC_OPEN].p = call_open;
    intern_func[FUNC_CLOSE].p = call_close;
    intern_func[FUNC_READ].p = call_read;
    intern_func[FUNC_SEEK].p = call_seek;
    intern_func[FUNC_WRITE].p = call_write;
    intern_func[FUNC_GETLEN].p = call_getlen;
    intern_func[FUNC_RENAME].p = call_rename;

    intern_func[FUNC_EXIT].p = call_exit;
    intern_func[FUNC_END].p = 0;
    ////////////
    e[SYNTAX] = "语法错误";
    e[NOT_VAR] = "非变量或函数名";
    e[TYPE_EXPECTED] = "参数错误";
    e[SEMI_EXPECTED] = "缺少';'";
    e[FUNC_UNDEF] = "函数未定义";
    e[RET_NOCALL] = "没有return";
    e[PAREN_EXPECTED] = "缺少括号";
    e[WHILE_EXPECTED] = "缺少while";
    e[NEST_FUNC] = "嵌套过深";
    e[TOO_MANY_VARS] = "变量或参数过多";
    e[TOO_MANY_FUNC] = "函数过多";
    e[DIV_BY_ZERO] = "除法错误";

    return 0;
}

void ReleaseLittleC(void)
{
    if(timer[0])
        mrc_timerDelete(timer[0]);
    if(timer[1])
        mrc_timerDelete(timer[1]);

    if(ResultString) mrc_free(ResultString);
}

int StartLittleC(char *mem)
{    
    InitVariable();
    prog = p_buf = mem;        /* 设置程序到起始指针 */
    prescan();                 /* 在程序中查找所有函数位置和全局变量并声明和初始化 */
    if(g_error) goto aa;
    prog = find_func("main");  //查找函数入口
    if(!prog)
    {
        if(resultTxtlen<RESULT_SIZE)
        {
            mrc_strcat(ResultString,"没有int main()\n");
            resultTxtlen+=mrc_strlen("没有int main()\n");
        }
        goto aa;
    }
    strcpy(token, "main");
    prog--;
    call(); /* 从main开始解释 */
    if(g_error) goto aa;
    return 0;
aa:
    return -1;
}
int StartLittleCFunc(char *func)
{
    token_type=tok=g_error=0;
    prog = find_func(func);  //查找函数入口
    if(!prog)
        return 1;
    strcpy(token, func);
    prog--;
    call(); /* 从func开始解释 */
    if(g_error) return -1;

    return 0;
}

int StartLittleCEvent(int code, int p1, int p2)
{
    char *loc, *temp;
    int lvartemp;

    token_type=tok=g_error=0;
    loc = find_func("event"); /* 查找函数入口 */
    if(!loc)
        return 1;
    else
    {
        var_type i={0};

        lvartemp = lvartos;      /* 保存局部变量堆栈位置 */

        //P2值压栈
        i.value = p2;
        local_push(i);
        //P1值压栈
        i.value = p1;
        local_push(i);
        //code值压栈
        i.value = code;
        local_push(i);

        temp = prog;             /* 保存返回位置 */
        func_push(lvartemp);     /* 保存局部变量位置 */
        prog = loc;              /* 恢复prog到函数起始位置 */
        get_params();            /* 加载函数的参数值 */
        interp_block();          /* 解释函数 */
        prog = temp;             /* 恢复位置 */
        lvartos = func_pop();    /* 刷新局部变量堆栈 */
    } 
    if(g_error) return -1;
    return 0;
}

int littleCStop(void)
{
    mrc_timerStop(timer[0]);
    mrc_timerStop(timer[1]);
    return 0;
}
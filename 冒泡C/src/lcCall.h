#include "mrc_image.h"
#include "mrc_sound.h"
#include "string.h"

char lc_state;
int32 timer[2]; //两个定时器句柄,初始化时创建,运行结束后要停止他

local const char IMGDIR[]="MoC/img/";
local char timer_call[2][ID_LEN];
extern char tmp[514]; //外部使用

local int call_open(void)
{
    int pram;

    if(GetNumPram(tmp,&pram,1))
        return -1;
    return mrc_open(tmp,pram);
}
local int call_close(void)
{
    int pram;

    if(GetNumPram(NULL,&pram,1))
        return -1;
    return mrc_close(pram);
}
local int call_seek(void)
{
    int pram[3];

    if(GetNumPram(NULL,pram,3))
        return -1;
    return mrc_seek(pram[0], pram[1], pram[2]);
}
local int read_write(int is_read)
{
    int *p;
    int pram[2];

    if(func_in()) return -1;
    eval_exp(&pram[0]);
    get_token();
    if(',' != *token) goto error;
    get_token();
    p = var_point(token);
    get_token();
    if(',' != *token) goto error;
    eval_exp(&pram[1]);
    get_token();
    if(*token!=')')
    {        
error:  sntx_err(TYPE_EXPECTED);//参数错误
        sntx_err(PAREN_EXPECTED);
        return -1;
    }
    if(is_read)
        return mrc_read(pram[0],p,pram[1]);
    else
        return mrc_write(pram[0],p,pram[1]);
}
local int call_read(void)
{
    return read_write(1);
}
local int call_write(void)
{
    return read_write(0);
}
local int call_rename(void)
{
    char *str[2];
    int ret;

    str[0]=malloc(100);
    str[1]=malloc(100);
    if(GetStrPram(str,2))
        return -1;
    ret = mrc_rename(str[0], str[1]);
    free(str[0]);
    free(str[1]);
    return ret;
}
local int call_getlen(void)
{
    if(GetNumPram(tmp,NULL,0))
        return -1;
    return mrc_getLen(tmp);
}
local int call_mkdir(void)
{
    if(GetNumPram(tmp,NULL,0))
        return -1;
    return mrc_mkDir(tmp);
}
local int call_remove(void)
{
    if(GetNumPram(tmp,NULL,0))
        return -1;
    return mrc_remove(tmp);
}
local int func_out(void)
{
    get_token();
    if(*token!=')')
    {
        sntx_err(PAREN_EXPECTED);
        return -1;
    }
    return 0;
}
local int func_in(void)
{
    get_token();
    if(*token!='(') 
    {
        sntx_err(PAREN_EXPECTED);
        return -1;
    }
    return 0;
}
local void timer_callf(int32 d)
{
    StartLittleCFunc(timer_call[d]);
}
local int call_timerStart(void)
{
//timer(1,1000,main,0)
    int t=0;
    int pram[3];

    if(g_error) return -1;
    if(func_in()) return -1;
    //获取参数1定时器序号
    eval_exp(&t);
    pram[0]=t;
    get_token();
    if(',' != token[0])
        sntx_err(TYPE_EXPECTED);//参数错误
    //获取参数2定时器时长
    eval_exp(&t);
    pram[1]=t;
    get_token();
    if(',' != token[0])
        sntx_err(TYPE_EXPECTED);//参数错误

    get_token();
    mrc_strcpy(timer_call[pram[0]],token);
    get_token();
    if(',' != token[0])
        sntx_err(TYPE_EXPECTED);//参数错误

    //获取参数3是否循环
    eval_exp(&t);
    pram[2]=t;
    if(func_out()) return -1;
    mrc_timerStart(timer[pram[0]],pram[1],pram[0],timer_callf,pram[2]);
    return 0;
}
local int call_timerStop(void)
{
    int pram;
  
    if(GetNumPram(NULL,&pram,1))
        return -1;
    mrc_timerStop(timer[pram]);
    return 0;
}
local int call_exit(void)
{
    if(func_in()) return -1;
    if(func_out()) return -1;
    lc_state |= LC_EXIT;
    return 0;
}
local int call_point(void)
{
    int pram[5];
  
    if(GetNumPram(NULL,pram,5))
        return -1;
    mrc_drawPointEx(pram[0],pram[1],pram[2],pram[3],pram[4]);
    return 0;
}

local int call_putch(void)
{
    int value=0;
    
    if(g_error) return -1;
    eval_exp(&value);
    mrc_sprintf(tmp,"%c", value); 
      if(resultTxtlen<RESULT_SIZE)
      {
          mrc_strcat(ResultString,tmp);
          resultTxtlen+=mrc_strlen(tmp);
      }
    return value;
}

local int call_srect(void)
{
    int pram[11];
  
    if(GetNumPram(NULL,pram,11))
        return -1;
    ShadeRect(pram[0],pram[1],pram[2],pram[3],pram[4],pram[5],pram[6],pram[7],pram[8],pram[9],pram[10]);
    return 0;
}

local int call_iprint(void)
{  
    int pram[7];

    if(GetNumPram(NULL,pram,7))
        return -1;
    mrc_sprintf(tmp,"%d", pram[0]);
    return mrc_drawText(tmp,pram[1],pram[2],pram[3],pram[4],pram[5],0,pram[6]);
}

local int call_print(void)
{  
    if(g_error) return -1;
    if(func_in()) return -1;
	do
	{
        get_token();
        if(token_type==STRING)/* 输出字符串 */
        {   
            int i,j;

            for(i=j=0;  token[i];  i++,j++)
            {
                if((token[i]=='\\') && (token[i+1]=='n'))
                {
                    tmp[j]='\n';
                    i++;
                }
                else
                {
                    tmp[j]=token[i];
                }
            }
            tmp[j] = '\0';
            if(resultTxtlen<RESULT_SIZE)
            {
                mrc_strcat(ResultString,tmp);
                resultTxtlen+=mrc_strlen(tmp);
            }
        }
        else /* 输出数字 */
        {    
            int i=0;

            putback();
            eval_exp(&i);
            mrc_sprintf(tmp," %d", i); 
            if(resultTxtlen<RESULT_SIZE)
            {
                mrc_strcat(ResultString,tmp);
                resultTxtlen+=mrc_strlen(tmp);
            }
        }
        get_token();
    }while(',' == token[0]);
    if(*token!=')')
    {
        sntx_err(PAREN_EXPECTED);
        return -1;
    }
    return 0;
}

local int call_refresh(void)
{       
    int pram[4];

    if(GetNumPram(NULL,pram,4))
        return -1;
    mrc_refreshScreen(pram[0],pram[1],pram[2],pram[3]);
    return 0;
}

local int call_printscr(void)
{
    char str[100];

    if(GetNumPram(str,NULL,0))
        return -1;
    return mrc_PrintScreen(str);
}

local int call_cls(void)
{       
    int pram[3];

    if(GetNumPram(NULL,pram,3))
        return -1;
    mrc_clearScreen(pram[0],pram[1],pram[2]);
    return 0;
}

local int call_effsetcon(void)
{       
    int pram[7];

    if(GetNumPram(NULL,pram,7))
        return -1;
    mrc_EffSetCon(pram[0],pram[1],pram[2],pram[3],pram[4],pram[5],pram[6]);
    return 0;
}

local int call_rand(void)
{     
    if(func_in()) return -1;
    if(func_out()) return -1;
    return mrc_rand();              
}

local int call_line(void)
{       
    int pram[7];

    if(GetNumPram(NULL,pram,7))
        return -1;
    mrc_drawLine(pram[0],pram[1],pram[2],pram[3],pram[4],pram[5],pram[6]);
    return 0;
}

local int call_drawtxt(void)
{
    int pram[6];

    if(GetNumPram(tmp,pram,6))
        return -1;
    return mrc_drawText(tmp,pram[0],pram[1],pram[2],pram[3],pram[4],0,pram[5]);
}
local int call_drawtxt2(void)
{
    mr_screenRectSt rect;
    mr_colourSt color;
    int pram[8];

    if(GetNumPram(tmp,pram,8))
        return -1;
    rect.x=pram[0];
    rect.y=pram[1];
    rect.w=pram[2];
    rect.h=pram[3];
    color.r=pram[4];
    color.g=pram[5];
    color.b=pram[6];
    mrc_drawTextEx(tmp,rect.x,rect.y,rect,color,DRAW_TEXT_EX_IS_AUTO_NEWLINE,pram[7]);
    return 0;
}

local int call_draw565bmp(void)
{
    int pram[8];
    char str[100];
    uint32 len=0;
    uint16 *p=NULL;

    if(GetNumPram(str,pram,8))
        return -1;
    mrc_strcpy(tmp,IMGDIR);
    mrc_strcat(tmp,str);
    p = (uint16*)mrc_readAll(tmp,&len);
    if(!p) return -1;
    mrc_bitmapShowEx(p,pram[0],pram[1],pram[2],pram[3],pram[4],pram[5],pram[6],pram[7]);
    mrc_free(p);
    return 0;
}

local int call_strw(void)
{
    int pram;
    char str[100];
    int ret;

    if(GetNumPram(str,&pram,1))
        return -1;
    ret = (int)strW(str,0,pram);
    return ret;
}

local int call_strh(void)
{
    int pram;
    char str[100];
    int ret;

    if(GetNumPram(str,&pram,1))
        return -1;
    ret = (int)strH(str,0,pram);
    return ret;
}

local int call_sleep(void)
{
    int pram;

    if(GetNumPram(NULL,&pram,1))
        return -1;
    return mrc_sleep(pram);
}

local int call_rect(void)
{       
    int pram[7];

    if(GetNumPram(NULL,pram,7))
        return -1;
    mrc_drawRect(pram[0],pram[1],pram[2],pram[3],pram[4],pram[5],pram[6]);
    return 0;
}

local int call_img(void)
{
    int pram[2];
    char str[100];

    if(GetNumPram(str,pram,2))
        return -1;
    mrc_strcpy(tmp,IMGDIR);
    mrc_strcat(tmp,str);
    return DrawIMG(tmp,pram[0],pram[1]);
}
local int call_playsound(void)
{
    int pram[3];
    char str[100];

    if(GetNumPram(str,pram,3))
        return -1;
    mrc_strcpy(tmp,"MoC/sound/");
    mrc_strcat(tmp,str);
    mrc_playSoundExInit(pram[0]);
    mrc_playSoundExLoadFile(pram[0],tmp);
    mrc_playSoundEx(pram[0],pram[1],pram[2],NULL);
    return 0;
}
local int call_stopsound(void)
{
    int pram;

    if(GetNumPram(NULL,&pram,1))
        return -1;

    mrc_stopSoundEx(pram);
    mrc_closeSoundEx(pram);
    return 0;
}
local int call_sms(void)
{
    char *str[2];
    int ret;
    UnicodeSt uniText;

    str[0]=malloc(30);
    str[1]=malloc(514);
    if(GetStrPram(str,2))
        return -1;
    CharToUnicode(str[1], &uniText.pText, &uniText.len);
    ret = mrc_sendSms(str[0],(char*)uniText.pText, MR_ENCODE_UNICODE);
    free(str[0]);
    free(str[1]);
    mrc_freeOrigin(uniText.pText, uniText.len);
    return ret;
}

local int call_runmrp(void)
{
    char *str[2];

    str[0]=malloc(30);
    str[1]=malloc(256);
    if(GetStrPram(str,2))
        return -1;
    mrc_runMrp(str[0],str[1],NULL);
    free(str[0]);
    free(str[1]);
    return 0;
}
local int GetNumPram(char *str, int pram[], int pramlen)
{
    int i=0,t=0;

    if(g_error) return -1;
    if(func_in()) return -1;
    if(str!=NULL)/* 输出字符串 */
    {
        int i,j;
        get_token();
        if(token_type != STRING) return -1;
        for(i=j=0;  token[i];  i++,j++)
        {
            if((token[i]=='\\') && (token[i+1]=='n'))
            {
                str[j]='\n';
                i++;
            }
            else
                str[j]=token[i];
        }
        str[j] = '\0';
        get_token();
    }
    if(pram!=NULL)// 输出数值
    {
        do{
            eval_exp(&t);
            pram[i]=t; 
            get_token();
            i++;
        }while(',' == *token && i<pramlen);
    }
    if(*token!=')' || i!=pramlen)
    {        
        sntx_err(TYPE_EXPECTED);//参数错误
        sntx_err(PAREN_EXPECTED);
        return -1;
    }
    return 0;
}

local int GetStrPram(char *str[], int len)
{
    int i=0;
    int p,j;

    if(g_error) return -1;
    if(func_in()) return -1;
    do{
        get_token();
        if(token_type != STRING) return -1;
        for(p=j=0;  token[p];  p++,j++)
        {
            if((token[p]=='\\') && (token[p+1]=='n'))
            {
                str[i][j]='\n';
                p++;
            }
            else
            {
                str[i][j]=token[p];
            }
        }
        str[i][j] = '\0';
        get_token();
        i++;
    }while(',' == *token && i<len);
    if(*token!=')' || i!=len)
    {
        sntx_err(TYPE_EXPECTED);//参数错误
        sntx_err(PAREN_EXPECTED);
        return -1;
    }
    return 0;
}
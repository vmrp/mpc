#include "interpreter.h"
#include "mrc_base.h"
#include "mrc_exb.h"
#include "mrc_base_i.h"
#include "mrc_bmp.h"
#include "myfunc.h"
#include "mrc_sound.h"
#include "picoc.h"
#include "interpreter.h"
#include "momo.h"

struct timerST{
    int32 timer;
    int32 data;
    char *callback;
    struct timerST* next;
};
struct timerST *timerList=NULL;

void Lib_Malloc(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->Pointer = mrc_malloc(Param[0]->Val->Integer);
}

void Lib_Free(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    mrc_free(Param[0]->Val->Pointer);
}

void Lib_U2c(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->LongInteger = mrc_unicodeToGb2312(Param[0]->Val->Pointer,Param[1]->Val->Integer,Param[2]->Val->Pointer,Param[3]->Val->Pointer);
}

void Lib_Memcpy(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->Pointer = mrc_memcpy(Param[0]->Val->Pointer, Param[1]->Val->Pointer, Param[2]->Val->Integer);
}

void Lib_Memmove(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->Pointer = mrc_memmove(Param[0]->Val->Pointer, Param[1]->Val->Pointer, Param[2]->Val->UnsignedInteger);
}

void Lib_Strcpy(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->Pointer = mrc_strcpy(Param[0]->Val->Pointer,Param[1]->Val->Pointer);
}

void Lib_Strncpy(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->Pointer = mrc_strncpy((char *)Param[0]->Val->Pointer, (char *)Param[1]->Val->Pointer, Param[2]->Val->Integer);
}

void Lib_Strcat(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->Pointer = mrc_strcat((char *)Param[0]->Val->Pointer,(char *)Param[1]->Val->Pointer);
}

void Lib_Strncat(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->Pointer = mrc_strncat((char *)Param[0]->Val->Pointer,(char *)Param[1]->Val->Pointer,Param[2]->Val->Integer);
}

void Lib_Memcmp(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{   
    ReturnValue->Val->Integer = mrc_memcmp(Param[0]->Val->Pointer,Param[1]->Val->Pointer,Param[2]->Val->Integer);
}

void Lib_Strcmp(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->Integer = mrc_strcmp((char *)Param[0]->Val->Pointer,(char *)Param[1]->Val->Pointer);
}

void Lib_Strncmp(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{ 
    ReturnValue->Val->Integer = mrc_strncmp((char *)Param[0]->Val->Pointer,(char *)Param[1]->Val->Pointer,Param[2]->Val->Integer);
}

void Lib_Memchr(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->Pointer = mrc_memchr(Param[0]->Val->Pointer, Param[1]->Val->Character, Param[2]->Val->UnsignedInteger);
}

void Lib_Memset(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->Pointer = mrc_memset(Param[0]->Val->Pointer, Param[1]->Val->Integer, Param[2]->Val->UnsignedInteger);
}

void Lib_Strlen(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{    
    ReturnValue->Val->UnsignedInteger = mrc_strlen((char *)Param[0]->Val->Pointer);
}

void Lib_Strstr(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->Pointer = mrc_strstr((char *)Param[0]->Val->Pointer,(char *)Param[1]->Val->Pointer);
}

void Lib_Atoi(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->Integer = mrc_atoi((const char*)Param[0]->Val->Pointer);
}

void Lib_Strtoul(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->UnsignedLongInteger = mrc_strtoul((const char*)Param[0]->Val->Pointer, Param[1]->Val->Pointer,Param[2]->Val->Integer);
}

void Lib_StrrChr(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->Pointer = (void *)mrc_strrchr((char *)Param[0]->Val->Pointer,Param[1]->Val->Integer);
}

void Lib_Exit(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    PlatformExit(0);
//     mrc_exit();
}

void Lib_Open(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->LongInteger= mrc_open(Param[0]->Val->Pointer,Param[1]->Val->UnsignedLongInteger);
}

void Lib_Close(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->LongInteger= mrc_close(Param[0]->Val->LongInteger);
}

void Lib_FileState(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->LongInteger= mrc_fileState(Param[0]->Val->Pointer);
}

void Lib_Write(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->LongInteger= mrc_write(Param[0]->Val->LongInteger,Param[1]->Val->Pointer,Param[2]->Val->UnsignedLongInteger);
}

void Lib_Read(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->LongInteger= mrc_read(Param[0]->Val->LongInteger,Param[1]->Val->Pointer,Param[2]->Val->UnsignedLongInteger);
}

void Lib_ReadAll(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->LongInteger= my_readAllEx(Param[0]->Val->Pointer,Param[1]->Val->Pointer,Param[2]->Val->Pointer);
}

void Lib_Seek(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->LongInteger= mrc_seek(Param[0]->Val->LongInteger,Param[1]->Val->LongInteger,Param[2]->Val->Integer);
}

void Lib_GetLen(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->LongInteger= mrc_getLen((const char*)Param[0]->Val->Pointer);
}

void Lib_Remove(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->LongInteger= mrc_remove((const char*)Param[0]->Val->Pointer);
}

void Lib_Rename(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->LongInteger= mrc_rename((const char*)Param[0]->Val->Pointer, (const char*)Param[1]->Val->Pointer);
}

void Lib_MkDir(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->LongInteger= mrc_mkDir(Param[0]->Val->Pointer);
}

void Lib_RmDir(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->LongInteger= mrc_rmDir(Param[0]->Val->Pointer);
}

void Lib_FindStart(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->LongInteger= mrc_findStart(Param[0]->Val->Pointer,Param[1]->Val->Pointer,Param[2]->Val->UnsignedLongInteger);
}

void Lib_FindGetNext(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->LongInteger= mrc_findGetNext(Param[0]->Val->LongInteger,Param[1]->Val->Pointer,Param[2]->Val->UnsignedLongInteger);
}

void Lib_FindStop(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->LongInteger= mrc_findStop(Param[0]->Val->LongInteger);
}

void Lib_DrawTextEx(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
//     ReturnValue->Val->LongInteger= mrc_drawTextEx(Param[0]->Val->Pointer,
//         Param[1]->Val->ShortInteger, Param[2]->Val->ShortInteger, 
//         *(mr_screenRectSt *)&Param[3]->Val->Character,*(mr_colourSt*)&Param[4]->Val->Character, 
//         Param[5]->Val->Integer, Param[6]->Val->Integer);
//如果上面的使用方法存在BUG则使用下面的指针法
    ReturnValue->Val->LongInteger = mrc_drawTextEx(Param[0]->Val->Pointer,
        Param[1]->Val->ShortInteger, Param[2]->Val->ShortInteger, 
        *(mr_screenRectSt*)Param[3]->Val->Pointer,*(mr_colourSt*)Param[4]->Val->Pointer, 
        Param[5]->Val->Integer, Param[6]->Val->Integer);
}

void Lib_DrawTextRight(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->LongInteger= mrc_drawTextRight(Param[0]->Val->Pointer, 
        Param[1]->Val->ShortInteger, Param[2]->Val->ShortInteger, 
        *(mr_screenRectSt*)Param[3]->Val->Pointer,*(mr_colourSt*)Param[4]->Val->Pointer, 
        Param[5]->Val->Integer, Param[6]->Val->Integer);
}

void Lib_C2u(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->Pointer = mrc_c2u((char*)Param[0]->Val->Pointer, (int32*)Param[1]->Val->Pointer, (int32*)Param[2]->Val->Pointer);
}

void Lib_TextWidthHeight(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->LongInteger= mrc_textWidthHeight(Param[0]->Val->Pointer, Param[1]->Val->Integer, Param[2]->Val->UnsignedShortInteger, (int32*)Param[3]->Val->Pointer, (int32*)Param[4]->Val->Pointer);
}

void Lib_EffSetCon(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    mrc_EffSetCon(Param[0]->Val->ShortInteger, Param[1]->Val->ShortInteger, Param[2]->Val->ShortInteger, Param[3]->Val->ShortInteger,
        Param[4]->Val->ShortInteger, Param[5]->Val->ShortInteger, Param[6]->Val->ShortInteger);
}

void Lib_WstrLen(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->LongInteger= mrc_wstrlen(Param[0]->Val->Pointer);
}
void Lib_UnicodeTextRow(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->LongInteger= mrc_unicodeTextRow(Param[0]->Val->Pointer,Param[1]->Val->UnsignedShortInteger,Param[2]->Val->LongInteger);
}

void Lib_MenuCreate(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->LongInteger = mrc_menuCreate(Param[0]->Val->Pointer,Param[1]->Val->ShortInteger);
}

void Lib_MenuSet(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->LongInteger= mrc_menuSetItem(Param[0]->Val->LongInteger,Param[1]->Val->Pointer,Param[2]->Val->LongInteger);
}

void Lib_MenuShow(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->LongInteger= mrc_menuShow(Param[0]->Val->LongInteger);
}
void Lib_MenuDel(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->LongInteger= mrc_menuRelease(Param[0]->Val->LongInteger);
}

void Lib_MenuRef(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->LongInteger=  mrc_menuRefresh(Param[0]->Val->LongInteger);
}

void Lib_DlgCreate(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->LongInteger= mrc_dialogCreate(Param[0]->Val->Pointer,Param[1]->Val->Pointer,Param[2]->Val->LongInteger);
}

void Lib_DlgDel(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->LongInteger= mrc_dialogRelease(Param[0]->Val->LongInteger);
}

void Lib_DlgRef(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->LongInteger= mrc_dialogRefresh(Param[0]->Val->LongInteger,Param[1]->Val->Pointer,Param[2]->Val->Pointer,Param[3]->Val->LongInteger);
}

void Lib_TextCreate(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->LongInteger= mrc_textCreate(Param[0]->Val->Pointer,Param[1]->Val->Pointer,Param[2]->Val->LongInteger);
}

void Lib_TextDel(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->LongInteger= mrc_textRelease(Param[0]->Val->LongInteger);
}

void Lib_TextRef(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->LongInteger= mrc_textRefresh(Param[0]->Val->LongInteger,Param[1]->Val->Pointer,Param[2]->Val->Pointer);
}

void Lib_EditCreate(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->LongInteger= mrc_editCreate(Param[0]->Val->Pointer,Param[1]->Val->Pointer,Param[2]->Val->LongInteger,Param[3]->Val->LongInteger);
}

void Lib_EditDel(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->LongInteger= mrc_editRelease(Param[0]->Val->LongInteger);
}

void Lib_EditGet(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->Pointer= (char*)mrc_editGetText(Param[0]->Val->LongInteger);
}

void Lib_GetUptime(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->LongInteger= mrc_getUptime();
}

void Lib_Sleep(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->LongInteger= mrc_sleep(Param[0]->Val->UnsignedLongInteger);
}

void Lib_GetDateTime(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->LongInteger= mrc_getDatetime(Param[0]->Val->Pointer);
}

void Lib_GetMemRemain(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->UnsignedLongInteger= mrc_getMemoryRemain();
}

void Lib_Shake(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->LongInteger= mrc_startShake(Param[0]->Val->LongInteger);
}

void Lib_GetSysMem(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->LongInteger= mrc_getSysMem();
}

void Lib_RunMrp(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->LongInteger= mrc_runMrp(Param[0]->Val->Pointer,Param[1]->Val->Pointer,NULL);
}

void Lib_GetParentPath(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
#ifdef _MPC_VC6_
    ReturnValue->Val->Pointer="";// mrc_GetParentPath();VC6.0下不支持
#else
    ReturnValue->Val->Pointer= mrc_GetParentPath();
#endif
}

void Lib_Sand(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    mrc_sand(Param[0]->Val->UnsignedLongInteger);
}

void Lib_Rand(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->UnsignedLongInteger= mrc_rand();
}

void Lib_SendSms(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    extern int32 mrc_sendSms(char* pNumber, char*pContent, int32 flags);
    ReturnValue->Val->Integer= mrc_sendSms(Param[0]->Val->Pointer,Param[1]->Val->Pointer,Param[2]->Val->Integer);
}

void Lib_Lcd(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->LongInteger= mrc_LCDCanSleep(Param[0]->Val->Character);
}

void Lib_GetScrBuf(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->Pointer= w_getScreenBuffer();
}

void Lib_SetScrBuf(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    w_setScreenBuffer(Param[0]->Val->Pointer);
}

void Lib_GetScrSize(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    mrc_getScreenSize(Param[0]->Val->Pointer,Param[1]->Val->Pointer);
}

void Lib_SetScrSize(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    mrc_setScreenSize(Param[0]->Val->Integer,Param[1]->Val->Integer);
}

void Lib_BmpShowFlip(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->LongInteger= mrc_bitmapShowFlip(Param[0]->Val->Pointer,
        Param[1]->Val->ShortInteger,
        Param[2]->Val->ShortInteger,
        Param[3]->Val->UnsignedShortInteger,
        Param[4]->Val->UnsignedShortInteger,
        Param[5]->Val->UnsignedShortInteger,
        Param[6]->Val->UnsignedShortInteger,
        Param[7]->Val->ShortInteger,
        Param[8]->Val->ShortInteger,
        Param[9]->Val->UnsignedShortInteger);
}

void Lib_Wap(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    extern void mrc_connectWAP(char* wap);
    mrc_connectWAP(Param[0]->Val->Pointer);
}

void Lib_FreeFileData(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    mrc_freeFileData(Param[0]->Val->Pointer,Param[1]->Val->LongInteger);
}

void Lib_FreeOrigin(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    mrc_freeOrigin(Param[0]->Val->Pointer,Param[1]->Val->LongInteger);
}

void TimerCb(int32 data)//所有定时器的回调,data为使用的定时器
{
    struct timerST *list=timerList;
    static char Source[256];
    char *ptr=NULL;

    while(list != NULL)
    {
        if(list->timer == data)
        {
            FuncRet=list->data;//FuncRet就是内部符号_ret
            ptr = list->callback;
            break;
        }
        list=list->next;
    }
    if(CheckBaseCall(ptr,&VoidType,1))
    {
        mrc_timerStop(data);
        return;
    }
    mrc_sprintf(Source,"%s(_ret);",ptr);
    PicocParse(" ", Source, strlen(Source), TRUE, TRUE, FALSE);
}
void Lib_TimerCreate(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    int32 timer;
    timer = mrc_timerCreate();
    if(timer)
    {
        struct timerST *NewNode;
        
        NewNode=mrc_malloc(sizeof(struct timerST));
        NewNode->next=timerList;
        NewNode->timer=timer;
        NewNode->callback=NULL;
        timerList=NewNode;
    }
    ReturnValue->Val->LongInteger = timer;
}

void Lib_TimerDel(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{   
    int32 timer=Param[0]->Val->LongInteger;
    struct timerST *list=timerList;
    struct timerST *prev=timerList;

    while(list != NULL)
    {
        if(list->timer == timer)
        {
            if(list==timerList)
                timerList=timerList->next;
            else
                prev->next=list->next;
            if(list->callback) mrc_free(list->callback);
            mrc_free(list);
            break;
        }
        prev=list;
        list=list->next;
    }
    mrc_timerDelete(timer);
}

void Lib_TimerStop(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    mrc_timerStop(Param[0]->Val->LongInteger);
}

void Lib_TimerStart(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    int32 timer = Param[0]->Val->LongInteger;
    char *callback=Param[3]->Val->Pointer;
    struct timerST *list=timerList;

    while(list != NULL)
    {
        if(list->timer == timer)
        {
            list->data = Param[2]->Val->LongInteger;
            list->callback=mrc_malloc(strlen(callback)+1);
            mrc_strcpy(list->callback,callback);
            break;
        }
        list=list->next;
    }
    ReturnValue->Val->LongInteger= mrc_timerStart(timer,Param[1]->Val->LongInteger,timer,TimerCb,Param[4]->Val->LongInteger);
}

void Lib_TimerSetTime(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->LongInteger= mrc_timerSetTimeEx(Param[0]->Val->LongInteger,Param[1]->Val->LongInteger);
}

void Lib_DrawImg(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->Integer= DrawIMG(Param[0]->Val->Pointer,Param[1]->Val->Integer,Param[2]->Val->Integer);
}

void Lib_ShadeRect(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    DrawShadeRect(Param[0]->Val->Integer, Param[1]->Val->Integer, Param[2]->Val->Integer, Param[3]->Val->Integer,Param[4]->Val->LongInteger,Param[5]->Val->LongInteger,Param[6]->Val->Integer);
}
/*
void Lib_(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
ReturnValue->Val->LongInteger= mrc_fileState(Param[0]->Val->Pointer);
}

void Lib_(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
ReturnValue->Val->LongInteger= mrc_fileState(Param[0]->Val->Pointer);
}
*/
void Lib_ReadFileFromMrp(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->LongInteger= mrc_readFileFromMrpEx(Param[0]->Val->Pointer,
        Param[1]->Val->Pointer,
        Param[2]->Val->Pointer,
        Param[3]->Val->Pointer,
        Param[4]->Val->LongInteger);
}

void Lib_ClearScreen(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs) 
{
    mrc_clearScreen(Param[0]->Val->Integer, Param[1]->Val->Integer, Param[2]->Val->Integer);
}

void Lib_refreshScreen(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs) 
{
    mrc_refreshScreen(Param[0]->Val->ShortInteger, Param[1]->Val->ShortInteger, Param[2]->Val->ShortInteger, Param[3]->Val->ShortInteger);
}

void Lib_drawRect(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs) 
{
    mrc_drawRect(Param[0]->Val->ShortInteger, Param[1]->Val->ShortInteger, Param[2]->Val->ShortInteger, Param[3]->Val->ShortInteger,
        Param[4]->Val->Character, Param[5]->Val->Character, Param[6]->Val->Character);
}

void Lib_drawLine(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs) 
{
    mrc_drawLine(Param[0]->Val->ShortInteger, Param[1]->Val->ShortInteger, Param[2]->Val->ShortInteger, Param[3]->Val->ShortInteger,
        Param[4]->Val->Character, Param[5]->Val->Character, Param[6]->Val->Character);
}

void Lib_DrawPoint(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs) 
{
    mrc_drawPoint(Param[0]->Val->ShortInteger, Param[1]->Val->ShortInteger, 
        Param[2]->Val->ShortInteger);
}

void Lib_drawPointEx(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs) 
{
    mrc_drawPointEx(Param[0]->Val->ShortInteger, Param[1]->Val->ShortInteger, 
        Param[2]->Val->ShortInteger, Param[3]->Val->ShortInteger, Param[4]->Val->ShortInteger);
}

void Lib_drawText(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs) 
{
    mrc_drawText((char*)Param[0]->Val->Pointer, Param[1]->Val->ShortInteger, Param[2]->Val->ShortInteger, 
        Param[3]->Val->Character, Param[4]->Val->Character, Param[5]->Val->Character,
        Param[6]->Val->Integer, Param[7]->Val->UnsignedShortInteger);
}


////////////////////////////////////////////////////////////////////
const char mrc_base_Defs[] = "\
#define _VERSION 1001\r\n\
#define _DEBUG 1\r\n\
typedef unsigned short uint16;\
typedef unsigned long uint32;\
typedef long int32;\
typedef unsigned char uint8;\
typedef char int8;\
typedef short int16;\
typedef struct{uint16 x;uint16 y;uint16 w;uint16 h;}rectst;\
typedef struct{uint8 r;uint8 g;uint8 b;}colorst;\
enum{KY_DOWN,KY_UP,MS_DOWN,MS_UP,MN_SLT,MN_RET,MR_DIALOG,MS_MOVE=12};\
enum{_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_STAR,_POUND,_UP,_DOWN,_LEFT,_RIGHT,_SLEFT=17,_SRIGHT,_SEND,_SELECT};\
enum{SEEK_SET,SEEK_CUR,SEEK_END};\
enum{IS_FILE=1,IS_DIR=2,IS_INVALID=8};\
enum{DLG_OK,DLG_CANCEL};\
enum{BM_OR,BM_XOR,BM_COPY,BM_NOT,BM_MERGENOT,BM_ANDNOT,BM_TRANS,BM_AND,BM_GRAY,BM_REVERSE};";





struct LibraryFunction mrc_base_Functions[100];
void mrc_base_SetupFunc(void)
{
    mrc_base_Functions[0].Func = Lib_Malloc;
    mrc_base_Functions[0].Prototype = "void* malloc(int);";

    mrc_base_Functions[1].Func = Lib_Free;
    mrc_base_Functions[1].Prototype = "void free(void*);";

    mrc_base_Functions[2].Func = Lib_U2c;
    mrc_base_Functions[2].Prototype = "int u2c(char*,int,char**,int*);";

    mrc_base_Functions[3].Func = Lib_Memcpy;
    mrc_base_Functions[3].Prototype = "void* memcpy(void*,void*,uint32);";

    mrc_base_Functions[4].Func = Lib_Memmove;
    mrc_base_Functions[4].Prototype = "void* memmove(void *,void*,uint32);";

    mrc_base_Functions[5].Func = Lib_Strcpy;
    mrc_base_Functions[5].Prototype = "char* strcpy(char*,char*);";

    mrc_base_Functions[6].Func = Lib_Strncpy;
    mrc_base_Functions[6].Prototype = "char* strncpy(char*,char*,int);";

    mrc_base_Functions[7].Func = Lib_Strcat;
    mrc_base_Functions[7].Prototype = "char* strcat(char*,char*);";

    mrc_base_Functions[8].Func = Lib_Strncat;
    mrc_base_Functions[8].Prototype = "char* strncat(char*,char*,int);";

    mrc_base_Functions[9].Func = Lib_Memcmp;
    mrc_base_Functions[9].Prototype = "int memcmp(void*,void*,uint32);";

    mrc_base_Functions[10].Func = Lib_Strcmp;
    mrc_base_Functions[10].Prototype = "int strcmp(char*,char*);";

    mrc_base_Functions[11].Func = Lib_Strncmp;
    mrc_base_Functions[11].Prototype = "int strncmp(char*,char*,int);";

    mrc_base_Functions[12].Func = Lib_Memchr;
    mrc_base_Functions[12].Prototype = "void *memchr(void*,char,uint32);";

    mrc_base_Functions[13].Func = Lib_Memset;
    mrc_base_Functions[13].Prototype = "void *memset(void*,int,uint32);";

    mrc_base_Functions[14].Func = Lib_Strlen;
    mrc_base_Functions[14].Prototype = "uint32 strlen(char*);";

    mrc_base_Functions[15].Func = Lib_Strstr;
    mrc_base_Functions[15].Prototype = "char* strstr(char*,char*);";

    mrc_base_Functions[16].Func = LibPrintf;//定义在clibrary.c中的函数
    mrc_base_Functions[16].Prototype = "void printf(char*,...);";

    mrc_base_Functions[17].Func = Lib_Atoi;
    mrc_base_Functions[17].Prototype = "int atoi(char*);";

    mrc_base_Functions[18].Func = Lib_Strtoul;
    mrc_base_Functions[18].Prototype = "long strtoul(char*,char**,int);";

    mrc_base_Functions[19].Func = Lib_StrrChr;
    mrc_base_Functions[19].Prototype = "char *strrchr(char*,char);";

    mrc_base_Functions[20].Func = Lib_Exit;
    mrc_base_Functions[20].Prototype = "void exit();";

    mrc_base_Functions[21].Func = Lib_ClearScreen;
    mrc_base_Functions[21].Prototype = "void cls(int,int,int);";

    mrc_base_Functions[22].Func = Lib_refreshScreen;
    mrc_base_Functions[22].Prototype = "void ref(int,int,int,int);";

    mrc_base_Functions[23].Func = Lib_drawRect;
    mrc_base_Functions[23].Prototype = "void drect(int,int,int,int,int,int,int);";

    mrc_base_Functions[24].Func = Lib_drawLine;
    mrc_base_Functions[24].Prototype = "void dline(int,int,int,int,int,int,int);";

    mrc_base_Functions[25].Func = Lib_drawPointEx;
    mrc_base_Functions[25].Prototype = "void dpointex(int,int,int,int,int);";

    mrc_base_Functions[26].Func = Lib_drawText;
    mrc_base_Functions[26].Prototype = "int dtext(char*,int,int,int,int,int,int,int);";

    mrc_base_Functions[27].Func = LibSPrintf;//定义在clibrary.c中的函数
    mrc_base_Functions[27].Prototype = "char *sprintf(char*,char*,...);";

    mrc_base_Functions[28].Func = Lib_Open;
    mrc_base_Functions[28].Prototype = "int open(char*,int);";

    mrc_base_Functions[29].Func = Lib_Close;
    mrc_base_Functions[29].Prototype = "int close(int);";

    mrc_base_Functions[30].Func = Lib_FileState;
    mrc_base_Functions[30].Prototype = "int filestate(char*);";

    mrc_base_Functions[31].Func = Lib_Write;
    mrc_base_Functions[31].Prototype = "int write(int,void*,int);";

    mrc_base_Functions[32].Func = Lib_Read;
    mrc_base_Functions[32].Prototype = "int read(int,void*,int);";

    mrc_base_Functions[33].Func = Lib_ReadAll;
    mrc_base_Functions[33].Prototype = "int readall(char*,char*,int*);";

    mrc_base_Functions[34].Func = Lib_Seek;
    mrc_base_Functions[34].Prototype = "int seek(int,int,int);";

    mrc_base_Functions[35].Func = Lib_GetLen;
    mrc_base_Functions[35].Prototype = "int getlen(char*);";

    mrc_base_Functions[36].Func = Lib_Remove;
    mrc_base_Functions[36].Prototype = "int remove(char*);";

    mrc_base_Functions[37].Func = Lib_Rename;
    mrc_base_Functions[37].Prototype = "int rename(char*,char*);";

    mrc_base_Functions[38].Func = Lib_MkDir;
    mrc_base_Functions[38].Prototype = "int mkdir(char*);";

    mrc_base_Functions[39].Func = Lib_RmDir;
    mrc_base_Functions[39].Prototype = "int rmdir(char*);";

    mrc_base_Functions[40].Func = Lib_FindStart;
    mrc_base_Functions[40].Prototype = "int findstart(char*,char*,int);";

    mrc_base_Functions[41].Func = Lib_FindGetNext;
    mrc_base_Functions[41].Prototype = "int findnext(int,char*,int);";

    mrc_base_Functions[42].Func = Lib_FindStop;
    mrc_base_Functions[42].Prototype = "int findstop(int);";

    mrc_base_Functions[43].Func = Lib_DrawTextEx;
    mrc_base_Functions[43].Prototype = "int dtextex(char*,short,short,rectst*,colorst*,int,short);";

    mrc_base_Functions[44].Func = Lib_DrawTextRight;
    mrc_base_Functions[44].Prototype = "int dtextright(char*,short,short,rectst*,colorst*,int,short);";

    mrc_base_Functions[45].Func = Lib_C2u;
    mrc_base_Functions[45].Prototype = "short* c2u(char*,int*,int*);";

    mrc_base_Functions[46].Func = Lib_TextWidthHeight;
    mrc_base_Functions[46].Prototype = "int textwh(char*,int,int,int*,int*);";

    mrc_base_Functions[47].Func = Lib_DrawPoint;
    mrc_base_Functions[47].Prototype = "void dpoint(int,int,int);";

    mrc_base_Functions[48].Func = Lib_EffSetCon;
    mrc_base_Functions[48].Prototype = "void effsetcon(int,int,int,int,int,int,int);";

    mrc_base_Functions[49].Func = Lib_WstrLen;
    mrc_base_Functions[49].Prototype = "int wstrlen(char*);";

    mrc_base_Functions[50].Func = Lib_UnicodeTextRow;
    mrc_base_Functions[50].Prototype = "int unitextrow(uint16*,int,int);";

    mrc_base_Functions[51].Func = Lib_MenuCreate;
    mrc_base_Functions[51].Prototype = "int menucreate(char*, int16);";

    mrc_base_Functions[52].Func = Lib_MenuSet;
    mrc_base_Functions[52].Prototype = "int menuset(int,char*,int);";

    mrc_base_Functions[53].Func = Lib_MenuShow;
    mrc_base_Functions[53].Prototype = "int menushow(int);";

    mrc_base_Functions[54].Func = Lib_MenuDel;
    mrc_base_Functions[54].Prototype = "int menudel(int);";

    mrc_base_Functions[55].Func = Lib_MenuRef;
    mrc_base_Functions[55].Prototype = "int menuref(int);";

    mrc_base_Functions[56].Func = Lib_DlgCreate;
    mrc_base_Functions[56].Prototype = "int dlgcreate(char*,char*,int);";

    mrc_base_Functions[57].Func = Lib_DlgDel;
    mrc_base_Functions[57].Prototype = "int dlgdel(int);";

    mrc_base_Functions[58].Func = Lib_DlgRef;
    mrc_base_Functions[58].Prototype = "int dlgref(int,char*,char*,int);";

    mrc_base_Functions[59].Func = Lib_TextCreate;
    mrc_base_Functions[59].Prototype = "int textcreate(char*,char*,int);";

    mrc_base_Functions[60].Func = Lib_TextDel;
    mrc_base_Functions[60].Prototype = "int textdel(int);";

    mrc_base_Functions[61].Func = Lib_TextRef;
    mrc_base_Functions[61].Prototype = "int textref(int,char*,char*);";

    mrc_base_Functions[62].Func = Lib_EditCreate;
    mrc_base_Functions[62].Prototype = "int editcreate(char*,char*,int,int);";

    mrc_base_Functions[63].Func = Lib_EditDel;
    mrc_base_Functions[63].Prototype = "int editdel(int);";

    mrc_base_Functions[64].Func = Lib_EditGet;
    mrc_base_Functions[64].Prototype = "char* editget(int);";

    mrc_base_Functions[65].Func = Lib_GetUptime;
    mrc_base_Functions[65].Prototype = "int getuptime();";

    mrc_base_Functions[66].Func = Lib_Sleep;
    mrc_base_Functions[66].Prototype = "int sleep(uint32);";

    mrc_base_Functions[67].Func = Lib_GetDateTime;
    mrc_base_Functions[67].Prototype = "int getdatetime(void*);";

    mrc_base_Functions[68].Func = Lib_GetMemRemain;
    mrc_base_Functions[68].Prototype = "uint32 getmemremain();";

    mrc_base_Functions[69].Func = Lib_Shake;
    mrc_base_Functions[69].Prototype = "int shake(int);";

    mrc_base_Functions[70].Func = Lib_GetSysMem;
    mrc_base_Functions[70].Prototype = "int getsysmem();";

    mrc_base_Functions[71].Func = Lib_RunMrp;
    mrc_base_Functions[71].Prototype = "int runmrp(char*,char*);";

    mrc_base_Functions[72].Func = Lib_GetParentPath;
    mrc_base_Functions[72].Prototype = "char* getparentpath();";

    mrc_base_Functions[73].Func = Lib_Sand;
    mrc_base_Functions[73].Prototype = "void sand(uint32);";

    mrc_base_Functions[74].Func = Lib_Rand;
    mrc_base_Functions[74].Prototype = "uint32 rand();";

    mrc_base_Functions[75].Func = Lib_SendSms;
    mrc_base_Functions[75].Prototype = "int sms(char*,char*,int);";

    mrc_base_Functions[76].Func = Lib_Lcd;
    mrc_base_Functions[76].Prototype = "int lcd(char);";

    mrc_base_Functions[77].Func = Lib_GetScrBuf;
    mrc_base_Functions[77].Prototype = "uint16 *getscrbuf();";

    mrc_base_Functions[78].Func = Lib_SetScrBuf;
    mrc_base_Functions[78].Prototype = "void setscrbuf(uint16*);";

    mrc_base_Functions[79].Func = Lib_GetScrSize;
    mrc_base_Functions[79].Prototype = "void getscrsize(int*,int*);";

    mrc_base_Functions[80].Func = Lib_SetScrSize;
    mrc_base_Functions[80].Prototype = "void setscrsize(int,int);";

    mrc_base_Functions[81].Func = Lib_BmpShowFlip;
    mrc_base_Functions[81].Prototype = "int bmpshowflip(uint16*,int16,int16,uint16,uint16,uint16,uint16,int16,int16,uint16);";

    mrc_base_Functions[82].Func = Lib_Wap;
    mrc_base_Functions[82].Prototype = "void wap(char*);";

    mrc_base_Functions[83].Func = Lib_ReadFileFromMrp;
    mrc_base_Functions[83].Prototype = "int readfilefrommrp(char*,char*,uint8**,long*,int);";

    mrc_base_Functions[84].Func = Lib_FreeFileData;
    mrc_base_Functions[84].Prototype = "void freefiledata(void*,int);";

    mrc_base_Functions[85].Func = Lib_FreeOrigin;
    mrc_base_Functions[85].Prototype = "void freeorigin(void*,int);";

    mrc_base_Functions[86].Func = Lib_TimerCreate;
    mrc_base_Functions[86].Prototype = "long timercreate();";

    mrc_base_Functions[87].Func = Lib_TimerDel;
    mrc_base_Functions[87].Prototype = "void timerdel(long);";

    mrc_base_Functions[88].Func = Lib_TimerStop;
    mrc_base_Functions[88].Prototype = "void timerstop(long);";

    mrc_base_Functions[89].Func = Lib_TimerStart;
    mrc_base_Functions[89].Prototype = "int timerstart(long,long,long,char*,int);";

    mrc_base_Functions[90].Func = Lib_TimerSetTime;
    mrc_base_Functions[90].Prototype = "int timersettime(long,long);";

    mrc_base_Functions[91].Func = Lib_DrawImg;
    mrc_base_Functions[91].Prototype = "int img(char*,int,int);";

    mrc_base_Functions[92].Func = Lib_ShadeRect;
    mrc_base_Functions[92].Prototype = "void shaderect(int,int,int,int,long,long,int);";

    mrc_base_Functions[93].Func = NULL;
    mrc_base_Functions[93].Prototype = NULL;
/*
    mrc_base_Functions[].Func = Lib_;
    mrc_base_Functions[].Prototype = NULL;

    mrc_base_Functions[].Func = Lib_;
    mrc_base_Functions[].Prototype = NULL;

    mrc_base_Functions[].Func = NULL;
    mrc_base_Functions[].Prototype = NULL;

    mrc_base_Functions[].Func = NULL;
    mrc_base_Functions[].Prototype = NULL;
    */
}

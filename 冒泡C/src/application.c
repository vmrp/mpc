#include "application.h"
#include "topwnd.h"
#include "littlec.h"
#include "mrc_base.h"
#include "momo.h"
#include "mrc_base_i.h"
#include "mainwnd.h"
#include "smp.h"
#include "mrc_skyfont.h"
#include "picoc.h"
// int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow);
char *tempfile_path;
char cdir[3]="c/";

// /* 字符串哈希函数 */
// static unsigned int TableHash(const char *Key, int Len)
// {
//     unsigned int Hash = Len;
//     int Offset;
//     int Count;
// 
//     for (Count = 0, Offset = 8; Count < Len; Count++, Offset+=7)
//     {
//         if (Offset > sizeof(unsigned int) * 8 - 7)
//             Offset -= sizeof(unsigned int) * 8 - 6;
// 
//         Hash ^= *Key++ << Offset;
//     }
// 
//     return Hash;
// }

int InitApplication(VOID)
{	
    char path[40];
    mrc_sprintf(path,"%stemp.c",STOREDIR);
    tempfile_path=mrc_malloc(mrc_strlen(path)+1);
    mrc_strcpy(tempfile_path,path);
    mrc_mkDir(cdir);
//     BMPScreen("logo.bmp",137,136,BM_COPY,0x0000);

    if(_VERSION != readCfg(CFG_FIRSTRUN))//第一次运行，初始化
    {
        char *data=NULL;
        int32 len=0;

        mrc_clearScreen(0,0,0);
        TextMiddle("初始化",0,1);
        data = (char*)mrc_readFileFromMrp("bat",&len,0);
        if(data)
        {
            PicocInitialise(HEAP_SIZE);
            PicocParse("bat", data, len, TRUE, FALSE, FALSE);
            PicocCleanup();
            mrc_freeFileData(data,len);
        }
    }


    InitTopWindow();
	ShowTopWindow(TOPWND_MAINWND, 0, 0);
	return 0;
}


int ExitApplication(VOID)
{        
    if(_FUNCIS_SET_ANY(FUNC_INITSUC)) 
    {
        if(!_FUNCIS_SET_ANY(FUNC_EXIT)) SaveCode((PSTR)tempfile_path);
        mrc_free(tempfile_path);
        mrc_clearScreen(0,0,0);
        TextMiddle("正在退出",0,1);
        momo_exit();
    }
	return 0;
}

int PauseApplication(VOID)
{
	return 0;
}


int ResumeApplication(VOID)
{
	return 0;
}


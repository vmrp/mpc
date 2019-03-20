#include "opendlg.h"
#include "topwnd.h"
#include "mainwnd.h"
#include "smp.h"
#include "smp_menu.h"
#include "smp_toolbar.h"
#include "smp_titlebar.h"
#include "smp_menuwnd.h"
#include "smp_msgbox.h"
#include "bmp.h"
#include "smp_list.h"
#include "string.h"
#include "i18n.h"
#include "smp_inputbox.h"
// #include "smp_advbar.h"
#include "application.h"


#define MAX_FILEN         1000
#define FILEN             MAX_FILEN * sizeof(PFILEST) //最大显示数量1000个

//主窗口上的子窗口ID列表
enum
{
	DLGWND_TOOLBAR = 1,        //工具条
	DLGWND_OPTMENU,	    //主菜单
	DLGWND_LIST,
    DLGWND_INPUTBOX,
    DLGWND_REMOVEMSG,
    DLGWND_MSGCOPYSAME,
    DLGWND_MSGCUTSAME,
    DLGWND_INPUTRENAME
};

/* 文件信息结构体 版本2(指针型) */
typedef struct FileSt
{
	char* name;
	uint8 type;
	struct FileSt *next;
}FILEST, *PFILEST;

typedef enum DIALOGSTATE{
    DIALOGSTATE_NOP,
    DIALOGSTATE_CUT,
    DIALOGSTATE_COPY
}DIALOGSTATE;

char SMP_DIALOG_CUR_PATH[128];   //当前搜索路径
char SMP_DIALOG_PATH[256];  //用来保存一个文件的完整路径
char SMP_DIALOG_TEMP_PATH[256]; //临时使用
char SMP_DIALOG_TEMP[128];//路径返回时使用
static int16 filenum;
static int16 dirnum;
static int16 count;//列表总数
static int16 Index;
static SMP_DIALOG_TYPE DialogType;     //要显示的类型
static DIALOGSTATE state;//程序状态
//链表
static PFILEST *file;  //文件
static PFILEST pFolder_head, pFolder_temp;   //文件夹链表
static PFILEST pFile_head, pFile_temp;       //文件链表
HWND hList;//列表窗口句柄
uint8 SMP_DIALOG_ID;

static const DWORD miOptions[] = 
{
    STR_SELECT,     //0选定
    STR_OPENDIR,    //1打开文件夹
    STR_NEWDIR,		//2新建文件夹
    STR_COPYFILE,   //3复制文件
    STR_RENAME,     //4重命名
    STR_CUT,        //5剪切
    STR_PASTE,      //6粘贴
    STR_REMOVE,     //7删除
    STR_RETURN	    //8返回	
};

static VOID ShowOptMenu(HWND hWnd)
{
    Index = SMP_List_GetHilightIndex(hList);

	SMP_Menu_ClearMenuItems();
	SMP_Menu_SetMenuItem2(0, miOptions, TABLESIZE(miOptions));
    if(!file[0])//当进入空文件夹时
    {
        SMP_Menu_DisableMenuItem(0,1);      //0选定
        SMP_Menu_DisableMenuItem(1,1);      //1打开文件夹
//         SMP_Menu_DisableMenuItem(2,1);   //2新建文件夹
        SMP_Menu_DisableMenuItem(3,1);      //3复制文件
        SMP_Menu_DisableMenuItem(4,1);      //4重命名
        SMP_Menu_DisableMenuItem(5,1);      //5剪切
//         SMP_Menu_DisableMenuItem(6,1);   //6粘贴
        SMP_Menu_DisableMenuItem(7,1);      //7删除
    }                                       
    else if(DialogType == DIALOG_SELECTDIR || file[Index]->type==MR_IS_DIR)
        SMP_Menu_DisableMenuItem(3,1);//停留在文件夹上
    else
        SMP_Menu_DisableMenuItem(1,1);//停留在文件上
	SMP_Menu_Popup(DLGWND_OPTMENU, SMP_MENUS_BOTTOMLEFT, hWnd, 0, _HEIGHT(hWnd) - SMP_ITEM_HEIGHT, NULL);
}

void ShowOpenDlg(uint8 Id, WID wndid, SMP_DIALOG_TYPE type)
{
    SMP_DIALOG_ID=Id;
	DialogType = type;
	HideTopWindow(wndid, 0, 0);
	ShowTopWindow(TOPWND_OPENDLG, NULL, (DWORD)wndid);
}

static void MakeDialogPath(void)
{
    if(!file) return;
    mrc_strcpy(SMP_DIALOG_PATH, SMP_DIALOG_CUR_PATH);
    mrc_strcat(SMP_DIALOG_PATH, file[Index]->name);
}
static void HideOpenDlg(HWND hWnd)
{
    MakeDialogPath();
	HideTopWindow(TOPWND_OPENDLG, FALSE, FALSE);
	ShowTopWindow((WID)_USERDATA(hWnd), NULL, DIALOG_GET);
}

static void InitSearch(void)//4.释放上次的文件结构空间并初始化搜索数据
{
	int i;

	for(i=0; i<count; i++)
	{
		if(file[i])
		{
			if(file[i]->name)
				mrc_free(file[i]->name);
			mrc_free(file[i]);
		}
	}
    mrc_memset(file,0,FILEN);
	pFile_head = pFile_temp = NULL;
	pFolder_head = pFolder_temp = NULL;
	dirnum = filenum = count = Index = 0;
}

static int32 NewFileStruct(PSTR name, int32 type)
{
	int32 name_len;
	PFILEST temp;
	PSTR temp2;

	temp = (PFILEST)mrc_malloc(sizeof(FILEST));
	if(!temp) return -1;
	mrc_memset(temp, 0, sizeof(FILEST));
	name_len = mrc_strlen(name);
	if(name_len<16)
        name_len = 16;
    else
        name_len += 1;
	temp2 = (PSTR)mrc_malloc(name_len);
	if(!temp2)
	{
		mrc_free(temp);
		return -1;
	}
	mrc_strcpy(temp2, name);
	temp->name = temp2;

	//进行累加
	count++;   //列表总数
	if(type == MR_IS_FILE)
		filenum++;
	else
		dirnum++;
	if(type == MR_IS_FILE)
	{
		if(filenum == 1)
			pFile_temp = pFile_head = temp;
		else
		{
			pFile_temp->next = temp;
			pFile_temp = temp;
		}
	}
	else
	{
		if(dirnum == 1)
			pFolder_temp = pFolder_head = temp;
		else
		{
			pFolder_temp->next = temp;
			pFolder_temp = temp;
		}
	}
	temp->type = (uint8)type;
	return MR_SUCCESS;
}

static void StartSearchDir(void)//遍历指定目录
{
	uint32 len;
	char path[150] = {0};
	char name_get[100] = {0};  
	int32 handel, ret, type;

	len = sizeof(name_get);                     //文件名缓冲长度
	InitSearch();//1.初始化
	handel = mrc_findStart(SMP_DIALOG_CUR_PATH, name_get, len);   //如果发生路径错误这一步就查找失败.
	if(handel > 0)               
	{
		ret ^= ret;//首先清零使循环得以进行
		while(!ret && count < MAX_FILEN)
		{
			mrc_strcpy(path, SMP_DIALOG_CUR_PATH); //合成路径
			mrc_strcat(path, name_get);
            type = mrc_fileState(path);

			//如果fileName为"."、或无效文件则放弃本次搜索
			if(name_get[0] != '.' && type != MR_IS_INVALID) 
			{

				if(DialogType == DIALOG_SELECTDIR)//选择文件夹
					if(type == MR_IS_FILE)
						goto next;
				if(MR_FAILED == NewFileStruct(name_get, type))
					goto end;
			}
next:
			ret = mrc_findGetNext(handel, name_get, len);
		}
end:
        mrc_findStop(handel);

        //5.文件排序并添加列表(并不是真正的排序,只是将文件夹放在前面,文件放在后面)
        type ^= type;
        pFolder_temp = pFolder_head;
        for(ret^=ret; ret<dirnum; ++ret)
        {
            file[type++] = pFolder_temp;
            if(!mrc_strcmp(SMP_DIALOG_TEMP,pFolder_temp->name))
                Index = (int16)type-1;
            pFolder_temp = pFolder_temp->next;
        }
        pFile_temp = pFile_head;
        for(ret^=ret; ret<filenum; ++ret)
        {
            file[type++] = pFile_temp;
            pFile_temp = pFile_temp->next;
        }
	}
}

static void RefList(void)//重新搜索并更新列表
{
    StartSearchDir();
    SMP_List_Refresh(hList);
    SGL_UpdateWindow(hList);
}
static void FileList_Return(HWND hWnd)//返回上级,hWnd:文件列表窗口句柄
{
	PSTR position;

	position = mrc_strrchr((PCSTR)SMP_DIALOG_CUR_PATH ,'/');
	if(position != NULL)       //如果查找成功说明不是根目录
	{
		int len;
        char buf[128]={0};

		len = position - SMP_DIALOG_CUR_PATH;
		mrc_strncpy(buf ,(PSTR)SMP_DIALOG_CUR_PATH, len);//从源字符串复制 n 个字符串到目标字符串
		position = mrc_strrchr((PCSTR)buf, '/');
		if(position)   //如果查找成功说明非第一级子目录
		{
			mrc_memset(SMP_DIALOG_CUR_PATH, 0, sizeof(SMP_DIALOG_CUR_PATH));
			len = position - buf + 1;//必需加1
			mrc_strncpy(SMP_DIALOG_CUR_PATH, (PSTR)buf, len);
            mrc_strcpy(SMP_DIALOG_TEMP,position+1);
		}else    //为一级子目录，相当于返回主页
        {
            mrc_strcpy(SMP_DIALOG_TEMP,buf);
            SMP_DIALOG_CUR_PATH[0]='\0';
        }
        StartSearchDir();
        SMP_List_Refresh(hList);
        SMP_List_HilightByIndex(hList,Index);
	}
}
static int GetFileNumber(void)//1.获取列表数目的回调函数
{
	return (int)count;
}

static void AddFileToList(int index, PSMPROWDATA pRowData)//2.给列表添加数据的回调函数
{
	int w, h;

	if (!pRowData || index < 0) //如果出错跳出
		return;

	//判断类型（文件/文件夹）
	switch(file[index]->type)
	{
	case MR_IS_FILE:
		pRowData->hBmp = SGL_LoadBitmap(BMP_FILE, &w, &h);
		break;
	case MR_IS_DIR:
		pRowData->hBmp = SGL_LoadBitmap(BMP_FOLDER, &w, &h);
		break;
    default:
        return;
	}

	pRowData->margin = 4;              //列表左间距
	pRowData->colWidth0 = (uint16)w;   //图标宽度
	pRowData->colHeight0 = (uint16)h;  //图标高度
	pRowData->cols[0].width = SCREEN_WIDTH - 4 - w - 20;  //列表宽度
	pRowData->cols[0].str = (PCWSTR)file[index]->name;      //文件名
}


static void OpenDir(HWND hWnd)
{
    HWND hMsgBox = SMP_MsgBox(0, hWnd, NULL, SGL_LoadString(STR_MSG),
        SGL_LoadString(STR_STARTFIND), ID_NON, NULL);
    MR_STRCAT(SMP_DIALOG_CUR_PATH, file[Index]->name);
    MR_STRCAT(SMP_DIALOG_CUR_PATH, "/");
    StartSearchDir();
    SMP_List_Refresh(hList);
    SMP_Close_MsgBox(hMsgBox);
}
static void MenuEvent(HWND hWnd, WORD code)
{
    Index = SMP_List_GetHilightIndex(hList);

    switch(code)
    {
    case STR_SELECT: 
        if(DialogType == DIALOG_SELECTDIR)//选择文件夹
        	HideOpenDlg(hWnd);
        else if(file[Index]->type == MR_IS_FILE)
        	HideOpenDlg(hWnd);
        return;
    case STR_OPENDIR:
        OpenDir(hWnd);
        return;
    case STR_NEWDIR:
        SMP_InputBox(DLGWND_INPUTBOX,hWnd,SGL_LoadString(STR_NEWDIR),SGL_LoadString(STR_NEWDIR),128,ES_ALPHA,NULL);
        return;
    case STR_RENAME://重命名
        {
            PSTR position;
            UnicodeSt uniText;

            MakeDialogPath();
            position = mrc_strrchr((PCSTR)SMP_DIALOG_PATH ,'/');
            if(!position) position=SMP_DIALOG_PATH;
            else position++;
            strcpy(SMP_DIALOG_TEMP_PATH,position);
            CharToUnicode(SMP_DIALOG_TEMP_PATH, &uniText.pText, &uniText.len);
            SMP_InputBox(DLGWND_INPUTRENAME,hWnd,SGL_LoadString(STR_RENAME),(PWSTR)uniText.pText,128,ES_ALPHA,NULL);
            mrc_freeOrigin(uniText.pText, uniText.len);
        }
        return;
    case STR_CUT:
        MakeDialogPath();
        state=DIALOGSTATE_CUT;
        return;
    case STR_COPYFILE:
        MakeDialogPath();
        state=DIALOGSTATE_COPY;
        return;
    case STR_PASTE://粘贴
        {
            PSTR position;

            if(state==DIALOGSTATE_NOP) break;
            if(mrc_fileState(SMP_DIALOG_PATH) == MR_IS_INVALID) break;
            position = mrc_strrchr((PCSTR)SMP_DIALOG_PATH ,'/');
            if(!position) position=SMP_DIALOG_PATH;
            else position++;
            strcpy(SMP_DIALOG_TEMP_PATH,SMP_DIALOG_CUR_PATH);
            strcat(SMP_DIALOG_TEMP_PATH,position);
            if(!mrc_strcmp(SMP_DIALOG_TEMP_PATH,SMP_DIALOG_PATH)) //路径相同不作处理
                break;
            if(state == DIALOGSTATE_COPY)//复制
            {
                if(mrc_fileState(SMP_DIALOG_TEMP_PATH) != MR_IS_INVALID)
                    SMP_MsgBox(DLGWND_MSGCOPYSAME, hWnd, NULL, SGL_LoadString(STR_MSG), SGL_LoadString(STR_WARNING1), ID_OK|ID_CANCEL|ID_YESNO , NULL);
                else
                {
                    CopyFile(SMP_DIALOG_TEMP_PATH,SMP_DIALOG_PATH);
                    RefList();
                }
            }
            else//剪切
            {
                if(mrc_fileState(SMP_DIALOG_TEMP_PATH) != MR_IS_INVALID)
                    SMP_MsgBox(DLGWND_MSGCUTSAME, hWnd, NULL, SGL_LoadString(STR_MSG), SGL_LoadString(STR_WARNING1), ID_OK|ID_CANCEL|ID_YESNO , NULL);
                else
                {
                    mrc_rename(SMP_DIALOG_PATH,SMP_DIALOG_TEMP_PATH);
                    state=DIALOGSTATE_NOP;
                    RefList();
                }
            }
        }
        return;
    case STR_REMOVE:
        SMP_MsgBox(DLGWND_REMOVEMSG, hWnd, NULL, SGL_LoadString(STR_MSG), SGL_LoadString(STR_REALREMOVE), ID_OK|ID_CANCEL|ID_YESNO , NULL);
        return;
    case STR_RETURN:
        HideTopWindow(TOPWND_OPENDLG, 0, 0);
        ShowTopWindow((WID)_USERDATA(hWnd), NULL, 0); 
        return;
    }
}

LRESULT OpenDlg_WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch(Msg)
	{
	case WM_CREATE:    //来自创建的消息，从此处创建各个控件
	{   
        HWND hControl;
		//创建标题栏
		hControl = SGL_CreateWindow(SMP_Titlebar_WndProc,
			0, 0, SCREEN_WIDTH, SMP_ITEM_HEIGHT,
			0, SMP_TITLEBAR_STATIC,0);
		SMP_Titlebar_SetContent(hControl, RESID_INVALID, SGL_LoadString(STR_SELECT), FALSE);
		_FGCOLOR(hControl)=COLOR_lightwhite;
		SGL_AddChildWindow(hWnd, hControl);

		//创建list
		hList = SGL_CreateWindow(SMP_List_WndProc,
			0, SMP_ITEM_HEIGHT, SCREEN_WIDTH, SCREEN_HEIGHT - 2*SMP_ITEM_HEIGHT,
			DLGWND_LIST, 0, 0);
		SMP_List_SetDataProvider(hList, GetFileNumber, AddFileToList);
		SGL_AddChildWindow(hWnd, hList);

		//创建工具栏
		hControl = SGL_CreateWindow(
			SMP_Toolbar_WndProc, 
			0, SCREEN_HEIGHT - SMP_ITEM_HEIGHT, 
			SCREEN_WIDTH,SMP_ITEM_HEIGHT,
			DLGWND_TOOLBAR, 0, 0);
		SMP_Toolbar_SetStrings(hControl, STR_MENU, RESID_INVALID, STR_BACK, FALSE);
		SGL_AddChildWindow(hWnd, hControl);
        file = NULL;
        SMP_DIALOG_TEMP[0]=0;
        //下面设置初始目录
        mrc_strcpy(SMP_DIALOG_CUR_PATH,cdir);
        break;  
    }
    case WM_PAINT:
//         mrc_advStopDraw();
        break;
	case WM_SHOW:
	{
		if(!file)
		{
			file = mrc_malloc(FILEN);
            if(!file)
            {
				HideTopWindow(TOPWND_OPENDLG, 0, 0);
				ShowTopWindow((WID)_USERDATA(hWnd), NULL, 0); 
                return -1;
            }
            count = 0;//!
            mrc_memset(file,0,FILEN);//!
			StartSearchDir();
		}
        state=DIALOGSTATE_NOP;
		break;
	}
	case WM_HIDE:
	{
		if(file)
		{	
			InitSearch();
			free(file);
		}
		file=NULL;
		break;
	}
	case WM_INITFOCUS:  
	{
		SGL_SetFocusWindow(hWnd, hList);
		break;
	}	
	case WM_KEYUP:  
	{
		switch(wParam)
		{
		case MR_KEY_SOFTLEFT: 
			ShowOptMenu(hWnd);
			return 1;
		case MR_KEY_SOFTRIGHT:  //返回
			if(!SMP_DIALOG_CUR_PATH[0])
			{
				HideTopWindow(TOPWND_OPENDLG, 0, 0);
				ShowTopWindow((WID)_USERDATA(hWnd), NULL, 0); 
			}else
				FileList_Return(hWnd);
			return 1;
		}
		break;
	}
	case WM_COMMAND:   
	{
		WID id = LOWORD(wParam);  
		WORD code = HIWORD(wParam);  

		switch(id)    
		{
		case DLGWND_LIST:
			{
				switch(code)
				{
				case SMP_LISTN_SELECTED:
				case SMP_LISTN_CLICKED:
					{
						Index = (int16)lParam;
                        if(!file[0])//没有文件
                            break;
						else if(file[Index]->type == MR_IS_FILE)//选择文件  
							HideOpenDlg(hWnd);
						else if(DialogType == DIALOG_SELECTDIR)//选择文件夹
                            HideOpenDlg(hWnd);
                        else//进入文件夹
                            OpenDir(hWnd);

					}
					break;
				}
			}
			break;

		case DLGWND_OPTMENU:
            MenuEvent(hWnd, code);
			break;

        case DLGWND_REMOVEMSG:
            if(code == ID_OK)
            {
                MakeDialogPath();
                if(file[Index]->type == MR_IS_FILE)
                    mrc_remove(SMP_DIALOG_PATH);
                else
                    removeDir(SMP_DIALOG_PATH);
                RefList();
            }
            break;

        case DLGWND_INPUTBOX: //创建目录
            if(code == INPUT_OK)
            {
                PSTR tt=NULL;

                tt = UnicodeToChar((PCWSTR)lParam);
                mrc_strcpy(SMP_DIALOG_PATH,SMP_DIALOG_CUR_PATH);
                mrc_strcat(SMP_DIALOG_PATH,tt);
                SGL_FREE(tt);
                mrc_mkDir(SMP_DIALOG_PATH);
                RefList();
            }
            break;

        case DLGWND_MSGCUTSAME:
            if(code == ID_OK)
            {
                removeDir(SMP_DIALOG_TEMP_PATH);
                mrc_rename(SMP_DIALOG_PATH,SMP_DIALOG_TEMP_PATH);
                state=DIALOGSTATE_NOP;
                RefList();
            }
            break;

        case DLGWND_MSGCOPYSAME: //粘贴
            if(code == ID_OK)
            {//不需要先删除
                CopyFile(SMP_DIALOG_TEMP_PATH,SMP_DIALOG_PATH);
                RefList();
            }
            break;

        case DLGWND_INPUTRENAME: //重命名
            if(code == INPUT_OK)
            {
                PSTR tt=NULL;
                
                tt = UnicodeToChar((PCWSTR)lParam);
                mrc_strcpy(SMP_DIALOG_TEMP_PATH,SMP_DIALOG_CUR_PATH);
                mrc_strcat(SMP_DIALOG_TEMP_PATH,tt);
                mrc_free(file[Index]->name);
                file[Index]->name = tt;//重置当前文件(夹)的名称,tt不需要释放
                mrc_rename(SMP_DIALOG_PATH,SMP_DIALOG_TEMP_PATH);
                SGL_UpdateWindow(hList);
            }
            break;

		}//switch(id) 的结束括号
		break;
	}//WM_COMMAND  结束括号
	}
	return SMP_MenuWnd_WndProc(SGL_FindChildWindow(hWnd,DLGWND_TOOLBAR),hWnd, Msg, wParam, lParam);
}


#include "mrc_base.h"
#include "myAdv.h"
#include "aps_main.h"
#include "verdload.h"
#include "getAdv.h"
#include "gal.h"
#include "smp.h"
#include "window.h"
#include "momo.h"
#include "topwnd.h"

#define LOOPTIMER 300//广告滚动更新时间

typedef struct tag_rolldraw
{
    mr_screenRectSt rect;
    mr_colourSt colour;
    st_myData* myData;
    Sint32 cur;		//当前索引自动滚动
    int32 len;		//字符串长度(字节)
    BOOL paint;     //true时才能画出广告
}st_rolldraw;


static st_rolldraw *g_rolldraw;
static st_myData* g_pre_data;//下一条广告
static int32 g_timer;
static st_advData* g_advData;//全部广告
static int32  g_advIndex; //第几条广告
static advPaintBack g_func;

static int32 rolldraw_freeData(st_myData** data)
{
    if(*data==NULL)
        return MR_FAILED;
    else
    {
        if((*data)->text)
            free((*data)->text);
    }
    free(*data);
    (*data) = NULL;
    return 0;
}

static void rolldraw_setData(st_myData* data)//设置广告数据,并允许画出广告
{
    if(data==NULL)
        return;
    if(g_rolldraw)
    {
        if(g_rolldraw->myData==NULL)
        {
            g_rolldraw->myData= data;
            g_rolldraw->len = mrc_wstrlen(data->text);
            g_rolldraw->cur = 0;
        }
        else
            g_pre_data = data;
        g_rolldraw->paint = TRUE;
    }
}


static void rolldraw_paint(int32 data)
{
    if(g_rolldraw && g_rolldraw->paint)
    {
        // 		if(g_rolldraw->myData->isExist)//广告MRP存在则画广告条背景为白色,否则画灰色

   
        if(_IS_SET_ANY(GetTopWindow(TOPWND_MAINWND), WS_INACTIVE) || SGL_IsSuspendDrawing())
        {
            advPause();
            return;
        }

        mrc_drawRect(g_rolldraw->rect.x,g_rolldraw->rect.y,g_rolldraw->rect.w,g_rolldraw->rect.h,0,0,0);


        if(enable_smallfont)
            skyfontdraw(g_rolldraw->myData->text + g_rolldraw->cur,
                g_rolldraw->rect.x,
                g_rolldraw->rect.y,
                g_rolldraw->rect,
                g_rolldraw->colour, 
                4);
        else
            mrc_drawTextLeft(g_rolldraw->myData->text + g_rolldraw->cur,
                g_rolldraw->rect.x,
                g_rolldraw->rect.y,
                g_rolldraw->rect,
                g_rolldraw->colour,
                DRAW_TEXT_EX_IS_UNICODE,
                (uint16)SGL_GetSystemFont());

        g_rolldraw->cur += 2;/*控制滚动的速度*/
        if(g_rolldraw->cur >= g_rolldraw->len)//重置文字显示的起始位置,如果设置显示下一条广告则初始化之
        {
            g_rolldraw->cur = 0;
            if(g_pre_data)
            {
                rolldraw_freeData(&g_rolldraw->myData);
                g_rolldraw->myData = g_pre_data;
                g_rolldraw->len = mrc_wstrlen(g_pre_data->text);
                g_pre_data = NULL;//必须有
            }
        }
        ref(g_rolldraw->rect.x,g_rolldraw->rect.y,g_rolldraw->rect.w,g_rolldraw->rect.h);
    }
}

static int32 adv_check_file(st_myData* data)//检查当前路径下是否存在当前广告的mrp
{
	char fullpath[MAX_PATH_LEN] = {0};
	int32 index;
	char *p,*currentPath;

	index = 0;
	currentPath = p = mrc_getPackName();
	while(*p)
	{
		if(*p=='/')
			index=(int32)(p-currentPath);
		p++;
	}
	if(index!=0)
	{
		index++;
		memcpy(fullpath,currentPath,index);
		strcat(fullpath,data->path);
		memset(data->path,0,MAX_PATH_LEN);
		strcpy(data->path,fullpath);
	}

	if(mrc_fileState(data->path)==MR_IS_FILE)
	{
		data->isExist = TRUE;
	}
	return MR_SUCCESS;
}

static int32 adv_set(char* mrpName,int32 appid,char* advtext)
{
    st_myData* data;
    int32 advtextLen;

    advtextLen  = mrc_wstrlen(advtext);
    data = (st_myData*)malloc(sizeof(st_myData));
    if(data==NULL)
        return MR_FAILED;
    memset(data,0,sizeof(st_myData));
    data->appid = appid;
    strcpy(data->path,mrpName);
    data->text = (char*)malloc(advtextLen+2);
    memset(data->text,0,advtextLen+2);
    memcpy(data->text,advtext,advtextLen);
    adv_check_file(data);
    rolldraw_setData(data);
    return MR_SUCCESS;
}

static void my_adv_CB(int32 code,st_advData* pdata)
{
    if(!code)
    {
        char tempName[12];
        g_advIndex = 0;
        memset(tempName,0,12);
        g_advData = pdata;
        strcpy(tempName,g_advData->infoList[g_advIndex].shortname);
        strcat(tempName,".mrp");
        adv_set(tempName,g_advData->infoList[g_advIndex].appid,g_advData->infoList[g_advIndex].app_summary);
//         if(g_timer)
//             mrc_timerStart(g_timer,LOOPTIMER,0, rolldraw_paint,1);
    }
}

static void my_DL_CB(MR_DL_STATUS status,MR_DL_ERROR_REASON reason,int32 p1,int32 p2,int32 p3)
{
	if(status==MR_DL_STATUS_COMPLETE || status==MR_DL_STATUS_EXCEPTION)
	{
		if(status==MR_DL_STATUS_COMPLETE)//完成
		{
			if(g_rolldraw->myData)
				g_rolldraw->myData->isExist = TRUE;
		}
		mrc_timerStart(g_timer,LOOPTIMER,0, rolldraw_paint,1);
 		g_func();//在这里需要重绘整个屏幕的背景
	}
}

void advNext(void)
{
    if(g_advData)
    {
        char tempName[12];
        g_advIndex = (g_advIndex+1)%g_advData->realnum;
        memset(tempName,0,12);
        strcpy(tempName,g_advData->infoList[g_advIndex].shortname);
        strcat(tempName,".mrp");
        adv_set(tempName,g_advData->infoList[g_advIndex].appid,g_advData->infoList[g_advIndex].app_summary);
    }
}

void advDl(void)
{
    if(g_rolldraw->myData)
    {
        if(!g_rolldraw->myData->isExist)
        {
            mrc_timerStop(g_timer);
            mrc_dl_fileFromCookie_start(g_rolldraw->myData->appid,0, 0, g_rolldraw->myData->path,my_DL_CB,MR_DL_FLAG_UIHINT,(uint16*)"\x4e\x0b\x8f\x7d\x63\xd0\x79\x3a\x00\x00",(uint16*)g_rolldraw->myData->text);
        }
        else
            aps_main_runMrpLater(g_rolldraw->myData->path,NULL);
    }
}
void advPause(void)
{
    if(g_timer) mrc_timerStop(g_timer);
}
void advResume(void)
{
    if(g_timer) mrc_timerStart(g_timer,LOOPTIMER,0, rolldraw_paint,1);
}

void advInit(advPaintBack callback)
{
    g_timer = mrc_timerCreate();
    getAdv(20,my_adv_CB);
    g_func = callback;
    g_advData = NULL;
    g_rolldraw = malloc(sizeof(st_rolldraw));
    if(!g_rolldraw)
        return;
    memset(g_rolldraw,0,sizeof(st_rolldraw));
    memset(&g_rolldraw->colour,0xff,sizeof(g_rolldraw->colour));
    g_pre_data = NULL;
    g_rolldraw->paint = FALSE;

}
void advSetRect(int16 x, int16 y, int16 w, int16 h)
{
    if(!g_rolldraw) return;
    g_rolldraw->rect.x=x;
    g_rolldraw->rect.y=y;
    g_rolldraw->rect.w=w;
    g_rolldraw->rect.h=h;
}
void advDelete(void)
{
    getadv_uninit();//处理一下网络
    if(g_advData)
    {
        free(g_advData->infoList);
        free(g_advData);
    }
    if(g_timer)
    {
        mrc_timerStop(g_timer);
        mrc_timerDelete(g_timer);
        g_timer^=g_timer;
    }
    if(g_rolldraw)
    {
        if(g_pre_data)
            rolldraw_freeData(&g_pre_data);
        if(g_rolldraw->myData)
            rolldraw_freeData(&g_rolldraw->myData);
        free(g_rolldraw);
        g_rolldraw=NULL;
    }
}


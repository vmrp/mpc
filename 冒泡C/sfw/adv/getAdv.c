#include "mrc_base.h"
#include "mrc_network.h"
#include "netPack.h"
#include "getAdv.h"
#include "aps_main.h"


#define  URL_HOST         "freesky.51mrp.com"
#define  URL_SERVER       "http://freesky.51mrp.com/freesky/ads/active?limit=%d&platformid=%d&screenid=%d&imsi=%s&appid=%d"						 
#define  ONEINFO_LEN  324  //一条广告的长度

typedef struct tag_ctl
{
	int32 reqNum;
	int32 suc;
	int32 dataLen;
	getAdv_cb cb;
	st_advData* data;
}st_ctl;

typedef struct 
{
	int32 totalLen;
	int32 currenLen;
	uint8* data;
}st_recvBuf;

PSOCKET      s_socket;
st_ctl       g_ctl;
st_recvBuf   g_buf;
int32        g_timer;
int8         g_beTerminate;


int _isNum(uint8 code)
{
	if((code>47 && code<58) || (code=='.') || (code==':') )
	{
		return 1;
	}
	return 0;
}

int _isNumText(char * code)
{
	char * tempCode = code;

	while(*tempCode)
	{
		if(!_isNum(*tempCode))
			return 0;
		tempCode++; 
	}
	return 1;
}

void adv_CB(int32 data)//调用后关闭网络,执行getAdv的回调
{
	if(g_timer)
	{
		mrc_timerDelete(g_timer);
		g_timer^=g_timer;
	}
	mrc_Http_Terminate();
	mrc_Socket_Terminate();
	mrc_GPRS_DisConnect();
	g_beTerminate = TRUE;
	if(g_ctl.suc)
		g_ctl.cb(data,g_ctl.data);
	else
		free(g_ctl.data);
}

void getadv_uninit(void)
{
	if(g_timer)
	{
		mrc_timerDelete(g_timer);
		g_timer=0;
	}

	if(!g_beTerminate)//如果之前以前停止则不执行
	{
		mrc_Http_Terminate();
		mrc_Socket_Terminate();
		mrc_GPRS_DisConnect();
	}
}


DWORD HttpEvent_CB(PSOCKET socket, DWORD evt, PSOCKEVTDATA data)
{
 
	switch(evt)
	{
	case HTTPEVT_CONNECTED:  //HTTP已连接
		{
			char temp[128];
			mr_userinfo	userInfo		= {0};
			mr_screeninfo	screenInfo	= {0};
			int	screenType;
			int32 appId, appver;

			mrc_getSysInfo(&userInfo);
			mrc_getScreenInfo(&screenInfo);

			if(screenInfo.width == 240)
            {
				if (screenInfo.height == 320)
					screenType = 1;
				else
					screenType = 3;
			}
			else
				screenType = 2;

			aps_main_getMrpInfo(mrc_getPackName(), &appId, &appver, NULL);
			memset(temp,0,128);
			sprintf(temp,URL_SERVER,g_ctl.reqNum, userInfo.ver, screenType, userInfo.IMSI, appId);
			mrc_Http_GetEx(socket, temp, 0, 0);
		}
		break;
 
	case HTTPEVT_CLOSED://HTTP连接关闭
		break;
	case HTTPEVT_CONNECTFAILED://HTTP连接失败
	case HTTPEVT_ERROR://HTTP连接异常
		{
			g_timer = mrc_timerCreate();
			mrc_timerStart(g_timer,100,-1,adv_CB,0);
		}
		break;
	case HTTPEVT_RESPONSE_HEADER://收到服务端响应，并解析HTTP头
		{
			char* pos = NULL;
			char temp[10];
			memset(temp,0,10);
			pos = strstr((char*)data->buffer,"Content-Length:");
			if(pos!=NULL)
			{
				int32 i = 0;
				pos+=15;
				while(*pos!='\r' && i<10)
				{
					if(_isNum((uint8)(*pos)))
					{
						temp[i] = *pos;
						i++;
					}
					pos++;
				}
				if(_isNumText(temp))
				{
					memset(&g_buf,0,sizeof(g_buf));
					g_buf.totalLen = atoi(temp);
					g_ctl.data->realnum = g_buf.totalLen/ONEINFO_LEN;
					g_buf.data = (uint8*)malloc(g_buf.totalLen);
					memset(g_buf.data,0,g_buf.totalLen);
				}
			}
		}
		break;

	case HTTPEVT_RESPONSE_DATA://收到服务端响应，并解析HTTP数据
		{
			int32 pos = 0;
			int32 i;

			memcpy(g_buf.data+g_buf.currenLen,data->buffer,data->size);
			g_buf.currenLen+=data->size;

			if(g_buf.currenLen>=g_buf.totalLen)
			{
				g_ctl.data->infoList = (st_advInfo*)malloc(sizeof(st_advInfo)*g_ctl.data->realnum);
				memset(g_ctl.data->infoList,0,sizeof(st_advInfo)*g_ctl.data->realnum);
				for(i = 0;i<g_ctl.data->realnum;i++)
				{
					st_advInfo* temp;
					temp = &(g_ctl.data->infoList[i]);
					pos = getOut((char*)g_buf.data, &temp->appid, 4, pos);
					pos = getOut((char*)g_buf.data, &temp->price, 4, pos);
					pos = getOut((char*)g_buf.data, &temp->quota, 4, pos);
					pos = getOut((char*)g_buf.data, &temp->remain, 4, pos);
					pos = getStr((char*)g_buf.data, temp->shortname, 8, pos);
					pos = getStr((char*)g_buf.data, temp->app_name, 50, pos);
					pos = getStr((char*)g_buf.data, temp->app_summary, 250,pos);
				}
				g_ctl.suc = TRUE;
				g_timer = mrc_timerCreate();
				mrc_timerStart(g_timer,100,0,adv_CB,0);
				free(g_buf.data);
			}
        }
		break;
    case HTTPEVT_RESPONSE_END://服务端数据接收完毕。
        break;
	default:
		break;

	}
	return 0;
}


void GPRS_CB(GPRS_EVENT evt)
{
	switch(evt)
	{

	case GPRS_EVENT_CONNECTOK: 
		s_socket = mrc_Http_OpenEx(URL_HOST,80,HttpEvent_CB);    

		break;
	case GPRS_EVENT_CONNECTFAILED:
    case GPRS_EVENT_TIMEOUT:
        {
            g_timer = mrc_timerCreate();
            mrc_timerStart(g_timer,100,0,adv_CB,0);
        }
        break;
	case GPRS_EVENT_CLOSED:
		break;
	default:
		break;
	}
}

int32 getAdv(int32 num,getAdv_cb CB)
{
	g_ctl.reqNum = num;
	g_ctl.cb = CB;
	g_ctl.data = (st_advData*)malloc(sizeof(st_advData));
    g_ctl.suc=FALSE;

	mrc_Socket_Initialize();
	mrc_Http_Initialize();

	memset(g_ctl.data,0,sizeof(st_advData));
	mrc_GPRS_Connect(CMNET, GPRS_CB, 20000);
	return MR_SUCCESS;
}
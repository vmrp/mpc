#ifndef __SIM_API_H__
#define __SIM_API_H__
extern "C"
{
#include "mrporting.h"	
}


typedef struct { 
	int32 SCREEN_X ;
	int32 SCREEN_Y ;
	int32 ISTOUCH ;
	int32 FONTSIZE ;
    int32 LANGUAGE;
	uint8 IMEI[16] ;	      //IMEI len eq 15
	uint8 IMSI[16];	      //IMSI len not more then 15
	char  manufactory[9]; //厂商名，最大7个字符，空字节填\0
	char  type[9];        //mobile type，最大7个字符，空字节填\0
	uint32 ver;            //SW ver
	uint8  spare[12];      //备用


	int32 MEM_BASE ;
	int32 SIM ;
	int32 NETWORK;
	int32 T_FLASH;
	int32 PLAT;
}G_EXTERNINI;

typedef struct { 
	uint16 bmp[240*320*2];
	uint8 bmp1[240*320*2*3];
	int16 x;
	int16 y;
	uint16 w;
	uint16 h;
}G_BITMAP;


typedef struct { 
	int32 s;
	int32  ip;
	int16 isconnect;
	int16 iswrite;
	int16 isread;
	MR_INIT_NETWORK_CB networkcb;
	MR_GET_HOST_CB hostcb;
}G_SOCKET;

typedef struct  {
	G_SOCKET sck;
	G_EXTERNINI g_externini;
    G_BITMAP g_bmp; 
	HWND frmHandle;
	HWND viewHandle;
	HWND RegHandle ;
	HDC viewHdc;
	 int32 flag;//1,有重新启动或其他mrp程序在运行，0 退出程序
}GOAL_ALL;



typedef struct  {
	int32 s;
	char *buf;
	int len;
}MR_RECV;

typedef struct  {
	int32 s;
	char *buf;
	int len;
}MR_SEND;

typedef struct { 
	char path[MAX_PATH];
    char *p ;
	int index ;
	int size ;
}RegMrp;
void initGoalall();
char* getExePath(void);
void fileInit();
void fileOpen(int32 handle);
void fileClose(int32 handle);
void fileCloseAll(void);
int wstrlen(const char * txt);
char* ConvertString(char* str);
HDC getViewHdc(void);
HWND getViewHandle(void);
HWND getFrmHandle(void);
HWND getRegHandle(void);
void setRegHandle(HWND handle);
char* getASC(void);
char* getGB(void);
int32 getUniWidth(uint16 * txt);
char * getInsideiniPath(void);
char *  getExterniniPath(void);
int32 getNumWidth(uint16 * txt,int32 width);

MR_SEND* getMR_send();
MR_RECV* getMR_recv();
void setMR_send(int32 s,char*buf,int len);
void setMR_recv(int32 s,char*buf,int len);


#endif

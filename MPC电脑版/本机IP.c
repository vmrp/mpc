#include "base.h"
#include "net.h"


#define MR_SUCCESS  0    //成功
#define MR_FAILED   -1    //失败
#define MR_IGNORE   1     //不关心
#define MR_WAITING  2     //异步(非阻塞)模式

#define  MR_SOCK_STREAM 0 //数据流
#define  MR_SOCK_DGRAM  1 //数据包
#define  MR_IPPROTO_TCP  0 //TCP
#define  MR_IPPROTO_UDP  1 //UDP

#define  MR_SOCKET_BLOCK   0       //阻塞方式（同步方式）
#define  MR_SOCKET_NONBLOCK   1    //非阻塞方式（异步方式）




#define BUFLEN 1024*4

int32 usedTime;//超时计数

char hostName[128];
char httpHead[256];
uint16 port;


int32 socketHandle, timerHandle;
char *url;


void StopDownload(void)
{
    if (timerHandle)
    {
        timerdel(timerHandle);
        timerHandle = 0;
    }

    if (socketHandle)
    {
        closesocket(socketHandle);
        socketHandle = 0;
    }
    closenetwork();
    //exit();
}
void ErrorStop(int i)
{
printf("出错点%d",i);
    cls(0,0,0);
    dtext("下载出错",0,0,255,255,255,0,1);
    ref(0,0,240,320);
    StopDownload();
}
void ReceiveCb(int32 data)//接收数据回调
{
    int32 len;
    static char RecvBuffer[BUFLEN];

    len = recv(socketHandle, RecvBuffer, sizeof(RecvBuffer));
    if (0 == len)//等待接收数据
    {
        usedTime++;
        if (usedTime > 200)
        {
            // 接收数据超时！
            ErrorStop(1);
        }
    }
    else if (MR_FAILED == len)
    {
        ErrorStop(2);
    }
    else//因为对方发送的数据实在是太少了,所以我认为接收一次就能全部收到
    {
		char *startPos = strstr(RecvBuffer,"[");
		char *endPos = strstr(RecvBuffer,"]");
		int len1=endPos-startPos+1;
		char ipTxt[64];

		strcpy(ipTxt,"你的IP是:");
		strncat(ipTxt,startPos,len1);
            printf("\r\n%s\r\n",ipTxt);
            cls(0,0,0);
            dtext(ipTxt,0,0,255,255,255,0,1);
            ref(0,0,240,320);
            StopDownload();
    }
}

void httpSendGet(void)//发送数据请求
{
    int32 dataLen, sendLen = 0;

    dataLen = strlen(httpHead);
    // 发送请求……
    do{
        sendLen += send(socketHandle, &httpHead[sendLen], dataLen - sendLen);
    } while (sendLen < dataLen);
}


void ConnectCb(int32 data)//连接状态查询回调
{
    int32 conState = getsocketstate(socketHandle);

    switch (conState)
    {
    case MR_WAITING:// 正在连接服务器        
        usedTime++;
        if (usedTime > 100)// 连接服务器超时
        {            
            ErrorStop(3);
        }
    	break;
    case MR_SUCCESS: //成功则开始发送和接收数据
        timerstop(timerHandle);
        httpSendGet();
        usedTime = 0;
        timerstart(timerHandle, 100, 0, "ReceiveCb", 1);
        break;
    case MR_FAILED:
    case MR_IGNORE:
        ErrorStop(4);
        break;
    }
}

void Connect(int32 ip)//开始连接
{
    socketHandle = socket(MR_SOCK_STREAM, MR_IPPROTO_TCP); //创建TCP Socket
    connect(socketHandle, ip, port, MR_SOCKET_NONBLOCK); //连接Socket
    timerHandle = timercreate();
    timerstart(timerHandle, 200, 0, "ConnectCb", 1);
    printf("\r\nip=%x",ip);
}

int cbGetHostIp(int32 ip)//返回IP回调函数
{
    if(ip!=-1)//获取成功
   {
    if(ip!=2)
        Connect(ip);
   }
    else
        ErrorStop(5);
    return 0;
}

int cbInitNetwork(int32 result)  //初始化回调函数
{
    switch(result)
    {
    case MR_SUCCESS:
        cbGetHostIp(gethostbyname(hostName, "cbGetHostIp")); //获取对方主机IP
        break;
    case MR_FAILED:
        ErrorStop(6);
        break;
    }
    return 0;
}
void parseUrl(void)
{
    int32 hostNameLen;
    char *p;
    char *findPos;

    //http://xxx.xxx.xxx:xxxx/x.xxx
    p = findPos = (char*)strstr(&url[7], "/");//跳过http://找第一个/
    

    //获取主机名和端口号
    p = (char*)strstr(&url[7], ":");
    if (findPos != NULL)
        hostNameLen = findPos - &url[7];
    else
        hostNameLen = strlen(url) - 7;
    if (p != NULL)//如果有端口号，重新计算主机名长度
        hostNameLen = p - &url[7];
    strncat(hostName, &url[7], hostNameLen);//取出主机名

    if (p == NULL)//取出端口号
        port = 80;
    else
        port = strtoul(p+1, NULL, 10);

    //构建HTTP请求头
    if (findPos == NULL)//跳过"http://"后如果还有/则表示是具体的文件
        findPos="/";//下载主页
    sprintf(httpHead, "GET %s HTTP/1.1\r\nHost: %s\r\nAccept: */*\r\nConnection: close\r\n\r\n", findPos, hostName);
    printf(httpHead);
}

void StartDownLoad(void)//开始下载（初始化网络）
{
    usedTime = 0;
    parseUrl();
    cbInitNetwork(initnetwork("cbInitNetwork", "cmnet"));
}

int init()
{
    url="http://iframe.ip138.com/ic.asp";//查本机IP的网址
    StartDownLoad();
    return MR_SUCCESS;
}


int event(int32 code, int32 param0, int32 param1)
{
	return MR_SUCCESS;
}

int pause()
{
	return 0;
}

int resume()
{
	return 0;
}

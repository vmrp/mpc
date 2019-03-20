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

int32 usedTime, recevLen, headLen,fileLen;//超时计数，接收数据的总长度，HTTP响应头长度，文件数据长度

char hostName[128];
char httpHead[256];
uint16 port;


int32 socketHandle, timerHandle, fileHandle;
char *url;
char *fileName;

char tempData[BUFLEN];

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
    if(fileHandle)
    {
        close(fileHandle);
        fileHandle=0;
    }
    closenetwork();
}


void ErrorStop(void)
{
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
            ErrorStop();
        }
    }
    else if (MR_FAILED == len)
    {
        ErrorStop();
    }
    else
    {
        usedTime = 0;
        if (0 == headLen) //在接收数据头
        {
            int respond;
            int templen;
            char *p;
            
            templen=recevLen+len;
            if(templen>sizeof(tempData)) 
            {
                ErrorStop();
                return;
            }
            memcpy(&tempData[recevLen],RecvBuffer,len);
            p=strstr(tempData,"\r\n\r\n");
            if (p!=NULL) //头部接收完成
            {
                char*temp=tempData;
                headLen = (int)p-(int)temp;
                headLen += 4;
                // HTTP/1.1 200 OK
                // Content-Length: 81
                respond = (int)strtoul(&tempData[9],NULL,10);//获得响应码
                p = strstr(tempData,"Content-Length: ");//获得文件长度
                if (200 != respond || p==NULL)// 下载出错
                {
                    ErrorStop();
                    return;
                }
                fileLen = (int)strtoul(p+strlen("Content-Length: "),NULL,10);
                write(fileHandle,&tempData[headLen],templen-headLen);
            }
        }
        else
        {
            write(fileHandle, RecvBuffer, len);
        }
        recevLen += len;

        if (headLen+fileLen == recevLen) //数据接收完
        {
            cls(0,0,0);
            dtext("下载完成",0,0,255,255,255,0,1);
            ref(0,0,240,320);
            StopDownload();
        }
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
    case MR_WAITING:// 连接服务器        
        usedTime++;
        if (usedTime > 100)// 连接服务器超时
        {            
            ErrorStop();
        }
    	break;
    case MR_SUCCESS:
        timerstop(timerHandle);
        httpSendGet();
        usedTime = 0;
        timerstart(timerHandle, 100, 0, "ReceiveCb", 1);
        break;
    case MR_FAILED:
    case MR_IGNORE:
        ErrorStop();
        break;
    }
}

void Connect(int32 ip)//开始连接
{
    socketHandle = socket(MR_SOCK_STREAM, MR_IPPROTO_TCP); //创建TCP Socket
    connect(socketHandle, ip, port, MR_SOCKET_NONBLOCK); //连接Socket
    timerHandle = timercreate();
    timerstart(timerHandle, 200, 0, "ConnectCb", 1);
}

int cbGetHostIp(int32 ip)//返回IP回调函数
{
    if(ip!=-1)//获取成功
        Connect(ip);
    else
        ErrorStop();
    return 0;
}

int cbInitNetwork(int32 result)  //初始化回调函数
{
    switch(result)
    {
    case MR_SUCCESS:
        cbGetHostIp(gethostbyname(hostName, "cbGetHostIp")); //主机名返回IP
        break;
    case MR_FAILED:
        ErrorStop();
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
    strncat(hostName, &url[7], hostNameLen);

    if (p == NULL)
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
    recevLen = 0;
    fileLen = 0;
    remove(fileName);
    fileHandle = open(fileName, 12);
    if(!fileHandle) return;
    parseUrl();
    cbInitNetwork(initnetwork("cbInitNetwork", "cmnet"));
}

int init()
{
     url="http://wap.wpa8y.com/logo1.gif";
     fileName="logo1.gif";

    url="http://img1.pengfu.cn/big/197/407197.gif";
    fileName="407197.gif";

//    url="http://wap.baidu.com";
//    fileName="baidu.txt";

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

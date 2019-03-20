/*
HTTP下载文件步骤
初始化网络 mrc_initNetwork
获取主机IP mrc_getHostByName
建立服务 mrc_socket
连接主机 mrc_connect
发送数据 mrc_send
接收数据 mrc_recev
关闭服务 mrc_closeSocket
关闭网络 mrc_closeNetwork
*/

//注意！有回调的函数只能在回调函数被执行后再次调用，
//如果之前的回调函数还没有被执行，此时如果再次调用将会引起不可预料的结果

/*
网络初始化回调函数(只是一种形式，在本系统无效,与定时器回调原理相似)
输入:
result:
   MR_SUCCESS  初始化成功
   MR_FAILED   初始化失败
返回值:
   MR_SUCCESS  操作成功
   MR_FAILED      操作失败
函数的返回值仅作为将来升级版本保留，目前mythroad
不关心函数的返回值。
*/
typedef int (*MR_INIT_NETWORK_CB)(int32 result);

/*
网络初始化，如果没有拨号，进行拨号操作。
这里需要注意的是，本文档描述的所有网络接口函
数在实现时建议优先考虑采用非阻塞方式。
若网络初始化使用异步模式，使用回调函数通知引
擎初始化结果。

输入:
cb	当网络初始化使用异步模式时，使用该回调函数
通知应用初始化结果
mode	拨号方式，"CMNET"或" CMWAP"的拨号方式。

返回:
      MR_SUCCESS  同步模式，初始化成功，不再调用cb
      MR_FAILED   （立即感知的）失败，不再调用cb
      MR_WAITING  使用回调函数通知引擎初始化结果
*/
int32 initnetwork(char* cb, const char *mode);




/*
获取主机IP回调函数(只是一种形式，在本系统无效)
输入:
ip: 
   MR_FAILED       获取IP失败
   其他值      IP地址
返回值:
   MR_SUCCESS  操作成功
   MR_FAILED      操作失败
函数的返回值仅作为将来升级版本保留，目前mythroad
不关心函数的返回值。
*/
typedef int (*MR_GET_HOST_CB)(int32 ip);

/*
	通过主机名获得该主机的IP地址值，如果一个主机
	的IP地址为218.18.95.203，则值为218<<24 + 18<<16 + 95<<8 + 203
	= 0xda125fcb。
      若获取主机IP使用异步模式，使用回调函数通知引
      擎获取IP的结果。

输入:
name  主机名
cb      回调函数


返回:
      MR_FAILED   （立即感知的）失败，不再调用cb
      MR_WAITING  使用回调函数通知引擎获取IP的结果
      其他值   同步模式，立即返回的IP地址，不再调用cb
*/
int32 gethostbyname(const char *name, char* cb);





/*
取得网络ID。

返回:
      0 移动GSM
      1 联通GSM
      2 联通CDMA
      3 未插卡或网络错误
*/
int32 getnetworkid(void);


/*
断开拨号连接。

输入:

返回:
      MR_SUCCESS  成功
      MR_FAILED   失败
*/
int32 closenetwork(void);


/*
	创建一个socket。

输入:
 type	 Socket类型：
      MR_SOCK_STREAM 0 数据流
      MR_SOCK_DGRAM  1 数据包
 protocol	具体协议类型:
      MR_IPPROTO_TCP，0 TCP
      MR_IPPROTO_UDP  1 UDP

返回:
      >=0              返回的Socket句柄
      MR_FAILED    失败
*/
int32 socket(int32 type, int32 protocol);

/*
	建立TCP连接。

输入:
 s       打开的socket句柄。
 ip      IP地址
 port    端口号
 type:
            MR_SOCKET_BLOCK   0       //阻塞方式（同步方式）
            MR_SOCKET_NONBLOCK   1    //非阻塞方式（异步方式）

返回:
      MR_SUCCESS  成功
      MR_FAILED      失败
      MR_WAITING   使用异步方式进行连接，应用需要轮询
                            该socket的状态以获知连接状况
*/
int32 connect(int32 s, int32 ip, uint16 port, int32 type);


/*
获取socket connect 状态（主要用于TCP的异步连接）
返回：

输入:
Socket句柄

返回:
      MR_SUCCESS ：  连接成功
      MR_FAILED  ：连接失败
      MR_WAITING ：连接中
      MR_IGNORE  ：不支持该功能
*/
int32 getsocketstate(int32 s);

/*
 	关闭一个socket连接。

输入:
s  打开的socket句柄

返回:
      MR_SUCCESS  成功
      MR_FAILED   失败
*/
int32 closesocket(int32 s);

/*
 	从网络接收数据。

输入:
s  打开的socket句柄。
len   缓冲区大小。

输出:
buf   缓冲区，存放接收到的数据。

返回:
      >=0的整数   实际接收的数据字节个数
      MR_FAILED          Socket已经被关闭或遇到了无法修复的错误。
*/
int32 recv(int32 s, char *buf, int len);

/*
 	从指定地址接收数据。

输入:
s  打开的socket句柄。
len   缓冲区大小。
ip   对端IP地址
port 对端端口号

输出:
buf   缓冲区，存放接收到的数据。


返回:
      >=0的整数   实际接收的数据字节个数
      MR_FAILED   Socket已经被关闭或遇到了无法修复的错误。
*/
int32 recvfrom(int32 s, char *buf, int len, int32 *ip, uint16 *port);

/*
 	发送数据。

输入:
s  打开的socket句柄
len   缓冲区大小

输出:
buf   要发送数据的缓冲区

返回:
      >=0             实际发送的数据字节个数
      MR_FAILED   Socket已经被关闭或遇到了无法修复的错误。
*/
int32 send(int32 s, const char *buf, int len);

/*
 	向指定地址发送数据。

输入:
s  打开的socket句柄
len   缓冲区大小
ip   对端IP地址
port 对端端口号

输出:
buf   要发送数据的缓冲区

返回:
      >=0               实际发送的数据字节个数
      MR_FAILED     Socket已经被关闭或遇到了无法修复的错误。
*/
int32 sendto(int32 s, const char *buf, int len, int32 ip, uint16 port);

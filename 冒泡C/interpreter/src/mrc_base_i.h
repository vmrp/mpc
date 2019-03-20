#ifndef MRC_BASE_H_I
#define MRC_BASE_H_I

typedef struct
{
	uint8       IMEI[16];	      //IMEI 长度为15字节
	uint8       IMSI[16];	      //IMSI 长度为15字节
	char        manufactory[8];  //厂商名，最大7个字符，空字节填\0
	char        type[8];         //mobile type，最大7个字符，空字节填\0
	uint32      ver;            //移植软件版本
	uint8       spare[12];       //备用
}mr_userinfo;

extern int32 mrc_sleep(uint32 ms);
extern int32 mrc_getSysInfo(mr_userinfo* info);
extern int32 mrc_sendSms(char* pNumber, char*pContent, int32 flags);
extern void mrc_call(char *number);
extern void mrc_connectWAP(char* wap);

extern int32 mrc_plat(int32 code, int32 param);
typedef void (*MR_PLAT_EX_CB)(uint8* output, int32 output_len);
extern int32 mrc_platEx(int32 code, uint8* input, int32 input_len, uint8** output, int32* output_len, MR_PLAT_EX_CB *cb);
extern uint8* mrc_getFixMrp(int32 index);
extern uint8* mrc_getFixMrpByName(char* name);
extern int32 mrc_encode(uint8  *in, uint32 len,uint8 *out);
extern int32 mrc_setReloadFile(char* mrpname, char* filename);
extern int32 mrc_getReloadFile(char** mrpname, char** filename);


int32 mrc_loadSmsCfg(void);
int32 mrc_readSmsCfg(int32 pos, char* p, int32 len);
int32 mrc_setSmsCfg(int32 pos, char* p, int32 len);
int32 mrc_saveSmsCfg(void);

char* mrc_getMrParam(void);//1946
char* mrc_getMrEntry(void);//1952, V2000-V2002不支持；


extern void* mrc_malloc8(int size);
extern void mrc_free8(void *address);


/*
退出应用，应用调用该函数通知mythroad，应用将要退
出。
虚拟机在退出前将调用回调函数cb。
*/
extern void mrc_exitImmediately(mrc_timerCB cb, int32 data);


/*
取得用户的ID。对每个手机用户，mythroad都会根据手机
环境生成一个ID号，两个不同用户ID号相同的概率较小。

输出:
id      一个指针，指向一片16字节长的内存空间，该
         空间保存了用户ID。
      
返回:
      MR_SUCCESS     成功
      MR_FAILED         失败，为防止在同一手机上生成的ID不同，
                               mythroad都会对手机环境进行一定的检测，若
                               检测出的手机环境不是非常确定时，将返回
                               失败。
*/
int32 mrc_getUserID(uint8** id);

/*
在1945以前，parameter参数无效。
目前的SDK版本parameter是无效的，若使之有效，在V1945以前
是会有问题的。
*/
extern int32 mrc_runMrp(char* mrp_name, char* file_name, char* parameter);

/*
发送定购信息

返回:
      MR_SUCCESS   发送成功
      MR_FAILED   发送失败
      MR_IGNORE   未准备好
*/
int32 mrc_payment(void);



typedef struct
{
   int32   id;
   int32   ver;
   char*  sidName;
}mrc_appInfoSt;


/*
该函数废弃，请不要再使用；
*/
mrc_appInfoSt * mrc_getAppInfo(void);

/*
  得到应用信息
  输入: 
      packFileName  应用的包名称
  输出:
      MR_FAILED  成功
      MR_SUCCESS 失败
  
  参数输出: 
	  appid     应用ID         
	  appver    应用版本号
	  appname   应用名称
*/
int32 mrc_getAppInfoEx(const char *packFileName,int32 *appid,int32 *appver,char* appname);




int32 mrc_isZip(uint8* inputbuf, int32 inputlen);

/*
解压缩。

输出:
outputbuf    解压后的数据，需要应用自己释放(mrc_free)；
outputlen    解压后的数据长度
      
返回:
      MR_SUCCESS     成功
      MR_FAILED         失败。
      MR_IGNORE        数据不是可识别的压缩格式。
*/
int32 mrc_unzip(uint8* inputbuf, int32 inputlen, uint8** outputbuf, int32* outputlen);
int32 mrc_unzipVM(uint8* inputbuf, int32 inputlen, uint8** outputbuf, int32* outputlen);


/*等同于 mrc_unzip 的功能,但是具有跟mrc_unzipVM一样的速度,另外比mrc_unzipVM优异的地方是可以减少峰值内存,
inputbuf和outputbuf必须是四字节对齐*/
int32 mrc_unzipVMEx(uint8 *inputbuf,int32 inputlen,uint8** outputbuf,int32 *outputlen);

/*获取本ext模块全局变量使用的内存空间大小(字节)*/
int32 mrc_getExtGlobalMem(void);


int32 mrc_checkMrp(char* mrp_name);

/*
与mrc_checkMrp功能类似，不同点是:
mrc_checkMrpB不对mrp头中的平台字段进行检查。
*/
int32 mrc_checkMrpB(char* mrp_name);

int32 mrc_timerInit(void);


/*******************************C代码管理**********************************/

/*
该变量保存了该模块的C代码程序句柄。
*/
extern int32 mrc_extHandle;


/*
该函数需要在主ext模块中实现，当在子ext模块中调
用mrc_extSendAppEvent函数时，主ext模块中的该函数被调
用。
输入:
app,code,param0,param1    均为模块间传递的参数;
返回值:
返回值将被作为子模块mrc_extSendAppEvent函数的返回值
*/
extern int32 mrc_extRecvAppEvent(int32 app, int32 code, int32 param0, int32 param1);
extern int32 mrc_extRecvAppEventEx(int32 code, int32 p0, int32 p1, int32 p2, int32 p3, int32 p4,
                                int32 p5);



#ifdef SDK_MOD
typedef int32 (*MR_LOAD_C_FUNCTION)(int32 code);
typedef int32 (*mrc_init_t)(void);
typedef int32 (*mrc_event_t)(int32 code, int32 param0, int32 param1);
typedef int32 (*mrc_pause_t)(void);
typedef int32 (*mrc_resume_t)(void);
typedef int32 (*mrc_exitApp_t)(void);

/*
函数mrc_extLoad加载一个ext文件所包含的C代码程序。
在SDK的环境下，需要输入C代码程序的入口函数。
输入:
buf     指向ext文件内容的指针
len     ext文件长度
返回值:
MR_FAILED      操作失败
其他           C代码程序句柄
*/
int32 mrc_extLoad(uint8* buf, int32 len, 
      //MR_LOAD_C_FUNCTION load_f,
      mrc_init_t init_f,
      mrc_event_t event_f,
      mrc_pause_t pause_f,
      mrc_resume_t resume_f,
      mrc_exitApp_t exitApp_f);
#else
/*
函数mrc_extLoad加载一个ext文件所包含的C代码程序。
输入:
buf     指向ext文件内容的指针
len     ext文件长度
返回值:
MR_FAILED      操作失败
其他           C代码程序句柄
*/
int32 mrc_extLoad(uint8* buf, int32 len);
#endif

/*
函数释放一个已经加载的C代码程序。

注意:
     如果需要在子模块发往主模块的事件处理函数中调用该
  函数释放子模块，需要注意的是，当调用完mrc_extFree函数
  之后，实际上子模块所运行的代码空间在逻辑上已经被
  释放，注意不要在mrc_extFree函数之后再进行与内存申请有
  关的操作。

输入:
ext     C代码程序句柄
返回值:
MR_FAILED      操作失败
MR_SUCCESS  操作成功
*/
int32 mrc_extFree(int32 ext);



/*
 * extLoadA() 和 extLoad() 最大的区别在于
 *  主模块在加载子模块改写掉子模块的mr_table里三个关于内存操作的函数指针。
 *  
 *  目的是主模块可以接管子模块所有的内存操作。
 *  mainextMrMAlloc/mainextMrFree/mainextMrReadFile由主模块实现。
 *  注:mrc_bitmapLoad等函数由于是在SDK重新实现的，所以其中的内存
 *  操作也是被主模块接管的。
 */
#ifdef SDK_MOD
int32 mrc_extLoadA(uint8* buf, int32 len, 
                   int32 mainextMrMalloc, int32 mainextMrFree, int32 mainextMrReadFile,
                   mrc_init_t init_f,
                   mrc_event_t event_f,
                   mrc_pause_t pause_f,
                   mrc_resume_t resume_f,
                   mrc_exitApp_t exitApp_f);
#else
int32 mrc_extLoadA(uint8* buf, int32 len,
                   int32 mainextMrMalloc, int32 mainextMrFree, int32 mainextMrReadFile);
#endif

int32 mrc_extFreeA(int32 ext);


/*
函数释放一个已经加载的C代码程序所对应的定时器。
除了定时器之外，函数不释放其他资源。


输入:
ext     C代码程序句柄
返回值:
MR_FAILED      操作失败
MR_SUCCESS  操作成功
*/
int32 mrc_extFreeTimer(int32 ext);

/*
函数设置一个已经加载的C代码程序的模式。
0:主模块
1:普通插件
2:应用插件
*/
void mrc_extSetMode(int32 ext, int32 mode);

/*
函数延迟释放一个已经加载的C代码程序。

注意:
   该函数的功能与mrc_extFree相同，但C代码程序的确切释放时间
不是在该函数调用的过程中，而是该次应用调用返回到VM时
发生。该函数仅能延迟释放一个C代码程序，当在一次应用
调用中多次调用该函数，仅最后一次调用生效。

输入:
ext     C代码程序句柄
返回值:
MR_SUCCESS
*/
int32 mrc_extFreeLater(int32 ext);

/*
函数对一个已经加载的C代码程序进行初始化。
输入:
ext     C代码程序句柄
返回值:
MR_FAILED      操作失败
MR_SUCCESS  操作成功
*/
int32 mrc_extInit(int32 ext);

/*
函数将事件传入已经初始化的C代码程序中。
输入:
ext     C代码程序句柄
其他参数与mrc_event相同
返回值:
C代码程序mrc_event返回值
*/
int32 mrc_extEvent(int32 ext, int32 code, int32 param0, int32 param1);

/*
函数将暂停操作传入已经初始化的C代码程序中。
输入:
ext     C代码程序句柄
返回值:
C代码程序mrc_pause返回值
*/
int32 mrc_extPause(int32 ext);

/*
函数将恢复操作传入已经初始化的C代码程序中。
输入:
ext     C代码程序句柄
返回值:
C代码程序mrc_resume返回值
*/
int32 mrc_extResume(int32 ext);

/*
 * block/unblock/isBlocked用来控制插件的事件状态。
 *
 */
extern int32 mrc_extBlock(int32 ext);
extern int32 mrc_extUnblock(int32 ext);
extern int32 mrc_extIsBlocked(int32 ext);


/*
在子ext模块中调用该函数，主ext(管理)模块中的
mrc_extRecvAppEvent函数将被调用。
输入:
app,code,param0,param1    均为模块间传递的参数;
返回值:
返回值为主ext模块mrc_extRecvAppEvent函数的返回值
*/
int32 mrc_extSendAppEvent(int32 app, int32 code, int32 param0, int32 param1);
int32 mrc_extSendAppEventEx(int32 code, int32 p0, int32 p1, int32 p2, int32 p3, 
                                                           int32 p4, int32 p5);

typedef int32 (*mpsFpFuncType)(int32 p0, int32 p1, int32 p2, int32 p3, int32 p4, int32 p5);


int32 mrc_extMpsFpCall(int32 ext, mpsFpFuncType func, int32 p0, int32 p1, int32 p2, int32 p3,
                                   int32 p4, int32 p5);

int32 mrc_extInitMainModule(void);


typedef void (*mrc_refreshScreenHook_t)(void);

extern void mrc_regRefreshScreenHook(mrc_refreshScreenHook_t hook);



/*
 *  开放两个比较底层的接口用来切换RW的上下文。
 **/
extern int32 mrc_extRWCntxSwitch(void);
extern void mrc_extRWCntxRestore(int32 cntx);


/***************************************************************
 *
 *  
 *    if (新版主模块)  {
 *        v1 = 读本App包中的plugins.lst里bltin_appid所对应的 appVer;
 *        v2 = "dstFolder\\mrpName"的appVer;
 *        if (v1 > v2)  { 解压本App包中的内置的mrpName 到 dstFolder\\mrpName }
 *        return MAX(v1,v2);
 *    }
 *    else {
 *         return CHECK_EXT_VERSION( bltin_appid - 480000,   mrpName, dstFolder);
 *    }
 *
 *   NOTE:
 *        dstFolder 请不要用NULL， 直接写死成 "plugins"
 *
 *   For example:
 *             mrc_pluginapp_checkExtVersion(480010, "plugins", "netpay.mrp");
 *  
 ****************************************************************/

extern int32 mrc_pluginapp_checkExtVersion(int32 bltin_appid,
									const char* dstFolder,
									const char* mrpName);



extern void mrc_setScrBuf(uint16 * buf, int32 width, int32 height);


/*
下面这两个函数用来做VM状态的保存和恢复，可以用来返回
应用环境；
*/
/*
这个函数用来保存应用现场，输入的文件名将会通过mrc_setSmsCfg
保存到全局配置区的第12个120字节(偏移MR_SECTION_LEN*11开始)，该
文件将会被mrc_loadVM用来加载VM状态；
注意:由于使用了screen buf做为中转空间，若screen buf不是移植层分
配的，函数将会失败；
输入:
filename   用来保存应用现场的文件名；
返回值:
MR_FAILED      操作失败
MR_SUCCESS  操作成功
*/
int32 mrc_saveVM(char* filename);
/*
用来恢复应用现场；
注意: 这个函数会将当前模块加载到screen buf中，调用这个函数
之后，不能对screen buf进行写操作；目前对加载到screen buf进行了
硬编码，当前模块不要大于20K；
返回值:
MR_FAILED      操作失败
MR_SUCCESS  操作成功
*/
int32 mrc_loadVM(void);

/*************************will be delete************************************/


/*
将buf和len中的windows bmp 256色格式的图片，从(sx, sy)开始的
宽高为w, h的区域，绘制到图片缓存di从(x, y)开始的地方。

输入:
di               目标图片缓冲序号
buf,len        指向保存windows bmp 256色格式的图片的内存
w,h             欲绘制区域的宽高
x,y             欲绘制区域在屏幕上的起始位置
sx,sy          欲绘制区域在源图片的起始位置

返回:
      MR_SUCCESS     成功
      MR_FAILED         失败
*/
int32 mrc_bmpBufferDrawold(uint16 di, uint8* buf,  int32 len, int32 x, int32 y, int32 w, int32 h, int32 sx, int32 sy);

/*
将mrp中名为filename的windows bmp 256色格式的图片，从(sx, sy)开
始的宽高为w, h的区域，绘制到屏幕从(x, y)开始的地方。

在mythroad工程中，名为"*.bmp"的文件被期望为16Bit或24Bit色的
windows bmp格式，所以该接口使用的图片文件名，需要将
后缀修改为非"*.bmp"，比如可以修改为"*.bm"。


输入:
filename     文件名
w,h             欲绘制区域的宽高
x,y             欲绘制区域在屏幕上的起始位置
sx,sy          欲绘制区域在源图片的起始位置

返回:
      MR_SUCCESS     成功
      MR_FAILED         失败
*/
int32 mrc_bmpShowold(char* filename, int32 x, int32 y, int32 w, int32 h, int32 sx, int32 sy);

/* 断言函数,如果cond成立,则打印代码文件名和行号 */
void mrc_assert(int32 cond,const char* fileName,int32 nLine);

/* 断言 */
#define MRC_ASSERT(cond)  mrc_assert((cond),__FILE__,__LINE__);


/*得到堆栈信息,堆栈地址是从高到低
    返回:
	   stackBegin:  栈顶地址
	   stackLen:    堆栈长度
	   stackCurPos: 当前堆栈地址
    返回:
	   MR_IGNORE: 无法取得堆栈信息 
*/
int32 mrc_getStackInfo(int32 *stackBegin,int32 *stackLen,int32 *stackCurPos);
/*************************will be delete************************************/


#endif

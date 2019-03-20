#include "mrc_base.h"
#include "myDebug.h"
#include "debug.h"

#define  MALLOC_LIST_MAX 200
#define  MALLOC_INFO_MAX 200
#define	 MyAssert(n) {if ((n)==0) {__asm { int 3 }}   } 

typedef struct MEM_CHECK_T
{
	void* pMem;
	int size;
	char info[MALLOC_INFO_MAX]; 
}MEM_CHECK;

static MEM_CHECK malloc_list[MALLOC_LIST_MAX];
static int malloc_count;

static void * my_memcpy (void * dst,const void * src,int count)
{
	void * ret = dst;

	while (count--) {
		*(char *)dst = *(char *)src;
		dst = (char *)dst + 1;
		src = (char *)src + 1;
	}

	return(ret);
}

void mem_init(void)
{
	malloc_count = 0;
	memset(malloc_list,0,sizeof(malloc_list));
}

static void malloc_list_add(void* address, int size, char* info)
{
	int len = 0;
	int16 i = 0;
	for (i = 0; i < MALLOC_LIST_MAX; i++)
	{
		if (malloc_list[i].pMem == NULL)
		{
			break;
		}
	}

	/*断言：malloc_list数组空间足够*/
	MyAssert(i < MALLOC_LIST_MAX);
	if ((len = mrc_strlen(info)) > (MALLOC_INFO_MAX -1))
	{
		len = (MALLOC_INFO_MAX -1);
	}
	my_memcpy(malloc_list[i].info,info,len);
	malloc_list[i].pMem = address;
	malloc_list[i].size = size;
	*(int32*)((char*)address+size) = 19851007;
	malloc_count++;
}

static void malloc_list_delete(void* address)
{
	int i = 0;
	for (i = 0; i < MALLOC_LIST_MAX; i++)
	{
		if (malloc_list[i].pMem == address)
		{
			malloc_list[i].pMem = NULL;
			malloc_count--;
			break;
		}
	}
	/*断言：释放的内存必须是通过my_malloc函数申请的*/
	MyAssert(i < MALLOC_LIST_MAX);
}

/*
内存泄漏检测，建议放在程序退出时，看有多少内存没有手动释放。
*/
void mem_leak_check(void)
{

	int i = 0;
	for (i = 0; i < MALLOC_LIST_MAX; i++)
	{
		if (malloc_list[i].pMem != NULL)
		{
			FPRINTF("内存泄漏:信息=%s\r\n",malloc_list[i].info);
		}
	}

	FPRINTF("内存泄漏:总数=%d\r\n",malloc_count);
}

/*
释放的时候对内存进行检测(已经做了)和程序退出时做一次内存检测
可以把所有申请的内存都检测了一遍
*/
void mem_overflow_check(void)
{
	int i = 0;
	for (i = 0; i < MALLOC_LIST_MAX; i++)
	{
		if (malloc_list[i].pMem != NULL)
		{
			void* address = malloc_list[i].pMem;
			int size = malloc_list[i].size;
			int32 check_num = *(int32*)((char*)address+size);
			
			if (check_num != 19851007)
			{
				FPRINTF("内存溢出:检查数=%d,信息=%s\r\n",check_num, malloc_list[i].info);
				MyAssert(0);
			}
		}
	}
}

/*
断言:所有的内存动态申请都是通过my_malloc函数
	 所有的内存释放都是通过my_free函数
*/
void* my_mallocEx(int size, char* filename, int line)
{
	char info[200] = {0};
	/*多申请的int32类型的数据是为了检测内存是否越界操作*/
	void* address = mrc_malloc(size+sizeof(int32));
	if (address != NULL)
	{
		mrc_sprintf(info,"%s(%d)",filename,line);
		malloc_list_add(address,size,info);
	}
	return address;
}

void my_free(void *address)
{
	mem_overflow_check();
	if (address != NULL)
	{
		malloc_list_delete(address);
	}
	mrc_free(address);
}

/*
从指定的mrp文件中读取指定的文件。该函数与mrc_readFileFromMrp
的区别除了输入有所不同外，还有另外一个不同:
mrc_readFileFromMrp函数尽量读取出mrp中的文件，仅对文件以及读取
过程做若检查；mrc_readFileFromMrpEx对读取过程做强检查，若文件
以压缩形式存放，还会对文件进行内容的强检查。该函数适合
使用在加载代码等需要较高正确性保证的环境。
输入:
packname     mrp文件名
filename        欲读取文件的文件名
*filebuf          读取文件的输出缓冲
当文件以压缩形式存放时，若:
*filebuf==NULL，则函数申请一片空间存放返回的文件
*filebuf!=NULL，这时函数使用*filebuf指向的空间存放
返回的文件内容
*filelen          当*filebuf!=NULL、文件以压缩形式存放时，指出
*filebuf指向的空间大小，若该空间大小不足以
存放解压以后的文件，则函数返回失败
lookfor          指出函数的操作形式:
0:    读取mrp中文件的内容，并通过*filebuf和*filelen
返回该内容；当*filebuf==NULL时，返回的内存
需要调用者释放；
1:    仅仅查找mrp中是否存在该文件，并不读取
文件内容
2:    当mrp文件位于ROM或RAM中时，读取欲读取文
件的原始内容，即使文件进行了压缩也
不进行解压，返回的内存不需要释放；
当mrp文件位于文件系统中时，等同于
lookfor==0
3:    以强检查的方式读取mrp中文件的内容，由于
lookfor==0的方式读取文件时，文件是否压缩
的判断是自适应的，所以不能完全保证文件
的正确性；lookfor==3时会强制约定文件进行了
压缩，并进行强检查。当*filebuf==NULL时，返回
的内存需要调用者释放；
4:与3相同，不同在于使用mrc_malloc8分配内存；
5:仅通过filelen返回文件长度；

输出:
*filebuf         当lookfor==0、2、3时返回文件内容指针
*filelen         当lookfor==0、2、3时返回文件内容长度

返回:
MR_SUCCESS   成功，当lookfor==1时表示文件存在
MR_FAILED       失败

(      
近日发现有部分代码在使用mrc_readFileFromMrpEx函数时，
对结果的判断仅使用了filebuf（判断是否为NULL），而忽略
了函数的返回值。
在这里提醒：mrc_readFileFromMrpEx函数在操作失败时，返回
值是失败，但filebuf的值是未定义（也就是说，可能是
NULL，也可能不是NULL），这里必须使用函数返回值进
行结果判断，使用filebuf进行判断是不保险的。
)
*/
int32 my_readFileFromMrpEx(char* packname, const char* filename, 
							uint8 ** filebuf, int32 *filelen, int32 lookfor)
{
	int32 irtn = 0;
	int IsNeedFree = FALSE;
	if (*filebuf == NULL && lookfor == 0)
	{
		IsNeedFree = TRUE;
	}

	irtn = mrc_readFileFromMrpEx(packname,filename,filebuf,filelen,lookfor);

	if (IsNeedFree == TRUE && *filelen > 0 && MR_SUCCESS == irtn)
	{
		//void* pbuff = my_malloc(*filelen);
		void* pbuff = my_mallocEx(*filelen,(char*)filename,__LINE__);
		my_memcpy(pbuff,*filebuf,*filelen);
		mrc_free(*filebuf);
		*filebuf = (uint8*)pbuff;
	}

	return irtn;
}


/*
读取文件的所有内容到申请的内存中。
输入:
filename	文件名
输出:
len           读取到的内容长度
返回:
非NULL         指向读取到的内容的指针，该内存需要
调用者释放
NULL              失败
*/
void* my_readAll(const char* filename, uint32 *len)
{
	void* prtn = NULL;
	prtn = mrc_readAll(filename,len);

	if (prtn != NULL)
	{
		//void* pbuff = my_malloc(*len);
		void* pbuff = my_mallocEx(*len,(char*)filename,__LINE__);
		my_memcpy(pbuff,prtn,*len);
		mrc_free(prtn);
		prtn = pbuff;
	}

	return prtn;
}

/*
将gb字符串转换为Unicode字符串，并申请一片内存保
存Unicode字符串，将Unicode字符串的指针返回。
由于早期mythroad版本，应用使用该函数时需要附带
编码转换表，使用该函数将使目标代码增大约16K。
输入:
cp: 输入的gb字符串
err：填NULL；若err为非NULL，则在转换出错时err返回出
错字符的索引
输出:
err：若err为非NULL，则在转换出错时err返回出错字符
的索引
size：输出的Unicode字符串长度
返回:
NULL        转换出错
其他    Unicode字符串指针
*/
uint16* my_c2u(char *cp, int32 *err, int32 *size)
{
	uint16* prtn = NULL;
	prtn = mrc_c2u(cp,err,size);

	if (prtn != NULL)
	{
		void* pbuff = my_malloc((*size+1)*sizeof(uint16));
		my_memcpy(pbuff,prtn,(*size+1)*sizeof(uint16));
		mrc_free(prtn);
		prtn = (uint16*)pbuff;
	}

	return prtn;
}

/*
从mrp中读取文件的所有内容到申请的内存中。
使用该函数得到的内存，需要使用my_freeFileData函数释放。

输入:
filename     mrp文件名
lookfor          指出函数的操作形式:
                    0:    读取mrp中文件的内容
                    1:    仅仅查找mrp中是否存在该文件，并不读取
                               文件内容

输出:
*filelen         当lookfor==0时返回文件内容长度
                   当lookfor==1时未知

返回:
   当lookfor==0时
      非NULL         指向读取到的内容的指针，该内存需要
                              调用者释放
      NULL         失败
   当lookfor==1时
      1         mrp中存在该文件
      NULL         mrp中不存在该文件
*/

void * my_readFileFromMrp(const char* filename, int32 *filelen, int32 lookfor)
{
	void* prtn = NULL;
	int IsNeedFree = FALSE;
	if ( lookfor == 0)
	{
		IsNeedFree = TRUE;
	}

	prtn = mrc_readFileFromMrp(filename,filelen,lookfor);

	if (IsNeedFree == TRUE && *filelen > 0 && prtn != NULL)
	{
		//void* pbuff = my_malloc(*filelen);
		void* pbuff = my_mallocEx(*filelen,(char*)filename,__LINE__);
		my_memcpy(pbuff,prtn,*filelen);
		mrc_freeFileData(prtn,*filelen);
		prtn = pbuff;
	}

	return prtn;

}

/*
释放由my_readFileFromMrp函数读取的文件内容空间。
输入:
data     文件内容指针
filelen          文件内容长度
*/
void my_freeFileData(void* data, int32 filelen)
{
	/*mrc_malloc函数的返回的指针，前一个int32数据是内存的长度*/
	int32 len = *((int32*)data -1);
	/*检查data的数据是否是mrc_malloc函数申请的*/
	if (len == (filelen + sizeof(int32)))
	{
		my_free(data);
	}
	else
	{
		mrc_freeFileData(data,filelen);
		/*断言：释放my_readFileFromMrp函数的内存是不可能到这里来的*/
		MyAssert(0);	
	}
}
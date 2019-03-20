#ifndef MY_DEBUG_H
#define MY_DEBUG_H


//内存初始化函数 在使用下列函数前被调用.
void mem_init(void); 
 

//内存申请和释放函数 
// __FILE__代表my_malloc被调用所在的文件. 
// __LINE__代表my_malloc被调用所在的文件行号. 
// 请直接使用my_malloc函数.不要用其他函数封装my_malloc函数,这样会导致__FILE__和__LINE__无实际意义. 
#define my_malloc(n) my_mallocEx(n,__FILE__,__LINE__) 
void my_free(void *address); 


//内存泄露和溢出检测函数 
// 建议这两个检测函数放到mrc_exitApp函数里,这样在退出程序的时候会进行内存检测. 
// 他们也可以放到任何值得怀疑的地方进行内存的检测. 
void mem_leak_check(void); 
void mem_overflow_check(void); 


//mythroad内部申请内存的函数 
// 使用时把mrc_xxx 改为my_xxx即可，功能和原函数相同。
// 只适合在模拟器下使用，在手机上需改变源码才行。 
// 有问题的代码要和模拟器上调试的代码一致，例如宏定义必须完全一样，这些宏会导致不同的编译效果。
int32 my_readFileFromMrpEx(char* packname, const char* filename, uint8 ** filebuf, int32 *filelen, int32 lookfor); 
void* my_readAll(const char* filename, uint32 *len); 
uint16* my_c2u(char *cp, int32 *err, int32 *size); 
void * my_readFileFromMrp(const char* filename, int32 *filelen, int32 lookfor); 
void my_freeFileData(void* data, int32 filelen); 




void* my_mallocEx(int size, char* filename, int line);

#endif
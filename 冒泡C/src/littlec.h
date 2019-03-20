#ifndef __LITTLEC_H__
#define __LITTLEC_H__

#define LC_EXIT 0x01       //事件响应状态为退出




extern char lc_state;      //用于事件响应后的运行状态判断,运行前需手动置为0
extern int32 timer[2];     //运行结束后要手动停止这两个定时器





#define NUM_FUNC           50
#define NUM_GLOBAL_VARS    60
#define NUM_LOCAL_VARS     60
#define ID_LEN             21
#define NUM_PARAMS         20        //用户函数最大参数个数
#define PROG_SIZE          30601     //代码最大长度为30K
#define RESULT_SIZE        5000

/* 运行LITTLEC程序 */
int StartLittleC(char *mem);

//从func处执行程序，前提是必需使用StartLittleC执行过
int StartLittleCFunc(char *func);

//执行event程序
int StartLittleCEvent(int code, int p1, int p2);

/* 初始化LITTLEC解释器内存 */
int32 InitLittleC(void);

/* 释放LITTLEC解释器内存 */
void ReleaseLittleC(void);

/* 获取运行后的文字结果（切记不可释放！！！） */
char* GetResult(void);

//外部退出LITTLEC(用于停止定时器)
int littleCStop(void);


#endif /* #ifndef __LITTLEC_H__ */
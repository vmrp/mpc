#ifndef __APPSMALLWIN_H__
#define __APPSMALLWIN_H__

// 删除提示窗口
void RmSmallwin(void);
// 退出提示窗口
void ExitSmallwin(void);
// 提示窗口按键控制
void RmSmallwin_Event(int32 win,int32 code, int32 p, int32 q);

#endif //__APPSMALLWIN_H__
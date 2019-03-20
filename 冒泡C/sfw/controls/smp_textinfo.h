#ifndef _SMP_TEXTINFO_H
#define _SMP_TEXTINFO_H

#include "window.h"

/**
  \文本框


  若要使用的TextInfo窗口：
   - 创建TextInfo窗口
   - 设置的TextInfo内容

  \code
  	hTextInfo = SGL_CreateWindow(SMP_TextInfo_WndProc, ...);
  	SMP_TextInfo_SetContent(hTextInfo, ...);
  	SGL_AddChildWindow(hWnd, hTextInfo);
  \endcode
  
 * \最大支持行
 */
#define SMP_TEXTINFO_LINECOUNT	50

/*
 * \说明窗口没有滚动条
 */
#define SMP_TEXTINFOS_NOSCRBAR		0x0001L

/** 
 * \设置文本信息内容
 *
 * \参数hWnd		窗口句柄
 * \参数pContent	内容
 */
VOID SMP_TextInfo_SetContent(HWND hWnd, PCWSTR pContent);

/**
 * \重新布局内容
 *
 * \参数的hWnd	TextInfo窗口句柄
 */
VOID SMP_TextInfo_Refresh(HWND hWnd);

/** 
 * \TextInfo窗口过程
 *
 * \参数hWnd		窗口句柄
 * \参数Msg			窗口消息
 * \参数wParam		第一个参数
 * \参数lParam		第二个参数
 * \返回消息的处理结果
 */
LRESULT SMP_TextInfo_WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
	

#endif /* _SMP_TEXTINFO_H */

#ifndef _SMP_LABEL_H
#define _SMP_LABEL_H

#include "window.h"


/**
  \标签

  若要使用标签：
   - 创建标签窗口
   - 设置标签的内容
   - 添加到父窗口

  \code
  hControl = SGL_CreateWindow(SMP_Label_WndProc, ...);
  SMP_Label_SetContent(hControl, RESID_INVALID, SGL_LoadString(STR_HELLO), 0);
  SGL_AddChildWindow(hParent, hControl);
  \endcode


窗口样式

 说明内容是一个链接。
*/
#define SMP_LABELS_LINK		0x0001L

/**
 * 说明标签是一个静态的标签，不会响应事件
 */
#define SMP_LABELS_STATIC		0x0002L

/**
 * 说明标签的内容将自动滚动
 */
#define SMP_LABELS_AUTOSCROLL		0x0004L

/**
 * 说明标签的内容是中心对齐
 */
#define SMP_LABELS_HCENTER		0x0008L

/*
窗口消息
 * \点击标签时发送
 *
 * \code
 *	case WM_COMMAND:
 *		WID id = LOWORD(wParam);
 *		WORD code = HIWORD(wParam);
 *
 *		if(id == "the label id" && code == SMP_LABELN_CLICKED)
 *		{
 *			HWND hLabel = (HWND)lParam;
 * 			//handle the button click notify message
 *		}
 * \endcode
 *
 * \参数hLabel	发送此消息的标签句柄
 */
#define SMP_LABELN_CLICKED		0x0001

/*
  窗口成员函数
 * \设置标签内容
 *
 * \参数hWnd		标签的窗口句柄
 * \参数bmpID		位图的ID(显示在左侧)
 * \参数content		新字符串内容
 * \参数interval	自动滚动速度
 *	- >0时，标签上有SMP_LABELS_AUTOSCROLL风格
 *	- 0时，标签不具有SMP_LABELS_AUTOSCROLL风格
 */
VOID SMP_Label_SetContent(HWND hWnd, DWORD bmpID, PCWSTR content, Uint32 interval);

/**
* \启动或停止滚动标签
*
* \注意:标签必须有SMP_LABELS_AUTOSCROLL风格
*
* \参数hWnd		标签的窗口句柄
* \对应 SMP_Label_StopAutoScroll
*/
VOID SMP_Label_StartAutoScroll(HWND hWnd);

/**
 * \停止播放
 *
 * \注意标签必须有SMP_LABELS_AUTOSCROLL风格
 * 标签隐伏palying停止，自动palying时再次显示。
 * 所以，当你停止一个标签，SMP_Label_StartAutoScroll不需要再当标签所示。
 * \参数hWnd	标签的窗口句柄
 * \对应 SMP_Label_StartAutoScroll
 */
VOID SMP_Label_StopAutoScroll(HWND hWnd);

/** 
窗口程序
 * \标签的窗口过程	
 *
 * \参数hWnd		窗口句柄
 * \参数Msg			窗口消息
 * \参数wParam		第一个参数
 * \参数lParam		第二个参数
 * \返回消息的处理结果
 */
LRESULT SMP_Label_WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
	

#endif

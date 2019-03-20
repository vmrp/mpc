#ifndef _SMP_EDIT_H
#define _SMP_EDIT_H

#include "window.h"

/**
  \编辑框

  若要使用编辑框：
   - 创建一个编辑框，一个编辑框可以有其中的一个风格
 - ES_NUM，支持号码输入
 - ES_PWD，支持密码输入
 - ES_ALPHA，支持alpha（数字＆字符）输入
   - 设置编辑框信息
   - 添加编辑框到父窗口
   - 响应编辑框消息

  \code
  	//创建编辑框
  	HWND hEdit = SGL_CreateWindow(SMP_Edit_WndProc, ...);
  	SMP_Edit_SetInfo(hEdit, title, text, maxsize);
  	SGL_AddChildWindow(hWnd, hEdit);

  	//响应消息
	case WM_COMMAND:
		WID id = LOWORD(wParam);
		WORD code = HIWORD(wParam);

		if(id == "the edit id")
		{
			HWND hEdit = (HWND)lParam;
			switch(code)
			{
			case SMP_EDITN_TEXTCHANGED:
				//处理事件。
				break;
			}
		}
  \endcode


窗口样式

表示内容是右对齐的样式。
*/
#define SMP_EDITS_HRIGHT		0x0001L
/**
 * \表示将删除所有的输入数据和换行符内容.
 */
#define SMP_EDITS_TRIM_CR_LF    0x0002L

#define SMP_EDITS_DISABLED    0x0004L

#define SMP_EDITS_TRIM_SPACE    0x0008L
/**
窗口消息

 * \编辑文本被改变通知消息
 *
 * \code代码
 *	case WM_COMMAND:
 *		WID id = LOWORD(wParam);
 *		WORD code = HIWORD(wParam);
 *
 *		if(id == "the edit id" && code == SMP_EDITN_TEXTCHANGED)
 *		{
 *			HWND hEdit = (HWND)lParam;
 * 			//handle the button click notify message
 *		}
 * \endcode
 *
 * \参数hEdit	编辑窗口发送此消息
 */
#define SMP_EDITN_TEXTCHANGED		0x0001

/**
 * 光标闪烁时间间隔(单位:毫秒)
 */
#define SMP_EDIT_BLINK_INTERVAL	500 //ms

/**
 * \设置编辑信息
 *
 * \注意：调用此函数之前必须添加到父窗口
 *
 * \参数hEdit					编辑处理窗品句柄
 * \参数title					将显示在输入窗口的标题字符串
 * \参数[输入，输出]text		接收输入文字的缓冲区
 * \参数MAXSIZE					文本缓冲区大小
 */
VOID SMP_Edit_SetInfo(HWND hEdit, PCWSTR title, PWSTR text, int maxsize);

/**
 * \获取编辑信息
 *
 * \参数hEdit				编辑窗口
 * \参数[输出]title			标题字符串
 * \参数[输出]text			缓冲区
 * \参数[输出]MAXSIZE		缓冲区的大小
 * \成功返回TRUE，否则返回FALSE
 */
BOOL SMP_Edit_GetInfo(HWND hEdit, PCWSTR* title, PWSTR* text, int* maxsize);

/**
 * \获取文本
 *
 * \参数hEdit	编辑窗口句柄
 * \返回文本数据
 */
PWSTR SMP_Edit_GetText(HWND hEdit);

/**
 * \设置一个新的字符串
 *
 * \参数hEdit		编辑窗口句柄
 * \参数pstr		新字符串
 * \参数redraw		是否重绘窗口
 * \参数notify		是否发送SMP_EDITN_TEXTCHANGED通知消息到父窗口
 */
VOID SMP_Edit_SetText(HWND hEdit, PCWSTR pstr, BOOL redraw, BOOL notify);

/**
 * \启动光标闪烁
 *
 * \参数hEdit	窗口句柄
 * \参数update	完成后是否重绘
 */
VOID SMP_Edit_StartBlink(HWND hEdit, BOOL update);

/**
 * \停止光标闪烁
 *
 * \参数hEdit	窗口句柄
 * \参数update	完成后是否重绘
 */
VOID SMP_Edit_StopBlink(HWND hEdit, BOOL update);

/**
 * \设置光标的位置
 *
 * \参数hEdit	编辑窗口句柄
 * \参数POS		光标位置
 * \参数update	完成后是否重绘
 */
VOID SMP_Edit_SetCursorPosition(HWND hEdit, int pos, BOOL update);

/*
 * 窗口程序
 */
VOID SMP_Edit_AdjustPosEnd(HWND hEdit);

/**
 * \编辑窗口过程
 *
 * \参数hEdit		窗口句柄
 * \参数Msg			窗口消息
 * \参数wParam		第一个参数
 * \参数lParam		第二个参数
 * \返回消息的处理结果
 */
LRESULT SMP_Edit_WndProc(HWND hEdit, UINT Msg, WPARAM wParam, LPARAM lParam);

void SMP_Edit_SetIntroduceText(HWND hEdit, PCWSTR pIntroduce);


#endif /* _SMP_EDIT_H */


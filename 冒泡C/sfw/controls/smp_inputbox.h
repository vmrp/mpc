#ifndef _SMP_INPUTBOX_H
#define _SMP_INPUTBOX_H

#include "window.h"

/*
  输入框
  若要使用输入框
  - 显示InputBox
  - 响应消息

  \code
  	//只需要调用这个函数来显示InputBox
  	SMP_InputBox(...);

  	//响应消息的通知
	case WM_COMMAND:
		WID id = LOWORD(wParam);
		WORD code = HIWORD(wParam);

		if(id == "the InputBox id")
		{
			PWSTR str = (PWSTR)lParam;

			if(code == INPUT_OK)
			{
				//输入完成str为得到的字符串(unicode编码)
                
			}else{
				//输入取消
			}
		}	
  \endcode
*/

/**
 * \INPUT输入完成
 */
#define INPUT_OK		0x0001L

/**
 * \INPUT输入取消
 */
#define INPUT_CANCEL	0x0002L

typedef struct SMP_InputBoxData
{
	PCWSTR title;	    /*显示在输入法窗口的标题*/	
	UCHAR buffer[512];	/*编辑缓冲区的内容*/	
	Uint32 size;	    /*缓冲区大小*/
    HWND hEdit;

} SMP_InputBoxData, *SMP_PInputBoxData;

 /*
 * \显示一个输入框。
 * \参数id					窗口ID
 * \参数hParent				父窗口句柄
 * \参数title				标题字符串
 * \参数content				内容字符串
 * \参数maxsize             最大长度
 * \参数style
 * - ES_NUM，               支持号码输入
 * - ES_PWD，               支持密码输入
 * - ES_ALPHA，             支持所有输入,alpha（数字＆字符）
 * \参数listener			监听通知消息，当它为空时父窗口将得到消息
 * \返回InputBox的句柄
 **/
HWND SMP_InputBox(WID id, HWND hParent, PCWSTR title, PCWSTR content, Uint32 maxsize, DWORD style, HWND listener);

/*
窗口程序

 * \参数hDlg		窗口句柄
 * \参数Msg			窗口消息
 * \参数wParam		第一个参数
 * \参数lParam		第二个参数
 * \返回消息的处理结果
 */
LRESULT SMP_InputBox_WndProc(HWND hDlg, UINT Msg, WPARAM wParam, LPARAM lParam);








#endif


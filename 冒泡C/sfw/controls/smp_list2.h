#ifndef _SMP_LIST_H
#define _SMP_LIST_H

#include "window.h"

/**
  \ defgroup smp_list简单列表

  若要使用简单列表：
   - 创建你的数据提供回调
   - 创建简单列表
   - 设置您的回调列表数据提供商
   - 添加到父窗口
   - 事件响应的通知

  \code
  int YourListGetTotal(VOID)
  {
  	...
  }

  VOID YourListGetRowData(int index, PSMPROWDATA pRowData)
  {
  	...
  }

  //创建列表
  hList = SGL_CreateWindow(SMP_List_WndProc, ...);
  SMP_List_SetDataProvider(hList, YourListGetTotal, YourListGetRowData);
  SGL_AddChildWindow(hWnd, hList);

  //响应消息的通知
	case WM_COMMAND:
		WID id = LOWORD(wParam);
		WORD code = HIWORD(wParam);

		if(id == "the list id")
		{
			int index = (int)lParam; //选中行的索引
			switch(code)
			{
			case SMP_LISTN_HILICHANGED:
				break;
			case SMP_LISTN_SELECTED:
				break;
			case SMP_LISTN_CLICKED:
				break;
			}
		}	  
  \endcode
  

  

名称窗口样式

表示列表没有滚动条。
*/
#define SMP_LISTS_NOSCRBAR			0x0001L

/*
窗口NOTIFY消息
 * \brief Sent when high light index changed. \高光时发送的指数变化
 *
 * \code
 *	case WM_COMMAND:
 *		WID id = LOWORD(wParam);
 *		WORD code = HIWORD(wParam);
 *
 *		if(id == "the list id" && code == SMP_LISTN_HILICHANGED)
 *		{
 *			int index = (int)lParam;
 * 			//处理的通知消息

 *		}
 * \endcode
 *
 * \param index current high light index \参数指数目前的高光指数
 */
#define SMP_LISTN_HILICHANGED			0x0001

/**
 * \brief Sent when KEY SELECT up. \的发送键选择的时候了
 *
 * \code
 *	case WM_COMMAND:
 *		WID id = LOWORD(wParam);
 *		WORD code = HIWORD(wParam);
 *
 *		if(id == "the list id" && code == SMP_LISTN_SELECTED)
 *		{
 *			int index = (int)lParam;
 * 			//处理的通知消息
 *		}
 * \endcode
 *
 * \param index current high light index\参数指数目前的高光指数
 */
#define SMP_LISTN_SELECTED			0x0002

/**
 * \brief Sent when MOUSE up.\发送时鼠标
 *
 * \code
 *	case WM_COMMAND:
 *		WID id = LOWORD(wParam);
 *		WORD code = HIWORD(wParam);
 *
 *		if(id == "the list id" && code == SMP_LISTN_CLICKED)
 *		{
 *			int index = (int)lParam;
 * 			//handle the notify message/处理的通知消息

 *		}
 * \endcode
 *
 * \param index current high light index * \参数指数目前的高光指数
 */
#define SMP_LISTN_CLICKED				0x0003

/**
 * \brief Max supported collumns.  \的最大支持collumns
 */
#define SMP_LIST_MAX_STR_COL		2

/**
 * \行信息
 */
typedef struct SMP_RowData
{	
	Uint16 margin;/* 左边距 */	
	Uint16 colWidth0;	/* 图标宽 */
	uint16 colHeight0;
	HBITMAP hBmp;/* 图标句柄 */	
	struct{ /* 信息 */	
		PCWSTR str;/* 字符串*/		
		Uint16 width;/* 宽*/
	}cols[SMP_LIST_MAX_STR_COL];	
} SMPROWDATA, *PSMPROWDATA;
/**
 * \brief The function provide to list to get total row count.\的功能提供名单，获得总行数
 *
 * \return the total rows of the list \返回列表中的总行数
 */
typedef int (*SMP_List_GetTotal)(VOID);

/**
 * \brief The function provide to list to get row data. \的功能提供名单，获得行数据
 *
 * \param index the row index\参数指标的行索引
 * \param[out] pRowData the row data\参数[出] pRowData行数据
 */
typedef VOID (*SMP_List_GetRowData)(int index, PSMPROWDATA pRowData);

/**
 * \brief The draw row function.\的抽签行功能
 *
 * \param index the row index\的抽签行功能
 * \param r the r rectangle\参数r的?矩形
 * \param pRowData the row data \参数pRowData行数据
 * \param hilight the high light row index\参数高亮高光行索引
 * \param flush if flush to the screen \参数刷新到屏幕上，如果刷新
 */
typedef VOID (*SMP_List_DrawRow)(int index, PRECT r, PSMPROWDATA pRowData, int hilight, BOOL flush);

/**
	窗口成员函数

 * \brief Set user data provide callbacks. \设置用户的数据提供了回调
 *
 * \param hList the list window handle \参数hList列表窗口句柄
 * \param fnGetTotal the function to get total row count \参数fnGetTotal的函数来获取总行数
 * \param fnGetRowData the function to get row data by index that from 0 
 \参数fnGetRowData函数来得到索引的行数据，从0
 */
VOID SMP_List_SetDataProvider(HWND hList, SMP_List_GetTotal fnGetTotal, SMP_List_GetRowData fnGetRowData);

/**
 * \brief Set a user specific drawing function.\设置了用户特定的绘图功能
 *
 * \param hList the List window handle \参数hList列表窗口句柄
 * \param fnDrawRow the new draw row function \参数fnDrawRow ROW函数的新画
 */
VOID SMP_List_SetDrawRowCallback(HWND hList, SMP_List_DrawRow fnDrawRow);

/**
 * \brief The default draw row function.\的默认行绘制功能
 *
 * \param index the index of the list item \参数索引的列表项的索引
 * \param r the list item draw rect\ r的参数列表项绘制矩形
 * \param pRowData the row data\参数pRowData行数据
 * \param hilight current hilight list item index\参数高亮当前高亮列表项的索引
 * \param flush if flush to screen\如果冲洗参数刷新屏幕
 */
VOID SMP_List_DefaultDrawRowCallback(int index, PRECT r, PSMPROWDATA pRowData, int hilight, BOOL flush);

/**
 * \brief High light a row by index.\声音高光按索引列
 *
 * \param hList the List window\参数hList列表窗口
 * \param index the row index\参数指标的行索引
 */
VOID SMP_List_HilightByIndex(HWND hList, int index);

/**
 * \brief Get current high light row index.\获取当前的高光行索引
 *
 * \param hList the List window handle \参数hList列表窗口句柄
 * \return the row index\返回该行的索引
 */
int SMP_List_GetHilightIndex(HWND hList);

/**
 * \brief Get the current page start item index and page size.
 * \获取当前页启动项指数和页面大小
 *
 * \param hList the list window handle\参数hList列表窗口句柄
 * \param[out] pagestart the current page start list item\参数[出] pagestart当前页开始列表项
 * \param[out] pagesize the list page size\参数[出]每页大小的列表页
 */
VOID SMP_List_GetPageInfo(HWND hList, int* pagestart, int* pagesize);

/**
 * \brief Page up the list.页这一名单
 *
 * \param hList the list window handle参数hList列表窗口句柄
 */
VOID SMP_List_PageUp(HWND hList);

/**
 * \brief Page down the list. \下一页清单
 *
 * \param hList the list window handle\参数hList列表窗口句柄
 */
VOID SMP_List_PageDown(HWND hList);

/**
 * \brief Line up the list. \该名单线
 *
 * \param hList the list window handle\参数hList列表窗口句柄
 */
VOID SMP_List_LineUp(HWND hList);

/**
 * \brief Line down the list.\在列表中向下线
 *
 * \param hList the list window handle\参数hList列表窗口句柄
 */
VOID SMP_List_LineDown(HWND hList);

/**
 * \brief Notify list that total list items changed.\通知列表，总清单项目变更
 *
 * \param hList the list window handle\参数hList列表窗口句柄
 */
VOID SMP_List_Refresh(HWND hList);

/**
 * \brief Reset the list. \重置列表
 * 
 * \param hList the list window handle\参数hList列表窗口句柄
 */
VOID SMP_List_Reset(HWND hList);

/*
窗口程序
 * \brief The simple list window procedure.\的简单列表的窗口过程
 *
 * \param hList the window handle* \参数hList的窗口句柄
 * \param Msg the window message * \参数消息的窗口消息
 * \param wParam the first parameter \参数wParam的第一个参数
 * \param lParam the second parameter \参数lParam的第二个参数
 * \return the result of message process  \返回消息的处理结果
 */
LRESULT SMP_List_WndProc(HWND hList, UINT Msg, WPARAM wParam, LPARAM lParam);


#endif /* _SMP_LIST_H */


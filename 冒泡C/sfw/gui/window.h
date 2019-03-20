#ifndef _SGL_WINDOWS_H
#define _SGL_WINDOWS_H

#include "types.h"
#include "gal.h"


/**

 - GUI库初始化
 - 创建顶层窗口
 - 新增顶级窗口到桌面

  \code
  	int InitApplication(VOID)
  	{
		HWND hWnd;
  		...

  		hWnd = SGL_CreateWindow(WND1_WndProc, ...);
  		SGL_AddChildWindow(HWND_DESKTOP, hWnd);
  		...
  	}
  \endcode

 创建顶层窗口
SGL是一个窗口对应一个窗口过程来响应窗口消息。
  \code
	LRESULT WND1_WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
	{
		switch(Msg)
		{
		case WM_CREATE:
			//分配必要的内存，并创建一些子窗口.
			break;			
		case WM_DESTROY:
			//释放资源
			break;
		case WM_SHOW:
			//在此情况下，您可以更新子窗口的位置和调整窗口.
			break;
		case WM_HIDE:
			break;
		case WM_MOUSEDOWN:
			//处理鼠标按下事件
			break;
		case WM_MOUSEUP:
			//处理鼠标释放事件
			break;
		case WM_KEYDOWN:
			//处理按键按下事件
			break;			
		case WM_KEYUP:
			//处理按键释放事件
			break;		
		case WM_COMMAND:
		    //处理子窗口的通知消息
			break;
        //其它消息
		... 
		}
		return 0;
	}
  \endcode
*/


#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */


/**
 * 平台最大支持的窗口句柄个数,控件也是一个窗口
 */
#ifndef SGL_MAX_WINDOW_COUNT
#define SGL_MAX_WINDOW_COUNT		60
#endif

/**
 * \长按键定时器启动的时钟周期
 */
#ifndef SGL_KEYREPEAT_START_INTERVAL
#define SGL_KEYREPEAT_START_INTERVAL		400
#endif

/**
 * \长按键定时器的时钟周期
 */
#ifndef SGL_KEYREPEAT_INTERVAL
#define SGL_KEYREPEAT_INTERVAL			100
#endif 

/**
 * 窗口过程SGL_WndProc类型定义 
 */
typedef LRESULT (*SGL_WndProc)(HWND, UINT, WPARAM, LPARAM);

/**
 * \一种应用事件过滤器类型的定义
 */
typedef LRESULT (*SGL_AppEventFilter)(UINT, WPARAM, LPARAM);

/**
 * \一种在SGL的系统窗口类型定义
 */
typedef struct SGL_Window
{
	WID id;							/* 窗口标识符*/
	Uint16 reserve;					/* 保留*/
	DWORD style;					/* 窗口样式,如WS_TRANSPARENT等窗口样式*/
	int left, top, width, height;	/* 窗口在父窗口坐标矩形*/
	Uint32 bgcolor, fgcolor;		/* 背景和前景颜色*/	
	SGL_WndProc wndproc;			/* 窗口过程*/	
	HWND parent;					/* 保持父窗口。窗口是顶层窗口时，父窗口是HWND_DESKTOP */	
	HWND child;						/* 第一个子窗口hanlde*/	
	HWND next;						/* 下一个同级窗口句柄*/	
	HWND prev;						/* 上一个同级窗口句柄*/	
	HWND focus;						/* 焦点子窗口*/	
	HWND listener;					/* 一个监听窗口，当设置SGL_NotifyParent将发送消息给它*/
	DWORD userdata;					/* 用户数据*/
	DWORD windata;					/* 窗口数据*/
}SGL_Window;

/**
 * \一个窗口的id
 */
#define _WID(hWnd) \
	(hWnd)->id

/**
 * \一个窗口的样式
 */
#define _STYLE(hWnd) \
	(hWnd)->style

/**
 * \宏添加一个窗口样式
 */
#define _SET_STYLE(hWnd, style) \
	(_STYLE(hWnd) |= (style))

/**
 * \宏清除一个窗口样式
 */
#define _CLR_STYLE(hWnd, style) \
	(_STYLE(hWnd) &= ~(style))

/**
 * \宏测试窗口是否有某种样式
 */
#define _IS_SET_ANY(hWnd, style) \
	(_STYLE(hWnd) & (style))

/**
 * \宏测试窗口是否有多种样式
 */
#define _IS_SET_ALL(hWnd, style) \
	((_STYLE(hWnd) & (style)) == (style))

/**
 * \将某种窗口样式反向设置，有变无，无变有
 */
#define _REV_STYLE(hWnd, style)		\
do{									\
if(_IS_SET_ANY(hWnd, style))		\
	_CLR_STYLE(hWnd, style);		\
else								\
	_SET_STYLE(hWnd, style);		\
}while(0)

/**
 * \窗口左侧位置
 */
#define _LEFT(hWnd) \
	(hWnd)->left

/**
 * \窗口顶部位置
 */
#define _TOP(hWnd) \
	(hWnd)->top

/**
 * \窗口宽度
 */
#define _WIDTH(hWnd) \
	(hWnd)->width

/**
 * \窗口高度
 */
#define _HEIGHT(hWnd) \
	(hWnd)->height

/**
 * \窗口矩形区域
 */
#define _WINRECT(hWnd, r) \
	(r).left = _LEFT(hWnd), \
	(r).top=_TOP(hWnd), \
	(r).width=_WIDTH(hWnd), \
	(r).height=_HEIGHT(hWnd)

/**
 * \窗口的背景颜色
 */
#define _BGCOLOR(hWnd) \
	(hWnd)->bgcolor

/**
 * \窗口前景色
 */
#define _FGCOLOR(hWnd) \
	(hWnd)->fgcolor

/**
 * \窗口的窗口过程
 */
#define _WNDPROC(hWnd) \
	(hWnd)->wndproc

/**
 * \窗口的用户数据
 */
#define _USERDATA(hWnd) \
	(hWnd)->userdata

/**
 * \设置窗体用户数据
 */
#define _SET_USERDATA(hWnd, data) \
	_USERDATA(hWnd) = (DWORD)(data)

/**
 * \一个特定类型的窗口用户数据
 */
#define _GET_USERDATA(hWnd, type) \
	((type)_USERDATA(hWnd))

/**
 * \HWND的窗口数据
 */
#define _WINDATA(hWnd) \
	(hWnd)->windata

/**
 * \宏设置HWND的窗口数据
 */
#define _SET_WINDATA(hWnd, data) \
	_WINDATA(hWnd) = (DWORD)(data)

/**
 * \一个特定类型窗口的窗口数据
 */
#define _GET_WINDATA(hWnd, type) \
	((type)_WINDATA(hWnd))

/* 父窗口 */
#define _PARENT(hWnd) \
	(hWnd)->parent

/* 窗口的后一个节点 */
#define _NEXT(hWnd) \
	(hWnd)->next

/* 窗口的前一个节点 */
#define _PREV(hWnd) \
	(hWnd)->prev

/* 子窗口 */
#define _CHILD(hWnd) \
	(hWnd)->child

/**
 * \顶层/模态窗口的焦点子窗口
 */
#define _FOCUS(hWnd) \
	(hWnd)->focus

/* 当前窗口的监听窗口 */
#define _LISTENER(hWnd) \
	(hWnd)->listener

/**
 * 桌面窗口
 */
#define HWND_DESKTOP	(HWND)(0xFFFFFFFF)

/**				
 * \检查hWnd是常态窗口
 */
#define IS_NORMAL_WINDOW(hWnd) \
	(NULL != hWnd && HWND_DESKTOP != hWnd)

//检查hWnd不是常态窗口
#define NOT_IS_NORMAL_WINDOW(hWnd) \
    (!IS_NORMAL_WINDOW(hWnd))

/**
 * \编辑控制消息的类型定义
 */
typedef struct 
{
	PCWSTR title;	/* 显示在输入法窗口的标题 */	
	PWSTR buffer;	/* 编辑缓冲区的内容 */	
	Uint32 size;	/* 缓冲区大小 */
} SGL_EditInfo, *SGL_EditInfo_p;

/**
 * \鼠标按下消息
 *
 * \code
 * case WM_MOUSEDOWN:
 *	int x = (int)wParam;
 *	int y = (int)lParam;
 * \endcode
 *
 * \参数x		窗口中鼠标的x位置
 * \参数y		窗口中鼠标的y位置
 */
#define WM_MOUSEDOWN		0x3001	  //12289

/**
 * \鼠标释放消息
 *
 * \code
 * case WM_MOUSEUP:
 *	int x = (int)wParam;
 *	int y = (int)lParam;
 * \endcode
 *
 * \参数x		窗口中鼠标的x位置
 * \参数y		窗口中鼠标的y位置
 */
#define WM_MOUSEUP			0x3002

/**
 * \鼠标移动的消息
 *
 * \code
 * case WM_MOUSEMOVE:
 *	int x = (int)wParam;
 *	int y = (int)lParam;
 * \endcode
 *
 * \参数x		窗口中鼠标的x位置
 * \参数y		窗口中鼠标的y位置
 */
#define WM_MOUSEMOVE		0x3003

/**
 * \用户按下一个键
 *
 * 当用户按下一个键，此消息发送到当前活动窗口
 * \code
 * case WM_KEYDOWN:
 *	DWORD key = wParam;
 * \endcode
 *
 * \参数key		按下的键的键码
 */
#define WM_KEYDOWN		0x3004

/**
 * \用户释放了一个按键
 *
 * 当用户释放一个键时，此消息发送到当前活动窗口。
 * \code
 * case WM_KEYUP:
 *	DWORD key = wParam;
 * \endcode
 * 参数key		释放的键的键码
 */
#define WM_KEYUP		0x3005

/**
 * \表示窗口已经获得输入焦点
 * 此消息被发送到窗口过程后的窗口获得输入焦点。
 */
#define WM_SETFOCUS		0x3006

/**
 * \表示该窗口已经失去输入焦点
 * 此消息被发送到窗口过程后的窗口失去输入焦点。
 */
#define WM_KILLFOCUS		0x3007

/**
 * \表示窗口已经建立，并给你一个机会来初始化您的CODE。
 *
 * 此消息被发送到窗口后，窗口已创建。
 * 您可以初始化自己的对象，当您收到此消息，
 * 并返回零到系统显示，以初始化的成功。
 * 如果返回非零到系统处理此消息后，创建的窗口将会销毁。
 */
#define WM_CREATE		0x3008

/**
 * \表示将会销毁该窗口。
 *
 * 调用SGL_DestroyWindow时,此消息被发送到窗口。
 * 当收到此消息你可以销毁你的CODE。
 */
#define WM_DESTROY		0x3009

/**
 * \重绘窗口
 * \code 
 * case WM_PAINT:
 *	PRECT pRgn = (PRECT) wParam;
 * \endcode
 *
 * \参数pRgn    无效区域，当NULL意味着所有的窗口区域
 * \从窗口过程返回非零指窗口自己处理它的子窗口，SGL将不发送WM_PAINT消息给它的子窗口
 */
#define WM_PAINT		0x300A

/**
 * \发送到窗口时，窗口显示
 * 此消息发送到窗口时，窗口变得可见。
 */
#define WM_SHOW		0x300B

/**
 * \发送到窗口时，窗口隐藏
 * 
 * 此消息发送到窗口的子窗口时删除它的父窗口，
 * 或当一个顶层窗口被另一个重叠。
 */
#define WM_HIDE			0x300C

/**
 * \这个消息表示是来自子窗口的通知消息
 * 此消息发送到窗口时一个子窗口已发送通知消息到父窗口。
 *
 * \code 
 * case WM_COMMAND:
 *	WID id = LOWORD(wParam);
 *	WORD code = HIWORD(wParam);
 * \endcode
 *
 * \参数id		子窗口ID
 * \参数code	子窗口通知消息代码
 */
#define WM_COMMAND		0x300D

/**
 * \菜单显示时发送到父窗口
 * 当一个菜单弹出时，此消息发送到父窗口让父窗口有机会去处理事情。
 * \code
 * case WM_MENUSHOW:
 	HMENU hMenu = (HMENU)lParam;
 * \endcode
 *
 * \参数hMenu	弹出的菜单句柄
 * \对应 WM_MENUHIDE
 */
#define WM_MENUSHOW		0x3100

/**
 * \菜单关闭时，发送到父窗口
 *
 * 菜单关闭时此消息发送到父窗口让父窗口有机会处理事件。
 *
 * \code
 * case WM_MENUHIDE:
 *	BOOL redraw = (BOOL)wParam;
 *	HMENU hMenu = (hMenu)lParam;
 * \endcode
 *
 * \参数redraw	顶端窗口将重绘
 * \参数hMenu	菜单窗口句柄
 * \对应 WM_MENUSHOW
 */
#define WM_MENUHIDE		0x3101

/**
 * \发送到父窗口时，鼠标按下在菜单面积上了.
 *
 * 不要执行
 */
#define WM_MENUMOUSEDOWN	0x3102

/**
 * \鼠标区域不在菜单中时，发送到父窗口上
 * 此消息发送到父窗口当鼠标出来的菜单，
 * 当返回0将关闭菜单，否则保留菜单。
 * \code 
 * case WM_MENUMOUSEUP:
 *	int x = (int)wParam;
 *	int y = (int)lParam;
 * \endcode
 *
 * \参数x	父窗口的鼠标x位置
 * \参数y	父窗口的鼠标y位置
 */
#define WM_MENUMOUSEUP		0x3103

/**
 * \brief GUI framework send this message to get the submenu handle
 * \GUI框架发送此消息得到子菜单句柄
 * 在一个弹出菜单。
 *
 * 菜单窗口必须实现了这个菜单。
 *
 * \code
 * case WM_GETSUBMENU:
 *	return hSubMenu;  //返回子菜单句柄
 * \endcode
 *
 * \param[out] pMenu the pointer to the sub menu handle return to framework
 参数[出] pMenu	    子菜单返回指针处理框架
 */
#define WM_GETSUBMENU		0x3104

/**
 * \模态窗口显示时发送到父窗口
 *
 * \code
 * case WM_MODALSHOW:
 *	HWND hDlg = (HWND)lParam;
 * \endcode
 *
 * \参数hDlg	模态窗口句柄
 */
#define WM_MODALSHOW		0x3200

/**
 * \模态窗口关闭时发送到父窗口
 *
 * \code
 * case WM_MODALHIDE:
 *	HWND hDlg = (HWND)lParam;
 * \endcode
 *
 * \参数hDlg	模态窗口句柄
 */
#define WM_MODALHIDE		0x3201

/**
 * \发送到父窗口时鼠标按下了模态窗口
 * \code
 * case WM_MODALMOUSEDOWN:
 * 	int x = (int)wParam;
 *	int y = (int)lParam;
 * \endcode
 *
 * \参数x   顶部窗口中鼠标的x位置
 * \参数y   鼠标的y位置SKY窗诠释
 */
#define WM_MODALMOUSEDOWN	0x3202

/**
 * \brief Sent to parent window when mouse up out of the modal window.
 * \发送到父窗口当鼠标出来的模态窗口
 * \code
 * case WM_MODALMOUSEUP:
 * 	int x = (int)wParam;
 *	int y = (int)lParam;
 * \endcode
 * \参数x为顶部窗口中鼠标的x位置
 * \参数y的鼠标的y位置SKY_ 窗诠释
 */
#define WM_MODALMOUSEUP		0x3203

/**
 * \GUI框架把这个消息发送到窗口，用于获取消息。
 *
 * 在编辑窗口必须实现这个消息。
 *
 * \code 
 * case WM_GETEDITINFO:
 *		SGL_EditInfo* info = (SGL_EditInfo*)wParam;
 *		info->title = GET_TITLE(hEdit);
 *		info->buffer = GET_BUFFER(hEdit);
 *		info->size = GET_MAXSIZE(hEdit);
 * \endcode
 *
 * \参数[输出]info   返回框架的编辑消息
 */
#define WM_GETEDITINFO		0x3300

/**
 * \发送到编辑窗口，当输入完成（取消不会发送）
 * 编辑窗口必须实现从系统接收输入法的内容
 * \code
 * case WM_SETEDITTEXT:
 *	PCWSTR pContent = (PCWSTR)wParam;
 * \endcode
 *
 * \参数pContent	从系统输入法接收的文本内容
 */
#define WM_SETEDITTEXT		0x3301

/**
 * \发送到顶层或模态窗口来初始化默认焦点的子窗口
 */
#define WM_INITFOCUS		0x3302

/**
 * \长按键事件
 * 为了使这个活动应该开启ENABLE_SGL_KEYREPEAT宏
 */
#define WM_KEYDOWNREPEAT		0x3303

/**
 * \第一个用户定义的消息。
 * SGL保留的范围从0x3800到0xEFFF用户定义的消息。
 * WM_USER是第一个用户定义的消息，你可以使用它。
 */
#define WM_USER			0x3800









/**
 * \最后一个用户的消息.
 *
 * 用户的消息不能超过这个值
 */
#define WM_LASTUSERMSG		0xEFFF

/**
 * \表明它是一个顶层窗口
 *
 * \注意这种样式来自框架。应用工程师不应该设置或清除这种窗口样式。
 */
#define WS_OVERLAPPED		0x80000000L

/**
 * \表示这是一个子窗口
 *
 * \注意这种样式来自框架。应用工程师不应该设置或清除这种窗口样式。
 */
#define WS_CHILD			0x40000000L

/**
 * \表示这是一个弹出菜单
 *
 * 此样式不仅用于显示一个弹出菜单。当一个菜单popuped在顶部/模态窗口，
 * 父窗口将得到样式（WS_INACTIVE | WS_POPUP）。因此，一个弹出菜单，不仅
 * 按样式WS_POPUP决定，WS_INACTIVE样式也应检查。
 *
 * \code
 *	if(!_IS_SET_ANY(hWnd, WS_INACTIVE) && _IS_SET_ANY(hWnd, WS_POPUP))
 *	{
 *		//是一个菜单
 *	}
 * \endcode
 *
 * \注意这种样式来自框架。应用工程师不应该设置或清除这种窗口样式。
 */
#define WS_POPUP			0x20000000L

/**
 * \表示这是一个模态窗口
 *
 * 不只是用来表明这种样式模态窗口。当在上面窗口显示一个模态，
 * 父窗口将得到样式（WS_INACTIVE | WS_MODAL）。因此，一个模态窗口不能只
 * 按样式WS_MODAL决定，WS_INACTIVE样式也应检查。
 *
 * \code
 *	if(!_IS_SET_ANY(hWnd, WS_INACTIVE) && _IS_SET_ANY(hWnd, WS_MODAL))
 *	{
 *		//是一个模态窗口
 *	}
 * \endcode
 *
 * \注意这种样式来自框架。应用工程师不应该设置或清除这种窗口样式。
 */
#define WS_MODAL			0x10000000L

/**
 * \表示窗口可见
 *
 * \注意这种样式来自框架。应用工程师不应该设置或清除这种窗口样式。
 */
#define WS_VISIBLE			0x08000000L

/**
 * 表示顶级窗口或模态窗口处于非活动状态
 *
 * 一个非活动窗口是指一个弹出菜单的顶级窗口或模态窗口显示。
 * 或模态窗口，在顶层窗口中。
 * 当一个弹出菜单中显示父窗口将得到（WS_INACTIVE | WS_POPUP）。
 * 当一个模态窗口中显示父窗口将得到（WS_INACTIVE | WS_MODAL）。
 *
 * \注意这种样式来自框架。应用工程师不应该设置或清除这种窗口样式。
 */
#define WS_INACTIVE			0x04000000L

/**
 * \窗口获得输入焦点。
 *
 * \注意这种样式来自框架。应用工程师不应该设置或清除这种窗口样式。
 */
#define WS_FOCUSED			0x02000000L

/**
 * \窗口是透明的。
 *
 * \GUI将不会画出窗口背景。
 */
#define WS_TRANSPARENT	0x01000000L

/**
 * \表示窗口可以停止焦点。
 *
 * 当子窗口创建的这种样式，SGL_FocusNext / SGL_FocusPrev
 * 可以停在子窗口的焦点。
 */
#define WS_TABSTOP			0x00800000L

/**
 * \brief Indicates the sgl should focus the child window except the window itself
 *
 * This style must used with WS_TABSTOP, when WS_FOCUSCHILD setted the 
 * child window will get the focus.
 * \SGL除应注重自身的窗口子窗口
 *
 * 此样式必须与WS_TABSTOP，使用的时候WS_FOCUSCHILD setted
 * 子窗口获得焦点。
 */
#define WS_FOCUSCHILD		0x00400000L

/**
 * \brief Restore the focus when shown again.
 *
 * You can create a top-level window with this style.
 * The window will restore the focus window that has the focus before hiden.
 * \还原焦点时再次显示。
 *
 * 您可以创建一个带有这种样式的顶层窗口。
 * 该窗口将恢复具有焦点的窗口前隐藏的焦点。
 */
#define WS_KEEPFOCUS		0x00200000L

/**
 * \表示当子窗口删除时框架不会重绘顶层窗口。
 * 
 * \注意只有在菜单和模态窗口中使用这种样式。
 */
#define WS_NOREDRAW		0x00100000L

/**
 * \鼠标在这个窗口有效。
 *
 * 也许在某些控件的鼠标处理中使用这种样式。
 */
#define WS_PRESSED			0x00080000L

/**
 * \表示对这个窗口按下键。
 *
 * 也许在某些控件的按键处理中使用这种样式。
 */
#define WS_KEYDOWN			0x00040000L

/**
 * \窗口被禁用。
 *
 * 这种样式应该由控制器执行。
 */
#define WS_DISABLED		0x00020000L

/**
 * \窗口边界。
 *
 * 这种样式应该由控制器执行。
 */
#define WS_BORDER			0x00010000L

/**
 * \系统样式位。
 */
#define WS_MASK_SYSTEM	0xffff0000L

/**
 * \用户样式位。
 */
#define WS_MASK_USER		0x0000ffffL

/**
 * \编辑窗口样式输入数字.
 */
#define ES_NUM		0x00008000L

/**
 * \编辑窗口样式输入密码.
 */
#define ES_PWD		0x00004000L

/**
 * \编辑窗口样式输入字符.
 */
#define ES_ALPHA	0x00002000L

/**
 * \初始化SKY GUI库，这个函数必须在运行应用程序代码前调用
 * \对应 SGL_Terminate
 */
VOID GUIAPI SGL_Initialize(VOID);

/**
 * \终止SKY GUI库，退出应用程序时调用
 * \对应 SGL_Initialize
 */
VOID GUIAPI	SGL_Terminate(VOID);

/**
 * \暂停所有GDI函数。
 * \禁止SGL刷屏, 调用此函数之后所作的刷新窗口操作不会反映到屏幕上	  
 * \使用这个函数可以防止过度刷新屏幕, 导致屏幕闪烁
 * \code 
 * 	SGL_SuspendDrawing();
 *	SGL_UpdateWindow(hWnd); //之后，hWnd不会得到WM_PAINT事件
 * \endcode
 * 
 * \对应 SGL_UnsuspendingDrawing
 */
VOID GUIAPI SGL_SuspendDrawing(VOID);

/**
 * \取消暂停GDI函数
 *
 * 调用过SGL_SuspendDrawing后调用
 * 
 * \对应 SGL_SuspendDrawing
 */
VOID GUIAPI SGL_UnsuspendDrawing(VOID);

/**
 * \判断当前窗口是否禁止刷屏
 */
BOOL GUIAPI SGL_IsSuspendDrawing(VOID);

/**
 * \获取screeen或桌面的大小
 *
 * \参数pSize	输出的桌面大小
 */
VOID GUIAPI SGL_GetScreenSize(PSIZE pSize);

/**
 * \获取当前系统字体
 *
 * \返回当前系统字体
 */
HFONT GUIAPI SGL_GetSystemFont(VOID);

/**
 * \设置系统字体。
 *
 * \参数font	新字体
 */
VOID GUIAPI SGL_SetSystemFont(HFONT font);

/**
 * \在父窗口中按标识符查找子窗口
 * \参数hWnd   父窗口句柄
 * \参数id	   子窗口ID
 * \返回找到的第一个子窗口句柄
 */
HWND GUIAPI SGL_FindChildWindow(HWND hWnd, WID id);

/**
 * \查找点x，y所在的子窗口
 * \参数hWnd	窗口句柄
 * \参数x	    左边的位置
 * \参数Y	    右边的位置
 * \返回找到的窗口句柄，否则返回null
 */
HWND GUIAPI SGL_FindChildWindowByPosition(HWND hWnd, int x, int y);

/**
 * \检索下一个控件窗口
 * 此功能retrives指定下一
 * 窗口\一个HWND。如果传递的参数为NULL
 * 的\一hChild，该函数将返回该窗口的第一个子窗口。

 * \参数hWnd	窗口句柄
 * \参数hChild	子窗口的句柄
 * \返回下一个子窗口的句柄。如果你传递NULL的
 * 在\一hChild参数，它将会传回的第一个子窗口
 * 窗外。当你的子窗口传递的最后一个，
 * 此函数将返回NULL。如果hWnd是不是父
 * 在hChild，这个函数将返回NULL。
 */
HWND GUIAPI SGL_GetNextChild (HWND hWnd, HWND hChild);

/** 
 * \获取子窗口的顶层窗口 (所谓顶层窗口就是整个屏幕显示的大窗口, 是HWND_DESKTOP的子窗口, 不是控件窗口)
 * \参数hWnd	子窗口句柄
 * \返回顶层窗口
 */
HWND GUIAPI SGL_GetTopWindow(HWND hWnd);

/**
 * 把窗口坐标系转换为屏幕坐标系
 * 窗口的位置信息是相对于父窗口的相对位置, 该函数把子窗口的位置坐标转换成屏幕的绝对坐标
 * \参数hWnd			窗口句柄
 * \参数[输入，输出]	X坐标
 * \参数[输入，输出]	Y坐标
 * 
 * \对应 SGl_ScreenToWindow
 */
VOID GUIAPI SGL_WindowToScreen (HWND hWnd, int* x, int* y);

/**
 * 把屏幕坐标系转换为窗口坐标系
 * 窗口的位置信息是相对于父窗口的相对位置, 该函数把屏幕的绝对坐标转换成窗口的位置坐标
 *
 * \参数hWnd			窗口句柄
 * \参数[输入，输出]	X坐标
 * \参数[输入，输出]	Y坐标
 * 
 * \对应 SGL_WindowToScreen
 */
VOID GUIAPI SGL_ScreenToWindow (HWND hWnd, int* x, int* y);

/**
 * \创建SGL窗口
 *
 * 参数:
 * WndProc		窗口回调
 * X			窗口的左边位置
 * y			窗口的顶部位置
 * w			窗口的宽度
 * h			窗口的高度
 * id			窗口ID
 * style		窗口样式
 * userdata		用户数据
 *
 * 返回： 
 *	- 成功返回	窗口句柄
 *	- 失败返回	NULL
 */
HWND GUIAPI SGL_CreateWindow(SGL_WndProc wndproc, int x, int y, int w, int h, WID id, DWORD style, DWORD userdata);

/**
 * \销毁SGL_CreateWindow创建的窗口
 *
 * \参数hWnd	被销毁窗口句柄
 * \成功返回TRUE，否则返回FALSE
 */
BOOL GUIAPI SGL_DestroyWindow(HWND hWnd);

/**
 * \发送消息到一个窗口
 *
 * \参数hWnd	窗口句柄
 * \参数Msg		窗口消息
 * \参数wParam	第一个参数
 * \参数lParam	第二个参数
 * \返回消息的处理结果
 */
LRESULT GUIAPI SGL_SendMessage(HWND hWnd, UINT Msg, DWORD wParam, DWORD lParam);

/**
 * \发送消息给HWND和它所有的子窗口
 *
 * \参数hWnd		窗口句柄
 * \参数Msg			窗口消息
 * \参数wParam		第一个参数
 * \参数lParam		第二个参数
 * \参数hExcept		不接收消息的窗口
 * \返回消息的处理结果
 */
VOID GUIAPI SGL_BroadcastMessageEx(HWND hWnd, UINT Msg, DWORD wParam, DWORD lParam, HWND hExcept);

/**
 * \发送消息给HWND和它所有的子窗口
 *
 * \参数hWnd		窗口句柄
 * \参数Msg			窗口消息
 * \参数wParam		第一个参数
 * \参数lParam		第二个参数
 * \返回消息的处理结果
 */
#define SGL_BroadcastMessage(hWnd, Msg, wParam, lParam) \
	SGL_BroadcastMessageEx(hWnd, Msg, wParam, lParam, NULL)

/**
 * \发送消息到一个窗口
 *
 * \参数hWnd		窗口句柄
 * \参数id			通知窗口的ID
 * \参数code		通知代码
 * \参数userdata	message消息
 * \返回该消息的处理结果
 */
#define SGL_SendNotifyMessage(hWnd, id, code, userdata) \
	SGL_SendMessage(hWnd, WM_COMMAND, (WPARAM)MAKELONG(id, code), (LPARAM)userdata)

/**
 * \发送消息给父窗口
 *
 * \参数hWnd		窗口句柄
 * \参数code		消息代码
 * \参数userdata	用户消息
 * \返回该消息的处理结果
 */
#define  SGL_NotifyParent(hWnd, code, userdata) \
	SGL_SendNotifyMessage((_LISTENER(hWnd)? _LISTENER(hWnd) : _PARENT(hWnd)), _WID(hWnd), code, userdata)

/**
 * \检查窗口是否可见
 *
 * \参数hWnd	窗口句柄
 * \可见返回TRUE，否则返回FALSE
 */
#define SGL_IsWindowVisible(hWnd) \
	(IS_NORMAL_WINDOW(hWnd) && _IS_SET_ANY(hWnd, WS_VISIBLE))

/**
 * \检查窗口处于活动状态
 *
 * \返回TRUE时，没有弹出菜单或模态窗口在顶层窗口显示
 */
#define SGL_IsWindowActive(hWnd) \
	(!_IS_SET_ANY(hWnd, WS_INACTIVE))

/**
 * \更新特定的窗口
 *
 * \参数hWnd		要更新的窗口句柄
 * \参数pClipRect	矩形区域
 */
VOID GUIAPI SGL_UpdateWindowEx(HWND hWnd, PRECT pClipRect);

/**
 * \刷新指定窗口(全屏刷新)
 *
 * \参数hWnd	要更新的窗口句柄
 */
#define SGL_UpdateWindow(hWnd)	\
	SGL_UpdateWindowEx(hWnd, NULL)

/**
 * \brief Paint a window with a clip region. \填充与剪辑地区的对外窗口
 *
 * \参数hWnd		窗口句柄
 * \参数pClipRect	裁剪区域
 * \注意通常你应该只使用SGL_UpdateWindowEx或SGL_UpdateWindow 
 * 和SGL_PaintWindowEx刚刚绘制窗口，但不刷新到屏幕上，也不会
 * 检查绘图锁和窗口可见的状态。
 * \对应 SGL_UpdateWindowEx, SGL_UpdateWindow
 */
VOID GUIAPI SGL_PaintWindowEx(HWND hWnd, PRECT pClipRect);

/**
 * \填充整个窗口
 *
 * \参数hWnd	窗口句柄
 * \注意与SGL_PaintWindowEx相同只是没有剪辑区域
 *	
 * \对应 SGL_PaintWindowEx, SGL_UpdateWindowEx, SGL_UpdateWindow
 */
VOID GUIAPI SGL_PaintWindow(HWND hWnd);

/**
 * \添加窗口作为指定父窗口的子窗口
 *
 * \参数hParent		父窗口句柄，当HWND_DESKTOP时hChild将成为顶级窗口
 * \参数hChild		子窗口句柄
 * \成功返回TRUE，否则返回FALSE
 */
BOOL GUIAPI SGL_AddChildWindow(HWND hParent, HWND hChild);

/**
 * \从父窗口节点删除该子窗口节点
 *
 * 参数hChild	子窗口句柄
 * 成功返回TRUE，否则返回FALSE
 */
BOOL GUIAPI SGL_RemoveChildWindow(HWND hChild);

/**
 * \平台将调用此函数来驱动GUI库
 *
 * \参数Msg		窗口消息
 * \参数wParam	第一个消息参数
 * \参数lParam	第二个消息参数
 */
VOID GUIAPI SGL_DispatchEvents(UINT Msg, WPARAM wParam, LPARAM lParam);

/**
 * \设置焦点窗口。
 * \参数hParent		父窗口，此窗口必须是top-level/modal窗口
 * \参数hChild		子窗口
 */
VOID GUIAPI SGL_SetFocusWindow(HWND hParent, HWND hChild);

/**
 * \获得焦点窗口的句柄。
 *
 * \参数的hWnd	top-level/modal窗口
 * \返回焦点子窗口句柄
 */
HWND GUIAPI SGL_GetFocusWindow(HWND hWnd);

/**
 * \hHead和hEnd之间找到一个可以得到焦点的子窗口
 *
 * \参数hHead   第一个窗口句柄
 * \参数hEnd    查找的最后一个窗口句柄
 * \参数next    TRUE寻找下一个，FALSE寻找上一个
 */
HWND GUIAPI SGL_FindFocusNext(HWND hHead, HWND hEnd, BOOL next);

/**
 * \在一个顶级窗口寻找下一个焦点子窗口
 *
 * \参数的hWnd窗口句柄，应该是顶级窗口或模态窗口
 * \参数next
 * - TRUE	下一个焦点
 * - FALSE	前一个焦点
 * \返回焦点子窗口句柄
 */
HWND GUIAPI SGL_FindFocus4TopWnd(HWND hWnd, BOOL next);

/**
 * \切换指定窗口的子窗体焦点
 *
 * \参数 hWnd 窗口句柄，应该是顶级窗口或模态窗口
 * \参数 next
 * - TRUE	下一个焦点
 * - FALSE	前一个焦点
 */
VOID GUIAPI SGL_FocusNext(HWND hWnd, BOOL next);

/**
 * \设置鼠标跟踪窗口。
 * \参数的hWnd新鼠标跟踪窗口
 */
VOID GUIAPI SGL_SetMouseTracker(HWND hWnd);

/**
 * \获取当前鼠标跟踪窗口。
 * \返回鼠标跟踪窗口
 */
HWND GUIAPI SGL_GetMouseTracker(VOID);

/**
 * \把指定窗口移动到其父窗口的第一个子窗口。
 *
 * \参数hWnd	子窗口句柄
 * \成功返回TRUE，否则返回FALSE
 */
BOOL GUIAPI SGL_BringWindowToTop(HWND hWnd);

/**
 * \关闭弹出菜单。
 *
 * \参数hMenu	菜单句柄
 * \参数redraw	强制刷新
 */
VOID GUIAPI SGL_CloseMenu(HMENU hMenu, BOOL redraw);

/**
 * \弹出菜单窗口。
 *
 * \参数hMenu		菜单窗口句柄
 * \参数X			菜单的左边位置
 * \参数y			菜单的顶部位置
 * \参数hParent		将收到菜单消息的父窗口句柄
 * \成功返回TRUE，否则返回FALSE
 */
BOOL GUIAPI SGL_PopupMenu(HMENU hMenu, int x, int y, HWND hParent);

/**
 * \选择一个菜单项。
 *
 * \参数hMenu		菜单句柄
 * \参数item		选择菜单项的ID
 * \参数userdata	用户数据
 */
VOID GUIAPI SGL_SelectMenuItem(HMENU hMenu, WID item, DWORD userdata);

/**
 * \获取当前弹出的菜单窗口句柄。
 *
 * \返回弹出菜单句柄，NULL时没有弹出窗口。
 */
HMENU GUIAPI SGL_GetPopupMenu(VOID);

/**
 * \在模态窗口绘制。
 *
 * \参数hDlg		窗口句柄
 * \参数hParent		父窗口句柄，不应该为NULL
 * \成功返回TRUE，否则返回FALSE
 */
BOOL GUIAPI SGL_DoModal(HWND hDlg, HWND hParent);

/**
 * \关闭模态窗口
 *
 * \参数hDlg	窗口句柄
 * \参数code	返回代码
 * \参数lParam  第二个参数
 * \成功返回TRUE，否则返回FALSE
 */
BOOL GUIAPI SGL_ExitModal(HWND hDlg, WORD code, LPARAM lParam);

/**
 * \获取当前模态窗口。
 *
 * \返回模态窗口句柄，在没有模态窗口时返回NULL。
 */
HWND GUIAPI SGL_GetModalWindow(VOID);

/**
 * \调用平台输入编辑框。
 *
 * \参数hEdit	编辑窗口句柄
 */
VOID GUIAPI SGL_TextInput(HWND hEdit);

/**
 * \设置一个应用程序事件过滤器。
 *
 * \参数filter	过滤器回调
 * \返回旧的句柄
 */
LRESULT GUIAPI SGL_SetAppEventFilter(SGL_AppEventFilter filter);

#ifdef ENABLE_SGL_KEYREPEAT		

//停止长按下某键
VOID GUIAPI SGL_StopKeyRepeat(VOID);

#endif


/**
 * \SGL窗口的过程，它是通用的SGL窗口
 *
 * \参数hWnd		窗口句柄
 * \参数Msg			窗口消息
 * \参数wParam		第一个参数
 * \参数lParam		第二个参数
 * \返回消息的处理结果
 */
LRESULT GUIAPI SGL_Window_WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

/**
 * \应用程序的入口,由框架调用。
 *
 * 在GUI和一些其他模块初始化，框架调用该函数
 * 给来初始化应用程序。所以这个功能
 * 必须由用户实现。
 *
 * \成功返回MR_SUCCESS，否则MR_FAILED
 */
int InitApplication(VOID); 

/** 
 * \退出应用程序,由框架调用。
 *
 * 在应用程序退出，框架调用该函数给用户
 * 做一些退出清理，如释放资源，关闭
 * 所有打开的文件等，这功能必须由应用程序实现。
 *
 * \成功返回MR_SUCCESS，否则MR_FAILED
 */
int ExitApplication(VOID);
/*
 * 应用暂停
 */
int PauseApplication(VOID);
/*
 * 应用恢复
 */
int ResumeApplication(VOID);

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* _SGL_WINDOWS_H */


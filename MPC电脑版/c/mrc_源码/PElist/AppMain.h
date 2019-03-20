#ifndef __APPMAIN_H__
#define __APPMAIN_H__

enum
{
	APPWINDOWS_VIEW, // 主窗口
	APPWINDOWS_DELREAME, // 删除小窗口
	APPWINDOWS_EXITREAME // 退出小窗口
};
// 取消返回
void Noback(void);
// 整个列表绘制
void FileViewBrowser_UI(void);
void RefreshList(void);
// 文件浏览器初始化
void FileViewBrowser_Main(void);
// 文件浏览器按键控制
void FileViewBrowser_Event(int32 code, int32 p, int32 q);

#endif // __APPMAIN_H__
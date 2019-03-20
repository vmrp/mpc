#ifndef _SMP_FLASHBOX_H
#define _SMP_FLASHBOX_H

#include "window.h"

/**

 Flash盒

  若要使用Flash盒：
   - 创建闪框窗口
   - 设置闪光灯框中的信息
   - 添加到父窗口


  \code
  //帧信息
  static const DWORD fbFrames[] = {
  	BMP_FRAME1,
  	BMP_FRAME2,
  	BMP_FRAME3
  };

  //你的代码
  hControl = SGL_CreateWindow(SMP_FlashBox_WndProc, ...);
  SMP_FlashBox_SetInfo(hControl, fbFrames, sizeof(fbFrames)/sizeof(fbFrames[0]), 1000, TRUE);
  SGL_AddChildWindow(hParent, hControl);
  
  \endcode


窗口成员函数

 * \闪光设置框中的信息。
 *
 * \参数的hWnd窗口句柄闪光灯箱
 * \参数财政资源管理处的闪光帧阵列
 * \参数数组的大小尺寸的帧
 * \参数间隔两帧之间的间隔值在MS
 * \参数循环在循环播放真闪光灯，假停在最后一帧palying
 * /
*/
VOID SMP_FlashBox_SetInfo(HWND hWnd, const DWORD* frms, Uint32 size, Uint32 interval, BOOL loop);

/**
 * \brief Play the flash from the index frame.播放从索引框架闪光灯
 *
 * \param hWnd the flash box window handle参数的hWnd窗口句柄闪光灯箱
 * \param index the index of the frame to play参数指标的帧索引播放
 * \sa SMP_FlashBox_Stop山SMP_FlashBox_Stop
 */
VOID SMP_FlashBox_Start(HWND hWnd, Uint32 index);

/**
 * \brief Continue playing.继续播放
 *
 * \param hWnd the flash box window handle参数的hWnd窗口句柄闪光灯箱
 * \sa SMP_FlashBox_Stop
 */
VOID SMP_FlashBox_Play(HWND hWnd);

/**
 * \brief Stop playing.停止播放
 *
 * The stop flash box will auto playing when shown again.闪光灯箱的停止播放时会自动再次显示
 *
 * \param hWnd the flash box window handle \参数的hWnd窗口句柄闪光灯箱
 * \sa SMP_FlashBox_Start, SMP_FlashBox_Play\山SMP_FlashBox_Start，SMP_FlashBox_Play
 */
VOID SMP_FlashBox_Stop(HWND hWnd);

/**
窗口程序
 * \brief The flash box window procedure.\闪光框的窗口过程
 *
 * \param hWnd the window handle\参数的hWnd窗口句柄
 * \param Msg the window message\参数消息的窗口消息
 * \param wParam the first parameter\参数wParam的第一个参数
 * \param lParam the second parameter \参数lParam的第二个参数
 * \return the result of message process \返回消息的处理结果
 */
LRESULT SMP_FlashBox_WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);



#endif

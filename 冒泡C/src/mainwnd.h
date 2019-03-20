#ifndef _MAINWND_H
#define _MAINWND_H

#include "window.h"

#define _VERSION 1001


extern char advFocus;

int32 SaveCode(PSTR name);


LRESULT MAINWND_WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

#endif

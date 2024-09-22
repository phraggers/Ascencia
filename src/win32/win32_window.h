/*============================================================
 * Project: Ascencia
 * File: win32_window.h
 * Author: Phraggers
 * Date: Sat Sep 14 2024
============================================================*/

#ifndef ASCENCIA_WIN32_WINDOW_H
#define ASCENCIA_WIN32_WINDOW_H

#include <util/types.h>
#include <platform/window.h>
#include <platform/input.h>
#include <win32/win32_api.h>

typedef struct
{
    PL_Window pl_window;
    PL_Input input;
    
    ptr handle;
    Win32_WNDCLASSA wndclass;
    u32 style;
    Win32_WNDPL wndpl;
    ptr hdc;
    ptr glrc;
} Win32_Window;

/* input */
PL_KEYCODE Win32_Keycode(u64 key, i64 lparam);
void Win32_ProcessXInputButton(Win32_XINPUT_GAMEPAD *xpad, PL_Gamepad *gamepad, u16 xbutton, PL_GAMEPADBTNCODE button_code, r64 dt);
void Win32_ProcessInput(r64 dt);
void Win32_UpdateInput(void);

/* window */
i64 Win32_WndProc(ptr handle, u32 msg, u64 wparam, i64 lparam);
void Win32_UpdateWindow(void);
void Win32_MessageLoop(void);
bool Win32_CreateWindow(void);

#endif /* ASCENCIA_WIN32_WINDOW_H */
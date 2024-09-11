
/*============================================================
 * Project: Ascencia [EXE]
 * File: window.c
 * Author: Phraggers
 * Date: Wed Sep 11 2024
============================================================*/

#include <util/types.h>
#include <util/platform.h>
#include <util/config.h>
#include <win32/shared.h>

// returns handle to window state struct
PL_Window *PL_GetWindow(void)
{
    PL_Window *result = &G_win32_data->win32_window.pl_window;
    return result;
}

// set window title
bool PL_SetWindowTitle(cstr title)
{
    if(!title)
    {
        return 0;
    }

    b32 result = WINAPI.SetWindowTextA(G_win32_data->win32_window.handle, title);

    if(!result)
    {
        PL_Log(LOG_ERROR, "PL_SetWindowTitle failed to set title: %s", title);
        return 0;
    }

    PL_Log(LOG_INFO, "PL_SetWindowTitle: %s", title);
    return 1;
}

// set window screen pos (any param set to -1 will be unchanged)
void PL_SetWindowPos(i32 x, i32 y, i32 w, i32 h)
{

}

// set desired framerate (no effect if vsync is enabled)
void PL_SetWindowFramerate(i32 framerate)
{

}

// enable vsync true/false
void PL_SetWindowVSync(b32 vsync)
{

}

// set window fullscreen true or false, no change if it is already that state
void PL_SetWindowFullscreen(b32 fullscreen)
{

}

// if window is not fullscreen, fullscreen it, and vice-versa, returns new state
b32 PL_ToggleWindowFullscreen(void)
{
    return 0;
}

// OS message box popups
void PL_MsgBox(const cstr title, cstr format, ...)
{

}

void PL_MsgBoxInfo(const cstr title, cstr format, ...)
{

}

void PL_MsgBoxError(const cstr title, cstr format, ...)
{

}
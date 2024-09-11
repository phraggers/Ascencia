
/*============================================================
 * Project: Ascencia [SHARED]
 * File: window.h
 * Author: Phraggers
 * Date: Wed Sep 11 2024
============================================================*/

#ifndef ASCENCIA_WINDOW_H
#define ASCENCIA_WINDOW_H

#include <util/types.h>

typedef struct sPL_Window
{
    irect dim;
    int refresh_hz;
    bool vsync_enabled;
    bool fullscreen;
    bool has_focus;
} PL_Window;

// returns handle to window state struct
PL_Window *PL_GetWindow(void);
// set window title
bool PL_SetWindowTitle(cstr title);
// set window screen pos (any param set to -1 will be unchanged)
void PL_SetWindowPos(i32 x, i32 y, i32 w, i32 h);
// set desired framerate (no effect if vsync is enabled)
void PL_SetWindowFramerate(i32 framerate);
// enable vsync true/false
void PL_SetWindowVSync(b32 vsync);
// set window fullscreen true or false, no change if it is already that state
void PL_SetWindowFullscreen(b32 fullscreen);
// if window is not fullscreen, fullscreen it, and vice-versa, returns new state
b32 PL_ToggleWindowFullscreen(void);

// OS message box popups
void PL_MsgBox(const cstr title, cstr format, ...);
void PL_MsgBoxInfo(const cstr title, cstr format, ...);
void PL_MsgBoxError(const cstr title, cstr format, ...);

#endif /* ASCENCIA_WINDOW_H */

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
#include <window/window.h>
#include <window/input.h>

local void WIN_FullscreenToggle(void);

/*=============================
    Platform Window Functions
===============================*/

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
        PL_Log(LOG_DEBUG, "PL_SetWindowTitle: empty string");
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
    ptr handle = G_win32_data->win32_window.handle;
    irect *pl_window_dim = &G_win32_data->win32_window.pl_window.dim;

    irect current_pos = {0};
    irect new_pos = {0};

    Win32_RECT tmprect = {0};
    WINAPI.GetClientRect(handle, &tmprect);
    current_pos.w= tmprect.w;
    current_pos.h = tmprect.h;
    WINAPI.GetWindowRect(handle, &tmprect);
    current_pos.x = tmprect.x;
    current_pos.y = tmprect.y;

    PL_Log(LOG_DEBUG, "PL_SetWindowPos: Current: (x:%d, y:%d, w:%d, h:%d)", current_pos.x, current_pos.y, current_pos.w, current_pos.h);
    
    if(x == -1) new_pos.x = current_pos.x;
    else new_pos.x = x;
    if(y == -1) new_pos.y = current_pos.y;
    else new_pos.y = y;
    if(w == -1) new_pos.w = current_pos.w;
    else new_pos.w = w;
    if(h == -1) new_pos.h = current_pos.h;
    else new_pos.h = h;

    WINAPI.MoveWindow(handle, new_pos.x, new_pos.y, new_pos.w, new_pos.h, 0);

    pl_window_dim->x = new_pos.x;
    pl_window_dim->y = new_pos.y;
    pl_window_dim->w = new_pos.w;
    pl_window_dim->h = new_pos.h;

    PL_Log(LOG_INFO, "PL_SetWindowPos(x:%d, y:%d, w:%d, h:%d)", new_pos.x, new_pos.y, new_pos.w, new_pos.h);
}

// adjust window screen pos from current pos
void PL_AdjustWindowPos(i32 x, i32 y, i32 w, i32 h)
{
    ptr handle = G_win32_data->win32_window.handle;
    irect *pl_window_dim = &G_win32_data->win32_window.pl_window.dim;

    irect current_pos = {0};
    irect new_pos = {0};

    Win32_RECT tmprect = {0};
    WINAPI.GetClientRect(handle, &tmprect);
    current_pos.w= tmprect.w;
    current_pos.h = tmprect.h;
    WINAPI.GetWindowRect(handle, &tmprect);
    current_pos.x = tmprect.x;
    current_pos.y = tmprect.y;

    PL_Log(LOG_DEBUG, "PL_AdjustWindowPos: Current: (x:%d, y:%d, w:%d, h:%d)", current_pos.x, current_pos.y, current_pos.w, current_pos.h);
    
    new_pos.x += current_pos.x;
    new_pos.y += current_pos.y;
    new_pos.w += current_pos.w;
    new_pos.h += current_pos.h;

    WINAPI.MoveWindow(handle, new_pos.x, new_pos.y, new_pos.w, new_pos.h, 0);

    pl_window_dim->x = new_pos.x;
    pl_window_dim->y = new_pos.y;
    pl_window_dim->w = new_pos.w;
    pl_window_dim->h = new_pos.h;

    PL_Log(LOG_INFO, "PL_AdjustWindowPos(x:%d, y:%d, w:%d, h:%d)", new_pos.x, new_pos.y, new_pos.w, new_pos.h);
}

// set desired framerate (no effect if vsync is enabled)
void PL_SetWindowRefresh(i32 refresh_hz)
{
    PL_Log(LOG_INFO, "PL_SetWindowRefresh: prev[%d] new[%d]", G_win32_data->win32_window.pl_window.refresh_hz, refresh_hz);
    G_win32_data->win32_window.pl_window.refresh_hz = refresh_hz;
}

// enable vsync true/false
void PL_SetWindowVSync(bool vsync)
{
    bool *vsync_enabled = &G_win32_data->win32_window.pl_window.vsync_enabled;

    if(vsync)
    {
        if(*vsync_enabled)
        {
            PL_Log(LOG_DEBUG, "PL_SetWindowVSync: already enabled");
            return;
        }
        else
        {
            PL_Log(LOG_INFO, "PL_SetWindowVSync: enable");
            WINAPI.wglSwapIntervalEXT(1);
            *vsync_enabled = 1;
        }
    }

    else
    {
        if(*vsync_enabled)
        {
            PL_Log(LOG_INFO, "PL_SetWindowVSync: disable");
            WINAPI.wglSwapIntervalEXT(0);
            *vsync_enabled = 0;
        }
        else
        {
            PL_Log(LOG_DEBUG, "PL_SetWindowVSync: already disabled");
            return;
        }
    }
}

// set window fullscreen true or false, no change if it is already that state
void PL_SetWindowFullscreen(bool fullscreen)
{
    bool *pfullscreen = &G_win32_data->win32_window.pl_window.fullscreen;

    if(fullscreen)
    {
        if(*pfullscreen)
        {
            PL_Log(LOG_DEBUG, "PL_SetWindowFullscreen: already 1");
            return;
        }
        else
        {
            PL_Log(LOG_INFO, "PL_SetWindowFullscreen: set 1");
            WIN_FullscreenToggle();
        }
    }

    else
    {
        if(*pfullscreen)
        {
            PL_Log(LOG_INFO, "PL_SetWindowFullscreen: set 0");
            WIN_FullscreenToggle();
        }
        else
        {
            PL_Log(LOG_DEBUG, "PL_SetWindowFullscreen: already 0");
            return;
        }
    }
}

// if window is not fullscreen, fullscreen it, and vice-versa, returns new state
bool PL_ToggleWindowFullscreen(void)
{
    bool *pfullscreen = &G_win32_data->win32_window.pl_window.fullscreen;

    if(*pfullscreen)
    {
        PL_Log(LOG_INFO, "PL_ToggleWindowFullscreen: set 0");
        WIN_FullscreenToggle();
    }
    else
    {
        PL_Log(LOG_INFO, "PL_SetWindowFullscreen: set 1");
        WIN_FullscreenToggle();
    }

    return *pfullscreen;
}

void PL_MsgBox(const cstr title, cstr format, ...)
{
    va_list args;
    va_start(args, format);
    cstr msg = PL_Alloc0(STRING_LEN);
    if(!msg) return;
    vsnprintf(msg, STRING_LEN, format, args);
    va_end(args);
    WINAPI.MessageBoxA(0, msg, title, 0x00000010L);
    PL_Free(msg);
}

PL_Input* PL_GetInput(void)
{
    PL_Input *result = &G_win32_data->win32_window.pl_window.input;
    return result;
}

PL_Mouse* PL_GetMouse(void)
{
    PL_Mouse *result = &G_win32_data->win32_window.pl_window.input.mouse;
    return result;
}

PL_Gamepad* PL_GetGamepad(i32 id)
{
    if(id < 0 || id > GP_MAX_COUNT)
    {
        return 0;
    }

    PL_Gamepad *result = &G_win32_data->win32_window.pl_window.input.gamepads[id];
    return result;
}

PL_ButtonState* PL_GetKeyState(PL_KEYCODE key)
{
    //TODO: write this
    return 0;
}

/*==========================
    Win32 Window Functions
============================*/

// thanks Raymond Chen, the win32GOAT
local void WIN_FullscreenToggle(void)
{
    ptr handle = G_win32_data->win32_window.handle;
    Win32_WNDPL *wndpl = &G_win32_data->win32_window.wndpl;
    bool *fullscreen = &G_win32_data->win32_window.pl_window.fullscreen;
    const u32 overlapped_window = (0x00C00000L | 0x00080000L | 0x00040000L | 0x00020000L | 0x00010000L);

    u32 style = WINAPI.GetWindowLongA(handle, (-16));
    if(style & overlapped_window)
    {
        Win32_MONITORINFO mi = {sizeof(mi)};

        if(WINAPI.GetWindowPlacement(handle, wndpl) &&
           WINAPI.GetMonitorInfoA(WINAPI.MonitorFromWindow(handle, 0x00000001), &mi))
        {
            WINAPI.SetWindowLongA(handle, (-16), style & ~overlapped_window);
            WINAPI.SetWindowPos(handle, ((ptr)0), mi.monitor.x, mi.monitor.y, mi.monitor.w - mi.monitor.x, mi.monitor.h - mi.monitor.y, 0x0200 | 0x0020);
            *fullscreen = 1;
        }
    }
    else
    {
        WINAPI.SetWindowLongA(handle, (-16), style | overlapped_window);
        WINAPI.SetWindowPlacement(handle, wndpl);
        WINAPI.SetWindowPos(handle, 0, 0, 0, 0, 0, 0x0002 | 0x0001 | 0x0004 | 0x0200 | 0x0020);
        *fullscreen = 0;
    }
}
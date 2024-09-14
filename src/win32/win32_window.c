/*============================================================
 * Project: Ascencia
 * File: win32_window.c
 * Author: Phraggers
 * Date: Fri Sep 13 2024
============================================================*/

#include <util/maths.h>
#include <platform/window.h>
#include <win32/win32_state.h>
#include <win32/win32_api.h>

local void FullscreenToggle(void);

PL_Window *PL_GetWindow(void)
{
    if((!G_win32_state) || (!G_win32_state->window.handle))
    {
        PL_Log(LOG_DEBUG, "GetWindow: invalid state");
        return 0;
    }

    PL_Window *result = &G_win32_state->window.pl_window;
    return result;
}

bool PL_SetWindowTitle(cstr title)
{
    if((!G_win32_state) ||
       (!G_win32_state->window.handle) ||
       (!title) ||
       (!strlen(title)))
    {
        PL_Log(LOG_DEBUG, "SetWindowTitle: invalid state or parameters");
        return 0;
    }

    memset(PL_GetWindowTitle(), 0, STRING_LEN);
    strcpy(PL_GetWindowTitle(), title);
    return 1;
}

cstr PL_GetWindowTitle(void)
{
    if((!G_win32_state) ||
       (!G_win32_state->window.handle))
    {
        PL_Log(LOG_DEBUG, "GetWindowTitle: invalid state");
        return 0;
    }

    cstr result = &G_win32_state->window.pl_window.title[0];
    return result;
}

void PL_SetWindowPos(i32 x, i32 y, i32 w, i32 h)
{
    ptr handle = G_win32_state->window.handle;
    irect *dim = &G_win32_state->window.pl_window.config->dim;

    irect current_pos = {0};
    irect new_pos = {0};

    Win32_RECT tmprect = {0};
    WINAPI.GetClientRect(handle, &tmprect);
    current_pos.w= tmprect.w;
    current_pos.h = tmprect.h;
    WINAPI.GetWindowRect(handle, &tmprect);
    current_pos.x = tmprect.x;
    current_pos.y = tmprect.y;

    PL_Log(LOG_DEBUG, "SetWindowPos: Current: (x:%d, y:%d, w:%d, h:%d)", current_pos.x, current_pos.y, current_pos.w, current_pos.h);
    
    if(x == -1) new_pos.x = current_pos.x;
    else new_pos.x = x;
    if(y == -1) new_pos.y = current_pos.y;
    else new_pos.y = y;
    if(w == -1) new_pos.w = current_pos.w;
    else new_pos.w = w;
    if(h == -1) new_pos.h = current_pos.h;
    else new_pos.h = h;

    WINAPI.MoveWindow(handle, new_pos.x, new_pos.y, new_pos.w, new_pos.h, 0);

    dim->x = new_pos.x;
    dim->y = new_pos.y;
    dim->w = new_pos.w;
    dim->h = new_pos.h;

    PL_Log(LOG_INFO, "SetWindowPos(x:%d, y:%d, w:%d, h:%d)", new_pos.x, new_pos.y, new_pos.w, new_pos.h);   
}

void PL_AdjustWindowPos(i32 x, i32 y, i32 w, i32 h)
{
    irect *dim = &G_win32_state->window.pl_window.config->dim;
    irect new_pos = {0};
    new_pos.x = clamp_min_i32(dim->x + x, 0);
    new_pos.y = clamp_min_i32(dim->y + y, 0);
    new_pos.w = clamp_min_i32(dim->w + w, 0);
    new_pos.h = clamp_min_i32(dim->h + h, 0);

    PL_SetWindowPos(new_pos.x, new_pos.y, new_pos.w, new_pos.h);
}

void PL_SetWindowRefresh(i32 refresh_hz)
{
    PL_Log(LOG_INFO, "SetWindowRefresh: prev[%d] new[%d]", G_win32_state->window.pl_window.config->refresh_hz, refresh_hz);
    G_win32_state->window.pl_window.config->refresh_hz = refresh_hz;
}

void PL_SetWindowVSync(bool vsync)
{
    bool *pvsync = &G_win32_state->window.pl_window.config->vsync;

    if(vsync)
    {
        if(*pvsync)
        {
            PL_Log(LOG_DEBUG, "SetWindowVSync: already enabled");
            return;
        }
        else
        {
            PL_Log(LOG_INFO, "SetWindowVSync: enabled");
            WGL.wglSwapIntervalEXT(1);
            *pvsync = 1;
        }
    }

    else
    {
        if(*pvsync)
        {
            PL_Log(LOG_INFO, "SetWindowVSync: disabled");
            WGL.wglSwapIntervalEXT(0);
            *pvsync = 0;
        }
        else
        {
            PL_Log(LOG_DEBUG, "SetWindowVSync: already disabled");
            return;
        }
    }
}

void PL_SetWindowFullscreen(bool fullscreen)
{
    bool *pfullscreen = &G_win32_state->window.pl_window.config->fullscreen;

    if(fullscreen)
    {
        if(*pfullscreen)
        {
            PL_Log(LOG_DEBUG, "SetWindowFullscreen: already 1");
            return;
        }
        else
        {
            PL_Log(LOG_INFO, "SetWindowFullscreen: set 1");
            FullscreenToggle();
        }
    }

    else
    {
        if(*pfullscreen)
        {
            PL_Log(LOG_INFO, "SetWindowFullscreen: set 0");
            FullscreenToggle();
        }
        else
        {
            PL_Log(LOG_DEBUG, "SetWindowFullscreen: already 0");
            return;
        }
    }
}

bool PL_ToggleWindowFullscreen(void)
{
    bool *pfullscreen = &G_win32_state->window.pl_window.config->fullscreen;

    if(*pfullscreen)
    {
        PL_Log(LOG_INFO, "ToggleWindowFullscreen: set 0");
        FullscreenToggle();
    }
    else
    {
        PL_Log(LOG_INFO, "SetWindowFullscreen: set 1");
        FullscreenToggle();
    }

    return *pfullscreen;
}

/*===========================
    Win32 Functions
============================*/

// thanks Raymond Chen, the win32GOAT
local void FullscreenToggle(void)
{
    ptr handle = G_win32_state->window.handle;
    Win32_WNDPL *wndpl = &G_win32_state->window.wndpl;
    bool *fullscreen = &G_win32_state->window.pl_window.config->fullscreen;
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
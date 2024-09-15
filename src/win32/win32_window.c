/*============================================================
 * Project: Ascencia
 * File: win32_window.c
 * Author: Phraggers
 * Date: Fri Sep 13 2024
============================================================*/

#include <util/maths.h>
#include <platform/window.h>
#include <platform/opengl.h>
#include <platform/input.h>
#include <platform/config.h>
#include <win32/win32_state.h>
#include <win32/win32_api.h>

local void FullscreenToggle(void);

PL_Window *PL_GetWindow(void)
{
    if(!G_win32_state)
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
            WGLAPI.wglSwapIntervalEXT(1);
            *pvsync = 1;
        }
    }

    else
    {
        if(*pvsync)
        {
            PL_Log(LOG_INFO, "SetWindowVSync: disabled");
            WGLAPI.wglSwapIntervalEXT(0);
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

local void FullscreenToggle(void);

i64 Win32_WndProc(ptr handle, u32 msg, u64 wparam, i64 lparam)
{
    if(!G_win32_state->running)
    {
        return WINAPI.DefWindowProcA(handle, msg, wparam, lparam);
    }

    Win32_Window *win32_window = &G_win32_state->window;
    PL_Window *pl_window = &win32_window->pl_window;

    switch(msg)
    {
        case 0x0012: //WM_QUIT
        case 0x0010: //WM_CLOSE
        case 0x0002: //WM_DESTROY
        {
            G_win32_state->running = 0;
        } break;

        case 0x001C: //WM_ACTIVATEAPP
        {
            if(wparam == 1)
            {
                pl_window->has_focus = 1;
            }
            else
            {
                pl_window->has_focus = 0;
            }
        } break;

        case 0x0003: //WM_MOVE
        {
            pl_window->config->dim.x = (int)((u32)(((u64)(lparam)) & 0xffff));
            pl_window->config->dim.y = (int)((u32)((((u64)(lparam)) >> 16) & 0xffff));
        } break;

        case 0x0005: //WM_SIZE
        {
            pl_window->config->dim.w = (int)((u32)(((u64)(lparam)) & 0xffff));
            pl_window->config->dim.h = (int)((u32)((((u64)(lparam)) >> 16) & 0xffff));
            if(win32_window->glrc && glViewport)
            {
                glViewport(0, 0, ((u32)(((u64)(lparam)) & 0xffff)), ((u32)((((u64)(lparam)) >> 16) & 0xffff)));
            }
        } break;

        case 0x0100: //WM_KEYDOWN
        case 0x0104: //WM_SYSKEYDOWN
        {
            if((lparam & (1 << 29))) //ALT key
            {
                if(lparam & 1 << 24) //RALT
                {
                    PL_GetKeyState(KEY_RALT)->is_down = 1;
                }
                else //LALT
                {
                    PL_GetKeyState(KEY_LALT)->is_down = 1;
                }
            }
            
            PL_KEYCODE key = Win32_Keycode((u32)wparam, lparam);
            PL_GetKeyState(key)->is_down = 1;
        } break;

        case 0x0101: //WM_KEYUP
        case 0x0105: //WM_SYSKEYUP
        {
            //TODO: fix this. currently, if holding ALT, when another keyup
            // happens, the game will treat this as ALT key up followed by
            // another ALT keydown next frame, rather than ALT staying down.

            if(PL_GetKeyState(KEY_RALT)->is_down)
            {
                PL_GetKeyState(KEY_RALT)->is_down = 0;
            }

            if(PL_GetKeyState(KEY_LALT)->is_down)
            {
                PL_GetKeyState(KEY_LALT)->is_down = 0;
            }

            PL_KEYCODE key = Win32_Keycode((u32)wparam, lparam);
            PL_GetKeyState(key)->is_down = 0;
        } break;

        case 0x200: //WM_MOUSEMOVE
        {
            Win32_POINT cursor = {0};
            WINAPI.GetCursorPos(&cursor);
            WINAPI.ScreenToClient(win32_window->handle, &cursor);
            PL_GetMouse()->px = cursor.x;
            PL_GetMouse()->py = cursor.y;
            PL_GetMouse()->rx = (norm32((r32)cursor.x, 0.0f, (r32)pl_window->config->dim.w) * 2.0f) - 1.0f;
            PL_GetMouse()->ry = (norm32((r32)cursor.y, 0.0f, (r32)pl_window->config->dim.h) * 2.0f) - 1.0f;
        } break;

        case 0x201: //WM_LBUTTONDOWN
        {
            PL_GetMouse()->buttons[MB_LEFT].is_down = 1;
        } break;

        case 0x207: //WM_MBUTTONDOWN
        {
            PL_GetMouse()->buttons[MB_MIDDLE].is_down = 1;
        } break;

        case 0x204: //WM_RBUTTONDOWN
        {
            PL_GetMouse()->buttons[MB_RIGHT].is_down = 1;
        } break;

        case 0x20B: //WM_XBUTTONDOWN
        {
            if(wparam & 0x0020) //MK_XBUTTON1
            {
                PL_GetMouse()->buttons[MB_X1].is_down = 1;
            }
            
            if(wparam & 0x0040) //MK_XBUTTON2
            {
                PL_GetMouse()->buttons[MB_X2].is_down = 1;
            }
        } break;

        case 0x202: //WM_LBUTTONUP
        {
            PL_GetMouse()->buttons[MB_LEFT].is_down = 0;
        } break;

        case 0x208: //WM_MBUTTONUP
        {
            PL_GetMouse()->buttons[MB_MIDDLE].is_down = 0;
        } break;

        case 0x205: //WM_RBUTTONUP
        {
            PL_GetMouse()->buttons[MB_RIGHT].is_down = 0;
        } break;

        case 0x20c: //WM_XBUTTONUP
        {
            if(wparam & 0x0020) //MK_XBUTTON1
            {
                PL_GetMouse()->buttons[MB_X1].is_down = 0;
            }
            
            if(wparam & 0x0040) //MK_XBUTTON2
            {
                PL_GetMouse()->buttons[MB_X2].is_down = 0;
            }
        } break;

        case 0x020a:
        {
            if(((short)((u32)((((u64)(wparam)) >> 16) & 0xffff))) < 0)
            {
                PL_GetMouse()->wheel--;
            }
            
            if(((short)((u32)((((u64)(wparam)) >> 16) & 0xffff))) > 0)
            {
                PL_GetMouse()->wheel++;
            }
            
        } break;

        default:
        {
            return WINAPI.DefWindowProcA(handle, msg, wparam, lparam);
        } break;
    }

    return 0;
}

// thanks Raymond Chen, the win32GOAT
local void FullscreenToggle(void)
{
    ptr handle = G_win32_state->window.handle;
    Win32_WNDPL *wndpl = &G_win32_state->window.wndpl;
    bool *fullscreen = &G_win32_state->window.pl_window.config->fullscreen;

    u32 style = WINAPI.GetWindowLongA(handle, (-16));
    if(style & Win32_WS_OVERLAPPED_WINDOW)
    {
        Win32_MONITORINFO mi = {sizeof(mi)};

        if(WINAPI.GetWindowPlacement(handle, wndpl) &&
           WINAPI.GetMonitorInfoA(WINAPI.MonitorFromWindow(handle, 0x00000001), &mi))
        {
            WINAPI.SetWindowLongA(handle, (-16), style & ~Win32_WS_OVERLAPPED_WINDOW);
            WINAPI.SetWindowPos(handle, ((ptr)0), mi.monitor.x, mi.monitor.y, mi.monitor.w - mi.monitor.x, mi.monitor.h - mi.monitor.y, 0x0200 | 0x0020);
            *fullscreen = 1;
        }
    }
    else
    {
        WINAPI.SetWindowLongA(handle, (-16), style | Win32_WS_OVERLAPPED_WINDOW);
        WINAPI.SetWindowPlacement(handle, wndpl);
        WINAPI.SetWindowPos(handle, 0, 0, 0, 0, 0, 0x0002 | 0x0001 | 0x0004 | 0x0200 | 0x0020);
        *fullscreen = 0;
    }
}

void Win32_UpdateWindow(void)
{
    Win32_RECT dim = {0};
    WINAPI.GetWindowRect(G_win32_state->window.handle, &dim);
    PL_GetWindow()->config->dim.x = dim.x;
    PL_GetWindow()->config->dim.y = dim.y;
    WINAPI.GetClientRect(G_win32_state->window.handle, &dim);
    PL_GetWindow()->config->dim.w = dim.w;
    PL_GetWindow()->config->dim.h = dim.h;

    if(G_win32_state->window.glrc && glViewport && WINAPI.SwapBuffers)
    {
        glViewport(0, 0, PL_GetWindow()->config->dim.w, PL_GetWindow()->config->dim.h);
        WINAPI.SwapBuffers(G_win32_state->window.hdc);
        glClearColor(0,0,0,0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
}

void Win32_MessageLoop(void)
{
    Win32_MSG msg;
    while(WINAPI.PeekMessageA(&msg, G_win32_state->window.handle, 0, 0, 1))
    {
        WINAPI.TranslateMessage(&msg);
        WINAPI.DispatchMessageA(&msg);
    }
    
    r64 dt = PL_TimerElapsed(PL_GetTimer()->last_perf);
    Win32_ProcessInput(dt);
}

bool Win32_CreateWindow(void)
{
    Win32_Window *window = &G_win32_state->window;
    Win32_WNDCLASSA *wndclass = &window->wndclass;
    PL_GetWindow()->config = &G_win32_state->config.window_config;
    
    wndclass->style = 0x0002 | 0x0001 | 0x0020;
    wndclass->wndproc = Win32_WndProc;
    wndclass->instance = WINAPI.GetModuleHandleA(0);
    wndclass->class_name = "PLWINDOWCLASS";
    
    if(!WINAPI.RegisterClassA(wndclass))
    {
        PL_Log(LOG_ERROR, "CreateWindow: Error registering WNDCLASS. Win32 ErrorCode(%u)", WINAPI.GetLastError());
        return 0;
    }
    
    // temp window to get OpenGL context settings
    ptr tmp_window = WINAPI.CreateWindowExA(0L, wndclass->class_name, "tmp_window", Win32_WS_OVERLAPPED_WINDOW, 0, 0, 1, 1, 0, 0, wndclass->instance, 0);
    
    if(!tmp_window)
    {
        PL_Log(LOG_ERROR, "CreateWindow: Failed to create Window. Win32 ErrorCode(%u)", WINAPI.GetLastError());
        return 0;
    }
    
    ptr tmp_dc = WINAPI.GetDC(tmp_window);
    ptr tmp_glrc = 0;
    int tmp_pixel_format = 0;
    
    { // tmp_window get pixel format
        Win32_PFD pfd = {0};
        pfd.ssize = sizeof(Win32_PFD);
        pfd.version = 1;
        pfd.flags = 0x00000004 | 0x00000020;
        pfd.pixel_type = 0;
        pfd.color_bits = 32;
        pfd.alpha_bits = 8;
        pfd.depth_bits = 24;
        tmp_pixel_format = WINAPI.ChoosePixelFormat(tmp_dc, &pfd);
        
        if(tmp_pixel_format)
        {
            if(WINAPI.SetPixelFormat(tmp_dc, tmp_pixel_format, &pfd))
            {
                tmp_glrc = WINAPI.wglCreateContext(tmp_dc);
            }
        }
        
        else
        {
            PL_Log(LOG_ERROR, "CreateWindow: Failed to get pixel format. Win32 ErrorCode(%u)", WINAPI.GetLastError());
            return 0;
        }
        
        if(!tmp_glrc)
        {
            PL_Log(LOG_ERROR, "CreateWindow: Failed to get WGL RenderContext. Win32 ErrorCode(%u)", WINAPI.GetLastError());
            return 0;
        }
    }
    
    window->style = 0x10000000L | Win32_WS_OVERLAPPED_WINDOW;
    window->handle = WINAPI.CreateWindowExA(0, wndclass->class_name, "Window", window->style,
                                           ((int)0x80000000), ((int)0x80000000),
                                           PL_GetWindow()->config->dim.w, PL_GetWindow()->config->dim.h, 0, 0,
                                           wndclass->instance, 0);
    
    if(!window->handle)
    {
        PL_Log(LOG_ERROR, "CreateWindow: Failed to create Window. Win32 ErrorCode(%u)", WINAPI.GetLastError());
        return 0;
    }
    
    window->wndpl.length = sizeof(Win32_WNDPL);
    WINAPI.GetWindowPlacement(window->handle, &window->wndpl);
    window->hdc = WINAPI.GetDC(window->handle);
    
    if(!WINAPI.wglMakeCurrent(tmp_dc, tmp_glrc))
    {
        PL_Log(LOG_ERROR, "CreateWindow: Failed to set OpenGL RenderContext. Win32 ErrorCode(%u)", WINAPI.GetLastError());
        return 0;
    }
    
    if(!Win32_LoadWGL())
    {
        PL_Log(LOG_ERROR, "CreateWindow: Failed to load WGL functions");
        return 0;
    }
    
    int pixel_format;
    b32 status;
    u32 num_formats;
    
    const int pixel_attribs[] =
    {
        WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
        WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
        WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
        WGL_SWAP_METHOD_ARB, WGL_SWAP_COPY_ARB,
        WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
        WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
        WGL_COLOR_BITS_ARB, 32,
        WGL_ALPHA_BITS_ARB, 8,
        WGL_DEPTH_BITS_ARB, 24,
        0
    };
    
    int context_attribs[] =
    {
        WGL_CONTEXT_MAJOR_VERSION_ARB, ASC_OPENGL_MAJ,
        WGL_CONTEXT_MINOR_VERSION_ARB, ASC_OPENGL_MIN,
        WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
        0
    };
    
    status = WGLAPI.wglChoosePixelFormatARB(window->hdc, pixel_attribs, 0, 1,
                                             &pixel_format, &num_formats);
    
    if(!status || !num_formats)
    {
        PL_Log(LOG_ERROR, "CreateWindow: Failed to get Windows OpenGL PixelFormat");
        return 0;
    }
    
    Win32_PFD pfd = {0};
    WINAPI.DescribePixelFormat(window->hdc, pixel_format, sizeof(Win32_PFD), &pfd);
    if(!WINAPI.SetPixelFormat(window->hdc, pixel_format, &pfd))
    {
        PL_Log(LOG_ERROR, "CreateWindow: Failed to set Windows OpenGL PixelFormat");
        return 0;
    }
    
    window->glrc = WGLAPI.wglCreateContextAttribsARB(window->hdc, 0, context_attribs);
    if(!window->glrc)
    {
        PL_Log(LOG_ERROR, "CreateWindow: Failed to create Windows OpenGL RenderContext. Win32 ErrorCode(%u)", WINAPI.GetLastError());
        return 0;
    }
    
    // unset and clear up tmp
    WINAPI.wglMakeCurrent(tmp_dc, 0);
    WINAPI.wglDeleteContext(tmp_glrc);
    WINAPI.ReleaseDC(tmp_window, tmp_dc);
    WINAPI.DestroyWindow(tmp_window);
    
    if(!WINAPI.wglMakeCurrent(window->hdc, window->glrc))
    {
        PL_Log(LOG_ERROR, "CreateWindow: Failed to set Windows OpenGL RenderContext. Win32 ErrorCode(%u)", WINAPI.GetLastError());
        return 0;
    }
    
    if(!Win32_LoadGL())
    {
        PL_Log(LOG_ERROR, "CreateWindow: Failed to load OpenGL functions");
        return 0;
    }
    
    PL_SetWindowVSync(PL_GetWindow()->config->vsync);
    Win32_UpdateWindow();
    
    return 1;
}
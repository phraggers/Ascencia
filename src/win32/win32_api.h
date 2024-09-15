/*============================================================
 * Project: Ascencia
 * File: win32_api.h
 * Author: Phraggers
 * Date: Thu Sep 12 2024
============================================================*/

#ifndef ASCENCIA_WIN32_API_H
#define ASCENCIA_WIN32_API_H

#include <util/types.h>

typedef union 
{ 
    struct {u32 LowPart; i32 HighPart;} s; 
    struct {u32 LowPart; i32 HighPart;} u; 
    i64 QuadPart;
} Win32_LARGE_INTEGER;

typedef struct
{
    int x,y;
} Win32_POINT;

typedef struct 
{
    ptr hwnd;
    u32 message;
    u64 wParam;
    i64 lParam;
    u32 time;
    Win32_POINT point;
    u32 unused;
} Win32_MSG;

typedef i64 (*pfn_WNDPROC)(ptr hwnd, u32 msg, u64 wparam, i64 lparam);

typedef struct 
{
    u32 style;
    pfn_WNDPROC wndproc;
    int class_extra_bytes;
    int wnd_extra_bytes;
    ptr instance;
    ptr icon;
    ptr cursor;
    ptr background;
    cstr menu_name;
    cstr class_name;
} Win32_WNDCLASSA;

typedef struct 
{
    int x;
    int y;
    int w;
    int h;
} Win32_RECT;

typedef struct
{
    u32 length;
    u32 flags;
    u32 showCmd;
    Win32_POINT min_pos;
    Win32_POINT max_pos;
    Win32_RECT  normal_pos;
    Win32_RECT  device;
} Win32_WNDPL;

typedef struct 
{
    u32 ssize;
    Win32_RECT monitor;
    Win32_RECT workspace;
    u32 flags;
} Win32_MONITORINFO;

typedef struct 
{
    u16 ssize;
    u16 version;
    u32 flags;
    u8 pixel_type;
    u8 color_bits;
    u8 red_bits;
    u8 red_shift;
    u8 green_bits;
    u8 green_shift;
    u8 blue_bits;
    u8 blue_shift;
    u8 alpha_bits;
    u8 alpha_shift;
    u8 accum_bits;
    u8 accum_red_bits;
    u8 accum_green_bits;
    u8 accum_blue_bits;
    u8 accum_alpha_bits;
    u8 depth_bits;
    u8 stencil_bits;
    u8 num_aux_buffers;
    u8 unused1;
    u8 unused2;
    u32 unused3;
    u32 visible_mask;
    u32 unused4;
} Win32_PFD;

typedef struct 
{
    u16 year;
    u16 month;
    u16 wday;
    u16 day;
    u16 hour;
    u16 min;
    u16 sec;
    u16 ms;
} Win32_SYSTEMTIME;

#define Win32_WS_OVERLAPPED_WINDOW (0x00000000L | 0x00C00000L | 0x00080000L | 0x00040000L | 0x00020000L | 0x00010000L)

 /* kernel32.dll */
 typedef ptr (*pfn_GetModuleHandleA)(const cstr module_name);
 typedef cstr (*pfn_GetCommandLineA)(void);
 typedef ptr (*pfn_LoadLibraryA)(const cstr lib_name);
 typedef ptr (*pfn_GetProcAddress)(ptr module, const cstr proc_name);
 typedef u32 (*pfn_GetModuleFileNameA)(ptr module, cstr filename, u32 size);
 typedef ptr (*pfn_AddDllDirectory)(cstr path);
 typedef u32 (*pfn_GetLastError)(void);
 typedef ptr (*pfn_CreateFileA)(cstr path, u32 access, u32 smode, ptr sec, u32 creation, u32 flags, ptr ftemplate);
 typedef b32 (*pfn_CloseHandle)(ptr handle);
 typedef b32 (*pfn_GetFileSizeEx)(ptr handle, Win32_LARGE_INTEGER *size);
 typedef b32 (*pfn_ReadFile)(ptr handle, ptr buffer, u32 bytes_to_read, u32 *bytes_read, ptr overlapped);
 typedef b32 (*pfn_WriteFile)(ptr handle, const ptr buffer, u32 bytes_to_write, u32 *bytes_written, ptr overlapped);
 typedef b32 (*pfn_DeleteFileA)(cstr path);
 typedef b32 (*pfn_CreateDirectoryA)(cstr path, ptr sec);
 typedef b32 (*pfn_RemoveDirectoryA)(cstr path);
 typedef b32 (*pfn_QueryPerformanceFrequency)(Win32_LARGE_INTEGER *frequency);
 typedef b32 (*pfn_QueryPerformanceCounter)(Win32_LARGE_INTEGER *frequency);
 typedef void (*pfn_GetLocalTime)(Win32_SYSTEMTIME *st);

 /* shlwapi.dll */
 typedef b32 (*pfn_PathFileExistsA)(cstr path);

 /* user32.dll */
typedef b32 (*pfn_TranslateMessage)(const Win32_MSG *msg);
typedef i64 (*pfn_DispatchMessageA)(const Win32_MSG *msg);
typedef b32 (*pfn_PeekMessageA)(Win32_MSG *msg, ptr hwnd, u32 filter_min, u32 filter_max, u32 remove_flag);
typedef i64 (*pfn_DefWindowProcA)(ptr hwnd, u32 msg, u64 wparam, i64 lparam);
typedef u16 (*pfn_RegisterClassA)(const Win32_WNDCLASSA *wndclass);
typedef ptr (*pfn_CreateWindowExA)(u32 ex_style, const cstr class_name, const cstr wnd_name, u32 style, int x, int y, int w, int h, ptr parent, ptr menu, ptr instance, ptr param);
typedef b32 (*pfn_DestroyWindow)(ptr hwnd);
typedef b32 (*pfn_MoveWindow)(ptr hwnd, int x, int y, int w, int h, b32 repaint);
typedef b32 (*pfn_SetWindowPos)(ptr hwnd, ptr zpos, int x, int y, int w, int h, u32 flags);
typedef b32 (*pfn_GetWindowPlacement)(ptr hwnd, Win32_WNDPL *wndpl);
typedef b32 (*pfn_SetWindowPlacement)(ptr hwnd, const Win32_WNDPL *wndpl);
typedef ptr (*pfn_GetDC)(ptr hwnd);
typedef int (*pfn_ReleaseDC)(ptr hwnd, ptr dc);
typedef b32 (*pfn_SetWindowTextA)(ptr hwnd, cstr text);
typedef int (*pfn_MessageBoxA)(ptr hwnd, const cstr text, const cstr caption, u32 type);
typedef b32 (*pfn_GetCursorPos)(Win32_POINT *point);
typedef b32 (*pfn_ScreenToClient)(ptr hwnd, Win32_POINT *point);
typedef i32 (*pfn_GetWindowLongA)(ptr hwnd, int index);
typedef i32 (*pfn_SetWindowLongA)(ptr hwnd, int index, i64 value);
typedef ptr (*pfn_MonitorFromWindow)(ptr hwnd, u32 flags);
typedef b32 (*pfn_GetMonitorInfoA)(ptr monitor, Win32_MONITORINFO *mi);
typedef b32 (*pfn_GetClientRect)(ptr hwnd, Win32_RECT *rect);
typedef b32 (*pfn_GetWindowRect)(ptr hwnd, Win32_RECT *rect);

 /* gdi32.dll */
 typedef int (*pfn_ChoosePixelFormat)(ptr hdc, const Win32_PFD *ppfd);
 typedef int (*pfn_DescribePixelFormat)(ptr hdc, int format, u32 bytes, Win32_PFD *ppfd);
 typedef b32 (*pfn_SetPixelFormat)(ptr hdc, int format, const Win32_PFD *ppfd);
 typedef b32 (*pfn_SwapBuffers)(ptr hdc);

 /* opengl32.dll */
 typedef ptr (*pfn_wglCreateContext)(ptr hdc);
 typedef b32 (*pfn_wglDeleteContext)(ptr hglrc);
 typedef i64 (*pfn_wglGetProcAddress)(const cstr fname);
 typedef b32 (*pfn_wglMakeCurrent)(ptr hdc, ptr hglrc);

typedef struct
{
    /* kernel32.dll */
    pfn_GetModuleHandleA GetModuleHandleA;
    pfn_GetCommandLineA GetCommandLineA;
    pfn_LoadLibraryA LoadLibraryA;
    pfn_GetProcAddress GetProcAddress;
    pfn_GetModuleFileNameA GetModuleFileNameA;
    pfn_AddDllDirectory AddDllDirectory;
    pfn_GetLastError GetLastError;
    pfn_CreateFileA CreateFileA;
    pfn_CloseHandle CloseHandle;
    pfn_GetFileSizeEx GetFileSizeEx;
    pfn_ReadFile ReadFile;
    pfn_WriteFile WriteFile;
    pfn_DeleteFileA DeleteFileA;
    pfn_CreateDirectoryA CreateDirectoryA;
    pfn_RemoveDirectoryA RemoveDirectoryA;
    pfn_QueryPerformanceFrequency QueryPerformanceFrequency;
    pfn_QueryPerformanceCounter QueryPerformanceCounter;
    pfn_GetLocalTime GetLocalTime;

    /* shlwapi.dll */
    pfn_PathFileExistsA PathFileExistsA;

    /* user32.dll */
    pfn_TranslateMessage TranslateMessage;
    pfn_DispatchMessageA DispatchMessageA;
    pfn_PeekMessageA PeekMessageA;
    pfn_DefWindowProcA DefWindowProcA;
    pfn_RegisterClassA RegisterClassA;
    pfn_CreateWindowExA CreateWindowExA;
    pfn_DestroyWindow DestroyWindow;
    pfn_MoveWindow MoveWindow;
    pfn_SetWindowPos SetWindowPos;
    pfn_GetWindowPlacement GetWindowPlacement;
    pfn_SetWindowPlacement SetWindowPlacement;
    pfn_GetDC GetDC;
    pfn_ReleaseDC ReleaseDC;
    pfn_SetWindowTextA SetWindowTextA;
    pfn_MessageBoxA MessageBoxA;
    pfn_GetCursorPos GetCursorPos;
    pfn_ScreenToClient ScreenToClient;
    pfn_GetWindowLongA GetWindowLongA;
    pfn_SetWindowLongA SetWindowLongA;
    pfn_MonitorFromWindow MonitorFromWindow;
    pfn_GetMonitorInfoA GetMonitorInfoA;
    pfn_GetClientRect GetClientRect;
    pfn_GetWindowRect GetWindowRect;
    
    /* gdi32.dll */
    pfn_ChoosePixelFormat ChoosePixelFormat;
    pfn_DescribePixelFormat DescribePixelFormat;
    pfn_SetPixelFormat SetPixelFormat;
    pfn_SwapBuffers SwapBuffers;
    
    /* opengl32.dll */
    pfn_wglCreateContext wglCreateContext;
    pfn_wglDeleteContext wglDeleteContext;
    pfn_wglGetProcAddress wglGetProcAddress;
    pfn_wglMakeCurrent wglMakeCurrent;
    
} Win32_API;

#define WINAPI G_win32_state->api

bool Win32_LoadAPI(void);
u64 Win32_GetPerfCount(void);

/*===== XINPUT  =====*/

typedef struct
{
    u16 buttons;
    u8 lt;
    u8 rt;
    i16 lx;
    i16 ly;
    i16 rx;
    i16 ry;
} Win32_XINPUT_GAMEPAD;

typedef struct
{
    u32 packet_number;
    Win32_XINPUT_GAMEPAD gamepad;
} Win32_XINPUT_STATE;

typedef struct
{
    u16 left;
    u16 right;
} Win32_XINPUT_VIBRATION;

typedef enum
{
    XINPUT_NOT_LOADED = 0,
    XINPUT_1_4,
    XINPUT_9_1_0,
    XINPUT_1_3
} Win32_XINPUT_VERSION;

typedef u32 (*pfn_XInputGetState)(u32 user_index, Win32_XINPUT_STATE *state);
typedef u32 (*pfn_XInputSetState)(u32 user_index, Win32_XINPUT_VIBRATION *vib);

typedef struct
{
    Win32_XINPUT_VERSION version;
    pfn_XInputGetState XInputGetState;
    pfn_XInputSetState XInputSetState;
} Win32_XInput;

#define XINPUT_API G_win32_state->xinput

bool Win32_LoadXInput(void);

#endif /* ASCENCIA_WIN32_API_H */
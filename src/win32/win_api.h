
/*============================================================
 * Project: Ascencia [EXE]
 * File: win_api.h
 * Author: Phraggers
 * Date: Mon Sep 09 2024
============================================================*/

#ifndef ASCENCIA_WIN32_WIN_API_H
#define ASCENCIA_WIN32_WIN_API_H

#include <win32/shared.h>

/* WINDOWS TYPES -> MY TYPES
ATOM        u16
BOOL        i32
BOOLEAN     u8
BYTE        u8
CCHAR       char
CHAR        char
COLORREF    u32
DWORD       u32
DWORDLONG   u64
DWORD_PTR   u64
DWORD32     u32
DWORD64     u64
FLOAT       r32
HACCEL      ptr
HALF_PTR    i32
HANDLE      ptr
HBITMAP     ptr
HBRUSH      ptr
HCOLORSPACE ptr
HCONV       ptr
HCONVLIST   ptr
HCURSOR     ptr
HDC         ptr
HDDEDATA    ptr
HDESK       ptr
HDROP       ptr
HDWP        ptr
HENHMETAFILE ptr
HFILE       i32
HFONT       ptr
HGDIOBJ     ptr
HGLOBAL     ptr
HHOOK       ptr
HICON       ptr
HINSTANCE   ptr
HKEY        ptr
HKL         ptr
HLOCAL      ptr
HMENU       ptr
HMETAFILE   ptr
HMODULE     ptr
HMONITOR    ptr
HPALETTE    ptr
HPEN        ptr
HRESULT     i32
HRGN        ptr
HRSRC       ptr
HSZ         ptr
HWINSTA     ptr
HWND        ptr
INT         i32
INT_PTR     i64
INT8        i8
INT16       i16
INT32       i32
INT64       i64
LANGID      u16
LCID        u32
LCTYPE      u32
LGRPID      u32
LONG        i32
LONGLONG    i64
LONG_PTR    i64
LONG32      i32
LONG64      i64
LPARAM      i64
LPBOOL      i32*
LPBYTE      u8*
LPCOLORREF  u32*
LPCSTR      const cstr
LPCTSTR     if(utf) {const wcstr} else {const cstr}
LPCVOID     const ptr
LPCWSTR     const wcstr
LPDWORD     u32*
LPHANDLE    ptr*
LPINT       i32*
LPLONG      int32*
LPSTR       cstr
LPTSTR      if(utf) {wcstr} else {cstr}
LPVOID      ptr
LPWORD      u16*
LPWSTR      wcstr
LRESULT     i64
PBOOL       i32*
PBOOLEAN    u8*
PBYTE       u8*
PCHAR       char*
PCSTR       const cstr
PCTSTR      if(utf) {const wcstr} else {const cstr}
PCWSTR      const wcstr
PDWORD      u32*
PDWORDLONG  u64*
PDWORD_PTR  u64*
PDWORD32    u32*
PDWORD64    u64*
PFLOAT      r32*
PHALF_PTR   i32*
PHANDLE     ptr*
PHKEY       ptr*
PINT        i32*
PINT_PTR    i32**
PINT8       i8*
PINT16      i16*
PINT32      i32*
PINT64      i64*
PLCID       u32*
PLONG       i32*
PLONGLONG   i64*
PLONG_PTR   i32**
PLONG32     i32*
PLONG64     i64*
POINTER_32  i32*
POINTER_64  i64*
POINTER_SIGNED i32*
POINTER_UNSIGNED ptr
PSHORT      i16*
PSIZE_T     u64*
PSSIZE_T    i64*
PSTR        cstr
PTBYTE      if(utf) {u16*} else {u8*}
PTCHAR      if(utf) {u16*} else {char*}
PTSTR       if(utf) {const wcstr} else {const cstr}
PUCHAR      u8*
PUHALF_PTR  u32*
PUINT       u32*
PUINT_PTR   u32**
PUINT8      u8*
PUINT16     u16*
PUINT32     u32*
PUINT64     u64*
PULONG      u32*
PULONGLONG  u64*
PULONG_PTR  u32**
PULONG32    u32*
PULONG64    u64*
PUSHORT     u16*
PVOID       ptr
PWCHAR      wcstr
PWORD       u16*
PWSTR       wcstr
QWORD       u64
SC_HANDLE   ptr
SC_LOCK     ptr
SERVICE_STATUS_HANDLE ptr
SHORT       i16
SIZE_T      u64
SSIZE_T     i64
TBYTE       if(utf) {u16} else {u8}
TCHAR       if(utf) {u16} else {char}
UCHAR       u8
UHALF_PTR   u32
UINT        u32
UINT_PTR    u64
UINT8       u8
UINT16      u16
UINT32      u32
UINT64      u64
ULONG       u32
ULONGLONG   u64
ULONG_PTR   u64
ULONG32     u32
ULONG64     u64

UNICODE_STRING 
typedef struct sUnicodeStr 
{
    u16  length;
    u16  max_length;
    wcstr  buffer;
} UnicodeStr;
typedef UnicodeStr *PUNICODE_STRING;
typedef const UnicodeStr *PCUNICODE_STRING;

USHORT      u16
USN         i64
VOID        void
WCHAR       u16
WORD        u16
WPARAM      u64
*/

typedef union sWin32_LARGE_INTEGER 
{ 
    struct {u32 LowPart; i32 HighPart;} s; 
    struct {u32 LowPart; i32 HighPart;} u; 
    i64 QuadPart;
} Win32_LARGE_INTEGER;

typedef struct sWin32_POINT
{
    int x,y;
} Win32_POINT;

typedef struct sWin32_MSG 
{
  ptr hwnd;
  u32 message;
  u64 wParam;
  i64 lParam;
  u32 time;
  Win32_POINT point;
  u32 private;
} Win32_MSG;

typedef i64 (*pWin32_WNDPROC)(ptr hwnd, u32 msg, u64 wparam, i64 lparam);

typedef struct sWin32_WNDCLASSA 
{
  u32 style;
  pWin32_WNDPROC wndproc;
  int class_extra_bytes;
  int wnd_extra_bytes;
  ptr instance;
  ptr icon;
  ptr cursor;
  ptr background;
  const cstr menu_name;
  const cstr class_name;
} Win32_WNDCLASSA;

typedef struct sWin32_RECT 
{
  int x;
  int y;
  int w;
  int h;
} Win32_RECT;

typedef struct sWin32_WNDPL
{
  u32 length;
  u32 flags;
  u32 showCmd;
  Win32_POINT min_pos;
  Win32_POINT max_pos;
  Win32_RECT  normal_pos;
  Win32_RECT  device;
} Win32_WNDPL;

typedef struct sWin32_MONITORINFO 
{
  u32 ssize;
  Win32_RECT monitor;
  Win32_RECT workspace;
  u32 flags;
} Win32_MONITORINFO;

typedef struct sWin32_PFD 
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

/* shlwapi.dll */
typedef b32 (*pWin32_PathFileExistsA)(cstr path);

/* user32.dll */
typedef b32 (*pWin32_TranslateMessage)(const Win32_MSG *msg);
typedef i64 (*pWin32_DispatchMessageA)(const Win32_MSG *msg);
typedef b32 (*pWin32_PeekMessageA)(Win32_MSG *msg, ptr hwnd, u32 filter_min, u32 filter_max, u32 remove_flag);
typedef i64 (*pWin32_DefWindowProcA)(ptr hwnd, u32 msg, u64 wparam, i64 lparam);
typedef u16 (*pWin32_RegisterClassA)(const Win32_WNDCLASSA *wndclass);
typedef ptr (*pWin32_CreateWindowExA)(u32 ex_style, const cstr class_name, const cstr wnd_name, u32 style, int x, int y, int w, int h, ptr parent, ptr menu, ptr instance, ptr param);
typedef b32 (*pWin32_DestroyWindow)(ptr hwnd);
typedef b32 (*pWin32_MoveWindow)(ptr hwnd, int x, int y, int w, int h, b32 repaint);
typedef b32 (*pWin32_SetWindowPos)(ptr hwnd, ptr zpos, int x, int y, int w, int h, u32 flags);
typedef b32 (*pWin32_GetWindowPlacement)(ptr hwnd, Win32_WNDPL *wndpl);
typedef b32 (*pWin32_SetWindowPlacement)(ptr hwnd, const Win32_WNDPL *wndpl);
typedef ptr (*pWin32_GetDC)(ptr hwnd);
typedef int (*pWin32_ReleaseDC)(ptr hwnd, ptr dc);
typedef b32 (*pWin32_SetWindowTextA)(ptr hwnd, cstr text);
typedef int (*pWin32_MessageBoxA)(ptr hwnd, const cstr text, const cstr caption, u32 type);
typedef b32 (*pWin32_GetCursorPos)(Win32_POINT *point);
typedef b32 (*pWin32_ScreenToClient)(ptr hwnd, Win32_POINT *point);
typedef i32 (*pWin32_GetWindowLongA)(ptr hwnd, int index);
typedef i32 (*pWin32_SetWindowLongA)(ptr hwnd, int index, i64 value);
typedef ptr (*pWin32_MonitorFromWindow)(ptr hwnd, u32 flags);
typedef b32 (*pWin32_GetMonitorInfoA)(ptr monitor, Win32_MONITORINFO *mi);
typedef b32 (*pWin32_GetClientRect)(ptr hwnd, Win32_RECT *rect);
typedef b32 (*pWin32_GetWindowRect)(ptr hwnd, Win32_RECT *rect);

/* gdi32.dll */
typedef int (*pWin32_ChoosePixelFormat)(ptr hdc, const Win32_PFD *ppfd);
typedef int (*pWin32_DescribePixelFormat)(ptr hdc, int format, u32 bytes, Win32_PFD *ppfd);
typedef b32 (*pWin32_SetPixelFormat)(ptr hdc, int format, const Win32_PFD *ppfd);
typedef b32 (*pWin32_SwapBuffers)(ptr hdc);

/* opengl32.dll */
typedef ptr (*pWin32_wglCreateContext)(ptr hdc);
typedef b32 (*pWin32_wglDeleteContext)(ptr hglrc);
typedef i64 (*pWin32_wglGetProcAddress)(const cstr fname);
typedef b32 (*pWin32_wglMakeCurrent)(ptr hdc, ptr hglrc);

/* wgl video driver */
typedef b32 (*pWin32_wglChoosePixelFormatARB)(ptr hdc, const int *attribi_list, const r32 *attribf_list, u32 max_formats, int *formats, u32 *nun_formats);
typedef ptr (*pWin32_wglCreateContextAttribsARB)(ptr hdc, ptr hglrc, const int *attrib_list);
typedef b32 (*pWin32_wglSwapIntervalEXT)(int interval);

typedef struct sWIN_API
{
    /* shlwapi.dll */
    pWin32_PathFileExistsA PathFileExistsA;

    /* user32.dll */
    pWin32_TranslateMessage TranslateMessage;
    pWin32_DispatchMessageA DispatchMessageA;
    pWin32_PeekMessageA PeekMessageA;
    pWin32_DefWindowProcA DefWindowProcA;
    pWin32_RegisterClassA RegisterClassA;
    pWin32_CreateWindowExA CreateWindowExA;
    pWin32_DestroyWindow DestroyWindow;
    pWin32_MoveWindow MoveWindow;
    pWin32_SetWindowPos SetWindowPos;
    pWin32_GetWindowPlacement GetWindowPlacement;
    pWin32_SetWindowPlacement SetWindowPlacement;
    pWin32_GetDC GetDC;
    pWin32_ReleaseDC ReleaseDC;
    pWin32_SetWindowTextA SetWindowTextA;
    pWin32_MessageBoxA MessageBoxA;
    pWin32_GetCursorPos GetCursorPos;
    pWin32_ScreenToClient ScreenToClient;
    pWin32_GetWindowLongA GetWindowLongA;
    pWin32_SetWindowLongA SetWindowLongA;
    pWin32_MonitorFromWindow MonitorFromWindow;
    pWin32_GetMonitorInfoA GetMonitorInfoA;
    pWin32_GetClientRect GetClientRect;
    pWin32_GetWindowRect GetWindowRect;

    /* gdi32.dll */
    pWin32_ChoosePixelFormat ChoosePixelFormat;
    pWin32_DescribePixelFormat DescribePixelFormat;
    pWin32_SetPixelFormat SetPixelFormat;
    pWin32_SwapBuffers SwapBuffers;

    /* opengl32.dll */
    pWin32_wglCreateContext wglCreateContext;
    pWin32_wglDeleteContext wglDeleteContext;
    pWin32_wglGetProcAddress wglGetProcAddress;
    pWin32_wglMakeCurrent wglMakeCurrent;

    /* wgl video driver */
    pWin32_wglChoosePixelFormatARB wglChoosePixelFormatARB;
    pWin32_wglCreateContextAttribsARB wglCreateContextAttribsARB;
    pWin32_wglSwapIntervalEXT wglSwapIntervalEXT;

} WIN_API;

#endif /* ASCENCIA_WIN32_WIN_API_H */
/*============================================================
 * Project: Ascencia
 * File: win32_wgl.h
 * Author: Phraggers
 * Date: Fri Sep 13 2024
============================================================*/

#ifndef ASCENCIA_WIN32_WGL_H
#define ASCENCIA_WIN32_WGL_H

#define WGL_SAMPLE_BUFFERS_ARB            0x2041
#define WGL_SAMPLES_ARB                   0x2042
#define WGL_DRAW_TO_WINDOW_ARB            0x2001
#define WGL_SUPPORT_OPENGL_ARB            0x2010
#define WGL_DOUBLE_BUFFER_ARB             0x2011
#define WGL_PIXEL_TYPE_ARB                0x2013
#define WGL_TYPE_RGBA_ARB                 0x202B
#define WGL_ACCELERATION_ARB              0x2003
#define WGL_FULL_ACCELERATION_ARB         0x2027
#define WGL_COLOR_BITS_ARB                0x2014
#define WGL_ALPHA_BITS_ARB                0x201B
#define WGL_DEPTH_BITS_ARB                0x2022
#define WGL_CONTEXT_MAJOR_VERSION_ARB     0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB     0x2092
#define WGL_CONTEXT_PROFILE_MASK_ARB      0x9126
#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB  0x00000001
#define WGL_SWAP_EXCHANGE_ARB             0x2028
#define WGL_SWAP_METHOD_ARB               0x2007
#define WGL_SWAP_COPY_ARB                 0x2029

typedef b32 (*pfn_wglChoosePixelFormatARB)(ptr hdc, const int *iattribs, const r32 *fattribs, u32 max_formats, int *formats, u32 *nun_formats);
typedef ptr (*pfn_wglCreateContextAttribsARB)(ptr hDC, ptr glrc, const int *attribs);
typedef b32 (*pfn_wglSwapIntervalEXT)(int interval);

typedef struct
{
    pfn_wglChoosePixelFormatARB wglChoosePixelFormatARB;
    pfn_wglCreateContextAttribsARB wglCreateContextAttribsARB;
    pfn_wglSwapIntervalEXT wglSwapIntervalEXT;
} Win32_WGL;

#define WGL G_win32_state->wgl

bool Win32_LoadWGL(void);
bool Win32_LoadGL(void);

#endif /* ASCENCIA_WIN32_WGL_H */
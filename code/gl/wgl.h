#ifndef WGL_H
/* ==============================================================
   Project: Ascencia
   File:    wgl.h
   Author:  Phraggers
   Date:    17-11-2024
   ============================================================== */

#define WGL_SAMPLE_BUFFERS_ARB                  0x2041
#define WGL_SAMPLES_ARB                         0x2042
#define WGL_DRAW_TO_WINDOW_ARB                  0x2001
#define WGL_SUPPORT_OPENGL_ARB                  0x2010
#define WGL_DOUBLE_BUFFER_ARB                   0x2011
#define WGL_PIXEL_TYPE_ARB                      0x2013
#define WGL_TYPE_RGBA_ARB                       0x202B
#define WGL_ACCELERATION_ARB                    0x2003
#define WGL_FULL_ACCELERATION_ARB               0x2027
#define WGL_COLOR_BITS_ARB                      0x2014
#define WGL_ALPHA_BITS_ARB                      0x201B
#define WGL_DEPTH_BITS_ARB                      0x2022
#define WGL_CONTEXT_MAJOR_VERSION_ARB           0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB           0x2092
#define WGL_CONTEXT_PROFILE_MASK_ARB            0x9126
#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB        0x00000001
#define WGL_SWAP_EXCHANGE_ARB                   0x2028
#define WGL_SWAP_METHOD_ARB                     0x2007
#define WGL_SWAP_COPY_ARB                       0x2029
#define WGL_STENCIL_BITS_ARB                    0x2023
#define WGL_CONTEXT_LAYER_PLANE_ARB             0x2093
#define WGL_CONTEXT_FLAGS_ARB                   0x2094
#define WGL_CONTEXT_DEBUG_BIT_ARB               0x0001
#define WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB  0x0002

typedef BOOL (WINAPI* pfn_wglChoosePixelFormatARB)(HDC hdc, const int *piAttribIList,
                                                   const FLOAT *pfAttribFList,
                                                   UINT nMaxFormats, int *piFormats,
                                                   UINT *nNunFormats);
typedef HGLRC (WINAPI* pfn_wglCreateContextAttribsARB)(HDC hDC, HGLRC hShareContext,
                                                       const int *attribList);
typedef BOOL (WINAPI* pfn_wglSwapIntervalEXT)(int interval);

pfn_wglChoosePixelFormatARB wglChoosePixelFormatARB;
pfn_wglCreateContextAttribsARB wglCreateContextAttribsARB;
pfn_wglSwapIntervalEXT wglSwapIntervalEXT;

#define WGL_H
#endif

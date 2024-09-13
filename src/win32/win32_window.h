/*============================================================
 * Project: Ascencia
 * File: win32_window.h
 * Author: Phraggers
 * Date: Fri Sep 13 2024
============================================================*/

#ifndef ASCENCIA_WIN32_WINDOW_H
#define ASCENCIA_WIN32_WINDOW_H

#include <util/types.h>
#include <platform/window.h>
#include <win32/win32_api.h>

typedef struct
{
    ptr handle;
    PL_Window pl_window;
    //Win32_WNDPL wndpl;
} Win32_Window;

#endif /* ASCENCIA_WIN32_WINDOW_H */
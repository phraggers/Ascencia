
/*============================================================
 * Project: Ascencia [EXE]
 * File: win_api.h
 * Author: Phraggers
 * Date: Mon Sep 09 2024
============================================================*/

#ifndef ASCENCIA_WIN32_WIN_API_H
#define ASCENCIA_WIN32_WIN_API_H

#include <win32/shared.h>

typedef union _LARGE_INTEGER { struct {u32 LowPart; i32 HighPart;} s; 
struct {u32 LowPart; i32 HighPart;} u; i64 QuadPart;} LARGE_INTEGER;

/* shlwapi.dll */
typedef b32 (*pWin32_PathFileExistsA)(cstr path);

/* user32.dll */
typedef b32 (*pWin32_SetWindowTextA)(ptr handle, cstr text);

typedef struct sWIN_API
{
    /* shlwapi.dll */
    pWin32_PathFileExistsA PathFileExistsA;

    /* user32.dll */
    pWin32_SetWindowTextA SetWindowTextA;

} WIN_API;

#endif /* ASCENCIA_WIN32_WIN_API_H */
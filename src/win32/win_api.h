
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

/* shlwapi.lib */
typedef b32 (*pWin32_PathFileExistsA)(cstr path);

typedef struct sWIN_API
{
    /* shlwapi.lib */
    pWin32_PathFileExistsA PathFileExistsA;
} WIN_API;

#endif /* ASCENCIA_WIN32_WIN_API_H */
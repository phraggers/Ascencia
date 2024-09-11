
/*============================================================
 * Project: Ascencia [EXE]
 * File: win_api.c
 * Author: Phraggers
 * Date: Wed Sep 11 2024
============================================================*/

#include <util/types.h>
#include <win32/shared.h>

local bool _Load_shlwapi(void);
local bool _Load_user32(void);

bool WIN_WinAPIInit(void)
{
    Assert(G_win32_data != 0);

    if(!_Load_shlwapi())
    {
        return 0;
    }
    if(!_Load_user32())
    {
        return 0;
    }

    return 1;
}

#define LOAD_WIN_FN(name) {WINAPI.##name = (pWin32_##name)WIN_GetProcAddress(lib, #name); if(!WINAPI.##name) return 0;}

local bool _Load_shlwapi(void)
{
    ptr lib = WIN_LoadLibrary("shlwapi.dll");

    if(!lib)
    {
        return 0;
    }

    LOAD_WIN_FN(PathFileExistsA);

    return 1;
}

local bool _Load_user32(void)
{
    ptr lib = WIN_LoadLibrary("user32.dll");

    if(!lib)
    {
        return 0;
    }

    LOAD_WIN_FN(SetWindowTextA);

    return 1;
}
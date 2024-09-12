/*============================================================
 * Project: Ascencia [EXE]
 * File: ascencia_win32.c
 * Author: Phraggers
 * Date: Sat Sep 07 2024
============================================================*/

#define ASCENCIA_STATE_INSTANCE
#include <util/types.h>
#include <platform/alloc.h>
#include <win32/win32_api.h>
#include <win32/win32_state.h>
#include <platform/string_helpers.h>

local bool WIN_Entry(void);
int WinMain(ptr a, ptr b, cstr c, int d) { return WIN_Entry()?0:-1; }
int main(int ac, char **av) { return WIN_Entry()?0:-1; }

local bool WIN_Entry(void)
{
    PL_Log(LOG_DEBUG, "WIN_ENTRY");

    if(!Win32_StateInit())
    {
        PL_Log(LOG_FATAL, "failed to initialize");
        return 0;
    }

    Win32_StateQuit();
    PL_Log(LOG_DEBUG, "EXIT SUCCESS");
    return 1;
}



/*============================================================
 * Project: Ascencia [EXE]
 * File: win32_ascencia.c
 * Author: Phraggers
 * Date: Sat Sep 07 2024
============================================================*/

#include <util/types.h>
#define ASCENCIA_STDLIB_INSTANCE
#include <util/stdlib_interface.h>
#define ASCENCIA_WIN32_INSTANCE
#include <win32/shared.h>

local bool WIN_Entry(void);
int WinMain(ptr a, ptr b, cstr c, int d) { return WIN_Entry()?0:-1; }
int main(int ac, char **av) { return WIN_Entry()?0:-1; }

local bool WIN_Entry(void)
{
    if(!WIN_InterfaceInit(G_win32_data))
    {
        return 0;
    }

    if(!WIN_WinAPIInit())
    {
        return 0;
    }

    if(!WIN_StdLibInit(G_stdlib_interface))
    {
        return 0;
    }

    WIN_SetBasePath();
    STD_strcpy(PL_GetPrefPath(), PL_GetBasePath());
    STD_strcat(PL_GetPrefPath(), "pref\\");
    STD_puts(PL_GetBasePath());
    STD_puts(PL_GetPrefPath());

    #ifdef DEBUG
    if(!PL_SetLogLevel(LOG_DEBUG, LOG_WARN))
    #else
    if(!PL_SetLogLevel(LOG_NONE, LOG_ERROR))
    #endif
    {
        return 0;
    }

    if(!WIN_ConfigInit())
    {
        return 0;
    }

    PL_Log(LOG_DEBUG, "Exit success");
    return 1;
}


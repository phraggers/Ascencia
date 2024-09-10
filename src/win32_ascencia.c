
/*============================================================
 * Project: Ascencia [EXE]
 * File: win32_ascencia.c
 * Author: Phraggers
 * Date: Sat Sep 07 2024
============================================================*/

#include <util/types.h>
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

    WIN_SetBasePath();
    snprintf(PL_GetPrefPath(), STRING_LEN, "%spref\\", PL_GetBasePath());

    #ifdef DEBUG
    if(!PL_SetLogLevel(LOG_DEBUG, LOG_WARN))
    #else
    if(!PL_SetLogLevel(LOG_NONE, LOG_ERROR))
    #endif
    {
        return 0;
    }

    PL_Log(LOG_INFO, "Ascencia [%d.%d.%d.%d]", ASC_VERSION_RLS, ASC_VERSION_MAJ, ASC_VERSION_MIN, ASC_VERSION_REV);
    PL_Log(LOG_INFO, "BasePath: %s", PL_GetBasePath());
    PL_Log(LOG_INFO, "PrefPath: %s", PL_GetPrefPath());

    if(!WIN_ConfigInit())
    {
        return 0;
    }

    PL_Log(LOG_DEBUG, "Exit success");
    return 1;
}


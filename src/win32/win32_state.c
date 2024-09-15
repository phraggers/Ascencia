/*============================================================
 * Project: Ascencia
 * File: win32_state.c
 * Author: Phraggers
 * Date: Thu Sep 12 2024
============================================================*/

#include <platform/alloc.h>
#include <platform/timing.h>
#include <win32/win32_state.h>

bool Win32_StateInit(void)
{
    G_win32_state = (Win32_State*)PL_Alloc0(sizeof(Win32_State));
    if(!G_win32_state)
    {
        PL_Log(LOG_FATAL, "StateInit: malloc error");
        return 0;
    }

    return 1;
}

void Win32_StateQuit(void)
{
    PL_LogCleanup();
}

void Win32_SetBasePath(void)
{
    Assert(G_win32_state != 0);

    if(Win32_GetBasePath() == 0)
    {
        WINAPI.GetModuleFileNameA(0, G_win32_state->base_path, STRING_LEN);

        for(cstr cp = &G_win32_state->base_path[STRING_LEN];
            *cp != '\\' && cp > &G_win32_state->base_path[0];
            cp--)
        {
            *cp = 0;
        }
    }

    PL_Log(LOG_INFO, "BasePath set: %s", Win32_GetBasePath());
}

cstr Win32_GetBasePath(void)
{
    Assert(G_win32_state);

    if(strlen(G_win32_state->base_path) == 0)
    {
        return 0;
    }

    return G_win32_state->base_path;
}

bool Win32_SetPrefPath(cstr path)
{
    Assert(G_win32_state);
    
    if((!path) || (!strlen(path)))
    {
        return 0;
    }

    strcpy(G_win32_state->pref_path, path);
    if(!WINAPI.PathFileExistsA(Win32_GetPrefPath()))
    {
        PL_Log(LOG_DEBUG, "SetPrefPath: directory doesn't exist");
        b32 result = WINAPI.CreateDirectoryA(Win32_GetPrefPath(), 0);
        if(result)
        {
            PL_Log(LOG_INFO, "SetPrefPath: created pref directory: %s", Win32_GetPrefPath());
        }
        else
        {
            PL_Log(LOG_ERROR, "SetPrefPath: failed to create pref directory: %s", Win32_GetPrefPath());
        }
    }

    PL_Log(LOG_INFO, "PrefPath set: %s", Win32_GetPrefPath());
    return 1;
}

cstr Win32_GetPrefPath(void)
{
    Assert(G_win32_state);

    if(strlen(G_win32_state->pref_path) == 0)
    {
        return 0;
    }

    return G_win32_state->pref_path;
}

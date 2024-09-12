
/*============================================================
 * Project: Ascencia
 * File: win32_state.c
 * Author: Phraggers
 * Date: Thu Sep 12 2024
============================================================*/

#include <win32/win32_state.h>
#include <platform/alloc.h>
#include <platform/string_helpers.h>

bool Win32_StateInit(void)
{
    G_win32_state = (Win32_State*)PL_Alloc0(sizeof(Win32_State));
    if(!G_win32_state)
    {
        PL_Log(LOG_FATAL, "win32_state malloc error");
        return 0;
    }

    #ifdef DEBUG
    PL_SetConsoleLogLevel(LOG_DEBUG);
    PL_SetFileLogLevel(LOG_WARN);
    #else
    PL_SetConsoleLogLevel(LOG_NONE);
    PL_SetFileLogLevel(LOG_ERROR);
    #endif

    cstr logstr = (cstr)PL_Alloc0(STRING_LEN);
    if(logstr)
    {
        PL_String_ShortFileSize(logstr, sizeof(Win32_State));
        PL_Log(LOG_INFO, "win32_state: %s", logstr);
        PL_Free(logstr);
    }

    if(!Win32_LoadAPI())
    {
        PL_Log(LOG_FATAL, "Failed to load Win32 API");
        return 0;
    }

    Win32_SetBasePath();
    cstr pref_path = (cstr)PL_Alloc0(STRING_LEN);
    if(pref_path)
    {
        strcpy(pref_path, Win32_GetBasePath());
        strcat(pref_path, "pref\\");
        Win32_SetPrefPath(pref_path);
        PL_Free(pref_path);
    }

    cstr logfilepath = (cstr)PL_Alloc0(STRING_LEN);
    if(logfilepath)
    {
        strcpy(logfilepath, Win32_GetBasePath());
        strcat(logfilepath, "log\\");

        if(!PL_SetLogFilePath(logfilepath))
        {
            PL_Log(LOG_ERROR, "failed to create log file");
            memset(G_win32_state->logging.logfile_path, 0, STRING_LEN);
        }

        PL_Free(logfilepath);
    }
    else
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
        G_win32_state->api.GetModuleFileNameA(0, G_win32_state->base_path, STRING_LEN);

        for(cstr cp = &G_win32_state->base_path[STRING_LEN];
            *cp != '\\' && cp > &G_win32_state->base_path[0];
            cp--)
        {
            *cp = 0;
        }
    }

    PL_Log(LOG_INFO, "BasePath Set: %s", Win32_GetBasePath());
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
    if(!G_win32_state->api.PathFileExistsA(Win32_GetPrefPath()))
    {
        PL_Log(LOG_DEBUG, "SetPrefPath: directory doesn't exist");
        b32 result = G_win32_state->api.CreateDirectoryA(Win32_GetPrefPath(), 0);
        if(result)
        {
            PL_Log(LOG_INFO, "SetPrefPath: created pref directory: %s", Win32_GetPrefPath());
        }
        else
        {
            PL_Log(LOG_ERROR, "SetPrefPath: failed to create pref directory: %s", Win32_GetPrefPath());
        }
    }

    PL_Log(LOG_DEBUG, "SetPrefPath: set %s", Win32_GetPrefPath());
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

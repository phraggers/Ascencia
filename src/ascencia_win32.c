/*============================================================
 * Project: Ascencia [EXE]
 * File: ascencia_win32.c
 * Author: Phraggers
 * Date: Sat Sep 07 2024
============================================================*/

#include <util/types.h>
#include <util/string_helpers.h>
#include <platform/alloc.h>
#include <platform/logging.h>
#define ASCENCIA_STATE_INSTANCE
#include <win32/win32_state.h>

local bool Entry(void);
local bool Init(void);
local bool Run(void);
local void Quit(void);

int WinMain(ptr a, ptr b, cstr c, int d) { return Entry()?0:-1; }
int main(int c, char **v) { return Entry()?0:-1; }

local bool Init(void)
{
    u64 startup_timer = Win32_GetPerfCount();

    if(!Win32_StateInit())
    {
        return 0;
    }

    PL_LogInit();

    cstr logstr = PL_String_New();
    PL_String_ShortFileSize(logstr, sizeof(Win32_State));
    PL_Log(LOG_INFO, "State: %s", logstr);
    PL_Free(logstr);
    
    if(!Win32_LoadAPI())
    {
        PL_Log(LOG_FATAL, "Init: Failed to load Win32 API");
        return 0;
    }

    Win32_SetBasePath();

    cstr pref_path = PL_String_New();
    strcpy(pref_path, Win32_GetBasePath());
    strcat(pref_path, "pref\\");
    Win32_SetPrefPath(pref_path);
    PL_Free(pref_path);

    cstr logfilepath = PL_String_New();
    strcpy(logfilepath, Win32_GetBasePath());
    strcat(logfilepath, "log\\");

    if(!PL_SetLogFilePath(logfilepath))
    {
        PL_Log(LOG_ERROR, "Init: failed to create log file");
        memset(G_win32_state->logging.logfile_path, 0, STRING_LEN);
    }
    
    PL_Free(logfilepath);

    if(!PL_ConfigInit(Win32_GetPrefPath()))
    {
        PL_Log(LOG_FATAL, "Init: config init failed");
        return 0;
    }

    if(!PL_KeybindsInit(Win32_GetPrefPath()))
    {
        PL_Log(LOG_FATAL, "Init: keybinds init failed");
        return 0;
    }

    PL_UpdateTimer();
    PL_UpdateClock();

    if(!Win32_LoadXInput())
    {
        PL_Log(LOG_ERROR, "Init: failed to load XInput");
    }

    if(!Win32_LoadXAudio())
    {
        PL_Log(LOG_ERROR, "Init: failed to load XAudio2");
    }

    if(!Win32_CreateWindow())
    {
        PL_Log(LOG_FATAL, "Init: CreateWindow failed");
        return 0;
    }

    PL_Log(LOG_INFO, "Startup: %.04f sec", PL_TimerElapsed(startup_timer));
    G_win32_state->running = 1;
    return 1;
}

local bool Run(void)
{
    while(G_win32_state->running)
    {
        Win32_MessageLoop();
        PL_UpdateClock();
        PL_UpdateTimer();
        //Win32_AudioFrame();
        PL_ProcessKeybinds();
        //PL_Frame();
        Win32_UpdateWindow();
        Win32_UpdateInput();
    }

    return 1;
}

local void Quit(void)
{
    Win32_StateQuit();
}

local bool Entry(void)
{
    if(!Init()) return 0;
    if(!Run()) return 0;
    Quit();
    return 1;
}
/*============================================================
 * Project: Ascencia
 * File: win32_state.h
 * Author: Phraggers
 * Date: Thu Sep 12 2024
============================================================*/

#ifndef ASCENCIA_WIN32_STATE_H
#define ASCENCIA_WIN32_STATE_H

#include <util/types.h>
#include <platform/logging.h>
#include <platform/config.h>
#include <platform/window.h>
#include <platform/input.h>
#include <platform/timing.h>
#include <platform/keybinds.h>
#include <win32/win32_api.h>
#include <win32/win32_wgl.h>
#include <win32/win32_window.h>

typedef struct
{
    PL_Logging logging;
    PL_Config config;
    PL_Clock clock;
    PL_Timer timer;
    PL_Keybindings keybinds;

    Win32_API api;
    Win32_WGL wgl;
    Win32_Window window;
    Win32_XInput xinput;
    Win32_XAudio xaudio;

    bool running;
    char base_path[STRING_LEN];
    char pref_path[STRING_LEN];
    char config_path[STRING_LEN];
    char keybinds_path[STRING_LEN];
} Win32_State;

#ifdef ASCENCIA_STATE_INSTANCE
Win32_State *G_win32_state;
#else
extern Win32_State *G_win32_state;
#endif

bool Win32_StateInit(void);
void Win32_StateQuit(void);
void Win32_SetBasePath(void);
cstr Win32_GetBasePath(void);
bool Win32_SetPrefPath(cstr path);
cstr Win32_GetPrefPath(void);

#endif /* ASCENCIA_WIN32_STATE_H */
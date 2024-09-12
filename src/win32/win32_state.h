/*============================================================
 * Project: Ascencia
 * File: win32_state.h
 * Author: Phraggers
 * Date: Thu Sep 12 2024
============================================================*/

#ifndef ASCENCIA_WIN32_STATE_H
#define ASCENCIA_WIN32_STATE_H

#include <util/types.h>
#include <win32/win32_api.h>
#include <platform/logging.h>

typedef struct
{
    Win32_API api;
    PL_Logging logging;

    char base_path[STRING_LEN];
    char pref_path[STRING_LEN];
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
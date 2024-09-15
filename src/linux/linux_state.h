/*============================================================
 * Project: Ascencia
 * File: linux_state.h
 * Author: Phraggers
 * Date: Sat Sep 14 2024
============================================================*/

#ifndef ASCENCIA_LINUX_STATE_H
#define ASCENCIA_LINUX_STATE_H

#include <util/types.h>
#include <platform/logging.h>
#include <platform/config.h>
#include <platform/window.h>

typedef struct
{
    PL_Window pl_window;
} Lin_Window;

typedef struct
{
    PL_Logging logging;
    PL_Config config;
    Lin_Window window;

    char base_path[STRING_LEN];
    char pref_path[STRING_LEN];
    char config_path[STRING_LEN];
} Lin_State;

#ifdef ASCENCIA_STATE_INSTANCE
Lin_State *G_linux_state;
#else
extern Lin_State *G_linux_state;
#endif

bool Lin_StateInit(cstr argv0);
void Lin_StateQuit(void);
void Lin_SetBasePath(cstr argv0);
cstr Lin_GetBasePath(void);
bool Lin_SetPrefPath(cstr path);
cstr Lin_GetPrefPath(void);

#endif /* ASCENCIA_LINUX_STATE_H */
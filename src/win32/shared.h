
/*============================================================
 * Project: Ascencia [EXE]
 * File: shared.h
 * Author: Phraggers
 * Date: Sat Sep 07 2024
============================================================*/

#ifndef ASCENCIA_WIN32_SHARED_H
#define ASCENCIA_WIN32_SHARED_H

#ifdef DEBUG
    #define ASC_STDLIB_DLL "asc_stdlib-d.dll"
    #define ASCENCIA_DLL "ascencia-d.dll"
#else
    #define ASC_STDLIB_DLL "asc_stdlib.dll"
    #define ASCENCIA_DLL "ascencia.dll"
#endif

#include <util/stdlib_interface.h>
#include <util/config.h>
#include <win32/win_api.h>
#include <util/platform.h>

typedef struct sWIN_WindowConfig
{
    irect dim;
} WIN_WindowConfig;

typedef struct sWIN_Config
{
    ASC_Version version;
    char base_path[MAX_PATH_LENGTH];
    char pref_path[MAX_PATH_LENGTH];
    WIN_WindowConfig window_config;
} WIN_Config;

typedef struct sWIN_Data
{
    WIN_API win_api;
    WIN_Config win_config;
    LOG_LEVEL console_loglevel;
    LOG_LEVEL logfile_loglevel;
    char logfile_path[MAX_PATH_LENGTH];
} WIN_Data;

#ifdef ASCENCIA_WIN32_INSTANCE
WIN_Data *G_win32_data;
#else
extern WIN_Data *G_win32_data;
#endif
#define WINAPI G_win32_data->win_api

#include <util/platform.h>

/* platform interface */
bool WIN_InterfaceInit(WIN_Data *win32_data);
ptr WIN_LoadLibrary(const cstr lib_name);
ptr WIN_GetProcAddress(ptr module, const cstr proc_name);

/* filesystem */
void WIN_SetBasePath(void);

/* api */
bool WIN_WinAPIInit(void);
bool WIN_StdLibInit(STD_interface *stdlib_interface);

/* win config */
bool WIN_ConfigInit(void);
bool WIN_ConfigSave(cstr path);
bool WIN_ConfigLoad(cstr path);

#endif /* ASCENCIA_WIN32_SHARED_H */
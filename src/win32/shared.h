
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

#define ASCENCIA_STDLIB_INSTANCE
#include "util/stdlib_interface.h"

#define ASC_MAX_PATH 0xff

/* win32_data */
typedef struct sWIN_Data
{
    bool running;
    ptr stdin_handle;
    ptr stdout_handle;
    ptr stderr_handle;
    char base_path[ASC_MAX_PATH];
    char pref_path[ASC_MAX_PATH];
} WIN_Data;

#ifdef ASCENCIA_WIN32_INSTANCE
WIN_Data *G_win32_data;
#else
extern WIN_Data *G_win32_data;
#endif

#include <util/platform.h>

/* platform interface */
bool WIN_InterfaceInit(WIN_Data *win32_data);
ptr WIN_LoadLibrary(const cstr lib_name);
ptr WIN_GetProcAddress(ptr module, const cstr proc_name);

/* filesystem */
void WIN_SetBasePath(void);

/* stdlib */
bool WIN_StdLibInit(STD_interface *stdlib_interface);

#endif /* ASCENCIA_WIN32_SHARED_H */
/*============================================================
 * Project: Ascencia
 * File: config.h
 * Author: Phraggers
 * Date: Fri Sep 13 2024
============================================================*/

#ifndef ASCENCIA_CONFIG_H
#define ASCENCIA_CONFIG_H

#include <util/types.h>

#define ASC_VERSION_RLS 0 // 0:dev 1:alpha 2:beta 3:release
#define ASC_VERSION_MAJ 0
#define ASC_VERSION_MIN 1
#define ASC_VERSION_REV 0

#define ASC_OPENGL_MAJ 4
#define ASC_OPENGL_MIN 5

#define ASC_WINDOW_WIDTH 960
#define ASC_WINDOW_HEIGHT 540
#define ASC_WINDOW_FULLSCREEN 0
#define ASC_WINDOW_VSYNC 1
#define ASC_WINDOW_REFRESH_HZ 60

typedef struct
{
    u8 rls, maj, min, rev;
} PL_Version;

typedef struct
{
    irect dim;
    bool fullscreen;
    bool vsync;
    int refresh_hz;
} PL_WindowConfig;

typedef struct
{
    r32 master_volume;
} PL_AudioConfig;

typedef struct
{
    PL_Version ascencia_version;
    r32 opengl_version;
    PL_WindowConfig window_config;
    PL_AudioConfig audio_config;
} PL_Config;

bool PL_ConfigInit(cstr config_path);
bool PL_ConfigSave(void);
bool PL_ConfigLoad(void);
cstr PL_GetConfigPath(void);
bool PL_SetConfigPath(cstr path);

#endif /* ASCENCIA_CONFIG_H */
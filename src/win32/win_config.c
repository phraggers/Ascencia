
/*============================================================
 * Project: Ascencia [EXE]
 * File: win_config.c
 * Author: Phraggers
 * Date: Mon Sep 09 2024
============================================================*/

#include <util/types.h>
#include <win32/shared.h>

#define WIN_CONFIG_NAME "win32_config.dat"

local void _InitDefaultConfig(void)
{
    WIN_Config *cfg = &G_win32_data->win_config;

    cfg->version.rls = ASC_VERSION_RLS;
    cfg->version.maj = ASC_VERSION_MAJ;
    cfg->version.min = ASC_VERSION_MIN;
    cfg->version.rev = ASC_VERSION_REV;
    
    cfg->window_config.dim.w = 960;
    cfg->window_config.dim.h = 540;
}

bool WIN_ConfigInit(void)
{
    Assert(G_win32_data != 0);

    cstr config_path = PL_Alloc0(STRING_LEN);
    if(!config_path)
    {
        PL_Log(LOG_FATAL, "ConfigInit: malloc error");
        return 0;
    }

    strcpy(config_path, PL_GetBasePath());
    strcat(config_path, "config\\");
    strcat(config_path, WIN_CONFIG_NAME);

    _InitDefaultConfig();
    PL_Log(LOG_DEBUG, "Win32 Config init");

    if(!PL_DoesFileExist("config"))
    {
        PL_Log(LOG_DEBUG, "Config directory not found, creating.");
        PL_CreateDirectory("config");
    }

    if(PL_DoesFileExist(config_path))
    {
        PL_Log(LOG_DEBUG, "Win32 Config found, loading.");
        if(WIN_ConfigLoad(config_path))
        {
            PL_Log(LOG_INFO, "Win32 Config Loaded: %s", config_path);
            PL_Free(config_path);
            return 1;
        }
        
        PL_Log(LOG_DEBUG, "Win32 Config read error, deleting");
        PL_DeleteFile(config_path);
    }

    if(!WIN_ConfigSave(config_path))
    {
        PL_Log(LOG_ERROR, "Win32 Config failed to save: %s", config_path);
        PL_Free(config_path);
        return 0;
    }

    PL_Log(LOG_DEBUG, "Win32 Config Init success");
    PL_Free(config_path);
    return 1;
}

bool WIN_ConfigSave(cstr path)
{
    if(!path)
    {
        PL_Log(LOG_DEBUG, "Win32 Config save: no path given");
        return 0;
    }

    ASC_FileData data = {0};
    data.buffer = (ptr)(&G_win32_data->win_config);
    data.size = (u64)(sizeof(WIN_Config));

    PL_Log(LOG_DEBUG, "Win32 Config: Writing file: %s", path);
    if(!PL_FileWrite(path, &data))
    {
        return 0;
    }

    PL_Log(LOG_INFO, "Win32 Config: saved %s", path);    
    return 1;
}

bool WIN_ConfigLoad(cstr path)
{
    if(!path)
    {
        PL_Log(LOG_DEBUG, "Win32 Config load: no path given");
        return 0;
    }

    ASC_FileData *data = PL_FileRead(path);
    if(!data)
    {
        PL_Log(LOG_DEBUG, "Win32 Config load: failed to read file: %s", path);
        return 0;
    }

    if(data->size != (u64)sizeof(WIN_Config))
    {
        PL_FileFree(data);
        PL_Log(LOG_WARN, "Win32 Config: %s config version mis-match, re-creating.", path);
        return 0;
    }
    
    PL_FileFree(data);
    PL_Log(LOG_INFO, "Win32 Config: %s : Loaded", path);

    return 1;
}
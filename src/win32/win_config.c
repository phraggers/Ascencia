
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

    cstr config_path = PL_Alloc0(MAX_PATH_LENGTH);
    if(!config_path)
    {
        PL_Log(LOG_FATAL, "ConfigInit: malloc error");
        return 0;
    }

    STD_strcpy(config_path, PL_GetBasePath());
    STD_strcat(config_path, "config\\");
    STD_strcat(config_path, WIN_CONFIG_NAME);

    _InitDefaultConfig();

    if(!PL_DoesFileExist("config"))
    {
        PL_CreateDirectory("config");
    }

    if(PL_DoesFileExist(config_path))
    {
        if(WIN_ConfigLoad(config_path))
        {
            PL_Free(config_path);
            return 1;
        }

        PL_DeleteFile(config_path);
    }

    if(!WIN_ConfigSave(config_path))
    {
        //TODO: error: failed to save config file
        PL_Free(config_path);
        return 0;
    }

    PL_Free(config_path);
    return 1;
}

bool WIN_ConfigSave(cstr path)
{
    ASC_FileData data = {0};
    data.buffer = (ptr)(&G_win32_data->win_config);
    data.size = (u64)(sizeof(WIN_Config));
    if(!PL_FileWrite(path, &data))
    {
        return 0;
    }

    #ifdef DEBUG
    cstr log_string = PL_Alloc0(MAX_PATH_LENGTH);
    STD_strcpy(log_string, path);
    STD_strcat(log_string, " : Config Saved");
    STD_puts(log_string);
    PL_Free(log_string);
    #endif
    
    return 1;
}

bool WIN_ConfigLoad(cstr path)
{
    ASC_FileData *data = PL_FileRead(path);
    if(!data)
    {
        return 0;
    }

    if(data->size != (u64)sizeof(WIN_Config))
    {
        PL_FileFree(data);
        #ifdef DEBUG
        cstr log_string = PL_Alloc0(MAX_PATH_LENGTH);
        STD_strcpy(log_string, path);
        STD_strcat(log_string, " : Config version mis-match, re-creating...");
        STD_puts(log_string);
        PL_Free(log_string);
        #endif
        return 0;
    }
    
    PL_FileFree(data);

    #ifdef DEBUG
    cstr log_string = PL_Alloc0(MAX_PATH_LENGTH);
    STD_strcpy(log_string, path);
    STD_strcat(log_string, " : Config Loaded");
    STD_puts(log_string);
    PL_Free(log_string);
    #endif

    return 1;
}
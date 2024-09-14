/*============================================================
 * Project: Ascencia
 * File: win32_config.c
 * Author: Phraggers
 * Date: Fri Sep 13 2024
============================================================*/

#include <util/string_helpers.h>
#include <platform/alloc.h>
#include <platform/config.h>
#include <win32/win32_state.h>

#define CONFIG_FILENAME "config.asccfg"

local void ConfigInit(void)
{
    PL_Config *config = &G_win32_state->config;
    config->ascencia_version.rls = ASC_VERSION_RLS;
    config->ascencia_version.maj = ASC_VERSION_MAJ;
    config->ascencia_version.min = ASC_VERSION_MIN;
    config->ascencia_version.rev = ASC_VERSION_REV;
    config->opengl_version = (r32)ASC_OPENGL_MAJ + ((r32)ASC_OPENGL_MIN/10.0f);
    config->window_config.dim.w = ASC_WINDOW_WIDTH;
    config->window_config.dim.h = ASC_WINDOW_HEIGHT;
    config->window_config.fullscreen = ASC_WINDOW_FULLSCREEN;
    config->window_config.vsync = ASC_WINDOW_VSYNC;
    config->window_config.refresh_hz = ASC_WINDOW_REFRESH_HZ;
}

bool PL_ConfigInit(cstr config_path)
{
    Assert(G_win32_state != 0);

    ConfigInit();

    if(PL_GetConfigPath() != 0)
    {
        PL_Log(LOG_WARN, "ConfigInit: config path already set");
        return 0;
    }

    if(!PL_SetConfigPath(config_path))
    {
        PL_Log(LOG_ERROR, "ConfigInit: failed to set config path");
        return 0;
    }

    PL_Log(LOG_DEBUG, "ConfigInit success");
    return 1;
}

bool PL_ConfigSave(void)
{
    if((!G_win32_state) || 
       (!strlen(G_win32_state->config_path)))
    {
        PL_Log(LOG_DEBUG, "ConfigSave: invalid parameters");
        return 0;
    }

    FILE *config_file = fopen(PL_GetConfigPath(), "wb");
    if(config_file)
    {
        u64 bytes_written = fwrite((ptr)(&G_win32_state->config), 1, sizeof(PL_Config), config_file);
        fclose(config_file);

        if(sizeof(PL_Config) != bytes_written)
        {
            cstr size1 = PL_Alloc0(STRING_LEN);
            if(!size1)
            {
                PL_Log(LOG_FATAL, "ConfigSave: malloc error");
                return 0;
            }
            PL_String_ShortFileSize(size1, sizeof(PL_Config));

            cstr size2 = PL_Alloc0(STRING_LEN);
            if(!size2)
            {
                PL_Log(LOG_FATAL, "ConfigSave: malloc error");
                return 0;
            }
            PL_String_ShortFileSize(size2, bytes_written);

            PL_Log(LOG_ERROR, "ConfigSave: bytes written mis-match writing %s (expected %s : wrote %s)", PL_GetConfigPath(), size1, size2);
            PL_Free(size1);
            PL_Free(size2);
            return 0;
        }
        else
        {
            cstr size = PL_Alloc0(STRING_LEN);
            if(!size)
            {
                PL_Log(LOG_FATAL, "ConfigSave: malloc error");
                return 0;
            }
            PL_String_ShortFileSize(size, bytes_written);

            PL_Log(LOG_INFO, "ConfigSave: saved %s (%s)", PL_GetConfigPath(), size);
            return 1;
        }
    }
    else
    {
        PL_Log(LOG_ERROR, "ConfigSave: failed to open config file for saving: %s", PL_GetConfigPath());
        return 0;
    }
}

bool PL_ConfigLoad(void)
{
    if((!G_win32_state) || 
       (!strlen(G_win32_state->config_path)))
    {
        PL_Log(LOG_DEBUG, "ConfigLoad: invalid parameters");
        return 0;
    }

    FILE *config_file = fopen(PL_GetConfigPath(), "rb");
    if(config_file)
    {
        fseek(config_file, 0, SEEK_END);
        u64 size = (u64)ftell(config_file);
        fseek(config_file, 0, SEEK_SET);

        if(size != sizeof(PL_Config))
        {
            cstr size1 = PL_Alloc0(STRING_LEN);
            if(!size1)
            {
                PL_Log(LOG_FATAL, "ConfigLoad: malloc error");
                return 0;
            }
            PL_String_ShortFileSize(size1, sizeof(PL_Config));

            cstr size2 = PL_Alloc0(STRING_LEN);
            if(!size2)
            {
                PL_Log(LOG_FATAL, "ConfigLoad: malloc error");
                return 0;
            }
            PL_String_ShortFileSize(size2, size);

            PL_Log(LOG_ERROR, "ConfigLoad: config file size mis-match: %s (expected %s, read %s)", PL_GetConfigPath(), size1, size2);

            PL_Free(size1);
            PL_Free(size2);
            fclose(config_file);
            return 0;
        }

        PL_Config *loaded_config = PL_Alloc0(sizeof(PL_Config));
        if(!loaded_config)
        {
            fclose(config_file);
            PL_Log(LOG_FATAL, "ConfigLoad: malloc error");
            return 0;
        }

        u64 bytes_read = fread((ptr)loaded_config, 1, size, config_file);
        fclose(config_file);

        if(bytes_read != sizeof(PL_Config))
        {
            PL_Free(loaded_config);

            cstr size1 = PL_Alloc0(STRING_LEN);
            if(!size1)
            {
                PL_Log(LOG_FATAL, "ConfigLoad: malloc error");
                return 0;
            }
            PL_String_ShortFileSize(size1, bytes_read);

            cstr size2 = PL_Alloc0(STRING_LEN);
            if(!size2)
            {
                PL_Log(LOG_FATAL, "ConfigLoad: malloc error");
                return 0;
            }
            PL_String_ShortFileSize(size2, sizeof(PL_Config));

            PL_Log(LOG_ERROR, "ConfigLoad: failed to read config file: %s (read %s, expected %s)", PL_GetConfigPath(), size1, size2);
            PL_Free(size1);
            PL_Free(size2);

            return 0;
        }

        u32 *current_version = (u32*)(&G_win32_state->config.ascencia_version);
        u32 *loaded_version = (u32*)(&loaded_config->ascencia_version);
        if(*current_version != *loaded_version)
        {
            PL_Free(loaded_config);
            PL_Log(LOG_ERROR, "ConfigLoad: config version mis-match");
            return 0;
        }

        memcpy((ptr)(&G_win32_state->config), (ptr)(loaded_config), sizeof(PL_Config));
        PL_Free(loaded_config);

        cstr sizestr = PL_Alloc0(STRING_LEN);
        if(!sizestr)
        {
            PL_Log(LOG_FATAL, "ConfigLoad: malloc error");
            return 0;
        }
        PL_String_ShortFileSize(sizestr, sizeof(PL_Config));

        PL_Log(LOG_INFO, "ConfigLoad: loaded config: %s (%s)", PL_GetConfigPath(), sizestr);
        PL_Free(sizestr);
        return 1;
    }
    else
    {
        PL_Log(LOG_ERROR, "ConfigSave: failed to open config file for reading: %s", PL_GetConfigPath());
        return 0;
    }
}

cstr PL_GetConfigPath(void)
{
    if((!G_win32_state) ||
       (strlen(G_win32_state->config_path) == 0))
    {
        return 0;
    }

    return G_win32_state->config_path;
}

bool PL_SetConfigPath(cstr path)
{
    if((!G_win32_state) || (!path))
    {
        PL_Log(LOG_DEBUG, "SetConfigPath: invalid parameters");
        return 0;
    }

    if(strlen(G_win32_state->config_path) > 0)
    {
        PL_Log(LOG_DEBUG, "SetConfigPath: existing string found, clearing");
        memset((ptr)G_win32_state->config_path, 0, STRING_LEN);
    }

    if(!WINAPI.PathFileExistsA(path))
    {
        PL_Log(LOG_DEBUG, "SetConfigPath: directory doesn't exist");
        b32 result = WINAPI.CreateDirectoryA(path, 0);
        if(result)
        {
            PL_Log(LOG_INFO, "SetConfigPath: created config directory: %s", path);
        }
        else
        {
            PL_Log(LOG_ERROR, "SetConfigPath: failed to create config directory: %s", path);
            return 0;
        }
    }

    strcpy(G_win32_state->config_path, path);
    if(G_win32_state->config_path[strlen(G_win32_state->config_path)-1] != '\\')
    {
        G_win32_state->config_path[strlen(G_win32_state->config_path)] = '\\';
    }
    strcat(G_win32_state->config_path, CONFIG_FILENAME);

    if(!WINAPI.PathFileExistsA(PL_GetConfigPath()))
    {
        PL_Log(LOG_DEBUG, "SetConfigPath: config file doesn't exist");
        
        if(!PL_ConfigSave())
        {
            PL_Log(LOG_ERROR, "SetConfigPath: failed to save config: %s", PL_GetConfigPath());
            return 0;
        }
        else
        {
            PL_Log(LOG_DEBUG, "SetConfigPath: config saved");
            return 1;
        }
    }

    if(!PL_ConfigLoad())
    {
        PL_Log(LOG_WARN, "SetConfigPath: failed to load config: %s", PL_GetConfigPath());
        PL_Log(LOG_DEBUG, "SetConfigPath: Deleting erroneous config");
        WINAPI.DeleteFileA(PL_GetConfigPath());

        if(!PL_ConfigSave())
        {
            PL_Log(LOG_ERROR, "SetConfigPath: failed to save config: %s", PL_GetConfigPath());
            return 0;
        }
        else
        {
            PL_Log(LOG_DEBUG, "SetConfigPath: config saved");
            return 1;
        }
    }
    else
    {
        PL_Log(LOG_DEBUG, "SetConfigPath: config loaded");
        return 1;
    }
}

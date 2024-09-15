/*============================================================
 * Project: Ascencia
 * File: linux_state.c
 * Author: Phraggers
 * Date: Sat Sep 14 2024
============================================================*/

#include <util/string_helpers.h>
#include <platform/alloc.h>
#include <linux/linux_state.h>

#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>

bool Lin_StateInit(cstr argv0)
{
    G_linux_state = (Lin_State*)PL_Alloc0(sizeof(Lin_State));
    if(!G_linux_state)
    {
        PL_Log(LOG_FATAL, "StateInit: malloc error");
        return 0;
    }

    switch(ASC_VERSION_RLS)
    {
        case 0:
        {
            PL_Log(LOG_INFO, "Ascencia [DEV] %d.%d.%d", ASC_VERSION_MAJ, ASC_VERSION_MIN, ASC_VERSION_REV);
        } break;
        case 1:
        {
            PL_Log(LOG_INFO, "Ascencia [ALPHA] %d.%d.%d", ASC_VERSION_MAJ, ASC_VERSION_MIN, ASC_VERSION_REV);
        } break;
        case 2:
        {
            PL_Log(LOG_INFO, "Ascencia [BETA] %d.%d.%d", ASC_VERSION_MAJ, ASC_VERSION_MIN, ASC_VERSION_REV);
        } break;
        case 3:
        {
            PL_Log(LOG_INFO, "Ascencia [RELEASE] %d.%d.%d", ASC_VERSION_MAJ, ASC_VERSION_MIN, ASC_VERSION_REV);
        } break;
    }

    cstr logstr = (cstr)PL_Alloc0(STRING_LEN);
    if(logstr)
    {
        PL_String_ShortFileSize(logstr, sizeof(Lin_State));
        PL_Log(LOG_INFO, "StateInit: %s", logstr);
        PL_Free(logstr);
    }
    else
    {
        PL_Log(LOG_FATAL, "StateInit: malloc error");
        return 0;
    }

    Lin_SetBasePath(argv0);

    cstr pref_path = (cstr)PL_Alloc0(STRING_LEN);
    if(pref_path)
    {
        strcpy(pref_path, Lin_GetBasePath());
        strcat(pref_path, "pref/");
        Lin_SetPrefPath(pref_path);
        PL_Free(pref_path);
    }
    else
    {
        PL_Log(LOG_FATAL, "StateInit: malloc error");
        return 0;
    }

    cstr logfilepath = (cstr)PL_Alloc0(STRING_LEN);
    if(logfilepath)
    {
        strcpy(logfilepath, Lin_GetBasePath());
        strcat(logfilepath, "log/");

        if(!PL_SetLogFilePath(logfilepath))
        {
            PL_Log(LOG_ERROR, "StateInit: failed to create log file");
            memset(G_linux_state->logging.logfile_path, 0, STRING_LEN);
        }

        PL_Free(logfilepath);
    }
    else
    {
        PL_Log(LOG_FATAL, "StateInit: malloc error");
        return 0;
    }

    if(!PL_ConfigInit(Lin_GetPrefPath()))
    {
        PL_Log(LOG_FATAL, "StateInit: config init failed");
        return 0;
    }

    return 1;
}

void Lin_StateQuit(void)
{
    PL_LogCleanup();
}

void Lin_SetBasePath(cstr argv0)
{
    if(Lin_GetBasePath() == 0)
    {
        strcpy(G_linux_state->base_path, argv0);

        for(cstr cp = &G_linux_state->base_path[STRING_LEN];
            *cp != '/' && cp > &G_linux_state->base_path[0];
            cp--)
        {
            *cp = 0;
        }
    }

    PL_Log(LOG_INFO, "BasePath Set: %s", Lin_GetBasePath());
}

cstr Lin_GetBasePath(void)
{
    if(strlen(G_linux_state->base_path) == 0)
    {
        return 0;
    }

    return G_linux_state->base_path;
}

bool Lin_SetPrefPath(cstr path)
{
    if((!path) || (!strlen(path)))
    {
        return 0;
    }

    strcpy(G_linux_state->pref_path, path);

    bool pref_dir_exists = 0;
    {
        DIR* dir = opendir(path);
        if (dir) 
        {
            closedir(dir);
            pref_dir_exists = 1;
        }
    }

    if(!pref_dir_exists)
    {
        b32 result = mkdir(Lin_GetPrefPath(), 0777) ? 0:1);
        if(result)
        {
            PL_Log(LOG_INFO, "SetPrefPath: created pref directory: %s", Lin_GetPrefPath());
        }
        else
        {
            PL_Log(LOG_ERROR, "SetPrefPath: failed to create pref directory: %s", Lin_GetPrefPath());
        }
    }

    return 1;
}

cstr Lin_GetPrefPath(void)
{
    if(strlen(G_linux_state->pref_path) == 0)
    {
        return 0;
    }

    return G_linux_state->pref_path;
}

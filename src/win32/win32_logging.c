/*============================================================
 * Project: Ascencia
 * File: logging.c
 * Author: Phraggers
 * Date: Thu Sep 12 2024
============================================================*/

#include <util/types.h>
#include <util/string_helpers.h>
#include <platform/alloc.h>
#include <platform/logging.h>
#include <win32/win32_state.h>

typedef struct
{
    char logstring[STRING_LEN];
    char timestamp[STRING_LEN];
    char fstring[STRING_LEN];
    char filetimestamp[STRING_LEN];
    char msgboxstring[STRING_LEN];
} LogState;
global LogState *G_logstate;

#define G_logstring G_logstate->logstring
#define G_timestamp G_logstate->timestamp
#define G_fstring G_logstate->fstring
#define G_filetimestamp G_logstate->filetimestamp
#define G_msgboxstring G_logstate->msgboxstring

local void LogInit(void)
{
    G_logstate = (LogState*)PL_Alloc0(sizeof(LogState));
    if(!G_logstate)
    {
        exit(-1);
    }
}

bool PL_LogInit(void)
{
    #ifdef DEBUG
    PL_SetConsoleLogLevel(LOG_DEBUG);
    PL_SetFileLogLevel(LOG_INFO);
    #else
    PL_SetConsoleLogLevel(LOG_NONE);
    PL_SetFileLogLevel(LOG_INFO);
    #endif

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

    return 1;
}

void PL_SetConsoleLogLevel(LOG_LEVEL level)
{
    Assert(G_win32_state);
    if(level < LOG_NONE)
    {
        level = LOG_NONE;
    } 
    if(level > LOG_DEBUG)
    {
        level = LOG_DEBUG;
    }
    G_win32_state->logging.console_loglevel = level;
}

void PL_SetFileLogLevel(LOG_LEVEL level)
{
    Assert(G_win32_state);
    if(level < LOG_NONE)
    {
        level = LOG_NONE;
    } 
    if(level > LOG_DEBUG)
    {
        level = LOG_DEBUG;
    }
    G_win32_state->logging.logfile_loglevel = level;
}

bool PL_Log(LOG_LEVEL level, cstr string, ...)
{
    if(!G_logstate)
    {
        LogInit();
    }

    #ifdef RELEASE
    if(level == LOG_DEBUG) return 1;
    #endif

    memset(G_logstring, 0, STRING_LEN);
    memset(G_timestamp, 0, STRING_LEN);
    memset(G_fstring, 0, STRING_LEN);
    
    PL_String_TimeStampNow(G_timestamp, "[%X]");
    strcat(G_logstring, G_timestamp);

    switch(level)
    {
        case LOG_FATAL:
        {
            strcat(G_logstring, " [FATAL] ");
        } break;
        case LOG_ERROR:
        {
            strcat(G_logstring, " [ERROR] ");
        } break;
        case LOG_WARN:
        {
            strcat(G_logstring, " [WARN_] ");
        } break;
        case LOG_INFO:
        {
            strcat(G_logstring, " [INFO_] ");
        } break;
        case LOG_DEBUG:
        {
            strcat(G_logstring, " [DEBUG] ");
        } break;
        case LOG_NONE: break;
    }

    va_list args;
    va_start(args, string);
    vsnprintf(G_fstring, STRING_LEN, string, args);
    va_end(args);
    snprintf(G_logstring+strlen(G_logstring), STRING_LEN, G_fstring);
    
    if(!G_win32_state)
    {
        // before state init, just puts() and return
        puts(G_logstring);
        return 1;
    }

    if(level <= G_win32_state->logging.console_loglevel)
    {
        puts(G_logstring);
    }

    if(level <= G_win32_state->logging.logfile_loglevel)
    {
        strcat(G_logstring, "\n");
        if(G_win32_state->logging.logfile_path[0] != 0)
        {
            FILE *logfile = fopen(G_win32_state->logging.logfile_path, "ab");
            if(logfile)
            {
                fwrite(G_logstring, 1, strlen(G_logstring), logfile);
                fclose(logfile);
            }
        }
    }

    if(level == LOG_FATAL)
    {
        PL_MsgBox("Fatal Error", G_logstring);
        if(G_win32_state != 0)
        {
            if(strlen(G_win32_state->logging.logfile_path) > 0)
            {
                while(strlen(G_win32_state->logging.logfile_path) > 0 &&
                      G_win32_state->logging.logfile_path[strlen(G_win32_state->logging.logfile_path)-1] != '_')
                {
                    G_win32_state->logging.logfile_path[strlen(G_win32_state->logging.logfile_path)-1] = 0;
                }
                strcat(G_win32_state->logging.logfile_path, "statedump.bin");

                FILE *dumpfile = fopen(G_win32_state->logging.logfile_path, "wb");
                if(dumpfile)
                {
                    fwrite((ptr)(&G_win32_state), 1, sizeof(Win32_State), dumpfile);
                    fclose(dumpfile);
                }
            }
        }
        exit(-1);
    }

    return 1;
}

bool PL_SetLogFilePath(cstr path)
{
    Assert(G_win32_state);

    if((!path) || (!strlen(path)))
    {
        return 0;
    }

    if(!WIN_API.PathFileExistsA(path))
    {
        PL_Log(LOG_DEBUG, "SetLogFilePath: directory doesn't exist");
        b32 result = WIN_API.CreateDirectoryA(path, 0);
        if(result)
        {
            PL_Log(LOG_INFO, "SetLogFilePath: created log directory: %s", path);
        }
        else
        {
            PL_Log(LOG_ERROR, "SetLogFilePath: failed to create log directory: %s", path);
            return 0;
        }
    }

    memset(G_filetimestamp, 0, STRING_LEN);

    PL_String_TimeStampNow(G_filetimestamp, "%y%m%d%H%M%S_log.txt");
    if(path[strlen(path)-1] != '\\')
    {
        path[strlen(path)] = '\\';
    }
    strcat(path, G_filetimestamp);
    strcpy(G_win32_state->logging.logfile_path, path);

    FILE *logfile = fopen(path, "wb");
    if(logfile)
    {
        cstr logfile_header = PL_String_New();
        strcpy(logfile_header, " === ASCENCIA LOG FILE === \n\n");
        fwrite(logfile_header, 1, strlen(logfile_header), logfile);
        fclose(logfile);
        PL_Free(logfile_header);
    }
    else
    {
        PL_Log(LOG_ERROR, "SetLogFilePath failed to open file to write: %s", path);
        return 0;
    }

    PL_Log(LOG_INFO, "SetLogFilePath success: %s", path);
    return 1;
}

cstr PL_GetLogFilePath(void)
{
    if(!G_win32_state)
    {
        return 0;
    }

    if(strlen(G_win32_state->logging.logfile_path) == 0)
    {
        return 0;
    }

    return G_win32_state->logging.logfile_path;
}

void PL_LogCleanup(void)
{
    if(!G_win32_state)
    {
        return;
    }

    if(!PL_GetLogFilePath())
    {
        return;
    }

    WIN_API.DeleteFileA(PL_GetLogFilePath());
}

void PL_MsgBox(const cstr title, cstr format, ...)
{
    if(!G_win32_state)
    {
        return;
    }

    va_list args;
    va_start(args, format);
    memset(G_msgboxstring, 0, STRING_LEN);
    vsnprintf(G_msgboxstring, STRING_LEN, format, args);
    va_end(args);
    WIN_API.MessageBoxA(0, G_msgboxstring, title, 0x00000010L);
}
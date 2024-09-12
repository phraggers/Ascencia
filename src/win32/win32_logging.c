
/*============================================================
 * Project: Ascencia
 * File: logging.c
 * Author: Phraggers
 * Date: Thu Sep 12 2024
============================================================*/

#include <util/types.h>
#include <platform/alloc.h>
#include <win32/win32_state.h>
#include <platform/logging.h>
#include <platform/string_helpers.h>

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
    #ifdef RELEASE
    if(level == LOG_DEBUG) return 1;
    #endif

    cstr logstring = PL_Alloc0(STRING_LEN);
    if(!logstring)
    {
        return 0;
    }

    cstr timestamp = PL_Alloc0(STRING_LEN);
    if(!timestamp)
    {
        return 0;
    }
    
    PL_String_TimeStampNow(timestamp, "[%X]");
    strcat(logstring, timestamp);
    PL_Free(timestamp);

    switch(level)
    {
        case LOG_FATAL:
        {
            strcat(logstring, " [FATAL] ");
        } break;
        case LOG_ERROR:
        {
            strcat(logstring, " [ERROR] ");
        } break;
        case LOG_WARN:
        {
            strcat(logstring, " [WARN_] ");
        } break;
        case LOG_INFO:
        {
            strcat(logstring, " [INFO_] ");
        } break;
        case LOG_DEBUG:
        {
            strcat(logstring, " [DEBUG] ");
        } break;
        case LOG_NONE: break;
    }

    cstr fstring = PL_Alloc0(STRING_LEN);
    if(!fstring)
    {
        PL_Free(logstring);
        return 0;
    }

    va_list args;
    va_start(args, string);
    vsnprintf(fstring, STRING_LEN, string, args);
    va_end(args);
    snprintf(logstring+strlen(logstring), STRING_LEN, fstring);
    PL_Free(fstring);
    
    if(!G_win32_state)
    {
        // before state init, just puts(), free and return
        puts(logstring);
        PL_Free(logstring);
        return 1;
    }

    if(level <= G_win32_state->logging.console_loglevel)
    {
        puts(logstring);
    }

    if(level <= G_win32_state->logging.logfile_loglevel)
    {
        strcat(logstring, "\n");
        if(G_win32_state->logging.logfile_path[0] != 0)
        {
            FILE *logfile = fopen(G_win32_state->logging.logfile_path, "ab");
            if(logfile)
            {
                fwrite(logstring, 1, strlen(logstring), logfile);
                fclose(logfile);
            }
        }
    }

    if(level == LOG_FATAL)
    {
        PL_MsgBox("Fatal Error", logstring);
        exit(-1);
    }

    PL_Free(logstring);

    return 1;
}

bool PL_SetLogFilePath(cstr path)
{
    Assert(G_win32_state);

    if((!path) || (!strlen(path)))
    {
        return 0;
    }

    if(!G_win32_state->api.PathFileExistsA(path))
    {
        PL_Log(LOG_DEBUG, "SetLogFilePath: directory doesn't exist");
        b32 result = G_win32_state->api.CreateDirectoryA(path, 0);
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

    cstr timestamp = (cstr)PL_Alloc0(STRING_LEN);
    if(!timestamp)
    {
        PL_Log(LOG_FATAL, "SetLogFilePath: malloc error");
        return 0;
    }

    PL_String_TimeStampNow(timestamp, "%y%m%d%H%M%S_log.txt");
    strcat(path, timestamp);
    PL_Free(timestamp);
    strcpy(G_win32_state->logging.logfile_path, path);

    FILE *logfile = fopen(path, "wb");
    if(logfile)
    {
        cstr logfile_header = (cstr)PL_Alloc0(STRING_LEN);
        if(logfile_header)
        {
            strcpy(logfile_header, " === ASCENCIA LOG FILE === \n\n");
            fwrite(logfile_header, 1, strlen(logfile_header), logfile);
            fclose(logfile);
            PL_Free(logfile_header);
        }
        else
        {
            PL_Log(LOG_FATAL, "SetLogFilePath: malloc error");
            return 0;
        }
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

    G_win32_state->api.DeleteFileA(PL_GetLogFilePath());
}

void PL_MsgBox(const cstr title, cstr format, ...)
{
    if(!G_win32_state)
    {
        return;
    }

    va_list args;
    va_start(args, format);
    cstr msg = PL_Alloc0(STRING_LEN);
    if(!msg) return;
    vsnprintf(msg, STRING_LEN, format, args);
    va_end(args);
    G_win32_state->api.MessageBoxA(0, msg, title, 0x00000010L);
    PL_Free(msg);
}
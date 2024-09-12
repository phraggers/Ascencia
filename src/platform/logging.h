
/*============================================================
 * Project: Ascencia
 * File: logging.h
 * Author: Phraggers
 * Date: Thu Sep 12 2024
============================================================*/

#ifndef ASCENCIA_LOGGING_H
#define ASCENCIA_LOGGING_H

#include <util/types.h>

typedef enum
{
    LOG_NONE = 0,
    LOG_FATAL,
    LOG_ERROR,
    LOG_WARN,
    LOG_INFO,
    LOG_DEBUG
} LOG_LEVEL;

typedef struct
{
    LOG_LEVEL console_loglevel;
    LOG_LEVEL logfile_loglevel;
    char logfile_path[STRING_LEN];
} PL_Logging;

void PL_SetConsoleLogLevel(LOG_LEVEL level);
void PL_SetFileLogLevel(LOG_LEVEL level);
bool PL_SetLogFilePath(cstr path);
cstr PL_GetLogFilePath(void);
bool PL_Log(LOG_LEVEL level, cstr string, ...);
void PL_LogCleanup(void);
void PL_MsgBox(const cstr title, cstr format, ...);

#endif /* ASCENCIA_LOGGING_H */
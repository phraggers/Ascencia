#ifndef LOG_H
/* ==============================================================
   File:    log.h
   Author:  Phraggers
   Date:    21-04-2025
   ============================================================== */

bool CORE_LogInit(void);
void CORE_LogQuit(void);

void CORE_LogDebug(char *string, ...); // console only debug spam
void CORE_LogInfo(char *string, ...); // misc info
void CORE_LogWarning(char *string, ...); // warning log
void CORE_LogError(char *string, ...); // error log
void CORE_LogFatal(char *string, ...); // FATAL and immediate quit
void CORE_LogPrint(char *string, ...); // unconditional print

#define LOG_H
#endif

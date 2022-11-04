
#ifndef _ASC_LOG_H_
#define _ASC_LOG_H_

// 1: DEBUG: console
// 2: INFO: console
// 3: WARN: console, logfile
// 4: ERROR: console, logfile, msgbox
// 5: FATAL: console, logfile, msgbox, state dump and quit

#define LOGLEVEL_ALL   0x00
#define LOGLEVEL_DEBUG 0x01
#define LOGLEVEL_INFO  0x02
#define LOGLEVEL_WARN  0x03
#define LOGLEVEL_ERROR 0x04
#define LOGLEVEL_FATAL 0x05
#define LOGLEVEL_NONE  0x06

#define LOGTARGET_CONSOLE 0x10
#define LOGTARGET_FILE    0x11
#define LOGTARGET_POPUP   0x12

typedef struct
{
	int Status;
	int Level;
	char Buffer[DEF_LOG_BUFFER_SIZE];
} Log_QueueElement;

typedef struct
{
	bool LogRunning;

	int ConsoleLevel;
	int FileLevel;
	int PopupLevel;

	char LastLogFilePath[DEF_LOG_BUFFER_SIZE];
	char LogFilePath[DEF_LOG_BUFFER_SIZE];
	bool LogFileLocked;

	Log_QueueElement Log_Queue[DEF_LOG_QUEUE_COUNT];
} ASC_S_Log;

// Init log and start running thread
bool Log_Init(void);

// Any future log will be output to target if log level is >= target level. Returns 1 on success or 0 on fail.
bool Log_SetLevel(int _Target, int _Level);

// sets logfile path, returns success
bool Log_SetLogFilePath(void);

// get logfile path, returns success
bool Log_GetLogFilePath(char* _Path);

// cleanup and delete logfile on quit
void Log_Quit(void);

// return 1 on success, 0 on fail
bool ASC_Log(int _Level, const char* _Fmt, ...);

#endif //_ASC_LOG_H_
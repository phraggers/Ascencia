
#include "Ascencia.h"
#include <stdlib.h>
#include <stdio.h>

#define LOG_BUFFER_SIZE 256
#define LOG_QUEUE_COUNT 24

#define LOGSTATUS_NONE    0
#define LOGSTATUS_STARTED 1
#define LOGSTATUS_WAITING 2
#define LOGSTATUS_DONE    3

static bool LogRunning = 0;

#if _DEBUG
static int ConsoleLevel = LOGLEVEL_ALL;
static int FileLevel = LOGLEVEL_INFO;
static int PopupLevel = LOGLEVEL_ERROR;
#else
static int ConsoleLevel = LOGLEVEL_WARN;
static int FileLevel = LOGLEVEL_ERROR;
static int PopupLevel = LOGLEVEL_FATAL;
#endif
static char LastLogFilePath[LOG_BUFFER_SIZE] = { 0 };
static char LogFilePath[LOG_BUFFER_SIZE] = { 0 };
static bool LogFileLocked = 0;

typedef struct
{
	int Status;
	int Level;
	char Buffer[LOG_BUFFER_SIZE];
} Log_QueueElement;

static Log_QueueElement Log_Queue[LOG_QUEUE_COUNT] = { 0 };

typedef struct
{
	int ConsoleLevel;
	int FileLevel;
	int PopupLevel;
	char LogFilePath[LOG_BUFFER_SIZE];
	bool LogFileLocked;
	Log_QueueElement Log_Queue[LOG_QUEUE_COUNT];
} S_Log;

bool Log_SetLevel(int _Target, int _Level)
{
	if (_Level < LOGLEVEL_ALL || _Level > LOGLEVEL_NONE)
	{
		return 0;
	}

	if (_Target < LOGTARGET_CONSOLE || _Target > LOGTARGET_POPUP)
	{
		return 0;
	}

	switch (_Target)
	{
		case LOGTARGET_CONSOLE:
		{
			ConsoleLevel = _Level;
			return 1;
		} break;

		case LOGTARGET_FILE:
		{
			FileLevel = _Level;
			return 1;
		} break;

		case LOGTARGET_POPUP:
		{
			PopupLevel = _Level;
			return 1;
		} break;
	}

	return 0;
}

int Log_GetLevel(int _Target)
{
	if (_Target < LOGTARGET_CONSOLE || _Target > LOGTARGET_POPUP)
	{
		return -1;
	}

	switch (_Target)
	{
		case LOGTARGET_CONSOLE:
		{
			return ConsoleLevel;
		} break;

		case LOGTARGET_FILE:
		{
			return FileLevel;
		} break;

		case LOGTARGET_POPUP:
		{
			return PopupLevel;
		} break;
	}

	return -1;
}

bool Log_SetLogFilePath()
{
	bool Result = 0;

	if (State)
	{
		char LogsDirPath[128] = { 0 };
		SDL_snprintf(LogsDirPath, 128, "%s/Logs", State->AppName);
		char* CreateLogsDir = SDL_GetPrefPath(State->AppOrg, LogsDirPath);
		SDL_memset(LogsDirPath, 0, 128);
		SDL_snprintf(LogsDirPath, 128, CreateLogsDir);
		SDL_free(CreateLogsDir);
		if(!Util_DirExists(LogsDirPath)) Util_MkDir(LogsDirPath);

		for (int i = 0; i < LOG_BUFFER_SIZE; i++) LogFilePath[i] = 0;
		char DateStamp[64] = { 0 };
		Util_TimeFormat(DateStamp, "Logs/%yyy-%M-%D_%h-%m-%s_Log.txt");

		SDL_snprintf(LastLogFilePath, LOG_BUFFER_SIZE, "%sLogs/LastLog.txt", State->PrefPath);
		if (SDL_snprintf(LogFilePath, LOG_BUFFER_SIZE, "%s%s", State->PrefPath, DateStamp) > 0)
		{
			Result = 1;
		}
	}

	else
	{
		SDL_snprintf(LastLogFilePath, LOG_BUFFER_SIZE, "LastLog.txt");
	}

	if (LogFileLocked)
	{
		while (LogFileLocked) {}
		LogFileLocked = 1;
		Util_CopyFile("CrashLog.txt", LogFilePath);
		Util_RemoveFile("CrashLog.txt");
		LogFileLocked = 0;
	}

	return Result;
}

bool Log_GetLogFilePath(char* _Path)
{
	if (SDL_snprintf(_Path, LOG_BUFFER_SIZE, LogFilePath) < 1)
	{
		return 0;
	}

	return 1;
}

inline static void Log_ClearBuffer(int _Index)
{
	for (int i = 0; i < LOG_BUFFER_SIZE; i++)
	{
		Log_Queue[_Index].Buffer[i] = 0;
	}

	Log_Queue[_Index].Level = 0;
	Log_Queue[_Index].Status = LOGSTATUS_NONE;
}

inline static int Log_BufferStringLength(int _Index)
{
	for (int i = 0; i < LOG_BUFFER_SIZE; i++)
	{
		if (Log_Queue[_Index].Buffer[i] == 0)
		{
			return i;
		}
	}

	return 0;
}

static void Log_ToConsole(int _Index)
{
	char DateStamp[24] = { 0 };
	char LevelStamp[7][8] = { "(LOG)", "(DEBUG)", "(INFO)", "(WARN)", "(ERROR)", "(FATAL)", "(LOG)" };
	Util_TimeFormat(DateStamp, "[%yyy-%M-%D %h:%m:%s]");

	if (Log_Queue[_Index].Level >= LOGLEVEL_ERROR)
	{
		fprintf(stderr, "%s %s %s\n", DateStamp, LevelStamp[Log_Queue[_Index].Level],
				Log_Queue[_Index].Buffer);
	}
	else
	{
		printf("%s %s %s\n", DateStamp, LevelStamp[Log_Queue[_Index].Level],
			   Log_Queue[_Index].Buffer);
	}
}

static bool Log_ToFile(int _Index)
{
	if (LogFileLocked)
	{
		while (LogFileLocked) {}
	}

	LogFileLocked = 1;

	if (!State)
	{
		SDL_snprintf(LogFilePath, LOG_BUFFER_SIZE, "CrashLog.txt");
	}

	FILE* LogFile;
#if MSVC
	fopen_s(&LogFile, LogFilePath, "ab");
#else
	LogFile = fopen(LogFilePath, "ab");
#endif

	char DateStamp[24] = { 0 };
	char LevelStamp[7][8] = { "(LOG)", "(DEBUG)", "(INFO)", "(WARN)", "(ERROR)", "(FATAL)", "(LOG)" };
	Util_TimeFormat(DateStamp, "[%yyy-%M-%D %h:%m:%s]");


	if (LogFile)
	{
		fprintf(LogFile, "%s %s %s\n", DateStamp, LevelStamp[Log_Queue[_Index].Level],
				Log_Queue[_Index].Buffer);
		fclose(LogFile);
		LogFileLocked = 0;

		return 1;
	}

	else
	{
		LogFileLocked = 0;
		return 0;
	}
}
static void Log_ToPopup(int _Index)
{
	if (Log_Queue[_Index].Level == LOGLEVEL_DEBUG)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Debug",
								 Log_Queue[_Index].Buffer, 0);
	}

	else if (Log_Queue[_Index].Level == LOGLEVEL_INFO)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Info",
								 Log_Queue[_Index].Buffer, 0);
	}

	else if (Log_Queue[_Index].Level == LOGLEVEL_WARN)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING, "Warning",
								 Log_Queue[_Index].Buffer, 0);
	}

	else if (Log_Queue[_Index].Level == LOGLEVEL_ERROR)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error",
								 Log_Queue[_Index].Buffer, 0);
	}

	else if (Log_Queue[_Index].Level == LOGLEVEL_FATAL)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Fatal",
								 Log_Queue[_Index].Buffer, 0);
	}

	else
	{
		return;
	}
}

void Log_Quit()
{
	ASC_Log(LOGLEVEL_INFO, "LOG: Quitting");

	int BusyCounter = 0;
	int Busy = LOG_QUEUE_COUNT;

	while (Busy)
	{
		for (int i = 0; i < LOG_QUEUE_COUNT; i++)
		{
			if (Log_Queue[i].Status == LOGSTATUS_DONE)
			{
				Log_ClearBuffer(i);
			}

			if (Log_Queue[i].Status == LOGSTATUS_NONE)
			{
				Busy--;
			}
		}

		if (Busy > 0)
		{
			Busy = LOG_QUEUE_COUNT;
		}

		BusyCounter++;

		if (BusyCounter > INT_MAX)
		{
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Stuck Thread!",
									 "Seems like Log thread got stuck. Force quitting!", 0);
			exit(1);
		}
	}

	LogRunning = 0;

	Util_CopyFile(LogFilePath, LastLogFilePath);
	Util_RemoveFile(LogFilePath);
}

bool ASC_Log(int _Level, const char* _Fmt, ...)
{
	if (_Level < LOGLEVEL_DEBUG || _Level > LOGLEVEL_FATAL)
	{
		return 0;
	}

	if (ConsoleLevel > _Level &&
		FileLevel > _Level &&
		PopupLevel > _Level)
	{
		return 0;
	}

	bool FreeQueueSlotFound = 0;

	while (!FreeQueueSlotFound)
	{
		for (int i = 0; i < LOG_QUEUE_COUNT; i++)
		{
			if (Log_Queue[i].Status == LOGSTATUS_NONE)
			{
				FreeQueueSlotFound = 1;

				Log_Queue[i].Status = LOGSTATUS_STARTED;
				char Result[LOG_BUFFER_SIZE] = { 0 };
				Log_Queue[i].Level = _Level;

				va_list args;
				va_start(args, _Fmt);
				SDL_vsnprintf(Log_Queue[i].Buffer, LOG_BUFFER_SIZE, _Fmt, args);
				va_end(args);

				if (PopupLevel <= _Level) Log_ToPopup(i);

				Log_Queue[i].Status = LOGSTATUS_WAITING;

				break;
			}
		}

		if (!FreeQueueSlotFound)
		{
			char DateStamp[24] = { 0 };
			Util_TimeFormat(DateStamp, "[%yyy-%M-%D %h:%m:%s]");
			fprintf(stderr, "%s (WARN) LOG: Log Queue Full!\n", DateStamp);
		}
	}

	if (_Level == LOGLEVEL_FATAL)
	{
		int BusyQueue = LOG_QUEUE_COUNT;
		
		while (BusyQueue)
		{
			BusyQueue = LOG_QUEUE_COUNT;

			for (int i = 0; i < LOG_QUEUE_COUNT; i++)
			{
				if (Log_Queue[i].Status == LOGSTATUS_NONE ||
					Log_Queue[i].Status == LOGSTATUS_DONE)
				{
					BusyQueue--;
				}
			}
		}

		exit(1);
	}

	return 1;
}

int Log_ThreadFn(void* _Data)
{
	S_Log* Log = (S_Log*)_Data;

	while (LogRunning)
	{
		for (int i = 0; i < LOG_QUEUE_COUNT; i++)
		{
			if (Log_Queue[i].Status == LOGSTATUS_WAITING)
			{
				if (ConsoleLevel <= Log_Queue[i].Level) Log_ToConsole(i);
				if (FileLevel <= Log_Queue[i].Level) Log_ToFile(i);
				Log_Queue[i].Status = LOGSTATUS_DONE;
			}

			if (Log_Queue[i].Status == LOGSTATUS_DONE)
			{
				Log_ClearBuffer(i);
			}
		}
	}

	if(Log) SDL_free(Log);

	return 0;
}

bool Log_Init()
{
	if (LogRunning) return 0;

	S_Log* Log = (S_Log*)SDL_malloc(sizeof(S_Log));
	if (!Log) return 0;

	SDL_memset(Log, 0, sizeof(S_Log));
	LogRunning = 1;

#if _DEBUG
	Log->ConsoleLevel = LOGLEVEL_ALL;
	Log->FileLevel = LOGLEVEL_INFO;
	Log->PopupLevel = LOGLEVEL_ERROR;
#else
	Log->ConsoleLevel = LOGLEVEL_WARN;
	Log->FileLevel = LOGLEVEL_ERROR;
	Log->PopupLevel = LOGLEVEL_FATAL;
#endif

	SDL_Thread* Thread = SDL_CreateThread(Log_ThreadFn, "ASC_Thread_Log", (void*)Log);
	SDL_DetachThread(Thread);

	ASC_Log(LOGLEVEL_DEBUG, "LOG: Alloc Log[0x%x] Size[%u]", Log, sizeof(S_Log));
	ASC_Log(LOGLEVEL_INFO, "LOG: Initialized");

	return 1;
}
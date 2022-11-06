
#include "Ascencia.h"
#include <stdlib.h>
#include <stdio.h>

#define LOGSTATUS_NONE    0
#define LOGSTATUS_STARTED 1
#define LOGSTATUS_WAITING 2
#define LOGSTATUS_DONE    3

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
			State->Log.ConsoleLevel = _Level;
			return 1;
		} break;

		case LOGTARGET_FILE:
		{
			State->Log.FileLevel = _Level;
			return 1;
		} break;

		case LOGTARGET_POPUP:
		{
			State->Log.PopupLevel = _Level;
			return 1;
		} break;
	}

	return 0;
}

bool Log_SetLogFilePath(void)
{
	bool Result = 0;

	if (State)
	{
		char LogsDirPath[128] = { 0 };
		SDL_snprintf(LogsDirPath, 128, "%s/Logs", DEF_APPNAME);
		char* CreateLogsDir = SDL_GetPrefPath(DEF_APPORG, LogsDirPath);
		SDL_memset(LogsDirPath, 0, 128);
		SDL_snprintf(LogsDirPath, 128, CreateLogsDir);
		SDL_free(CreateLogsDir);
		if(!Util_DirExists(LogsDirPath)) Util_MkDir(LogsDirPath);

		for (int i = 0; i < DEF_LOG_BUFFER_SIZE; i++) State->Log.LogFilePath[i] = 0;
		char DateStamp[64] = { 0 };
		Util_TimeFormat(DateStamp, "Logs/%yyy-%M-%D_%h-%m-%s_Log.txt");

		SDL_snprintf(State->Log.LastLogFilePath, DEF_LOG_BUFFER_SIZE, "%sLogs/_LastLog.txt", State->PrefPath);
		if (SDL_snprintf(State->Log.LogFilePath, DEF_LOG_BUFFER_SIZE, "%s%s", State->PrefPath, DateStamp) > 0)
		{
			Result = 1;
		}
	}

	return Result;
}

bool Log_GetLogFilePath(char* _Path)
{
	if (SDL_snprintf(_Path, DEF_LOG_BUFFER_SIZE, State->Log.LogFilePath) < 1)
	{
		return 0;
	}

	return 1;
}

inline static void Log_ClearBuffer(int _Index)
{
	for (int i = 0; i < DEF_LOG_BUFFER_SIZE; i++)
	{
		State->Log.Log_Queue[_Index].Buffer[i] = 0;
	}

	State->Log.Log_Queue[_Index].Level = 0;
	State->Log.Log_Queue[_Index].Status = LOGSTATUS_NONE;
}

inline static int Log_BufferStringLength(int _Index)
{
	for (int i = 0; i < DEF_LOG_BUFFER_SIZE; i++)
	{
		if (State->Log.Log_Queue[_Index].Buffer[i] == 0)
		{
			return i;
		}
	}

	return 0;
}

static void Log_ToConsole(int _Index)
{
	char DateStamp[24] = { 0 };
	char LevelStamp[7][8] = { "[ LOG ]", "[DEBUG]", "[INFO ]", "[WARN ]", "[ERROR]", "[FATAL]", "[ LOG ]" };
	Util_TimeFormat(DateStamp, "[%yyy-%M-%D %h:%m:%s]");

	if (State->Log.Log_Queue[_Index].Level >= LOGLEVEL_ERROR)
	{
		fprintf(stderr, "%s %s %s\n", DateStamp, LevelStamp[State->Log.Log_Queue[_Index].Level],
				State->Log.Log_Queue[_Index].Buffer);
	}
	else
	{
		printf("%s %s %s\n", DateStamp, LevelStamp[State->Log.Log_Queue[_Index].Level],
			   State->Log.Log_Queue[_Index].Buffer);
	}
}

static bool Log_ToFile(int _Index)
{
	if (State->Log.LogFileLocked)
	{
		while (State->Log.LogFileLocked) {}
	}

	State->Log.LogFileLocked = 1;

	SDL_RWops* LogFile = SDL_RWFromFile(State->Log.LogFilePath, "ab");

	char DateStamp[24] = { 0 };
	char LevelStamp[7][8] = { "[ LOG ]", "[DEBUG]", "[INFO ]", "[WARN ]", "[ERROR]", "[FATAL]", "[ LOG ]" };
	Util_TimeFormat(DateStamp, "[%yyy-%M-%D %h:%m:%s]");


	if (LogFile)
	{
		ASC_AllocArray(char, DEF_LOG_BUFFER_SIZE, LogFileBuffer);

		SDL_snprintf(LogFileBuffer, DEF_LOG_BUFFER_SIZE, "%s %s %s\n", 
					 DateStamp, LevelStamp[State->Log.Log_Queue[_Index].Level],
					 State->Log.Log_Queue[_Index].Buffer);

		SDL_RWwrite(LogFile, LogFileBuffer, 1, Util_StrLen(LogFileBuffer));
		SDL_RWclose(LogFile);

		ASC_Free(LogFileBuffer);
		State->Log.LogFileLocked = 0;

		return 1;
	}

	else
	{
		State->Log.LogFileLocked = 0;
		return 0;
	}
}
static void Log_ToPopup(int _Index)
{
	if (State->Log.Log_Queue[_Index].Level == LOGLEVEL_DEBUG)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Debug",
								 State->Log.Log_Queue[_Index].Buffer, 0);
	}

	else if (State->Log.Log_Queue[_Index].Level == LOGLEVEL_INFO)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Info",
								 State->Log.Log_Queue[_Index].Buffer, 0);
	}

	else if (State->Log.Log_Queue[_Index].Level == LOGLEVEL_WARN)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING, "Warning",
								 State->Log.Log_Queue[_Index].Buffer, 0);
	}

	else if (State->Log.Log_Queue[_Index].Level == LOGLEVEL_ERROR)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error",
								 State->Log.Log_Queue[_Index].Buffer, 0);
	}

	else if (State->Log.Log_Queue[_Index].Level == LOGLEVEL_FATAL)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Fatal",
								 State->Log.Log_Queue[_Index].Buffer, 0);
	}

	else
	{
		return;
	}
}

void Log_Quit(void)
{
	ASC_Log(LOGLEVEL_INFO, "LOG: Quitting");

	int BusyCounter = 0;
	int Busy = DEF_LOG_QUEUE_COUNT;

	while (Busy)
	{
		for (int i = 0; i < DEF_LOG_QUEUE_COUNT; i++)
		{
			if (State->Log.Log_Queue[i].Status == LOGSTATUS_DONE)
			{
				Log_ClearBuffer(i);
			}

			if (State->Log.Log_Queue[i].Status == LOGSTATUS_NONE)
			{
				Busy--;
			}
		}

		if (!DEF_MULTITHREAD)
		{
			Busy = 0;
		}
		
		if (Busy > 0)
		{
			Busy = DEF_LOG_QUEUE_COUNT;
		}

		//TODO: make this better, maybe a timer or something? or just make sure it cant get stuck
		BusyCounter++;

		if (BusyCounter > INT_MAX)
		{
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Stuck Thread!",
									 "Seems like Log thread got stuck. Force quitting!", 0);
			exit(1);
		}
	}

	State->Log.LogRunning = 0;

	Util_CopyFile(State->Log.LogFilePath, State->Log.LastLogFilePath);
	Util_RemoveFile(State->Log.LogFilePath);
}

int Log_ThreadFn(void* _Data)
{
	ASC_S_Log* Log = (ASC_S_Log*)_Data;

	int BusyQueue = DEF_LOG_QUEUE_COUNT;

	while (BusyQueue)
	{
		BusyQueue = DEF_LOG_QUEUE_COUNT;

		for (int i = 0; i < DEF_LOG_QUEUE_COUNT; i++)
		{
			if (Log->Log_Queue[i].Status == LOGSTATUS_WAITING)
			{
				if (Log->ConsoleLevel <= Log->Log_Queue[i].Level) Log_ToConsole(i);
				if (Log->FileLevel <= Log->Log_Queue[i].Level) Log_ToFile(i);
				Log->Log_Queue[i].Status = LOGSTATUS_DONE;
			}

			if (Log->Log_Queue[i].Status == LOGSTATUS_DONE)
			{
				Log_ClearBuffer(i);
			}

			if (Log->Log_Queue[i].Status == LOGSTATUS_NONE ||
				Log->Log_Queue[i].Status == LOGSTATUS_DONE)
			{
				BusyQueue--;
			}
		}

		if (!DEF_MULTITHREAD)
		{
			BusyQueue = 0;
		}
	}

	Log->LogRunning = 0;
	return 0;
}

bool ASC_Log(int _Level, const char* _Fmt, ...)
{
	if (_Level < LOGLEVEL_DEBUG || _Level > LOGLEVEL_FATAL)
	{
		return 0;
	}

	if (State->Log.ConsoleLevel > _Level &&
		State->Log.FileLevel > _Level &&
		State->Log.PopupLevel > _Level)
	{
		return 0;
	}

	bool FreeQueueSlotFound = 0;

	while (!FreeQueueSlotFound)
	{
		for (int i = 0; i < DEF_LOG_QUEUE_COUNT; i++)
		{
			if (State->Log.Log_Queue[i].Status == LOGSTATUS_NONE)
			{
				FreeQueueSlotFound = 1;

				State->Log.Log_Queue[i].Status = LOGSTATUS_STARTED;
				State->Log.Log_Queue[i].Level = _Level;

				va_list args;
				va_start(args, _Fmt);
				SDL_vsnprintf(State->Log.Log_Queue[i].Buffer, DEF_LOG_BUFFER_SIZE, _Fmt, args);
				va_end(args);

				if (State->Log.PopupLevel <= _Level) Log_ToPopup(i);

				State->Log.Log_Queue[i].Status = LOGSTATUS_WAITING;

				break;
			}
		}

		if (!FreeQueueSlotFound)
		{
			if (!DEF_MULTITHREAD)
			{
				return 0;
			}

			char DateStamp[24] = { 0 };
			Util_TimeFormat(DateStamp, "[%yyy-%M-%D %h:%m:%s]");
			fprintf(stderr, "%s (WARN) LOG: Log Queue Full!\n", DateStamp);
		}
	}

	if (!State->Log.LogRunning)
	{
		State->Log.LogRunning = 1;

		if (!DEF_MULTITHREAD)
		{
			Log_ThreadFn((void*)&State->Log);
			State->Log.LogRunning = 0;
		}

		else
		{
			SDL_Thread* Thread = SDL_CreateThread(Log_ThreadFn, "ASC_Thread_Log", (void*)&State->Log);
			SDL_DetachThread(Thread);
		}
	}

	if (_Level == LOGLEVEL_FATAL)
	{
		int BusyQueue = DEF_LOG_QUEUE_COUNT;
		
		while (BusyQueue)
		{
			BusyQueue = DEF_LOG_QUEUE_COUNT;

			for (int i = 0; i < DEF_LOG_QUEUE_COUNT; i++)
			{
				if (State->Log.Log_Queue[i].Status == LOGSTATUS_NONE ||
					State->Log.Log_Queue[i].Status == LOGSTATUS_DONE)
				{
					BusyQueue--;
				}
			}

			if (!DEF_MULTITHREAD)
			{
				exit(1);
			}
		}

		exit(1);
	}

	return 1;
}

bool Log_Init(void)
{
	State->Log.LogRunning = 0;

#if _DEBUG
	State->Log.ConsoleLevel = LOGLEVEL_ALL;
	State->Log.FileLevel = LOGLEVEL_INFO;
	State->Log.PopupLevel = LOGLEVEL_ERROR;
#else
	State->Log.ConsoleLevel = LOGLEVEL_WARN;
	State->Log.FileLevel = LOGLEVEL_ERROR;
	State->Log.PopupLevel = LOGLEVEL_FATAL;
#endif

	ASC_Log(LOGLEVEL_INFO, "LOG: Initialized");

	return 1;
}
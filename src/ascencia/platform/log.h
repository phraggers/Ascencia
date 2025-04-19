// Ascencia
// platform/log.h

#pragma once

#define LOG_PRINT(Prefix, Message) Core->Log.Print(Prefix, Message)
#define LOG_DEBUG(Prefix, Message) Core->Log.Log(LOGLEVEL_DEBUG, Prefix, Message)
#define LOG_INFO(Prefix, Message) Core->Log.Log(LOGLEVEL_INFO, Prefix, Message)
#define LOG_WARNING(Prefix, Message) Core->Log.Log(LOGLEVEL_WARNING, Prefix, Message)
#define LOG_ERROR(Prefix, Message) Core->Log.Log(LOGLEVEL_ERROR, Prefix, Message)
#define LOG_FATAL(Prefix, Message) Core->Log.Log(LOGLEVEL_FATAL, Prefix, Message)

enum eLogLevel
{
	LOGLEVEL_NONE = 0,
	LOGLEVEL_DEBUG,
	LOGLEVEL_INFO,
	LOGLEVEL_WARNING,
	LOGLEVEL_ERROR,
	LOGLEVEL_FATAL,
	LOGLEVEL_MAX
};

struct sLogState
{
	eLogLevel ConsoleLogLevel;
	eLogLevel LogFileLogLevel;
};

class cLog
{
	private:

	bool IsErrorLogged;
	SDL_Semaphore* Lock;

	public:

	sLogState State;
	std::string LogFilePath;
	std::string LogDirPath;

	cLog();
	bool Init(void);
	void Quit(void);
	void Print(std::string Prefix, std::string Message); // always print to console & file, ignoring level
	void Log(eLogLevel LogLevel, std::string Prefix, std::string Message);
};
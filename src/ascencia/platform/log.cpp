// Ascencia
// platform/log.cpp

#include <ascencia/platform/core.h>

#define LOGTAG_PRINT "[-ALL-]"
#define LOGTAG_DEBUG "[DEBUG]"
#define LOGTAG_INFO "[INFO_]"
#define LOGTAG_WARNING "[WARNG]"
#define LOGTAG_ERROR "[ERROR]"
#define LOGTAG_FATAL "[FATAL]"

#if 0
typedef struct SDL_DateTime
{
	int year;                  /**< Year */
	int month;                 /**< Month [01-12] */
	int day;                   /**< Day of the month [01-31] */
	int hour;                  /**< Hour [0-23] */
	int minute;                /**< Minute [0-59] */
	int second;                /**< Seconds [0-60] */
	int nanosecond;            /**< Nanoseconds [0-999999999] */
	int day_of_week;           /**< Day of the week [0-6] (0 being Sunday) */
	int utc_offset;            /**< Seconds east of UTC */
} SDL_DateTime;
#endif

static std::string LOG_TimeStamp(void)
{
	std::string Result;
	SDL_Time TimeNow;
	SDL_DateTime DateTimeNow;
	SDL_GetCurrentTime(&TimeNow);
	SDL_TimeToDateTime(TimeNow, &DateTimeNow, 1);
	std::stringstream ss;
	ss << std::setfill('0') << std::setw(2) << DateTimeNow.hour << ":";
	ss << std::setfill('0') << std::setw(2) << DateTimeNow.minute << ":";
	ss << std::setfill('0') << std::setw(2) << DateTimeNow.second;
	Result = std::string(ss.str());
	return Result;
}

static void LOG_ConsolePrint(eLogLevel Level, std::string TimeStamp, std::string Prefix, std::string Message)
{
#if(BUILD_DEBUG)
	switch (Level)
	{
		case LOGLEVEL_MAX: std::cout << " " << termcolor::white << LOGTAG_PRINT << termcolor::reset; break;
		case LOGLEVEL_DEBUG: std::cout << " " << termcolor::bright_grey << LOGTAG_DEBUG << termcolor::reset; break;
		case LOGLEVEL_INFO: std::cout << " " << termcolor::bright_green << LOGTAG_INFO << termcolor::reset; break;
		case LOGLEVEL_WARNING: std::cout << " " << termcolor::bright_yellow << LOGTAG_WARNING << termcolor::reset; break;
		case LOGLEVEL_ERROR: std::cout << " " << termcolor::bright_red << LOGTAG_ERROR << termcolor::reset; break;
		case LOGLEVEL_FATAL: std::cout << " " << termcolor::white << termcolor::on_red << LOGTAG_FATAL << termcolor::reset; break;
	}

	std::cout << " " << termcolor::yellow << TimeStamp << termcolor::reset 
		<< termcolor::bright_blue << " [" << Prefix << "] " << termcolor::reset;

	switch (Level)
	{
		case LOGLEVEL_MAX: std::cout << termcolor::white << Message; break;
		case LOGLEVEL_DEBUG: std::cout << termcolor::bright_grey << Message; break;
		case LOGLEVEL_INFO: std::cout << termcolor::bright_green << Message; break;
		case LOGLEVEL_WARNING: std::cout << termcolor::bright_yellow << Message; break;
		case LOGLEVEL_ERROR: std::cout << termcolor::bright_red << Message; break;
		case LOGLEVEL_FATAL: std::cout << termcolor::white << termcolor::on_red << Message; break;
	}

	std::cout << termcolor::reset << std::endl;
#endif
}

static void LOG_LogFilePrint(std::string Message)
{
	std::ofstream LogFile;
	LogFile.open(Core->Log.LogFilePath, std::ios::out | std::ios::app);
	if (LogFile.is_open())
	{
		LogFile << Message << std::endl;
		LogFile.close();
	}
}

cLog::cLog()
{
	memset(&State, 0, sizeof(sLogState));
	IsErrorLogged = 0;
	Lock = 0;
	State.ConsoleLogLevel = CFG_CONSOLELOGLEVEL;
	State.LogFileLogLevel = CFG_LOGFILELOGLEVEL;
	
	const char* SDLBasePath = SDL_GetBasePath();
	LogDirPath = std::string(SDLBasePath);
	LogDirPath.append("Log");
	std::filesystem::create_directory(LogDirPath);
	if (SDLBasePath[strlen(SDLBasePath)] == '/') LogDirPath.append("/");
	else LogDirPath.append("\\");

	LogFilePath = std::string(LogDirPath);

	SDL_Time TimeNow;
	SDL_DateTime DateTimeNow;
	SDL_GetCurrentTime(&TimeNow);
	SDL_TimeToDateTime(TimeNow, &DateTimeNow, 1);
	{
		std::stringstream ss;
		ss << DateTimeNow.year;
		ss << std::setfill('0') << std::setw(2) << DateTimeNow.month;
		ss << std::setfill('0') << std::setw(2) << DateTimeNow.day;
		ss << std::setfill('0') << std::setw(2) << DateTimeNow.hour;
		ss << std::setfill('0') << std::setw(2) << DateTimeNow.minute;
		ss << std::setfill('0') << std::setw(2) << DateTimeNow.second << "_Log.txt";
		LogFilePath.append(ss.str());
	}

	std::string LogFileHeader;
	{
		std::stringstream ss;
		ss << " | =-=-=-=-= | " << CFG_APP_NAME << " LogFile | ";
		ss << std::setfill('0') << std::setw(2) << DateTimeNow.day << "/";
		ss << std::setfill('0') << std::setw(2) << DateTimeNow.month << "/";
		ss << DateTimeNow.year << " | ";
		ss << std::setfill('0') << std::setw(2) << DateTimeNow.hour << ":";
		ss << std::setfill('0') << std::setw(2) << DateTimeNow.minute << ":";
		ss << std::setfill('0') << std::setw(2) << DateTimeNow.second;
		ss << " | =-=-=-=-= |\n\n";
		LogFileHeader.append(ss.str());
	}

	std::ofstream LogFile;
	LogFile.open(LogFilePath, std::ios::out);
	if (LogFile.is_open())
	{
		LogFile << LogFileHeader;
		LogFile.close();
	}
}

bool cLog::Init(void)
{
	Lock = SDL_CreateSemaphore(1);
	if (!Lock)
	{
		LOG_FATAL("Log::Init", std::string("Error creating Log Lock Semaphore: ", SDL_GetError()));
		return 0;
	}

	return 1;
}

void cLog::Quit()
{
	if (!IsErrorLogged)
	{
		std::string LastRunPath = LogDirPath;
		LastRunPath.append("LastRun.txt");
		if (std::filesystem::exists(LastRunPath))
		{
			std::filesystem::remove(LastRunPath);
		}
		std::filesystem::copy_file(std::filesystem::path(LogFilePath), std::filesystem::path(LastRunPath));
		std::filesystem::remove(LogFilePath);
	}
}

#define LOG_LOCK if (Lock) {SDL_WaitSemaphore(Lock);}
#define LOG_UNLOCK 	if (Lock) {SDL_SignalSemaphore(Lock);}

void cLog::Print(std::string Prefix, std::string Message)
{
	LOG_LOCK;

	std::string TimeStamp = LOG_TimeStamp();
	std::string Tag = std::string(LOGTAG_PRINT);
	LOG_ConsolePrint(LOGLEVEL_MAX, TimeStamp, Prefix, Message);

	std::string Format = " ";
	Format.append(Tag);
	Format.append(" ");
	Format.append(TimeStamp);
	Format.append(" [");
	Format.append(Prefix);
	Format.append("] ");
	Format.append(Message);
	LOG_LogFilePrint(Format);

	LOG_UNLOCK;
}

void cLog::Log(eLogLevel LogLevel, std::string Prefix, std::string Message)
{
	LOG_LOCK;

	if (State.ConsoleLogLevel > LogLevel && State.LogFileLogLevel > LogLevel)
	{
		LOG_UNLOCK;
		return;
	}

	if (LogLevel >= LOGLEVEL_ERROR)
	{
		IsErrorLogged = 1;
	}

	if (Message.back() == '\n')
	{
		Message.pop_back();
	}

	std::string TimeStamp = LOG_TimeStamp();

	if (State.ConsoleLogLevel <= LogLevel)
	{
		LOG_ConsolePrint(LogLevel, TimeStamp, Prefix, Message);
	}
	if (State.LogFileLogLevel <= LogLevel)
	{
		std::string Format = " ";
		switch (LogLevel)
		{
			case LOGLEVEL_DEBUG: Format.append(LOGTAG_DEBUG); break;
			case LOGLEVEL_INFO: Format.append(LOGTAG_INFO); break;
			case LOGLEVEL_WARNING: Format.append(LOGTAG_WARNING); break;
			case LOGLEVEL_ERROR: Format.append(LOGTAG_ERROR); break;
			case LOGLEVEL_FATAL: Format.append(LOGTAG_FATAL); break;
		}
		Format.append(" ");
		Format.append(TimeStamp);
		Format.append(" [");
		Format.append(Prefix);
		Format.append("] ");
		Format.append(Message);
		LOG_LogFilePrint(Format);
	}

	LOG_UNLOCK;
}
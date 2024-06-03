/*============================================================
 * Project: Ascencia
 * File: log.h
 * Author: Phraggers
 * Date: Mon May 13 2024
============================================================*/

#ifdef ASC_HEAD

#define GLOGPATH_LEN 512
static char *gLogPath = 0;

static bool ASC_LogInit(void);
static void ASC_LogQuit(void);

#if ASC_DEBUG
static void ASC_Fatal_(const cstr file, const int line, const cstr format, ...);
static void ASC_Error_(const cstr file, const int line, const cstr format, ...);
static void ASC_InfoLog_(const cstr file, const int line, const cstr format, ...);
static void ASC_DebugLog_(const cstr file, const int line, const cstr format, ...);
#define ASC_Fatal(...) ASC_Fatal_(__FILE__, __LINE__, __VA_ARGS__)
#define ASC_Error(...) ASC_Error_(__FILE__, __LINE__, __VA_ARGS__)
#define ASC_InfoLog(...) ASC_InfoLog_(__FILE__, __LINE__, __VA_ARGS__)
#define ASC_DebugLog(...) ASC_DebugLog_(__FILE__, __LINE__, __VA_ARGS__)
#else
static void ASC_Fatal_(const cstr format, ...);
static void ASC_Error_(const cstr format, ...);
static void ASC_InfoLog_(const cstr format, ...);
#define ASC_Fatal(...) ASC_Fatal_(__VA_ARGS__)
#define ASC_Error(...) ASC_Error_(__VA_ARGS__)
#define ASC_InfoLog(...) ASC_InfoLog_(__VA_ARGS__)
#define ASC_DebugLog(...)
#endif

#endif

/*========================================================================================*/

#ifdef ASC_IMPL

static void ASC_LogToFile(const char *msg)
{
    if(!gLogPath) return;

    SDL_Time time;
    SDL_GetCurrentTime(&time);
    SDL_TimeToDateTime(time, &gApp->clock, 1);

    FILE *logFile = 0;
    logFile = fopen(gLogPath, "a");

    if(logFile)
    {
        fprintf(logFile, "[%02d:%02d:%02d] %s", gApp->clock.hour, gApp->clock.minute, gApp->clock.second, msg);
        fflush(logFile);
        fclose(logFile);
    }
}

static bool ASC_LogInit(void)
{
    gLogPath = (char*)malloc(GLOGPATH_LEN);

    memset(gLogPath, 0, GLOGPATH_LEN);
    strcpy(gLogPath, gApp->config.pref_path);
    ASC_UpdateClock();

    //"yyyy-mm-dd_hh-mm-ss_log.txt"
    char dateStamp[29] = {0};
    sprintf(dateStamp, "%04d-%02d-%02d_%02d-%02d-%02d_log.txt", gApp->clock.year, gApp->clock.month, gApp->clock.day, gApp->clock.hour, gApp->clock.minute, gApp->clock.second);
    strcat(gLogPath, dateStamp);

    FILE *logFile = 0;
    logFile = fopen(gLogPath, "w");

    if(logFile)
    {
        fprintf(logFile, "[%02d:%02d:%02d] Log Init\n", gApp->clock.hour, gApp->clock.minute, gApp->clock.second);
        fflush(logFile);
        fclose(logFile);
    }

    return 1;
}

static void ASC_LogQuit(void)
{
    if(gLogPath)
    {
        remove(gLogPath);
        free(gLogPath);
        gLogPath = 0;
    }
}

#if ASC_DEBUG
static void ASC_Fatal_(const cstr file, const int line, const cstr format, ...)
#else
static void ASC_Fatal_(const cstr format, ...)
#endif
{
    char buffer[1024] = {0};
    cstr filesub = strstr(file, "src");
    
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, 1024, format, args);
    va_end(args);

    #if ASC_DEBUG
    fprintf(stderr, "\033[1;31m[FATAL]\033[0m [%02d:%02d:%02d] [\033[0;33m%s:\033[0m \033[0;36m%d\033[0m]: %s\n", gApp->clock.hour, gApp->clock.minute, gApp->clock.second, filesub, line, buffer);
    #else
    fprintf(stderr, buffer);
    #endif
    fflush(stderr);

    ASC_LogToFile(buffer);
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "FATAL ERROR", buffer, 0);
    exit(-1);
}

#if ASC_DEBUG
static void ASC_Error_(const cstr file, const int line, const cstr format, ...)
#else
static void ASC_Error_(const cstr format, ...)
#endif
{
    char buffer[1024] = {0};
    cstr filesub = strstr(file, "src");
    
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, 1024, format, args);
    va_end(args);

    #if ASC_DEBUG
    fprintf(stderr, "\033[1;31m[ERROR]\033[0m [%02d:%02d:%02d] [\033[0;33m%s:\033[0m \033[0;36m%d\033[0m]: %s\n", gApp->clock.hour, gApp->clock.minute, gApp->clock.second, filesub, line, buffer);
    #else
    fprintf(stderr, buffer);
    #endif
    fflush(stderr);

    ASC_LogToFile(buffer);
}

#if ASC_DEBUG
static void ASC_InfoLog_(const cstr file, const int line, const cstr format, ...)
#else
static void ASC_InfoLog_(const cstr format, ...)
#endif
{
    char buffer[1024] = {0};
    cstr filesub = strstr(file, "src");
    
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, 1024, format, args);
    va_end(args);

    #if ASC_DEBUG
    fprintf(stdout, "\033[1;32m[INFO]\033[0m [%02d:%02d:%02d] [\033[0;33m%s:\033[0m \033[0;36m%d\033[0m]: %s\n", gApp->clock.hour, gApp->clock.minute, gApp->clock.second, filesub, line, buffer);
    #else
    fprintf(stdout, buffer);
    #endif
    fflush(stdout);

    ASC_LogToFile(buffer);
}

#if ASC_DEBUG
static void ASC_DebugLog_(const cstr file, const int line, const cstr format, ...)
{
    char buffer[1024] = {0};
    cstr filesub = strstr(file, "src");
    
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, 1024, format, args);
    va_end(args);

    fprintf(stdout, "\033[1;36m[DEBUG]\033[0m [%02d:%02d:%02d] [\033[0;33m%s:\033[0m \033[0;36m%d\033[0m]: %s\n", gApp->clock.hour, gApp->clock.minute, gApp->clock.second, filesub, line, buffer);
    fflush(stdout);
}
#endif

#endif //LOG_H_
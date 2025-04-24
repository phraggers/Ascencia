/* ==============================================================
   File:    log.c
   Author:  Phraggers
   Date:    21-04-2025
   ============================================================== */

typedef struct _CORE_sLogEntry
{
    char string[0xff];
    struct _CORE_sLogEntry *next;
} CORE_sLogEntry;

typedef struct
{
    PL_Mutex lock;
    PL_sThread *thread;
    CORE_sLogEntry *log_list;
    char log_file[0xff];
} CORE_sLogState;

static CORE_sLogState *gLogState;

static void CORE_LogAddEntry(char *string)
{
    if(gLogState && string)
    {
        //NOTE: locks mutex so we can multithread this without writing list out of order
        if(PL_WaitMutex(&gLogState->lock, 0))
        {
            CORE_sLogEntry *entry = (CORE_sLogEntry*)PL_Alloc0(sizeof(CORE_sLogEntry));
            if(entry)
            {
                if(gLogState->log_list)
                {
                    CORE_sLogEntry *prev = gLogState->log_list;
                    while(prev->next)
                    {
                        prev = prev->next;
                    }
                    prev->next = entry;
                }
                else
                {
                    gLogState->log_list = entry;
                }

                strcpy(entry->string, string);
            }

            PL_UnlockMutex(&gLogState->lock);
        }
    }
}

static void CORE_LogRemoveEntry(void)
{
    if(gLogState->log_list)
    {
        PL_WaitMutex(&gLogState->lock, 0);
        CORE_sLogEntry *entry = gLogState->log_list->next;
        PL_Free(gLogState->log_list);
        gLogState->log_list = 0;
        if(entry)
        {
            gLogState->log_list = entry;
        }
        PL_UnlockMutex(&gLogState->lock);
    }
}

static void CORE_LogPrintConsole(char *string)
{
    //TODO: platform print fn
    printf("%s\n", string);
}

static void CORE_LogPrintFile(char *string)
{
    //TODO: write log to file
}

PL_THREADFN(CORE_LogThreadFn)
{
    while(gState->running)
    {
        if(!gLogState)
        {
            return 1;
        }

        if(gLogState->log_list)
        {
            //TODO: log level
            if(1)
            {
                CORE_LogPrintConsole(gLogState->log_list->string);
            }

            if(1)
            {
                CORE_LogPrintFile(gLogState->log_list->string);
            }
        }
    }

    return 0;
}

bool CORE_LogInit(void)
{
    gLogState = (CORE_sLogState*)PL_Alloc0(sizeof(CORE_sLogState));
    if(!gLogState)
    {
        return 0;
    }

    strcpy(gLogState->log_file, PL_GetBasePath());
    //TODO: timestamp
    strcat(gLogState->log_file, "_log.txt");

    FILE* logfile = fopen(gLogState->log_file, "w");
    fprintf(logfile, " =-=-=-=-= ASCENCIA LogFile =-=-=-=-=-=\n");
    fclose(logfile);

    gLogState->lock = PL_CreateMutex();
    gLogState->thread = PL_CreateThread(CORE_LogThreadFn, 0);

    return 1;
}

void CORE_LogQuit(void)
{
    if(gLogState && gLogState->thread)
    {
        PL_CloseThread(gLogState->thread, 0);
    }

    char *ofilepath = (char*)PL_Alloc0(0xff);
    strcpy(ofilepath, PL_GetBasePath());
    strcat(ofilepath, "LastRunLog.txt");

    FILE *ifile = fopen(gLogState->log_file, "rb");
    if(ifile)
    {
        long size;
        fseek(ifile, 0, SEEK_END);
        size = ftell(ifile);
        fseek(ifile, 0, SEEK_SET);
        char *buffer = (char*)PL_Alloc0(size);
        fread(buffer, 1, size, ifile);
        fclose(ifile);

        FILE *ofile = fopen(ofilepath, "wb");
        if(ofile)
        {
            fwrite(buffer, 1, size, ofile);
            fclose(ofile);
        }

        PL_Free(buffer);
    }


    remove(gLogState->log_file);
}

void CORE_LogDebug(char *string, ...)
{
#if defined(DEBUG)

#endif
}

void CORE_LogInfo(char *string, ...)
{

}

void CORE_LogWarning(char *string, ...)
{

}

void CORE_LogError(char *string, ...)
{

}

void CORE_LogFatal(char *string, ...)
{

}

void CORE_LogPrint(char *string, ...)
{

}

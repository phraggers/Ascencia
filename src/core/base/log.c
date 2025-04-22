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
    bool lock;
    CORE_sLogEntry *log_list;
} CORE_sLogState;

static CORE_sLogState *gLogState;

static void CORE_LogAddEntry(char *string)
{
    if(gLogState && string)
    {
        gLogState->lock = 1;

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

        gLogState->lock = 0;
    }
}

bool CORE_LogInit()
{
    gLogState = (CORE_sLogState*)PL_Alloc0(sizeof(CORE_sLogState));
    if(!gLogState)
    {
        return 0;
    }

    return 1;
}

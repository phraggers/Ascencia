/* ==============================================================
   File:    thread.c
   Author:  Phraggers
   Date:    21-04-2025
   ============================================================== */

PL_sThread *PL_CreateThread(PL_fnThread *function, void *user_data)
{
    PL_sThread *result = (PL_sThread*)PL_Alloc(sizeof(PL_sThread));
    if(!result)
    {
        return 0;
    }

    result->handle = (void*)CreateThread(0, 0, function, 0, 0, (DWORD*)&result->id);
    if(!result->handle)
    {
        PL_Free(result);
        return 0;
    }

    return result;
}

b32 PL_CloseThread(PL_sThread *thread, unsigned long *return_value)
{
    if(!thread)
    {
        return 0;
    }

    DWORD ecode = 0;
    WaitForSingleObject((HANDLE)thread->handle, 0);
    if(!GetExitCodeThread((HANDLE)thread->handle, &ecode))
    {
        return 0;
    }

    int attempts = 100;
    while(ecode == 259 && attempts >= 0)
    {
        WaitForSingleObject((HANDLE)thread->handle, 15);
        if(!GetExitCodeThread((HANDLE)thread->handle, &ecode))
        {
            return 0;
        }
    }

    if(ecode)
    {
        if(attempts < 0)
        {
            return 0; //thread hanging
        }
        else
        {
            return 0; //other error
        }
    }

    if(return_value)
    {
        *return_value = (unsigned long)ecode;
    }

    PL_Free(thread);
    return 1;
}

PL_Mutex PL_CreateMutex(void)
{
    PL_Mutex result = {0};
    result.handle = (void*)CreateMutexA(0, 0, 0);
    return result;
}

void PL_DestroyMutex(PL_Mutex *mutex)
{
    if(mutex)
    {
        CloseHandle((HANDLE)mutex->handle);
    }
}

b32 PL_WaitMutex(PL_Mutex *mutex, u32 wait_ms)
{
    if(mutex)
    {
        DWORD ms = (wait_ms == 0) ? INFINITE : wait_ms;
        DWORD result = WaitForSingleObject((HANDLE)mutex->handle, ms);

        if(result == WAIT_OBJECT_0)
        {
            return 1;
        }
    }

    return 0;
}

b32 PL_UnlockMutex(PL_Mutex *mutex)
{
    if(mutex)
    {
        BOOL result = ReleaseMutex((HANDLE)mutex->handle);

        if(result)
        {
            return 1;
        }
    }

    return 0;
}

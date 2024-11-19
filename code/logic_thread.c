/* ==============================================================
   Project: Ascencia
   File:    logic_thread.c
   Author:  Phraggers
   Date:    18-11-2024
   ============================================================== */

#include "platform.h"

DWORD WINAPI LogicThread(LPVOID param)
{
    g_state->logic_thread.mutex = CreateMutexA(0, 0, 0);
    WaitForSingleObject(g_state->logic_thread.mutex, INFINITE);
    ReleaseMutex(g_state->logic_thread.mutex);

    r32 target_ms = 1000.0f / (r32)PL_LOGIC_THREAD_HZ;
    u64 timer = PL_QueryTimer();
    r32 elapsed_ms = 0.0f;

    while(g_state->running)
    {
        timer = PL_QueryTimer();


        elapsed_ms = (PL_TimeElapsed(timer, PL_QueryTimer()) * 1000.0f);
        if((u32)elapsed_ms < target_ms)
        {
            Sleep((u32)(target_ms - elapsed_ms));
        }
    }

    return 0;
}

b32 PL_StartLogicThread(void)
{
    sThread *thread = &g_state->logic_thread;
    thread->handle = CreateThread(0, 0, LogicThread, 0, 0, &thread->id);
    if(!thread->handle) return 0;
    return 1;
}

b32 PL_QuitLogicThread(void)
{
    sThread *thread = &g_state->logic_thread;
    WaitForSingleObject(thread->handle, 0);
    DWORD ecode = 0;
    if(!GetExitCodeThread(thread->handle, &ecode)) return 0;

    int attempts = 100;
    while(ecode == 259 && attempts >= 0)
    {
        WaitForSingleObject(thread->handle, 15);
        if(!GetExitCodeThread(thread->handle, &ecode)) return 0;
    }

    if(ecode)
    {
        if(attempts < 0) return 0; //thread hanging
        else return 0; //error
    }

    CloseHandle(g_state->logic_thread.mutex);
    return 1;
}

/*============================================================
 * Project: Ascencia
 * File: win32_threads.c
 * Author: Phraggers
 * Date: Tue Sep 24 2024
============================================================*/

#include <util/types.h>
#include <platform/threads.h>
#include <platform/timing.h>
#include <win32/win32_api.h>
#include <win32/win32_state.h>

//typedef u32 (*WIN32_THREAD_START_ROUTINE)(ptr param);
//typedef ptr (*pfn_CreateThread)(ptr sec, u64 stack_size, WIN32_THREAD_START_ROUTINE start_address, ptr param, u32 flags, u32 *thread_id);

/* Threads:
    - main (window, input)
    - gfx (graphics, physics)
    - world (entities and creatures logic)
*/

/* TODO: NEED THREADSAFE LOGGING
*/

u32 Routine_GFX(ptr param);
u32 Routine_WORLD(ptr param);

bool PL_ThreadsInit(void)
{
    Assert(G_win32_state);

    PL_Log(LOG_INFO, "[MAIN] Starting GFX Thread...");
    G_win32_state->threads.thread_gfx.thread = WIN_API.CreateThread(0, 0, &Routine_GFX, &G_win32_state->threads, 0, 0);

    if(!G_win32_state->threads.thread_gfx.thread)
    {
        PL_Log(LOG_ERROR, "[MAIN] ERROR starting GFX Thread");
        return 0;
    }

    PL_Log(LOG_INFO, "[MAIN] Starting WORLD Thread...");
    G_win32_state->threads.thread_world.thread = WIN_API.CreateThread(0, 0, &Routine_WORLD, &G_win32_state->threads, 0, 0);

    if(!G_win32_state->threads.thread_world.thread)
    {
        PL_Log(LOG_ERROR, "[MAIN] ERROR starting WORLD Thread");
        return 0;
    }

    return 1;
}

void PL_UpdateThreads(void)
{
    while(THREAD_TRANSFER.lock) {}
    THREAD_TRANSFER.lock = 1;
    THREAD_TRANSFER.update_GFX = 1;
    THREAD_TRANSFER.update_WORLD = 1;
    THREAD_TRANSFER.lock = 0;
}

u32 Routine_GFX(ptr param)
{
    PL_Threads *THREADS_STRUCT = (PL_Threads*)param;
    PL_ThreadTransfer *TRANSFER = &THREADS_STRUCT->thread_transfer;
    Thread_GFX *GFX = &THREADS_STRUCT->thread_gfx;

    //PL_Log(LOG_INFO, "[GFX] Thread Started");
    GFX->running = 1;

    while(GFX->running)
    {
        if(G_win32_state->running != GFX->running)
        {
            GFX->running = G_win32_state->running;
        }

        while(!TRANSFER->update_GFX)
        {
            //TODO: Sleep
        }
        
    }

    //PL_Log(LOG_INFO, "[GFX] Thread Quit");
    return 0;
}

u32 Routine_WORLD(ptr param)
{
    PL_Threads *THREADS_STRUCT = (PL_Threads*)param;
    PL_ThreadTransfer *TRANSFER = &THREADS_STRUCT->thread_transfer;
    Thread_WORLD *WORLD = &THREADS_STRUCT->thread_world;

    //PL_Log(LOG_INFO, "[WORLD] Thread Started");
    WORLD->running = 1;

    while(WORLD->running)
    {
        if(G_win32_state->running != WORLD->running)
        {
            WORLD->running = G_win32_state->running;
        }

        while(!TRANSFER->update_WORLD)
        {
            //TODO: Sleep
        }
    }

    //PL_Log(LOG_INFO, "[WORLD] Thread Quit");
    return 0;
}

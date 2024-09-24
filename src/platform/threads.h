/*============================================================
 * Project: Ascencia
 * File: threads.h
 * Author: Phraggers
 * Date: Tue Sep 24 2024
============================================================*/

#ifndef ASCENCIA_THREADS_H
#define ASCENCIA_THREADS_H

typedef struct
{
    bool lock;
    bool update_GFX;
    bool update_WORLD;
} PL_ThreadTransfer;

typedef struct
{
    bool running;
    ptr thread;
} Thread_GFX;

typedef struct
{
    bool running;
    ptr thread;
} Thread_WORLD;

typedef struct
{
    PL_ThreadTransfer thread_transfer;
    Thread_GFX thread_gfx;
    Thread_WORLD thread_world;
} PL_Threads;

#define THREAD_TRANSFER G_win32_state->threads.thread_transfer

bool PL_ThreadsInit(void);
void PL_UpdateThreads(void);

#endif /* ASCENCIA_THREADS_H */
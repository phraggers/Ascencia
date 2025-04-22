#ifndef THREAD_H
/* ==============================================================
   File:    thread.h
   Author:  Phraggers
   Date:    21-04-2025
   ============================================================== */

#if defined(WIN32)
#define PL_THREADFN_RETTYPE unsigned long
#else
#define PL_THREADFN_RETTYPE int
#endif
#define PL_THREADFN(name) PL_THREADFN_RETTYPE name (void* user_data)
typedef PL_THREADFN((PL_fnThread));

typedef struct
{
    void *handle;
} PL_Mutex;

typedef struct
{
    u32 id;
    void *handle;
} PL_sThread;

/* create thread calling PL_fnThread function, passes user_data.
   (use macro PL_THREADFN to define thread function)
   this function allocates PL_sThread.
   (struct members should be treated as opaque)
   returns 0 on error. */
PL_sThread *PL_CreateThread(PL_fnThread *function, void *user_data);

/* close thread, frees memory allocated by PL_CreateThread.
   'thread' becomes invalid pointer.
   returns success. */
b32 PL_CloseThread(PL_sThread *thread, PL_THREADFN_RETTYPE *return_value);

/* create mutex. */
PL_Mutex PL_CreateMutex(void);

/* destroy mutex */
void PL_DestroyMutex(PL_Mutex *mutex);

/* waits for mutex to be unlocked before locking and continuing.
   If wait_ms is non-zero, it only waits for that long.
   returns 1 if mutex succesfully locked, returns 0 if error or timeout occurred.*/
b32 PL_WaitMutex(PL_Mutex *mutex, u32 wait_ms);

/* unlocks mutex.
   returns success. */
b32 PL_UnlockMutex(PL_Mutex *mutex);

#define THREAD_H
#endif

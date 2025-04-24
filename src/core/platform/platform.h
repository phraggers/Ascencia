#ifndef PLATFORM_H
/* ==============================================================
   File:    platform.h
   Author:  Phraggers
   Date:    22-04-2025
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

typedef struct
{
    bool is_initialized;
} PL_sWindow;

/* alloc memblock of size, returns ptr to new block */
void *PL_Alloc(u64 size);

/* alloc memblock of size, zeroes memblock, returns ptr to new block */
void *PL_Alloc0(u64 size);

/* realloc */
void *PL_ReAlloc(void *old, u64 size);

/* realloc and zero */
void *PL_ReAlloc0(void *old, u64 size);

/* free allocated memblock */
b32 PL_Free(void *mem);

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

/* returns path to dir where this exe is.
   path always ends with '/' (or '\\' on win32). */
char* PL_GetBasePath(void);

#define PLATFORM_H
#endif

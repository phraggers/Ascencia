/*============================================================
 * Project: Ascencia
 * File: thread.h
 * Author: Phraggers
 * Date: Mon May 27 2024
============================================================*/

#ifdef ASC_HEAD

bool ASC_ThreadInit(void);
void ASC_ThreadQuit(void);
u32 ASC_GetData(void);
void ASC_SetData(u32 data);

#endif

#ifdef ASC_IMPL

bool gRun;
u32 gData;
SDL_Semaphore *gDataLock;
SDL_Semaphore *gRunLock;
SDL_Thread *workerThread;

int _WorkerThread(ptr data)
{
    bool running = 1;

    while(running)
    {
        SDL_Delay(50); // at 60fps, this is 3 frames (nice round number, no other reason)
        
        SDL_WaitSemaphore(gDataLock);
        u32 *idata;
        idata = (u32*)data;
        SDL_PostSemaphore(gDataLock);
        
        SDL_WaitSemaphore(gRunLock);
        running = gRun;
        SDL_PostSemaphore(gRunLock);
    }

    return 0;
}

bool ASC_ThreadInit(void)
{
    gDataLock = SDL_CreateSemaphore(1);
    gData = 0;
    gRunLock = SDL_CreateSemaphore(1);
    gRun = 1;
    workerThread = SDL_CreateThread(_WorkerThread, "ASC_TestThread", (ptr)&gData);
    return 1;
}

void ASC_ThreadQuit(void)
{
    SDL_WaitSemaphore(gRunLock);
    gRun = 0;
    SDL_PostSemaphore(gRunLock);

    SDL_WaitThread(workerThread, 0);
    SDL_DestroySemaphore(gDataLock);
    SDL_DestroySemaphore(gRunLock);
}

u32 ASC_GetData(void)
{
    SDL_WaitSemaphore(gDataLock);
    u32 result = gData;
    SDL_PostSemaphore(gDataLock);
    return result;
}

void ASC_SetData(u32 data)
{
    SDL_WaitSemaphore(gDataLock);
    gData = data;
    SDL_PostSemaphore(gDataLock);
}

#endif
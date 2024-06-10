/*============================================================
 * Project: Ascencia
 * File: logic.h
 * Author: Phraggers
 * Date: Mon Jun 10 2024
============================================================*/

#ifdef ASC_HEAD

struct ASC_Logic
{
    bool running;
    SDL_Thread *thread;
    SDL_Semaphore *dataLock;
};

static bool ASC_LogicInit(void);
int ASC_LogicThread(ptr data);

#endif

/*===============================================================================*/

#ifdef ASC_IMPL

static bool ASC_LogicInit(void)
{
    gApp->logic.dataLock = SDL_CreateSemaphore(1);
    gApp->logic.running = 1;
    gApp->Logic.thread = SDL_CreateThread(ASC_LogicThread, "ASC_LogicThread", (ptr)&gApp->logic);

    if(!gApp->logic.dataLock || !gApp->logic.thread)
    {
        ASC_Error("ASC_LogicInit: Error occurred while creating LogicThread, %s", SDL_GetError());
        gApp->logic.running = 0;
        return 0;
    }

    return 1;
}

int ASC_LogicThread(ptr data)
{
    ASC_InfoLog("LogicThread: Spawned");
    ASC_Logic *logic = (ASC_Logic*)data;
    bool running = logic->running;

    while(running)
    {
        SDL_Delay(15);
        SDL_WaitSemaphore(logic->dataLock);
        running = logic->running;

        //TODO: load logic dll, call init, frame functions etc
        // all logic done through dll hooks
        // need to create an interface/ include header

        SDL_PostSemaphore(logic->dataLock);
    }

    ASC_InfoLog("LogicThread: Quit");
    return 1;
}

#endif
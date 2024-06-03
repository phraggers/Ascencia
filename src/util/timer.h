/*============================================================
 * Project: Ascencia
 * File: timer.h
 * Author: Phraggers
 * Date: Tue May 14 2024
============================================================*/

#ifdef ASC_HEAD

struct ASC_Timer
{
    u64 perfFreq; // process cycles per second
    u64 frameStartPerf;
    u64 ticks; // frames since start
    r64 runTime; //seconds since SDL init
    r32 lastFrameDelta; //seconds elapsed last frame
    r32 avgFrameTime;
};

static bool ASC_TimerInit(void);
static void ASC_TimerFrameStart(void);
static void ASC_TimerFrameEnd(void);
static r32 ASC_TimerPerfDelta(u64 perf); //get seconds elapsed since given perf
static r32 ASC_TimerFrameDelta(void); //get seconds elapsed since frame start

#endif

/*====================================================================================*/

#ifdef ASC_IMPL

static bool ASC_TimerInit(void)
{
    ASC_Timer *timer = &gApp->timer;
    timer->perfFreq = SDL_GetPerformanceFrequency();
    timer->frameStartPerf = SDL_GetPerformanceCounter();
    return 1;
}

static void ASC_TimerFrameStart(void)
{
    ASC_Timer *timer = &gApp->timer;
    timer->frameStartPerf = SDL_GetPerformanceCounter();
}

static void ASC_TimerFrameEnd(void)
{
    ASC_Timer *timer = &gApp->timer;
    u64 frameEndPerf = SDL_GetPerformanceCounter();
    u64 framePerfDelta = frameEndPerf - timer->frameStartPerf;
    timer->lastFrameDelta = (r32)((r32)framePerfDelta / (r32)timer->perfFreq);

    if(!gApp->config.vsync)
    {
        r32 desiredFrameTime = 1.0f/(r32)gApp->config.fps;
        
        if(timer->lastFrameDelta < desiredFrameTime)
        {
            r32 timeToBlock = desiredFrameTime - timer->lastFrameDelta;
            r64 nsToBlock = (r64)timeToBlock * (r64)SDL_NS_PER_SECOND;
            SDL_DelayNS((u64)nsToBlock);

            frameEndPerf = SDL_GetPerformanceCounter();
            framePerfDelta = frameEndPerf - timer->frameStartPerf;
            timer->lastFrameDelta = (r32)((r32)framePerfDelta / (r32)timer->perfFreq);
        }
    }

    timer->runTime = (r64)SDL_NS_TO_SECONDS((r64)SDL_GetTicksNS());
    timer->ticks++;

    timer->avgFrameTime += timer->lastFrameDelta;
    if(timer->avgFrameTime > 0.0f) timer->avgFrameTime /= 2.0f;

    if(timer->ticks%60==0)
    {
        ASC_DebugLog("FrameTick: Elapsed: %.02fs | AvgFrameTime: %.04fs", timer->runTime, timer->avgFrameTime);
    }
}

static r32 ASC_TimerPerfDelta(u64 perf)
{
    ASC_Timer *timer = &gApp->timer;
    r32 result = 0.0f;
    u64 currentPerf = SDL_GetPerformanceCounter();
    u64 currentPerfDelta = currentPerf - perf;
    result = (r32)((r32)currentPerfDelta / (r32)timer->perfFreq);
    return result;
}

static r32 ASC_TimerFrameDelta(void)
{
    ASC_Timer *timer = &gApp->timer;
    r32 result = ASC_TimerPerfDelta(timer->frameStartPerf);
    return result;
}

#endif //TIMER_H_
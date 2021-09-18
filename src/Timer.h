#ifndef TIMER_H
/* ========================================================================
   $File:    Timer [Header] $
   $Project: Ascencia $
   $Date:    18-09-2021 $
   $Author:  Phil Bagshaw $
   $Notice:  (c)Phragware 2021 $
   ======================================================================== */

static struct
Asc_Timer
{
    uint64 T[10]; // timer slots for TimerT funcs
    uint64 FrameStart; // perf count at frame start
    uint64 FrameEnd; // perf count before frame limit
    uint64 PerfFreq; // perf count in seconds

    real64 PreviousFrameTime; // ms
    real64 TargetFrameTime; // ms
    int TargetFPS;

    //NOTE: Since Init
    uint64 TotalFrames;
    real32 AverageFPS;

    //NOTE: Sample over last 1 sec
    uint8 SampleFrames;
    uint32 SampleTicks;
    real32 SampleAverageFPS;
} Timer;

static uint8
ASC_TimerTStart(int Index)
{
    if(Index < 0 || Index > 9)
    {
        return 0;
    }

    Timer.T[Index] = SDL_GetPerformanceCounter();
    return 1;
}

static real64
ASC_TimerTDelta(int Index)
{
    if(Index < 0 || Index > 9 || Timer.T[Index] == 0)
    {
        return 0;
    }

    return (real64)((SDL_GetPerformanceCounter() - Timer.T[Index])*1000) / Timer.PerfFreq;
}

static real64
ASC_TimerTEnd(int Index)
{
    if(Index < 0 || Index > 9 || Timer.T[Index] == 0)
    {
        return 0;
    }

    real64 Result = (real64)((SDL_GetPerformanceCounter() - Timer.T[Index])*1000) / Timer.PerfFreq;
    Timer.T[Index] = 0;
    return Result;
}

static real64
ASC_TimerGetPerfElapsed(uint64 Previous)
{
    return (real64)((SDL_GetPerformanceCounter() - Previous)*1000) / Timer.PerfFreq;
}

static real64
ASC_TimerGetPerfInterval(uint64 Start, uint64 End)
{
    return (real64)((End - Start)*1000) / Timer.PerfFreq;
}

//NOTE: Called before app loop
static void
ASC_TimerInit(int TargetFPS)
{
    ZeroMem(Timer);
    Timer.FrameStart = SDL_GetPerformanceCounter();
    Timer.FrameEnd = SDL_GetPerformanceCounter();
    Timer.PerfFreq = SDL_GetPerformanceFrequency();
    Timer.TargetFrameTime = 1000.0f/TargetFPS;
    Timer.TargetFPS = TargetFPS;
}

//NOTE: Called at start of each frame
static void
ASC_TimerNewFrame()
{
    Timer.FrameStart = SDL_GetPerformanceCounter();
    Timer.TotalFrames++;
    Timer.SampleFrames++;
}

//NOTE: Called at end of each frame before limit
static void
ASC_TimerEndFrame()
{
    Timer.FrameEnd = SDL_GetPerformanceCounter();
    Timer.PreviousFrameTime = ASC_TimerGetPerfInterval(Timer.FrameStart, Timer.FrameEnd);
    Timer.AverageFPS = (real32)Timer.TotalFrames / (SDL_GetTicks()/1000.0f);
    Timer.SampleAverageFPS = ((real32)Timer.SampleFrames /
                              ((SDL_GetTicks()-Timer.SampleTicks)/1000.0f));

    if(Timer.SampleFrames == Timer.TargetFPS)
    {
        Timer.SampleFrames = 0;
        Timer.SampleTicks = SDL_GetTicks();
    }

    if(Timer.TotalFrames%(Timer.TargetFPS * 10) == 0) //print fps every 10 sec
        SDL_Log("AvgFps: %.02f/Total, %.02f/1Sec", Timer.AverageFPS, Timer.SampleAverageFPS);
}

//NOTE: called AFTER all other timer funcs
static void
ASC_TimerFrameLimit()
{
    if(ASC_TimerGetPerfInterval(Timer.FrameStart, Timer.FrameEnd) < Timer.TargetFrameTime)
    {
        SDL_Delay((uint32)(Timer.TargetFrameTime - Timer.PreviousFrameTime));
    }
}

#define TIMER_H
#endif

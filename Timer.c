#include "Ascencia.h"

static bool Timer_IsInit;

bool Timer_IsTimerInit(void)
{
    return Timer_IsInit;
}

void Timer_Init(int _TargetFPS)
{
    ASC_Log(LOGLEVEL_DEBUG, "TIMER: Initializing, Target %d FPS", _TargetFPS);
    State->Timer.TargetFPS = _TargetFPS;
    State->Timer.PerfFreq = SDL_GetPerformanceFrequency();
    State->Timer.Frame.Start = SDL_GetPerformanceCounter();
    State->Timer.Frame.End = SDL_GetPerformanceCounter();
    State->Timer.Frame.TargetTime = 1000.0f / State->Timer.TargetFPS;
    
    for (int i = 0; i < DEF_TIMER_SLOTS; i++)
    {
        State->Timer.Timers[i] = 0;
    }

    Timer_IsInit = 1;
}

inline r64 Timer_GetPerfElapsed(u64 _Previous)
{
    if (!Timer_IsInit) return 0;
    return (r64)((u64)(SDL_GetPerformanceCounter() - _Previous) * 1000.0f) / State->Timer.PerfFreq;
}

inline r64 Timer_GetPerfInterval(u64 _Start, u64 _End)
{
    if (!Timer_IsInit) return 0;
    return (r64)((u64)(_End - _Start) * 1000.0f) / State->Timer.PerfFreq;
}

void Timer_FrameStart(void)
{
    if (!Timer_IsInit) return;
    State->Timer.Frame.Start = SDL_GetPerformanceCounter();
    State->Timer.Total.Frames++;
    State->Timer.Sample.Frames++;
}

void Timer_FrameEnd(void)
{
    if (!Timer_IsInit) return;
    State->Timer.Frame.End = SDL_GetPerformanceCounter();
    State->Timer.Frame.PreviousTime = Timer_GetPerfInterval(State->Timer.Frame.Start, State->Timer.Frame.End);
    State->Timer.Total.AvgFPS = (r32)State->Timer.Total.Frames / (SDL_GetTicks() / 1000.0f);
    State->Timer.Sample.AvgFPS = ((r32)State->Timer.Sample.Frames /
                             ((SDL_GetTicks() - State->Timer.Sample.Ticks) / 1000.0f));

    if (State->Timer.Sample.Frames == State->Timer.TargetFPS)
    {
        State->Timer.Sample.Frames = 0;
        State->Timer.Sample.Ticks = SDL_GetTicks();
    }

    if (Timer_GetPerfInterval(State->Timer.Frame.Start, State->Timer.Frame.End) < State->Timer.Frame.TargetTime)
    {
        r64 SleepTime = State->Timer.Frame.TargetTime - State->Timer.Frame.PreviousTime;
        SDL_Delay((u32)SleepTime);
        SleepTime -= (u32)SleepTime; //truncate to value right of decimal point
        SleepTime += SDL_GetPerformanceCounter();
        while (SleepTime > SDL_GetPerformanceCounter()) {};
    }
}

int Timer_Add(void)
{
    if (!Timer_IsInit) return 0;

    for (int i = 1; i < DEF_TIMER_SLOTS; i++)
    {
        if (State->Timer.Timers[i] == 0)
        {
            ASC_Log(LOGLEVEL_DEBUG, "TIMER: Timer[%d] Started", i);
            State->Timer.Timers[i] = SDL_GetPerformanceCounter();
            return i;
        }
    }

    ASC_Log(LOGLEVEL_ERROR, "TIMER: Out of Timer slots!");
    return 0;
}

r64 Timer_Delta(int _ID)
{
    if (!Timer_IsInit) return 0;
    r64 Result = 0;

    if (State->Timer.Timers[_ID] > 0)
    {
        Result = Timer_GetPerfElapsed(State->Timer.Timers[_ID]);
        ASC_Log(LOGLEVEL_DEBUG, "TIMER: Timer[%d] Delta[%.02f]", Result);
    }

    else
    {
        ASC_Log(LOGLEVEL_ERROR, "TIMER: TimerDelta invalid Timer Slot [%d]", _ID);
    }

    return Result;
}

r64 Timer_End(int _ID)
{
    if (!Timer_IsInit) return 0;

    if (State->Timer.Timers[_ID] > 0)
    {
        r64 Result = Timer_GetPerfElapsed(State->Timer.Timers[_ID]);
        ASC_Log(LOGLEVEL_DEBUG, "TIMER: Timer[%d] Ended[%.02f]", _ID, Result);
        State->Timer.Timers[_ID] = 0;
        return Result;
    }

    ASC_Log(LOGLEVEL_ERROR, "TIMER: TimerEnd invalid Timer Slot [%d]", _ID);

    return 0;
}
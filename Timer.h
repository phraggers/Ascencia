
#ifndef _ASC_TIMER_H_
#define _ASC_TIMER_H_

#define TIMER_SLOTS 32

typedef struct
{
    u64 Timers[TIMER_SLOTS]; // timer slots
    u64 PerfFreq;
    int TargetFPS;

    struct
    {
        u64 Start;
        u64 End;
        r64 PreviousTime;
        r64 TargetTime;
    } Frame;

    struct
    {
        u64 Frames; // total frames since init
        r32 AvgFPS;
    } Total;

    struct
    {
        u8 Frames;
        u32 Ticks;
        r32 AvgFPS;
    } Sample; // over last second
} ASC_Timer;

bool Timer_IsTimerInit();
void Timer_Init(int _TargetFPS);
inline r64 Timer_GetPerfElapsed(u64 _Previous);
inline r64 Timer_GetPerfInterval(u64 _Start, u64 _End);
void Timer_FrameStart();
void Timer_FrameEnd();
int Timer_Add(); // returns timer ID
r64 Timer_Delta(int _ID);
r64 Timer_End(int _ID);

#endif //_ASC_TIMER_H_
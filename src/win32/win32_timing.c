/*============================================================
 * Project: Ascencia
 * File: win32_timing.c
 * Author: Phraggers
 * Date: Sat Sep 14 2024
============================================================*/

#include <util/types.h>
#include <util/maths.h>
#include <platform/timing.h>
#include <win32/win32_api.h>
#include <win32/win32_state.h>

local void InitTimer(void)
{
    Assert(G_win32_state != 0);
    Win32_LARGE_INTEGER freqResult;
    WINAPI.QueryPerformanceFrequency(&freqResult);
    G_win32_state->timer.freq = (u64)freqResult.QuadPart;
}

void PL_UpdateClock(void)
{
    if(!G_win32_state)
    {
        return;
    }

    PL_Clock *clock = &G_win32_state->clock;
    Win32_SYSTEMTIME st = {0};
    WINAPI.GetLocalTime(&st);
    clock->year = st.year;
    clock->month = (u8)st.month;
    clock->day = (u8)st.day;
    clock->wday = (u8)st.wday;
    clock->hr = (u8)st.hour;
    clock->min = (u8)st.min;
    clock->sec = (u8)st.sec;
    clock->ms = st.ms;
}

void PL_UpdateTimer(void)
{
    if(!G_win32_state)
    {
        return;
    }

    if(!PL_GetTimer()->freq)
    {
        InitTimer();
    }

    PL_GetTimer()->sec_since_last_update = PL_TimerElapsed(PL_GetTimer()->last_perf);
    PL_GetTimer()->last_perf = PL_TimerStart();
}

PL_Clock *PL_GetClock(void)
{
    if(!G_win32_state)
    {
        return 0;
    }

    PL_Clock *result = &G_win32_state->clock;
    return result;
}

PL_Timer *PL_GetTimer(void)
{
    if(!G_win32_state)
    {
        return 0;
    }

    PL_Timer *result = &G_win32_state->timer;
    return result;
}

u64 PL_TimerStart(void)
{
    if((!G_win32_state) || (!WINAPI.QueryPerformanceCounter))
    {
        return 0;
    }

    Win32_LARGE_INTEGER perf;
    WINAPI.QueryPerformanceCounter(&perf);
    return (u64)perf.QuadPart;
}

r64 PL_TimerElapsed(u64 perf)
{
    r64 result = PL_TimerElapsed2(perf, PL_TimerStart());
    return result;
}

r64 PL_TimerElapsed2(u64 start, u64 end)
{
    r64 result = divide64(((r64)end - (r64)start), (r64)PL_GetTimer()->freq);
    return result;
}
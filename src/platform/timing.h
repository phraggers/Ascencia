/*============================================================
 * Project: Ascencia
 * File: timing.h
 * Author: Phraggers
 * Date: Sat Sep 14 2024
============================================================*/

#ifndef ASCENCIA_TIMING_H
#define ASCENCIA_TIMING_H

#include <util/types.h>

typedef struct
{
    u16 year; //4 digit
    u8 month; //1-12
    u8 day; //1-31
    u8 wday; //0:sun 6:sat
    u8 hr; //0-23
    u8 min; //0-59
    u8 sec; //0-59
    u16 ms; //0-999
} PL_Clock;

typedef struct
{
    u64 freq;
    u64 last_perf;
    r64 sec_since_last_update;
} PL_Timer;

void PL_UpdateClock(void);
void PL_UpdateTimer(void);
PL_Clock *PL_GetClock(void);
PL_Timer *PL_GetTimer(void);
u64 PL_TimerStart(void); //get perf
r64 PL_TimerElapsed(u64 perf); //seconds elapsed
r64 PL_TimerElapsed2(u64 start, u64 end); // sec elapsed from start to end

#endif /* ASCENCIA_TIMING_H */
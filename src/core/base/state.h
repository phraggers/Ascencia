#ifndef CORE_STATE_H
/* ==============================================================
   File:    state.h
   Author:  Phraggers
   Date:    21-04-2025
   ============================================================== */

typedef struct
{
    bool running;
    PL_sWindow window;
} CORE_sState;

bool CORE_StateInit(void);

#if defined(CORE_STATE_INSTANCE)
CORE_sState *gState;
#else
extern CORE_sState *gState;
#endif

#define CORE_STATE_H
#endif

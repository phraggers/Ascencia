/* ==============================================================
   File:    core_entry.c
   Author:  Phraggers
   Date:    21-04-2025
   ============================================================== */

#include <shared/types.h>

#include <core/platform/alloc.h>
#include <core/platform/thread.h>
#include <core/platform/window.h>
#define CORE_STATE_INSTANCE
#include <core/base/state.h>
#include <core/base/log.h>

#include <core/base/log.c>
#include <core/base/state.c>

bool CORE_Init(void)
{
    if(!CORE_StateInit())
    {
        return 0;
    }

    return 1;
}

bool CORE_Run(void)
{

    return 1;
}

void CORE_Quit(void)
{

    return;
}

bool CORE_Entry(void)
{
    if(!CORE_Init()) return 0;
    if(!CORE_Run()) return 0;
    CORE_Quit();
    return 1;
}

int main(void) {return CORE_Entry() ? 0:-1;}

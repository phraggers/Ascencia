/* ==============================================================
   File:    core_entry.c
   Author:  Phraggers
   Date:    21-04-2025
   ============================================================== */

#include <shared/types.h>

bool CORE_Init(void);
bool CORE_Run(void);
void CORE_Quit(void);
bool CORE_Entry(void);

#if defined(WIN32)
#include <core/platform/win32/win32.c>
#elif defined(LINUX)
#include <core/platform/linux/linux.c>
#elif defined(MACOS)
#include <core/platform/macos/macos.c>
#endif

int main(void) {return CORE_Entry() ? 0:-1;}

bool CORE_Init(void)
{

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

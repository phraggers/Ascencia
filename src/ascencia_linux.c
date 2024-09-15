/* ==============================================================
   Project: Ascencia
   File:    ascencia_linux.c
   Author:  Phraggers
   Date:    14-09-2024
   ============================================================== */

#include <util/types.h>
#include <platform/logging.h>
#define ASCENCIA_STATE_INSTANCE
#include <linux/linux_state.h>

local bool LIN_Entry(cstr argv0);
int main(int c, char **v) { return LIN_Entry(v[0])?0:1;}

local bool LIN_Entry(cstr argv0)
{
    if(!Lin_StateInit(argv0))
    {
        PL_Log(LOG_FATAL, "failed to initialize");
        return 0;
    }

    Lin_StateQuit();
    return 1;
}
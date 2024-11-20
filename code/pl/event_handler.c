/* ==============================================================
   Project: Ascencia
   File:    event_handler.c
   Author:  Phraggers
   Date:    18-11-2024
   ============================================================== */

#include <pl/platform.h>

b32 PL_StartEventHandler(void)
{
    MSG msg = {0};
    while(g_state->running &&
          GetMessage(&msg, g_state->window.handle, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }

    return 1;
}

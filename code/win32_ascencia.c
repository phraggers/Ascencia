/* ==============================================================
   Project: Ascencia
   File:    win32_ascencia.c
   Author:  Phraggers
   Date:    17-11-2024
   ============================================================== */

#define ASC_INSTANCE
#include <win32/platform.h>

#include <win32/win32_alloc.c>
#include <win32/win32_dir.c>
#include <win32/platform.c>
#include <win32/event_handler.c>
#include <win32/render_thread.c>
#include <win32/audio_thread.c>
#include <win32/logic_thread.c>

local b32 ASC_Entry(void);
local b32 ASC_Init(void);
local b32 ASC_Run(void);
local void ASC_Quit(void);

int CALLBACK WinMain(HINSTANCE i, HINSTANCE p, LPSTR c, int s) {return ASC_Entry() ? 0:-1;}
int main(void) {return ASC_Entry ? 0:-1;}

local b32 ASC_Entry(void)
{
    if(!ASC_Init()) return 0;
    if(!ASC_Run()) return 0;
    ASC_Quit();
    return 1;
}

local b32 ASC_Init(void)
{
    g_state = (sState*)PL_Alloc0(sizeof(*g_state));
    if(!g_state) return 0;
    srand(time(0));
    if(!PL_TimerInit()) return 0;
    if(!PL_CreateWindow("Ascencia", 960, 540)) return 0;
    return 1;
}

local b32 ASC_Run(void)
{
    if(!PL_StartRenderThread()) return 0;
    if(!PL_StartAudioThread()) return 0;
    if(!PL_StartLogicThread()) return 0;
    if(!PL_StartEventHandler()) return 0;

    return 1;
}

local void ASC_Quit(void)
{
    int ecode = 0;

    if(!PL_QuitLogicThread()) ecode = -1;
    if(!PL_QuitAudioThread()) ecode = -1;
    if(!PL_QuitRenderThread()) ecode = -1;

    PostQuitMessage(ecode);
    return;
}

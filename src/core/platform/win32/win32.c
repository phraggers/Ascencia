/* ==============================================================
   File:    win32.c
   Author:  Phraggers
   Date:    21-04-2025
   ============================================================== */

#include <windows.h>
#include <shared/types.h>
bool CORE_Entry(void);
int CALLBACK WinMain(HINSTANCE i, HINSTANCE p, LPSTR c, int s) {return CORE_Entry() ? 0:-1;}

#include <core/platform/window.h>
#include <core/base/state.h>
#include <core/base/log.h>

#include <core/platform/win32/window.c>

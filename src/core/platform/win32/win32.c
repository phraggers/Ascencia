/* ==============================================================
   File:    win32.c
   Author:  Phraggers
   Date:    21-04-2025
   ============================================================== */

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <shared/types.h>
bool CORE_Entry(void);
int CALLBACK WinMain(HINSTANCE i, HINSTANCE p, LPSTR c, int s) {return CORE_Entry() ? 0:-1;}

#include <core/platform/platform.h>
#include <core/base/base.h>

#include <core/platform/win32/window.c>
#include <core/platform/win32/dir.c>
#include <core/platform/win32/thread.c>
#include <core/platform/win32/alloc.c>

#ifndef WIN32_ALLOC_H
/* ==============================================================
   Project: Ascencia
   File:    win32_alloc.h
   Author:  Phraggers
   Date:    21-11-2024
   ============================================================== */

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <util/types.h>

void *PL_Alloc(u64 size);
void *PL_Alloc0(u64 size);
void *PL_ReAlloc(void *old, u64 size);
void *PL_ReAlloc0(void *old, u64 size);
b32 PL_Free(void *mem);

#define WIN32_ALLOC_H
#endif

#ifndef ALLOC_H
/* ==============================================================
   File:    alloc.h
   Author:  Phraggers
   Date:    21-04-2025
   ============================================================== */

void *PL_Alloc(u64 size);
void *PL_Alloc0(u64 size);
void *PL_ReAlloc(void *old, u64 size);
void *PL_ReAlloc0(void *old, u64 size);
b32 PL_Free(void *mem);

#define ALLOC_H
#endif


/*============================================================
 * Project: Ascencia
 * File: alloc.h
 * Author: Phraggers
 * Date: Thu Sep 12 2024
============================================================*/

#ifndef ASCENCIA_ALLOC_H
#define ASCENCIA_ALLOC_H

#include <util/types.h>

// get memblock on heap
ptr PL_Alloc(u64 size);

// get memblock on heap cleared to 0
ptr PL_Alloc0(u64 size);

// reallocate memblock
ptr PL_ReAlloc(ptr old_mem, u64 new_size);

// reallocate memblock and clear new space to 0 if larger
ptr PL_ReAlloc0(ptr old_mem, u64 new_size);

// free allocated memblock
b32 PL_Free(ptr mem);

#endif /* ASCENCIA_ALLOC_H */
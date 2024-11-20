#ifndef LZ77_H
/* ==============================================================
   File:    lz77.h
   Author:  Phraggers
   Date:    20-11-2024
   ============================================================== */

#include <util/types.h>

typedef struct
{
    void *data;
    u32 size;
} LZ77_result;

LZ77_result LZ77_Compress(void *input, u32 input_size);
LZ77_result LZ77_Decompress(void *input);

#define LZ77_H
#endif

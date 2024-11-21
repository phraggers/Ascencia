/* ==============================================================
   File:    lz77.c
   Author:  Phraggers
   Date:    20-11-2024
   ============================================================== */

#include <util/lz77.h>

LZ77_result LZ77_Compress(void *input, u32 input_size)
{
    u8 ptr_len, tmp_ptr_len;
    u16 ptr_pos, out_ptr;
    u32 out_size, out_la_ref, lb, la;
    u8 *in_bp = (u8*)input;
    u8 *out_bp = (u8*)malloc(input_size*2);

    *((u32*)out_bp) = input_size;
    u32 compressed_ptr = out_size = 4;

    for(u32 byte_index = 0;
        byte_index < input_size;
        ++byte_index)
    {
        ptr_pos = 0;
        ptr_len = 0;

        for(u16 tmp_ptr_pos = 1;
            (tmp_ptr_pos < 0x1000) && (tmp_ptr_pos <= byte_index);
            ++tmp_ptr_pos)
        {
            lb = byte_index - tmp_ptr_pos;
            la = byte_index;

            for(tmp_ptr_len = 0;
                in_bp[la++] == in_bp[lb++];
                ++tmp_ptr_len)
            {
                if(tmp_ptr_len == 0x0f) break;
            }

            if(tmp_ptr_len > ptr_len)
            {
                ptr_pos = tmp_ptr_pos;
                ptr_len = tmp_ptr_len;
                if(ptr_len == 0x0f) break;
            }
        }


        byte_index += ptr_len;

        if(ptr_len && (byte_index == input_size))
        {
            out_ptr = (ptr_pos << 4) | (ptr_len - 1);
            out_la_ref = byte_index - 1;
        }
        else
        {
            out_ptr = (ptr_pos << 4) | ptr_len;
            out_la_ref = byte_index;
        }

        *((u32 *) (out_bp + compressed_ptr)) = out_ptr;
        compressed_ptr += 2;
        *(out_bp + compressed_ptr++) = *(in_bp + out_la_ref);
        out_size += 3;
    }

    realloc((void*)out_bp, (u64)out_size);
    LZ77_result result = {0};
    result.data = (void*)out_bp;
    result.size = out_size;
    return result;
}

LZ77_result LZ77_Decompress(void *input)
{
    u8 ptr_len;
    u16 input_ptr, ptr_pos;
    u32 compressed_ptr, byte_index, ptr_offset, uncompressed_size;
    u8 *in_bp = (u8*)input;
    uncompressed_size = *((u32*)in_bp);
    u8 *out_bp = (u8*)malloc(uncompressed_size);
    compressed_ptr = 4;

    for(byte_index = 0;
        byte_index < uncompressed_size;
        ++byte_index)
    {
        input_ptr = *((u32*)(in_bp + compressed_ptr));
        compressed_ptr += 2;
        ptr_pos = input_ptr >> 4;
        ptr_len = input_ptr & 0x0f;

        if(ptr_pos)
        {
            for(ptr_offset = byte_index - ptr_pos;
                ptr_len > 0;
                --ptr_len)
            {
                out_bp[byte_index++] = out_bp[ptr_offset++];
            }
        }
        *(out_bp + byte_index) = *(in_bp + compressed_ptr++);
    }

    LZ77_result result = {0};
    result.data = (void*)out_bp;
    result.size = uncompressed_size;
    return result;
}

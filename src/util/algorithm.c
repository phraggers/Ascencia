/*============================================================
 * Project: Ascencia
 * File: algorithm.c
 * Author: Phraggers
 * Date: Sun Sep 15 2024
============================================================*/

#include <util/types.h>
#include <util/maths.h>
#include <util/algorithm.h>
#include <util/string_helpers.h>

#include <time.h>

global u32 G_seed;

local void default_seed(void);
local u32 qed_hash(ptr input, u64 size);
local u32 MurmurOAAT_32(ptr data, size_t size, u32 seed);
local u64 FNV1a_64(ptr data, u64 size);

/* === SEED === */

local void default_seed(void)
{
    time_t t = time(0);
    u32 result = qed_hash(&t, sizeof(time_t));
    srand(result);
    G_seed = result;
}

void rand_seed(ptr v, u64 size)
{
    u32 result = qed_hash(&v, size);
    srand(result);
    G_seed = result;
}

/* === BIT MANIP === */

u32 rotl(u32 v, i32 s)
{
    s &= 31;
    u32 result = ((v<<s) | (v>>(32-s)));
    return result;
}

u32 rotr(u32 v, i32 s)
{
    s &= 31;
    u32 result = ((v>>s) | (v<<(32-s)));
    return result;
}

u32 rot_xor(u8 *d, u64 s)
{
    u32 result = 0x55555555;

    for(u64 byte_index = 0;
        byte_index < s; 
        byte_index++)
    {
        result ^= d[byte_index];
        result = rotl(result, 5);
    }

    return result;
}


/* === RAND === */

r32 rand32(void)
{
    if(!G_seed)
    {
        default_seed();
    }
    
    r32 result = norm32((r32)rand(), 0.0f, (r32)RAND_MAX);
    return result;
}

r64 rand64(void)
{
    if(!G_seed)
    {
        default_seed();
    }
    
    r64 mant_div = 9007199254740991.0; // 2^53 - 1
    u64 mantissa = ((u64)(rand()) << 21) ^ (rand() >> 2);
    return (r64)mantissa / mant_div;
}

/* === HASH === */

local u32 qed_hash(ptr input, u64 size)
{
    char placeholder[4] = {0};
    for(u8 i=0;i<(u8)size;i++)
    {
        i8 *p=(i8*)input;
        p+=i;
        placeholder[i]=*p;
    }
    i8 *in = (i8*)input;
    if(size<4) in = placeholder;
    u32 res = (u32)size;
    u32 tmp;
    u64 len = size;
    int rlen;
    
    rlen = len & 3;
    len >>= 2;
    
    while(len > 0)
    {
        res += Reinterpret(u16, in);
        tmp = (Reinterpret(u16, in + 2) << 11) ^ res;
        res = (res << 16) ^ tmp;
        in += 2 * sizeof(u16);
        res += res >> 11;
        len--;
    }
    
    switch (rlen)
    {
        case 1:
        {
            res += *in;
            res ^= res << 10;
            res += res >> 1;
        } break;
        case 2:
        {
            res += Reinterpret(u16, in);
            res ^= res << 11;
            res += res >> 17;
        } break;
        case 3:
        {
            res += Reinterpret(u16, in);
            res ^= res << 16;
            res ^= (in[sizeof(u16)]) << 18;
            res += res >> 11;
        } break;
    }
    
    res ^= res << 3;
    res += res >> 5;
    res ^= res << 4;
    res += res >> 17;
    res ^= res << 25;
    res += res >> 6;

    return res;
}

local u32 MurmurOAAT_32(ptr data, size_t size, u32 seed)
{
    u8 *byte = (u8*)data;
    u32 result = seed;
    
    for(u64 byte_index = 0;
         byte_index < size;
         byte_index++) 
    {
        result ^= byte[byte_index];
        result *= 0x5bd1e995;
        result ^= result >> 15;
    }

    return result;
}

local u64 FNV1a_64(ptr data, u64 size)
{
    u8 *byte = (u8*)data;
    u64 result = 0xcbf29ce484222325UL;

    for (u64 byte_index = 0;
         byte_index < size;
         byte_index++) 
    {
        result ^= byte[byte_index];
        result *= 0x00000100000001B3UL;
    }

    return result;
}

hash4_result hash4(ptr input, u64 size)
{
    hash4_result result = {0};
    u32 hash = MurmurOAAT_32(input, size, 0x12345678);
    memcpy(&result, &hash, 4);
    return result;   
}

hash8_result hash8(ptr input, u64 size)
{
    hash8_result result = {0};
    u64 hash = FNV1a_64(input, size);
    memcpy(&result, &hash, 8);
    return result;
}

hash16_result hash16(ptr input, u64 size)
{
    hash16_result result = {0};

    u32 hash[2] = 
    {
        MurmurOAAT_32(input, size, 0x12345678),
        MurmurOAAT_32(input, size, 0x23456789)
    };

    hash8_result hash_result[2] =
    {
        hash8(&hash[0], 4),
        hash8(&hash[1], 4)
    };

    memcpy(&result, &hash_result, 16);

    return result;
}

hash32_result hash32(ptr input, u64 size)
{
    hash32_result result = {0};

    u32 hash[4] = 
    {
        MurmurOAAT_32(input, size, 0x12345678),
        MurmurOAAT_32(input, size, 0x23456789),
        MurmurOAAT_32(input, size, 0x34567890),
        MurmurOAAT_32(input, size, 0x45678901)
    };

    hash8_result hash_result[4] =
    {
        hash8(&hash[0], 4),
        hash8(&hash[1], 4),
        hash8(&hash[2], 4),
        hash8(&hash[3], 4)
    };

    memcpy(&result, &hash_result, 32);

    return result;
}

hash64_result hash64(ptr input, u64 size)
{
    hash64_result result = {0};

    u32 hash[8] = 
    {
        MurmurOAAT_32(input, size, 0x12345678),
        MurmurOAAT_32(input, size, 0x23456789),
        MurmurOAAT_32(input, size, 0x34567890),
        MurmurOAAT_32(input, size, 0x45678901),
        MurmurOAAT_32(input, size, 0x56789012),
        MurmurOAAT_32(input, size, 0x67890123),
        MurmurOAAT_32(input, size, 0x78901234),
        MurmurOAAT_32(input, size, 0x89012345)
    };

    hash8_result hash_result[8] =
    {
        hash8(&hash[0], 4),
        hash8(&hash[1], 4),
        hash8(&hash[2], 4),
        hash8(&hash[3], 4),
        hash8(&hash[4], 4),
        hash8(&hash[5], 4),
        hash8(&hash[6], 4),
        hash8(&hash[7], 4)
    };

    memcpy(&result, &hash_result, 64);

    return result;
}

hash128_result hash128(ptr input, u64 size)
{
    hash128_result result = {0};

    u32 hash[16] = 
    {
        MurmurOAAT_32(input, size, 0x12345678),
        MurmurOAAT_32(input, size, 0x23456789),
        MurmurOAAT_32(input, size, 0x34567890),
        MurmurOAAT_32(input, size, 0x45678901),
        MurmurOAAT_32(input, size, 0x56789012),
        MurmurOAAT_32(input, size, 0x67890123),
        MurmurOAAT_32(input, size, 0x78901234),
        MurmurOAAT_32(input, size, 0x89012345),
        MurmurOAAT_32(input, size, 0x90123456),
        MurmurOAAT_32(input, size, 0x01234567),
        MurmurOAAT_32(input, size, 0x11234567),
        MurmurOAAT_32(input, size, 0x22345678),
        MurmurOAAT_32(input, size, 0x33456789),
        MurmurOAAT_32(input, size, 0x44567890),
        MurmurOAAT_32(input, size, 0x55678901),
        MurmurOAAT_32(input, size, 0x66789012)
    };

    hash8_result hash_result[16] =
    {
        hash8(&hash[0], 4),
        hash8(&hash[1], 4),
        hash8(&hash[2], 4),
        hash8(&hash[3], 4),
        hash8(&hash[4], 4),
        hash8(&hash[5], 4),
        hash8(&hash[6], 4),
        hash8(&hash[7], 4),
        hash8(&hash[8], 4),
        hash8(&hash[9], 4),
        hash8(&hash[10], 4),
        hash8(&hash[11], 4),
        hash8(&hash[12], 4),
        hash8(&hash[13], 4),
        hash8(&hash[14], 4),
        hash8(&hash[15], 4)
    };

    memcpy(&result, &hash_result, 128);

    return result;
}
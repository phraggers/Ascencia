/*============================================================
 * Project: Ascencia
 * File: algorithm.h
 * Author: Phraggers
 * Date: Sun Sep 15 2024
============================================================*/

#ifndef ASCENCIA_ALGORITHM_H
#define ASCENCIA_ALGORITHM_H

#include <util/types.h>
#include <util/maths.h>

/* === BIT MANIP === */

u32 rotl(u32 v, i32 s);
u32 rotr(u32 v, i32 s);
u32 rot_xor(u8 *d, u64 s);

/* === RAND === */

/* rand */
r32 rand32(void);
r64 rand64(void);
void rand_seed(ptr v, u64 size); //optional (else rand will seed using time)

#define RANDSEED(v) rand_seed((ptr)v, sizeof *(v))
// Rand macro helpers to cast to PL int types 8-32
#define RANDI8 ((i8)round_i32(((rand32()*(r32)UINT8_MAX)-((r32)UINT8_MAX/2))))
#define RANDU8 ((u8)round_u32((rand32()*(r32)UINT8_MAX)))
#define RANDI16 ((i16)round_i32(((rand32()*UINT16_MAX)-((r32)UINT16_MAX/2))))
#define RANDU16 ((u16)round_u32((rand32()*(r32)UINT16_MAX)))
#define RANDI32 (round_i32(((rand32()*UINT32_MAX)-((r32)UINT32_MAX/2))))
#define RANDU32 (round_u32((rand32()*(r32)UINT32_MAX)))

/* === HASH === */

/* result types */
typedef struct {u8 r[4];} hash4_result;
typedef struct {u8 r[8];} hash8_result;
typedef struct {u8 r[16];} hash16_result;
typedef struct {u8 r[32];} hash32_result;
typedef struct {u8 r[64];} hash64_result;
typedef struct {u8 r[128];} hash128_result;

/* get hashed bytes from input (size = sizeof input) */
hash4_result hash4(ptr input, u64 size);
hash8_result hash8(ptr input, u64 size);
hash16_result hash16(ptr input, u64 size);
hash32_result hash32(ptr input, u64 size);
hash64_result hash64(ptr input, u64 size);
hash128_result hash128(ptr input, u64 size);

/* get hash result as string */
#define HashString4(buffer, hash_result) PL_String_Hex(buffer, (u8*)(&hash_result), 4);
#define HashString8(buffer, hash_result) PL_String_Hex(buffer, (u8*)(&hash_result), 8);
#define HashString16(buffer, hash_result) PL_String_Hex(buffer, (u8*)(&hash_result), 16);
#define HashString32(buffer, hash_result) PL_String_Hex(buffer, (u8*)(&hash_result), 32);
#define HashString64(buffer, hash_result) PL_String_Hex(buffer, (u8*)(&hash_result), 64);
#define HashString128(buffer, hash_result) PL_String_Hex(buffer, (u8*)(&hash_result), 128);

#endif /* ASCENCIA_ALGORITHM_H */
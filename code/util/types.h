#ifndef TYPES_H
/* ==============================================================
   File:    types.h
   Author:  Phraggers
   Date:    20-11-2024
   ============================================================== */

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <float.h>

typedef int8_t i8;
typedef uint8_t u8;
typedef int16_t i16;
typedef uint16_t u16;
typedef int32_t i32;
typedef uint32_t u32;
typedef int64_t i64;
typedef uint64_t u64;
typedef _Bool b8;
typedef i32 b32;
typedef float r32;
typedef double r64;

#define local static
#define persist static
#define global static

#define TYPES_H
#endif
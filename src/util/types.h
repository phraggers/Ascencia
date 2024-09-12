
/*============================================================
 * Project: Ascencia
 * File: types.h
 * Author: Phraggers
 * Date: Sat Sep 07 2024
============================================================*/

#ifndef ASCENCIA_UTIL_TYPES_H
#define ASCENCIA_UTIL_TYPES_H

#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#define STRING_LEN 0xff

#include <stdint.h>
#include <limits.h>
    /* uint types */
    typedef uint8_t u8;
    typedef uint16_t u16;
    typedef uint32_t u32;
    typedef uint64_t u64;

    /* int types */
    typedef int8_t i8;
    typedef int16_t i16;
    typedef int32_t i32;
    typedef int64_t i64;

    /* asm types */
    typedef u8 byte;
    typedef u16 word;
    typedef u32 dword;
    typedef u64 qword;

#ifndef __cplusplus
#include <stdbool.h>
#endif
    typedef bool b8;
    typedef i32 b32;
    
#include <float.h>
    typedef float r32;
    typedef double r64;
    
typedef void* ptr;
typedef char* cstr;

#define local static
#define global static
#define persist static

#define KB(v) ((v)*1024LL)
#define MB(v) (KB(v)*1024LL)
#define GB(v) (MB(v)*1024LL)

#ifdef DEBUG
#define Assert(exp) if(!(exp)) {*(int*)0=0;}
#else
#define Assert(exp)
#endif
    // get number of elements in array
#define ArrayCount(arr) (sizeof(arr) / sizeof((arr)[0]))
    // swap ptr A and B values of type
#define SwapPtr(type, a, b) {type *tmp=b; b=a; a=tmp;}
    // re-interpret data bits(v) as new type(t)
#define Reinterpret(t,v) (*((const t*)(v)))

typedef struct
{
    int x,y,w,h;
} irect;

typedef struct
{
    r32 x,y,w,h;
} frect;

#endif /* ASCENCIA_UTIL_TYPES_H */
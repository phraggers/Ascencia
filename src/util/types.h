/*============================================================
 * Project: Ascencia
 * File: types.h
 * Author: Phraggers
 * Date: Sun May 12 2024
============================================================*/
#ifndef TYPES_H_
#define TYPES_H_

    #include <stdlib.h>
    #include <stdio.h>
    #include <string.h>
    #include <time.h>

    #include <stdint.h>
    #include <limits.h>
    typedef int8_t i8;
    typedef uint8_t u8;
    typedef int16_t i16;
    typedef uint16_t u16;
    typedef int32_t i32;
    typedef uint32_t u32;
    typedef int64_t i64;
    typedef uint64_t u64;
    typedef bool b8;
    typedef i32 b32;

    #include <float.h>
    typedef float r32;
    typedef double r64;

    typedef void* ptr;
    typedef char* cstr;

    #if defined(WIN32) || \
    defined(_WIN32) || \
    defined(__WIN32__) || \
    defined(__NT__) || \
    defined(_WIN64)
        #define ASC_WINDOWS
    #elif defined(__APPLE__)
        #include <TargetConditionals.h>
    #elif defined(TARGET_OS_MAC)
        #define ASC_MACOS
    #elif defined(__linux__)
        #define ASC_LINUX
    #else
        #error "Unsupported Compiler"
    #endif

    #if defined(_DEBUG) || \
    defined(DEBUG) || \
    !defined(NDEBUG)
        #define ASC_DEBUG 1
    #endif

    // convert kilobytes to bytes
    #define KB(v) ((v)*1024LL)
    // convert megabytes to bytes
    #define MB(v) (KB(v)*1024LL)
    // convert gigabytes to bytes
    #define GB(v) (MB(v)*1024LL)

    // break on expression being false (only when ASC_DEBUG defined)
    #if ASC_DEBUG
        #define ASC_Assert(exp) if(!(exp)) {*(int*)0=0;}
    #else
        #define ASC_Assert(exp)
    #endif

    // get number of elements in array
    #define ASC_ArrayCount(arr) (sizeof(arr) / sizeof((arr)[0]))
    // swap ptr A and B values of type
    #define ASC_SwapPtr(type, a, b) {type *tmp=b; b=a; a=tmp;}
    // re-interpret data bits(v) as new type(t)
    #define ASC_Reinterpret(t,v) (*((const t*)(v)))

#endif //TYPES_H_
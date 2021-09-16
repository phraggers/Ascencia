#ifndef ASCENCIA_H
/* ========================================================================
   $File:    Ascencia [Header] $
   $Project: Ascencia $
   $Date:    09-09-2021 $
   $Author:  Phil Bagshaw $
   $Notice:  (c)Phragware 2021 $
   ======================================================================== */

#if DEBUG
#define Assert(Expression) if(!(Expression)) {*(int*)0 = 0;}
#else
#define Assert(Expression)
#endif //DEBUG

#define Kilobytes(Value) ((Value)*1024LL)
#define Megabytes(Value) (Kilobytes(Value)*1024LL)
#define Gigabytes(Value) (Megabytes(Value)*1024LL)
#define Terabytes(Value) (Gigabytes(Value)*1024LL)

#define ArrayCount(Array) (sizeof(Array) / sizeof((Array)[0]))

#define internal static
#define persist static
#define global static

#define PI32 3.14159265359f

#define INT8_MIN -0x80
#define INT8_MAX 0x7f
#define UINT8_MAX 0xffU
#define INT16_MIN -0x8000
#define INT16_MAX 0x7fff
#define UINT16_MAX 0xffffU
#define INT32_MIN -0x80000000LL
#define INT32_MAX 0x7FFFFFFFLL
#define UINT32_MAX 0xffffffffULL
#define INT64_MIN -0x8000000000000000LL
#define INT64_MAX 0x7FFFFFFFFFFFFFFFLL
#define UINT64_MAX 0xffffffffffffffffULL
#define REAL32_MIN 1.17549e-038f
#define REAL32_MAX 3.40282e+038f
#define REAL64_MIN 2.22507e-308
#define REAL64_MAX 1.79769e+308

typedef char int8;
typedef unsigned char uint8;
typedef short int16;
typedef unsigned short uint16;
typedef int int32;
typedef unsigned int uint32;
typedef long long int64;
typedef unsigned long long uint64;
typedef float real32;
typedef double real64;

global struct
Asc_Memory
{
    bool Initialized;
    uint64 PersistantSize;
    uint64 TransientSize;
    void *Persistant;
    void *Transient;
} Memory;

#define KEYDOWN_COUNT 0xf

global struct
Asc_Application
{
    SDL_Window *Window;
    SDL_GLContext GLContext;
    SDL_Rect Dimension;
    bool Running;
    bool Exposed;
    bool Minimized;
    bool Maximized;
    bool MouseInWindow;
    bool KeyboardFocus;
    SDL_Keycode Keydowns[KEYDOWN_COUNT];
    int MouseX;
    int MouseY;
    bool MouseLeft;
    bool MouseRight;
} Application;

#define ASCENCIA_H
#endif

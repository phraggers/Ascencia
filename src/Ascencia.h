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
#endif
#define Kilobytes(Value) ((Value)*1024LL)
#define Megabytes(Value) (Kilobytes(Value)*1024LL)
#define Gigabytes(Value) (Megabytes(Value)*1024LL)
#define Terabytes(Value) (Gigabytes(Value)*1024LL)
#define ArrayCount(Array) (sizeof(Array) / sizeof((Array)[0]))

#define internal static
#define persist static
#define global static

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

#define PI32 3.14159265359f

global struct
Asc_Memory
{
    bool Initialized;

    uint64 PersistantSize;
    uint64 TransientSize;
    
    //IMPORTANT: Init to 0 on startup
    void *Persistant;
    void *Transient;
} Memory;

global struct
Asc_Application
{
    SDL_Window *Window;
    SDL_Renderer *Renderer;
    int ScreenWidth;
    int ScreenHeight;
    bool Running;
    
} Application;

#define ASCENCIA_H
#endif

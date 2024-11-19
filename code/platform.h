#ifndef PLATFORM_H
/* ==============================================================
   Project: Ascencia
   File:    platform.h
   Author:  Phraggers
   Date:    17-11-2024
   ============================================================== */

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <float.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <mmsystem.h>
#include <dsound.h>
#include <xinput.h>

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
#define global static
#define persist static

#define PL_OPENGL_MAJ 4
#define PL_OPENGL_MIN 5

#define PL_ENABLE_VSYNC 1
#define PL_TARGET_FPS 60
#define PL_AUDIO_THREAD_HZ 60
#define PL_LOGIC_THREAD_HZ 30

#define PL_AUDIO_SAMPLERATE 48000
#define PL_AUDIO_CHANNELS 2
#define PL_AUDIO_BITSPERSAMPLE 16

typedef struct
{
    u64 perf_freq;
    u64 frames;
    u32 min_ms_wait;
    r32 target_ms;
    u64 frame_timer;
    r32 elapsed_ms;
} sTimer;

typedef struct
{
    DWORD id;
    HANDLE handle;
    HANDLE mutex;
} sThread;

typedef struct
{
    HWND handle;
    WNDCLASSEXA wndclass;
    int style;
    HDC dc;
    HGLRC rc;
    WINDOWPLACEMENT wp;
    b32 vsync;
    int w, h;
    int target_fps;
} sWindow;

typedef struct
{
    b32 is_init;
    LPDIRECTSOUNDBUFFER dsbuffer;
    int sample_rate;
    u32 sample_index;
    int bytes_per_sample;
    u32 dsbuffer_size;
    u32 margin_bytes;
} sAudio;

typedef struct
{
    //vars
    b32 running;

    //modules
    sTimer timer;
    sWindow window;
    sAudio audio;

    //threads
    sThread render_thread;
    sThread audio_thread;
    sThread logic_thread;

} sState;
global sState *g_state;

void *PL_Alloc(u64 size);
void *PL_Alloc0(u64 size);
void *PL_ReAlloc(void *old, u64 size);
void *PL_ReAlloc0(void *old, u64 size);
b32 PL_Free(void *mem);

b32 PL_TimerInit(void);
inline u64 PL_QueryTimer(void);
inline r32 PL_TimeElapsed(u64 start, u64 end);
b32 PL_RenderFrameTimerStart(void);
void PL_RenderFrameTimerEnd(void);
b32 PL_CreateWindow(const char *title, int w, int h);
b32 PL_InitDSound(void);
void PL_ClearSoundBuffer(void);
void PL_FillSoundBuffer(DWORD byte_to_lock, DWORD bytes_to_write,
                        int sample_count, i16 *samples);
void PL_AudioPlay(void);

b32 PL_StartEventHandler(void);
b32 PL_StartRenderThread(void);
b32 PL_QuitRenderThread(void);
b32 PL_StartAudioThread(void);
b32 PL_QuitAudioThread(void);
b32 PL_StartLogicThread(void);
b32 PL_QuitLogicThread(void);

#include "gl/gl.h"
#include "gl/wgl.h"

#define PLATFORM_H
#endif

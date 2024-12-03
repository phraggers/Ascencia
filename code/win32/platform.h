#ifndef PLATFORM_H
/* ==============================================================
   Project: Ascencia
   File:    platform.h
   Author:  Phraggers
   Date:    17-11-2024
   ============================================================== */

// https://github.com/kuba--/zip
#include <zip/zip.h>
#include <zip/miniz.h>

#include <util/types.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <mmsystem.h>
#include <dsound.h>
#include <xinput.h>

#include <direct.h>

#include <win32/win32_alloc.h>
#include <win32/win32_dir.h>

#include <win32/gl/gl.h>
#include <win32/gl/wgl.h>

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

#ifdef ASC_INSTANCE
sState *g_state;
#else
extern sState *g_state;
#endif

b32 PL_TimerInit(void);
inline u64 PL_QueryTimer(void);
inline r32 PL_TimeElapsed(u64 start, u64 end);

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

#define PLATFORM_H
#endif

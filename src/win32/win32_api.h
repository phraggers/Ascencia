/*============================================================
 * Project: Ascencia
 * File: win32_api.h
 * Author: Phraggers
 * Date: Thu Sep 12 2024
============================================================*/

#ifndef ASCENCIA_WIN32_API_H
#define ASCENCIA_WIN32_API_H

#include <util/types.h>
#include <util/maths.h>

typedef union 
{ 
    struct {u32 LowPart; i32 HighPart;} s; 
    struct {u32 LowPart; i32 HighPart;} u; 
    i64 QuadPart;
} Win32_LARGE_INTEGER;

typedef struct
{
    int x,y;
} Win32_POINT;

typedef struct 
{
    ptr hwnd;
    u32 message;
    u64 wParam;
    i64 lParam;
    u32 time;
    Win32_POINT point;
    u32 unused;
} Win32_MSG;

typedef i64 (*pfn_WNDPROC)(ptr hwnd, u32 msg, u64 wparam, i64 lparam);

typedef struct 
{
    u32 style;
    pfn_WNDPROC wndproc;
    int class_extra_bytes;
    int wnd_extra_bytes;
    ptr instance;
    ptr icon;
    ptr cursor;
    ptr background;
    cstr menu_name;
    cstr class_name;
} Win32_WNDCLASSA;

typedef struct 
{
    int x;
    int y;
    int w;
    int h;
} Win32_RECT;

typedef struct
{
    u32 length;
    u32 flags;
    u32 showCmd;
    Win32_POINT min_pos;
    Win32_POINT max_pos;
    Win32_RECT  normal_pos;
    Win32_RECT  device;
} Win32_WNDPL;

typedef struct 
{
    u32 ssize;
    Win32_RECT monitor;
    Win32_RECT workspace;
    u32 flags;
} Win32_MONITORINFO;

typedef struct 
{
    u16 ssize;
    u16 version;
    u32 flags;
    u8 pixel_type;
    u8 color_bits;
    u8 red_bits;
    u8 red_shift;
    u8 green_bits;
    u8 green_shift;
    u8 blue_bits;
    u8 blue_shift;
    u8 alpha_bits;
    u8 alpha_shift;
    u8 accum_bits;
    u8 accum_red_bits;
    u8 accum_green_bits;
    u8 accum_blue_bits;
    u8 accum_alpha_bits;
    u8 depth_bits;
    u8 stencil_bits;
    u8 num_aux_buffers;
    u8 unused1;
    u8 unused2;
    u32 unused3;
    u32 visible_mask;
    u32 unused4;
} Win32_PFD;

typedef struct 
{
    u16 year;
    u16 month;
    u16 wday;
    u16 day;
    u16 hour;
    u16 min;
    u16 sec;
    u16 ms;
} Win32_SYSTEMTIME;

#define Win32_WS_OVERLAPPED_WINDOW (0x00000000L | 0x00C00000L | 0x00080000L | 0x00040000L | 0x00020000L | 0x00010000L)

 /* kernel32.dll */
 typedef ptr (*pfn_GetModuleHandleA)(const cstr module_name);
 typedef cstr (*pfn_GetCommandLineA)(void);
 typedef ptr (*pfn_LoadLibraryA)(const cstr lib_name);
 typedef ptr (*pfn_GetProcAddress)(ptr module, const cstr proc_name);
 typedef u32 (*pfn_GetModuleFileNameA)(ptr module, cstr filename, u32 size);
 typedef ptr (*pfn_AddDllDirectory)(cstr path);
 typedef u32 (*pfn_GetLastError)(void);
 typedef ptr (*pfn_CreateFileA)(cstr path, u32 access, u32 smode, ptr sec, u32 creation, u32 flags, ptr ftemplate);
 typedef b32 (*pfn_CloseHandle)(ptr handle);
 typedef b32 (*pfn_GetFileSizeEx)(ptr handle, Win32_LARGE_INTEGER *size);
 typedef b32 (*pfn_ReadFile)(ptr handle, ptr buffer, u32 bytes_to_read, u32 *bytes_read, ptr overlapped);
 typedef b32 (*pfn_WriteFile)(ptr handle, const ptr buffer, u32 bytes_to_write, u32 *bytes_written, ptr overlapped);
 typedef b32 (*pfn_DeleteFileA)(cstr path);
 typedef b32 (*pfn_CreateDirectoryA)(cstr path, ptr sec);
 typedef b32 (*pfn_RemoveDirectoryA)(cstr path);
 typedef b32 (*pfn_QueryPerformanceFrequency)(Win32_LARGE_INTEGER *frequency);
 typedef b32 (*pfn_QueryPerformanceCounter)(Win32_LARGE_INTEGER *frequency);
 typedef void (*pfn_GetLocalTime)(Win32_SYSTEMTIME *st);

 /* shlwapi.dll */
 typedef b32 (*pfn_PathFileExistsA)(cstr path);

 /* user32.dll */
typedef b32 (*pfn_TranslateMessage)(const Win32_MSG *msg);
typedef i64 (*pfn_DispatchMessageA)(const Win32_MSG *msg);
typedef b32 (*pfn_PeekMessageA)(Win32_MSG *msg, ptr hwnd, u32 filter_min, u32 filter_max, u32 remove_flag);
typedef i64 (*pfn_DefWindowProcA)(ptr hwnd, u32 msg, u64 wparam, i64 lparam);
typedef u16 (*pfn_RegisterClassA)(const Win32_WNDCLASSA *wndclass);
typedef ptr (*pfn_CreateWindowExA)(u32 ex_style, const cstr class_name, const cstr wnd_name, u32 style, int x, int y, int w, int h, ptr parent, ptr menu, ptr instance, ptr param);
typedef b32 (*pfn_DestroyWindow)(ptr hwnd);
typedef b32 (*pfn_MoveWindow)(ptr hwnd, int x, int y, int w, int h, b32 repaint);
typedef b32 (*pfn_SetWindowPos)(ptr hwnd, ptr zpos, int x, int y, int w, int h, u32 flags);
typedef b32 (*pfn_GetWindowPlacement)(ptr hwnd, Win32_WNDPL *wndpl);
typedef b32 (*pfn_SetWindowPlacement)(ptr hwnd, const Win32_WNDPL *wndpl);
typedef ptr (*pfn_GetDC)(ptr hwnd);
typedef int (*pfn_ReleaseDC)(ptr hwnd, ptr dc);
typedef b32 (*pfn_SetWindowTextA)(ptr hwnd, cstr text);
typedef int (*pfn_MessageBoxA)(ptr hwnd, const cstr text, const cstr caption, u32 type);
typedef b32 (*pfn_GetCursorPos)(Win32_POINT *point);
typedef b32 (*pfn_ScreenToClient)(ptr hwnd, Win32_POINT *point);
typedef i32 (*pfn_GetWindowLongA)(ptr hwnd, int index);
typedef i32 (*pfn_SetWindowLongA)(ptr hwnd, int index, i64 value);
typedef ptr (*pfn_MonitorFromWindow)(ptr hwnd, u32 flags);
typedef b32 (*pfn_GetMonitorInfoA)(ptr monitor, Win32_MONITORINFO *mi);
typedef b32 (*pfn_GetClientRect)(ptr hwnd, Win32_RECT *rect);
typedef b32 (*pfn_GetWindowRect)(ptr hwnd, Win32_RECT *rect);

 /* gdi32.dll */
 typedef int (*pfn_ChoosePixelFormat)(ptr hdc, const Win32_PFD *ppfd);
 typedef int (*pfn_DescribePixelFormat)(ptr hdc, int format, u32 bytes, Win32_PFD *ppfd);
 typedef b32 (*pfn_SetPixelFormat)(ptr hdc, int format, const Win32_PFD *ppfd);
 typedef b32 (*pfn_SwapBuffers)(ptr hdc);

 /* opengl32.dll */
 typedef ptr (*pfn_wglCreateContext)(ptr hdc);
 typedef b32 (*pfn_wglDeleteContext)(ptr hglrc);
 typedef i64 (*pfn_wglGetProcAddress)(const cstr fname);
 typedef b32 (*pfn_wglMakeCurrent)(ptr hdc, ptr hglrc);

typedef struct
{
    /* kernel32.dll */
    pfn_GetModuleHandleA GetModuleHandleA;
    pfn_GetCommandLineA GetCommandLineA;
    pfn_LoadLibraryA LoadLibraryA;
    pfn_GetProcAddress GetProcAddress;
    pfn_GetModuleFileNameA GetModuleFileNameA;
    pfn_AddDllDirectory AddDllDirectory;
    pfn_GetLastError GetLastError;
    pfn_CreateFileA CreateFileA;
    pfn_CloseHandle CloseHandle;
    pfn_GetFileSizeEx GetFileSizeEx;
    pfn_ReadFile ReadFile;
    pfn_WriteFile WriteFile;
    pfn_DeleteFileA DeleteFileA;
    pfn_CreateDirectoryA CreateDirectoryA;
    pfn_RemoveDirectoryA RemoveDirectoryA;
    pfn_QueryPerformanceFrequency QueryPerformanceFrequency;
    pfn_QueryPerformanceCounter QueryPerformanceCounter;
    pfn_GetLocalTime GetLocalTime;

    /* shlwapi.dll */
    pfn_PathFileExistsA PathFileExistsA;

    /* user32.dll */
    pfn_TranslateMessage TranslateMessage;
    pfn_DispatchMessageA DispatchMessageA;
    pfn_PeekMessageA PeekMessageA;
    pfn_DefWindowProcA DefWindowProcA;
    pfn_RegisterClassA RegisterClassA;
    pfn_CreateWindowExA CreateWindowExA;
    pfn_DestroyWindow DestroyWindow;
    pfn_MoveWindow MoveWindow;
    pfn_SetWindowPos SetWindowPos;
    pfn_GetWindowPlacement GetWindowPlacement;
    pfn_SetWindowPlacement SetWindowPlacement;
    pfn_GetDC GetDC;
    pfn_ReleaseDC ReleaseDC;
    pfn_SetWindowTextA SetWindowTextA;
    pfn_MessageBoxA MessageBoxA;
    pfn_GetCursorPos GetCursorPos;
    pfn_ScreenToClient ScreenToClient;
    pfn_GetWindowLongA GetWindowLongA;
    pfn_SetWindowLongA SetWindowLongA;
    pfn_MonitorFromWindow MonitorFromWindow;
    pfn_GetMonitorInfoA GetMonitorInfoA;
    pfn_GetClientRect GetClientRect;
    pfn_GetWindowRect GetWindowRect;
    
    /* gdi32.dll */
    pfn_ChoosePixelFormat ChoosePixelFormat;
    pfn_DescribePixelFormat DescribePixelFormat;
    pfn_SetPixelFormat SetPixelFormat;
    pfn_SwapBuffers SwapBuffers;
    
    /* opengl32.dll */
    pfn_wglCreateContext wglCreateContext;
    pfn_wglDeleteContext wglDeleteContext;
    pfn_wglGetProcAddress wglGetProcAddress;
    pfn_wglMakeCurrent wglMakeCurrent;
    
} Win32_API;

#define WINAPI G_win32_state->api

bool Win32_LoadAPI(void);
u64 Win32_GetPerfCount(void);

/* === XINPUT  === */

typedef struct
{
    u16 buttons;
    u8 lt;
    u8 rt;
    i16 lx;
    i16 ly;
    i16 rx;
    i16 ry;
} Win32_XINPUT_GAMEPAD;

typedef struct
{
    u32 packet_number;
    Win32_XINPUT_GAMEPAD gamepad;
} Win32_XINPUT_STATE;

typedef struct
{
    u16 left;
    u16 right;
} Win32_XINPUT_VIBRATION;

typedef enum
{
    XINPUT_NOT_LOADED = 0,
    XINPUT_1_4,
    XINPUT_9_1_0,
    XINPUT_1_3
} Win32_XINPUT_VERSION;

typedef u32 (*pfn_XInputGetState)(u32 user_index, Win32_XINPUT_STATE *state);
typedef u32 (*pfn_XInputSetState)(u32 user_index, Win32_XINPUT_VIBRATION *vib);

typedef struct
{
    Win32_XINPUT_VERSION version;
    pfn_XInputGetState XInputGetState;
    pfn_XInputSetState XInputSetState;
} Win32_XInput;

#define XINPUT_API G_win32_state->xinput

bool Win32_LoadXInput(void);

/* === XAUDIO2 === */

typedef struct IUnknown IUnknown;

extern const Win32_GUID Win32_IID_IXAudio2 = {0x2B02E3CF, 0x2E0B, 0x4ec3, 0xBE, 0x45, 0x1B, 0x2A, 0x3F, 0xE7, 0x21, 0x0D};
extern const Win32_GUID Win32_IID_IXAudio2Extension = {0x84ac29bb, 0xd619, 0x44d2, 0xb1, 0x97, 0xe4, 0xac, 0xf7, 0xdf, 0x3e, 0xd6};

#define XAUDIO2_MAX_BUFFER_BYTES                        0x80000000
#define XAUDIO2_MAX_QUEUED_BUFFERS                      64
#define XAUDIO2_MAX_AUDIO_CHANNELS                      64
#define XAUDIO2_MIN_SAMPLE_RATE                         1000
#define XAUDIO2_MAX_SAMPLE_RATE                         200000
#define XAUDIO2_MAX_VOLUME_LEVEL                        16777216.0f
#define XAUDIO2_MIN_FREQ_RATIO                          (1/1024.0f)
#define XAUDIO2_MAX_FREQ_RATIO                          1024.0f
#define XAUDIO2_DEFAULT_FREQ_RATIO                      2.0f
#define XAUDIO2_MAX_FILTER_ONEOVERQ                     1.5f
#define XAUDIO2_MAX_FILTER_FREQUENCY                    1.0f
#define XAUDIO2_MAX_LOOP_COUNT                          254
#define XAUDIO2_MAX_INSTANCES                           8
#define XAUDIO2_MAX_RATIO_TIMES_RATE_XMA_MONO           600000
#define XAUDIO2_MAX_RATIO_TIMES_RATE_XMA_MULTICHANNEL   300000
#define XAUDIO2_COMMIT_NOW                              0
#define XAUDIO2_COMMIT_ALL                              0
#define XAUDIO2_INVALID_OPSET                           (u32)(-1)
#define XAUDIO2_NO_LOOP_REGION                          0
#define XAUDIO2_LOOP_INFINITE                           255
#define XAUDIO2_DEFAULT_CHANNELS                        0
#define XAUDIO2_DEFAULT_SAMPLERATE                      0
#define XAUDIO2_DEBUG_ENGINE                            0x0001
#define XAUDIO2_VOICE_NOPITCH                           0x0002
#define XAUDIO2_VOICE_NOSRC                             0x0004
#define XAUDIO2_VOICE_USEFILTER                         0x0008
#define XAUDIO2_PLAY_TAILS                              0x0020
#define XAUDIO2_END_OF_STREAM                           0x0040
#define XAUDIO2_SEND_USEFILTER                          0x0080
#define XAUDIO2_VOICE_NOSAMPLESPLAYED                   0x0100
#define XAUDIO2_STOP_ENGINE_WHEN_IDLE                   0x2000
#define XAUDIO2_1024_QUANTUM                            0x8000
#define XAUDIO2_NO_VIRTUAL_AUDIO_CLIENT                 0x10000
#define XAUDIO2_DEFAULT_FILTER_TYPE                     LowPassFilter
#define XAUDIO2_DEFAULT_FILTER_FREQUENCY                XAUDIO2_MAX_FILTER_FREQUENCY
#define XAUDIO2_DEFAULT_FILTER_ONEOVERQ                 1.0f
#define XAUDIO2_ANY_PROCESSOR                           0xffffffff
#define XAUDIO2_USE_DEFAULT_PROCESSOR                   0x00000000
#define XAUDIO2_QUANTUM_NUMERATOR                       1
#define XAUDIO2_QUANTUM_DENOMINATOR                     100
#define XAUDIO2_QUANTUM_MS (1000.0f * XAUDIO2_QUANTUM_NUMERATOR / XAUDIO2_QUANTUM_DENOMINATOR)
#define FACILITY_XAUDIO2                                0x896
#define XAUDIO2_E_INVALID_CALL                          ((int)0x88960001)
#define XAUDIO2_E_XMA_DECODER_ERROR                     ((int)0x88960002)
#define XAUDIO2_E_XAPO_CREATION_FAILED                  ((int)0x88960003)
#define XAUDIO2_E_DEVICE_INVALIDATED                    ((int)0x88960004)

typedef struct IXAudio2 IXAudio2;
typedef struct IXAudio2Voice IXAudio2Voice;
typedef struct IXAudio2SourceVoice IXAudio2SourceVoice;
typedef struct IXAudio2SubmixVoice IXAudio2SubmixVoice;
typedef struct IXAudio2MasteringVoice IXAudio2MasteringVoice;
typedef struct IXAudio2EngineCallback IXAudio2EngineCallback;
typedef struct IXAudio2VoiceCallback IXAudio2VoiceCallback;

typedef struct
{
    u32 d1;
    u16 d2;
    u16 d3;
    u8 d4[8];
} Win32_GUID;

typedef struct
{
    u16 wFormatTag;
    u16 nChannels;
    u32 nSamplesPerSec;
    u32 nAvgBytesPerSec;
    u16 nBlockAlign;
    u16 wBitsPerSample;
    u16 cbSize;
} Win32_WAVEFORMATEX;

typedef enum
{
    AudioCategory_Other = 0,
    AudioCategory_ForegroundOnlyMedia = 1,
    AudioCategory_Communications = 3,
    AudioCategory_Alerts = 4,
    AudioCategory_SoundEffects = 5,
    AudioCategory_GameEffects = 6,
    AudioCategory_GameMedia = 7,
    AudioCategory_GameChat = 8,
    AudioCategory_Speech = 9,
    AudioCategory_Movie = 10,
    AudioCategory_Media = 11,
    AudioCategory_FarFieldSpeech = 12,
    AudioCategory_UniformSpeech = 13,
    AudioCategory_VoiceTyping = 14,
} Win32_AUDIO_STREAM_CATEGORY;

#pragma pack(push, 1)

typedef enum
{
    LowPassFilter,
    BandPassFilter,
    HighPassFilter,
    NotchFilter,
    LowPassOnePoleFilter,
    HighPassOnePoleFilter
} Win32_XAUDIO2_FILTER_TYPE;

typedef struct
{
    u32 CreationFlags;
    u32 ActiveFlags;
    u32 InputChannels;
    u32 InputSampleRate;
} Win32_XAUDIO2_VOICE_DETAILS;

typedef struct
{
    u32 Flags;
    IXAudio2Voice* pOutputVoice;
} Win32_XAUDIO2_SEND_DESCRIPTOR;

typedef struct
{
    u32 SendCount;
    Win32_XAUDIO2_SEND_DESCRIPTOR* pSends;
} Win32_XAUDIO2_VOICE_SENDS;

typedef struct
{
    IUnknown* pEffect;
    b32 InitialState;
    u32 OutputChannels;
} Win32_XAUDIO2_EFFECT_DESCRIPTOR;

typedef struct
{
    u32 EffectCount;
    Win32_XAUDIO2_EFFECT_DESCRIPTOR* pEffectDescriptors;
} Win32_XAUDIO2_EFFECT_CHAIN;

typedef struct
{
    Win32_XAUDIO2_FILTER_TYPE Type;
    float Frequency;
    float OneOverQ;
} Win32_XAUDIO2_FILTER_PARAMETERS;

typedef struct
{
    u32 Flags;
    u32 AudioBytes;
    const byte* pAudioData;
    u32 PlayBegin;
    u32 PlayLength;
    u32 LoopBegin;
    u32 LoopLength;
    u32 LoopCount;
    void* pContext;
} Win32_XAUDIO2_BUFFER;

typedef struct
{
    const u32* pDecodedPacketCumulativeBytes;
    u32 PacketCount;
} Win32_XAUDIO2_BUFFER_WMA;

typedef struct
{
    void* pCurrentBufferContext;
    u32 BuffersQueued;
    u64 SamplesPlayed;
} Win32_XAUDIO2_VOICE_STATE;

typedef struct XAUDIO2_PERFORMANCE_DATA
{
    u64 AudioCyclesSinceLastQuery;
    u64 TotalCyclesSinceLastQuery;
    u32 MinimumCyclesPerQuantum;
    u32 MaximumCyclesPerQuantum;
    u32 MemoryUsageInBytes;
    u32 CurrentLatencyInSamples;
    u32 GlitchesSinceEngineStarted;
    u32 ActiveSourceVoiceCount;
    u32 TotalSourceVoiceCount;
    u32 ActiveSubmixVoiceCount;
    u32 ActiveResamplerCount;
    u32 ActiveMatrixMixCount;
    u32 ActiveXmaSourceVoices;
    u32 ActiveXmaStreams;
} Win32_XAUDIO2_PERFORMANCE_DATA;

typedef struct
{
    u32 TraceMask;
    u32 BreakMask;
    b32 LogThreadID;
    b32 LogFileline;
    b32 LogFunctionName;
    b32 LogTiming;
} Win32_XAUDIO2_DEBUG_CONFIGURATION;

typedef struct IXAudio2 { struct IXAudio2Vtbl * lpVtbl; } IXAudio2; 
typedef struct IXAudio2Vtbl IXAudio2Vtbl; 
struct IXAudio2Vtbl
{
    int (*QueryInterface)(IXAudio2 *This, const Win32_GUID * const riid, void** ppvInterface);
    u32 (*AddRef)(IXAudio2 *This);
    u32 (*Release)(IXAudio2 *This);
    int (*RegisterForCallbacks)(IXAudio2 *This, IXAudio2EngineCallback* pCallback);
    void (*UnregisterForCallbacks)(IXAudio2 *This, IXAudio2EngineCallback* pCallback);
    int (*CreateSourceVoice)(IXAudio2 *This, IXAudio2SourceVoice** ppSourceVoice, const Win32_WAVEFORMATEX* pSourceFormat, u32 Flags, float MaxFrequencyRatio, IXAudio2VoiceCallback* pCallback, const Win32_XAUDIO2_VOICE_SENDS* pSendList, const Win32_XAUDIO2_EFFECT_CHAIN* pEffectChain);
    int (*CreateSubmixVoice)(IXAudio2 *This, IXAudio2SubmixVoice** ppSubmixVoice, u32 InputChannels, u32 InputSampleRate, u32 Flags, u32 ProcessingStage, const Win32_XAUDIO2_VOICE_SENDS *pSendList, const Win32_XAUDIO2_EFFECT_CHAIN *pEffectChain);
    int (*CreateMasteringVoice)(IXAudio2 *This, IXAudio2MasteringVoice** ppMasteringVoice, u32 InputChannels, u32 InputSampleRate, u32 Flags, cstr szDeviceId, const Win32_XAUDIO2_EFFECT_CHAIN* pEffectChain, Win32_AUDIO_STREAM_CATEGORY StreamCategory);
    int (*StartEngine)(IXAudio2 *This);
    void (*StopEngine)(IXAudio2 *This);
    int (*CommitChanges)(IXAudio2 *This, u32 OperationSet);
    void (*GetPerformanceData)(IXAudio2 *This, Win32_XAUDIO2_PERFORMANCE_DATA* pPerfData);
    void (*SetDebugConfiguration)(IXAudio2 *This, const Win32_XAUDIO2_DEBUG_CONFIGURATION* pDebugConfiguration, ptr Reserved);
};

typedef struct IXAudio2Extension { struct IXAudio2ExtensionVtbl * lpVtbl; } IXAudio2Extension; typedef struct IXAudio2ExtensionVtbl IXAudio2ExtensionVtbl; 
struct IXAudio2ExtensionVtbl
{
    int (*QueryInterface)(IXAudio2Extension *This, const Win32_GUID * const riid, void** ppvInterface);
    u32 (*AddRef)(IXAudio2Extension *This);
    u32 (*Release)(IXAudio2Extension *This);
    void (*GetProcessingQuantum)(IXAudio2Extension *This, u32 *quantumNumerator, u32 *quantumDenominator);
    void(*GetProcessor)(IXAudio2Extension *This, u32* processor);
};

#define IXAudio2VoiceFunctions \
void (*GetVoiceDetails)(IXAudio2Voice *This, Win32_XAUDIO2_VOICE_DETAILS* pVoiceDetails); \
int (*SetOutputVoices)(IXAudio2Voice *This, const Win32_XAUDIO2_VOICE_SENDS* pSendList); \
int (*SetEffectChain)(IXAudio2Voice *This, const Win32_XAUDIO2_EFFECT_CHAIN* pEffectChain); \
int (*EnableEffect)(IXAudio2Voice *This, u32 EffectIndex, u32 OperationSet); \
int (*DisableEffect)(IXAudio2Voice * This, u32 EffectIndex, u32 OperationSet ); \
void (*GetEffectState)(IXAudio2Voice *This, u32 EffectIndex, b32* pEnabled); \
int (*SetEffectParameters)(IXAudio2Voice *This, u32 EffectIndex, const void* pParameters, u32 ParametersByteSize, u32 OperationSet); \
int (*GetEffectParameters)(IXAudio2Voice *This, u32 EffectIndex, void* pParameters, u32 ParametersByteSize); \
int (*SetFilterParameters)(IXAudio2Voice *This, const Win32_XAUDIO2_FILTER_PARAMETERS* pParameters, u32 OperationSet); \
void (*GetFilterParameters)(IXAudio2Voice *This, Win32_XAUDIO2_FILTER_PARAMETERS* pParameters); \
int (*SetOutputFilterParameters)(IXAudio2Voice *This, IXAudio2Voice* pDestinationVoice, const Win32_XAUDIO2_FILTER_PARAMETERS* pParameters, u32 OperationSet); \
void (*GetOutputFilterParameters)(IXAudio2Voice *This, IXAudio2Voice* pDestinationVoice,   Win32_XAUDIO2_FILTER_PARAMETERS* pParameters); \
int (*SetVolume)(IXAudio2Voice *This, float Volume, u32 OperationSet); \
void (*GetVolume)(IXAudio2Voice *This, float* pVolume); \
int (*SetChannelVolumes)(IXAudio2Voice *This, u32 Channels, const float* pVolumes, u32 OperationSet); \
void (*GetChannelVolumes)(IXAudio2Voice *This, u32 Channels, float* pVolumes); \
int (*SetOutputMatrix)(IXAudio2Voice *This, IXAudio2Voice* pDestinationVoice, u32 SourceChannels, u32 DestinationChannels, const float* pLevelMatrix, u32 OperationSet); \
void (*GetOutputMatrix)(IXAudio2Voice * This, IXAudio2Voice* pDestinationVoice, u32 SourceChannels, u32 DestinationChannels, float* pLevelMatrix); \
void (*DestroyVoice)(IXAudio2Voice *This);

typedef struct IXAudio2Voice { struct IXAudio2VoiceVtbl * lpVtbl; } IXAudio2Voice; typedef struct IXAudio2VoiceVtbl IXAudio2VoiceVtbl; 
struct IXAudio2VoiceVtbl
{
    IXAudio2VoiceFunctions;
};

typedef struct IXAudio2SourceVoice { struct IXAudio2SourceVoiceVtbl * lpVtbl; } IXAudio2SourceVoice; typedef struct IXAudio2SourceVoiceVtbl IXAudio2SourceVoiceVtbl; 
struct IXAudio2SourceVoiceVtbl
{
    IXAudio2VoiceFunctions;

    int (*Start)(IXAudio2SourceVoice *This, u32 Flags, u32 OperationSet);
    int (*Stop)(IXAudio2SourceVoice *This, u32 Flags, u32 OperationSet);
    int (*SubmitSourceBuffer)(IXAudio2SourceVoice *This, const Win32_XAUDIO2_BUFFER* pBuffer,   const Win32_XAUDIO2_BUFFER_WMA* pBufferWMA);
    int (*FlushSourceBuffers)(IXAudio2SourceVoice *This);
    int (*Discontinuity)(IXAudio2SourceVoice *This);
    int (*ExitLoop)(IXAudio2SourceVoice *This, u32 OperationSet);
    void (*GetState)(IXAudio2SourceVoice *This, Win32_XAUDIO2_VOICE_STATE* pVoiceState, u32 Flags);
    int (*SetFrequencyRatio)(IXAudio2SourceVoice *This, float Ratio, u32 OperationSet);
    void (*GetFrequencyRatio)(IXAudio2SourceVoice *This, float* pRatio);
    int (*SetSourceSampleRate)(IXAudio2SourceVoice *This, u32 NewSourceSampleRate);
};

typedef struct IXAudio2SubmixVoice { struct IXAudio2SubmixVoiceVtbl * lpVtbl; } IXAudio2SubmixVoice; typedef struct IXAudio2SubmixVoiceVtbl IXAudio2SubmixVoiceVtbl; 
struct IXAudio2SubmixVoiceVtbl
{
    IXAudio2VoiceFunctions;
};

typedef struct IXAudio2MasteringVoice { struct IXAudio2MasteringVoiceVtbl * lpVtbl; } IXAudio2MasteringVoice; typedef struct IXAudio2MasteringVoiceVtbl IXAudio2MasteringVoiceVtbl; 
struct IXAudio2MasteringVoiceVtbl
{
    IXAudio2VoiceFunctions;

    int (*GetChannelMask)(IXAudio2MasteringVoice * This, u32* pChannelmask);
};

typedef struct IXAudio2EngineCallback { struct IXAudio2EngineCallbackVtbl * lpVtbl; } IXAudio2EngineCallback; typedef struct IXAudio2EngineCallbackVtbl IXAudio2EngineCallbackVtbl; 
struct IXAudio2EngineCallbackVtbl
{
    void (*OnProcessingPassStart)(IXAudio2EngineCallback *This);
    void (*OnProcessingPassEnd)(IXAudio2EngineCallback *This);
    void (*OnCriticalError)(IXAudio2EngineCallback *This, int Error);
};

typedef struct IXAudio2VoiceCallback { struct IXAudio2VoiceCallbackVtbl * lpVtbl; } IXAudio2VoiceCallback; typedef struct IXAudio2VoiceCallbackVtbl IXAudio2VoiceCallbackVtbl; 
struct IXAudio2VoiceCallbackVtbl
{
    void (*OnVoiceProcessingPassStart)(IXAudio2VoiceCallback *This, u32 BytesRequired); 
    void (*OnVoiceProcessingPassEnd)(IXAudio2VoiceCallback *This); 
    void (*OnStreamEnd)(IXAudio2VoiceCallback *This); 
    void (*OnBufferStart)(IXAudio2VoiceCallback *This, void* pBufferContext); 
    void (*OnBufferEnd)(IXAudio2VoiceCallback *This, void* pBufferContext); 
    void (*OnLoopEnd)(IXAudio2VoiceCallback *This, void* pBufferContext); 
    void (*OnVoiceError)(IXAudio2VoiceCallback *This, void* pBufferContext, int Error);
};

// IXAudio2
#define IXAudio2_QueryInterface(This,riid,ppvInterface) ((This)->lpVtbl->QueryInterface(This,riid,ppvInterface))
#define IXAudio2_AddRef(This) ((This)->lpVtbl->AddRef(This))
#define IXAudio2_Release(This) ((This)->lpVtbl->Release(This))
#define IXAudio2_RegisterForCallbacks(This,pCallback) ((This)->lpVtbl->RegisterForCallbacks(This,pCallback))
#define IXAudio2_UnregisterForCallbacks(This,pCallback) ((This)->lpVtbl->UnregisterForCallbacks(This,pCallback))
#define IXAudio2_CreateSourceVoice(This,ppSourceVoice,pSourceFormat,Flags,MaxFrequencyRatio,pCallback,pSendList,pEffectChain) ((This)->lpVtbl->CreateSourceVoice(This,ppSourceVoice,pSourceFormat,Flags,MaxFrequencyRatio,pCallback,pSendList,pEffectChain))
#define IXAudio2_CreateSubmixVoice(This,ppSubmixVoice,InputChannels,InputSampleRate,Flags,ProcessingStage,pSendList,pEffectChain) ((This)->lpVtbl->CreateSubmixVoice(This,ppSubmixVoice,InputChannels,InputSampleRate,Flags,ProcessingStage,pSendList,pEffectChain))
#define IXAudio2_CreateMasteringVoice(This,ppMasteringVoice,InputChannels,InputSampleRate,Flags,DeviceId,pEffectChain,StreamCategory) ((This)->lpVtbl->CreateMasteringVoice(This,ppMasteringVoice,InputChannels,InputSampleRate,Flags,DeviceId,pEffectChain,StreamCategory))
#define IXAudio2_StartEngine(This) ((This)->lpVtbl->StartEngine(This))
#define IXAudio2_StopEngine(This) ((This)->lpVtbl->StopEngine(This))
#define IXAudio2_CommitChanges(This,OperationSet) ((This)->lpVtbl->CommitChanges(This,OperationSet))
#define IXAudio2_GetPerformanceData(This,pPerfData) ((This)->lpVtbl->GetPerformanceData(This,pPerfData))
#define IXAudio2_SetDebugConfiguration(This,pDebugConfiguration,pReserved) ((This)->lpVtbl->SetDebugConfiguration(This,pDebugConfiguration,pReserved))

// IXAudio2Extension
#define IXAudio2Extension_GetProcessingQuantum(This,quantumNumerator,quantumDenominator) ((This)->lpVtbl->GetProcessingQuantum(This,quantumNumerator,quantumDenominator))
#define IXAudio2Extension_GetProcessor(This,processor) ((This)->lpVtbl->GetProcessor(This,processor))

// IXAudio2Voice
#define IXAudio2Voice_GetVoiceDetails(This,pVoiceDetails) ((This)->lpVtbl->GetVoiceDetails(This,pVoiceDetails))
#define IXAudio2Voice_SetOutputVoices(This,pSendList) ((This)->lpVtbl->SetOutputVoices(This,pSendList))
#define IXAudio2Voice_SetEffectChain(This,pEffectChain) ((This)->lpVtbl->SetEffectChain(This,pEffectChain))
#define IXAudio2Voice_EnableEffect(This,EffectIndex,OperationSet) ((This)->lpVtbl->EnableEffect(This,EffectIndex,OperationSet))
#define IXAudio2Voice_DisableEffect(This,EffectIndex,OperationSet) ((This)->lpVtbl->DisableEffect(This,EffectIndex,OperationSet))
#define IXAudio2Voice_GetEffectState(This,EffectIndex,pEnabled) ((This)->lpVtbl->GetEffectState(This,EffectIndex,pEnabled))
#define IXAudio2Voice_SetEffectParameters(This,EffectIndex,pParameters,ParametersByteSize, OperationSet) ((This)->lpVtbl->SetEffectParameters(This,EffectIndex,pParameters,ParametersByteSize,OperationSet))
#define IXAudio2Voice_GetEffectParameters(This,EffectIndex,pParameters,ParametersByteSize) ((This)->lpVtbl->GetEffectParameters(This,EffectIndex,pParameters,ParametersByteSize))
#define IXAudio2Voice_SetFilterParameters(This,pParameters,OperationSet) ((This)->lpVtbl->SetFilterParameters(This,pParameters,OperationSet))
#define IXAudio2Voice_GetFilterParameters(This,pParameters) ((This)->lpVtbl->GetFilterParameters(This,pParameters))
#define IXAudio2Voice_SetOutputFilterParameters(This,pDestinationVoice,pParameters,OperationSet) ((This)->lpVtbl->SetOutputFilterParameters(This,pDestinationVoice,pParameters,OperationSet))
#define IXAudio2Voice_GetOutputFilterParameters(This,pDestinationVoice,pParameters) ((This)->lpVtbl->GetOutputFilterParameters(This,pDestinationVoice,pParameters))
#define IXAudio2Voice_SetVolume(This,Volume,OperationSet) ((This)->lpVtbl->SetVolume(This,Volume,OperationSet))
#define IXAudio2Voice_GetVolume(This,pVolume) ((This)->lpVtbl->GetVolume(This,pVolume))
#define IXAudio2Voice_SetChannelVolumes(This,Channels,pVolumes,OperationSet) ((This)->lpVtbl->SetChannelVolumes(This,Channels,pVolumes,OperationSet))
#define IXAudio2Voice_GetChannelVolumes(This,Channels,pVolumes) ((This)->lpVtbl->GetChannelVolumes(This,Channels,pVolumes))
#define IXAudio2Voice_SetOutputMatrix(This,pDestinationVoice,SourceChannels,DestinationChannels,pLevelMatrix,OperationSet) ((This)->lpVtbl->SetOutputMatrix(This,pDestinationVoice,SourceChannels,DestinationChannels,pLevelMatrix,OperationSet))
#define IXAudio2Voice_GetOutputMatrix(This,pDestinationVoice,SourceChannels,DestinationChannels,pLevelMatrix) ((This)->lpVtbl->GetOutputMatrix(This,pDestinationVoice,SourceChannels,DestinationChannels,pLevelMatrix))
#define IXAudio2Voice_DestroyVoice(This) ((This)->lpVtbl->DestroyVoice(This))

// IXAudio2SourceVoice
#define IXAudio2SourceVoice_GetVoiceDetails IXAudio2Voice_GetVoiceDetails
#define IXAudio2SourceVoice_SetOutputVoices IXAudio2Voice_SetOutputVoices
#define IXAudio2SourceVoice_SetEffectChain IXAudio2Voice_SetEffectChain
#define IXAudio2SourceVoice_EnableEffect IXAudio2Voice_EnableEffect
#define IXAudio2SourceVoice_DisableEffect IXAudio2Voice_DisableEffect
#define IXAudio2SourceVoice_GetEffectState IXAudio2Voice_GetEffectState
#define IXAudio2SourceVoice_SetEffectParameters IXAudio2Voice_SetEffectParameters
#define IXAudio2SourceVoice_GetEffectParameters IXAudio2Voice_GetEffectParameters
#define IXAudio2SourceVoice_SetFilterParameters IXAudio2Voice_SetFilterParameters
#define IXAudio2SourceVoice_GetFilterParameters IXAudio2Voice_GetFilterParameters
#define IXAudio2SourceVoice_SetOutputFilterParameters IXAudio2Voice_SetOutputFilterParameters
#define IXAudio2SourceVoice_GetOutputFilterParameters IXAudio2Voice_GetOutputFilterParameters
#define IXAudio2SourceVoice_SetVolume IXAudio2Voice_SetVolume
#define IXAudio2SourceVoice_GetVolume IXAudio2Voice_GetVolume
#define IXAudio2SourceVoice_SetChannelVolumes IXAudio2Voice_SetChannelVolumes
#define IXAudio2SourceVoice_GetChannelVolumes IXAudio2Voice_GetChannelVolumes
#define IXAudio2SourceVoice_SetOutputMatrix IXAudio2Voice_SetOutputMatrix
#define IXAudio2SourceVoice_GetOutputMatrix IXAudio2Voice_GetOutputMatrix
#define IXAudio2SourceVoice_DestroyVoice IXAudio2Voice_DestroyVoice
#define IXAudio2SourceVoice_Start(This,Flags,OperationSet) ((This)->lpVtbl->Start(This,Flags,OperationSet))
#define IXAudio2SourceVoice_Stop(This,Flags,OperationSet) ((This)->lpVtbl->Stop(This,Flags,OperationSet))
#define IXAudio2SourceVoice_SubmitSourceBuffer(This,pBuffer,pBufferWMA) ((This)->lpVtbl->SubmitSourceBuffer(This,pBuffer,pBufferWMA))
#define IXAudio2SourceVoice_FlushSourceBuffers(This) ((This)->lpVtbl->FlushSourceBuffers(This))
#define IXAudio2SourceVoice_Discontinuity(This) ((This)->lpVtbl->Discontinuity(This))
#define IXAudio2SourceVoice_ExitLoop(This,OperationSet) ((This)->lpVtbl->ExitLoop(This,OperationSet))
#define IXAudio2SourceVoice_GetState(This,pVoiceState,Flags) ((This)->lpVtbl->GetState(This,pVoiceState,Flags))
#define IXAudio2SourceVoice_SetFrequencyRatio(This,Ratio,OperationSet) ((This)->lpVtbl->SetFrequencyRatio(This,Ratio,OperationSet))
#define IXAudio2SourceVoice_GetFrequencyRatio(This,pRatio) ((This)->lpVtbl->GetFrequencyRatio(This,pRatio))
#define IXAudio2SourceVoice_SetSourceSampleRate(This,NewSourceSampleRate) ((This)->lpVtbl->SetSourceSampleRate(This,NewSourceSampleRate))

// IXAudio2SubmixVoice
#define IXAudio2SubmixVoice_GetVoiceDetails IXAudio2Voice_GetVoiceDetails
#define IXAudio2SubmixVoice_SetOutputVoices IXAudio2Voice_SetOutputVoices
#define IXAudio2SubmixVoice_SetEffectChain IXAudio2Voice_SetEffectChain
#define IXAudio2SubmixVoice_EnableEffect IXAudio2Voice_EnableEffect
#define IXAudio2SubmixVoice_DisableEffect IXAudio2Voice_DisableEffect
#define IXAudio2SubmixVoice_GetEffectState IXAudio2Voice_GetEffectState
#define IXAudio2SubmixVoice_SetEffectParameters IXAudio2Voice_SetEffectParameters
#define IXAudio2SubmixVoice_GetEffectParameters IXAudio2Voice_GetEffectParameters
#define IXAudio2SubmixVoice_SetFilterParameters IXAudio2Voice_SetFilterParameters
#define IXAudio2SubmixVoice_GetFilterParameters IXAudio2Voice_GetFilterParameters
#define IXAudio2SubmixVoice_SetOutputFilterParameters IXAudio2Voice_SetOutputFilterParameters
#define IXAudio2SubmixVoice_GetOutputFilterParameters IXAudio2Voice_GetOutputFilterParameters
#define IXAudio2SubmixVoice_SetVolume IXAudio2Voice_SetVolume
#define IXAudio2SubmixVoice_GetVolume IXAudio2Voice_GetVolume
#define IXAudio2SubmixVoice_SetChannelVolumes IXAudio2Voice_SetChannelVolumes
#define IXAudio2SubmixVoice_GetChannelVolumes IXAudio2Voice_GetChannelVolumes
#define IXAudio2SubmixVoice_SetOutputMatrix IXAudio2Voice_SetOutputMatrix
#define IXAudio2SubmixVoice_GetOutputMatrix IXAudio2Voice_GetOutputMatrix
#define IXAudio2SubmixVoice_DestroyVoice IXAudio2Voice_DestroyVoice

// IXAudio2MasteringVoice
#define IXAudio2MasteringVoice_GetVoiceDetails IXAudio2Voice_GetVoiceDetails
#define IXAudio2MasteringVoice_SetOutputVoices IXAudio2Voice_SetOutputVoices
#define IXAudio2MasteringVoice_SetEffectChain IXAudio2Voice_SetEffectChain
#define IXAudio2MasteringVoice_EnableEffect IXAudio2Voice_EnableEffect
#define IXAudio2MasteringVoice_DisableEffect IXAudio2Voice_DisableEffect
#define IXAudio2MasteringVoice_GetEffectState IXAudio2Voice_GetEffectState
#define IXAudio2MasteringVoice_SetEffectParameters IXAudio2Voice_SetEffectParameters
#define IXAudio2MasteringVoice_GetEffectParameters IXAudio2Voice_GetEffectParameters
#define IXAudio2MasteringVoice_SetFilterParameters IXAudio2Voice_SetFilterParameters
#define IXAudio2MasteringVoice_GetFilterParameters IXAudio2Voice_GetFilterParameters
#define IXAudio2MasteringVoice_SetOutputFilterParameters IXAudio2Voice_SetOutputFilterParameters
#define IXAudio2MasteringVoice_GetOutputFilterParameters IXAudio2Voice_GetOutputFilterParameters
#define IXAudio2MasteringVoice_SetVolume IXAudio2Voice_SetVolume
#define IXAudio2MasteringVoice_GetVolume IXAudio2Voice_GetVolume
#define IXAudio2MasteringVoice_SetChannelVolumes IXAudio2Voice_SetChannelVolumes
#define IXAudio2MasteringVoice_GetChannelVolumes IXAudio2Voice_GetChannelVolumes
#define IXAudio2MasteringVoice_SetOutputMatrix IXAudio2Voice_SetOutputMatrix
#define IXAudio2MasteringVoice_GetOutputMatrix IXAudio2Voice_GetOutputMatrix
#define IXAudio2MasteringVoice_DestroyVoice IXAudio2Voice_DestroyVoice
#define IXAudio2MasteringVoice_GetChannelMask(This,pChannelMask) ((This)->lpVtbl->GetChannelMask(This,pChannelMask))

#pragma pack(pop)

typedef enum
{
    XAUDIO_NOT_LOADED = 0,
    XAUDIO_2_9,
    XAUDIO_2_9D,
    XAUDIO_2_8,
    XAUDIO_2_7
} Win32_XAUDIO_VERSION;

typedef int (*pfn_XAudio2Create)(IXAudio2 **ppXAudio2, u32 Flags, u32 XAudio2Processor);

typedef struct
{
    Win32_XAUDIO_VERSION version;

    IXAudio2 *handle;
    IXAudio2MasteringVoice *master_voice;
    IXAudio2SourceVoice *src_voice;
    Win32_WAVEFORMATEX fmt;
    Win32_XAUDIO2_BUFFER buffer_header;
    
    pfn_XAudio2Create XAudio2Create;
} Win32_XAudio;

#endif /* ASCENCIA_WIN32_API_H */
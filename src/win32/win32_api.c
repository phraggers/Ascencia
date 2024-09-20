/*============================================================
 * Project: Ascencia
 * File: win32_api.c
 * Author: Phraggers
 * Date: Thu Sep 12 2024
============================================================*/

#include <platform/alloc.h>
#include <platform/logging.h>
#include <win32/win32_state.h>
#include <win32/win32_api.h>

/* platform allocations (kernel32.dll) */
extern ptr HeapAlloc(ptr heap, u32 flags, u64 bytes);
extern ptr HeapReAlloc(ptr heap, u32 flags, ptr old_mem, u64 bytes);
extern i32 HeapFree(ptr heap, u32 flags, ptr mem);
extern ptr GetProcessHeap(void);

/* kernel32.dll */
extern ptr GetModuleHandleA(const cstr module_name);
extern cstr GetCommandLineA(void);
extern ptr LoadLibraryA(const cstr lib_name);
extern ptr GetProcAddress(ptr module, const cstr proc_name);
extern u32 GetModuleFileNameA(ptr module, cstr filename, u32 size);
extern ptr AddDllDirectory(cstr path);
extern u32 GetLastError(void);
extern ptr CreateFileA(cstr path, u32 access, u32 smode, ptr sec, u32 creation, u32 flags, ptr ftemplate);
extern b32 CloseHandle(ptr handle);
extern b32 GetFileSizeEx(ptr handle, Win32_LARGE_INTEGER *size);
extern b32 ReadFile(ptr handle, ptr buffer, u32 bytes_to_read, u32 *bytes_read, ptr overlapped);
extern b32 WriteFile(ptr handle, const ptr buffer, u32 bytes_to_write, u32 *bytes_written, ptr overlapped);
extern b32 DeleteFileA(cstr path);
extern b32 CreateDirectoryA(cstr path, ptr sec);
extern b32 RemoveDirectoryA(cstr path);
extern b32 QueryPerformanceFrequency(Win32_LARGE_INTEGER *frequency);
extern b32 QueryPerformanceCounter(Win32_LARGE_INTEGER *frequency);
extern void GetLocalTime(Win32_SYSTEMTIME *st);

#define LOAD_KERNEL_FN(name) {api->##name = &##name; numfn++;}
#define LOAD_WIN_FN(lib, name) {api->##name = (pfn_##name)api->GetProcAddress(lib, #name); if(!api->##name) {PL_Log(LOG_FATAL, "failed to load DLL FN: %s", #name); return 0;} else numfn++;}

ptr PL_Alloc(u64 size)
{
    if(!size)
    {
        return 0;
    }

    ptr result = 0;
    result = HeapAlloc(GetProcessHeap(), 0, size);
    return result;
}

ptr PL_Alloc0(u64 size)
{
    if(!size)
    {
        return 0;
    }

    ptr result = 0;
    result = HeapAlloc(GetProcessHeap(), 0x00000008, size);
    return result;
}

ptr PL_ReAlloc(ptr old_mem, u64 new_size)
{
    if(old_mem == 0 || new_size == 0)
    {
        return 0;
    }

    ptr result = 0;
    result = HeapReAlloc(GetProcessHeap(), 0, old_mem, new_size);
    return result;
}

ptr PL_ReAlloc0(ptr old_mem, u64 new_size)
{
    if(old_mem == 0 || new_size == 0)
    {
        return 0;
    }

    ptr result = 0;
    result = HeapReAlloc(GetProcessHeap(), 0x00000008, old_mem, new_size);
    return result;
}

b32 PL_Free(ptr mem)
{
    if(!mem) 
    {
        return 0;
    }

    b32 result = 0;

    if(HeapFree(GetProcessHeap(), 0, mem))
    {
        result = 1;
    }

    mem = 0;
    return result;
}

bool Win32_LoadAPI(void)
{
    Assert(G_win32_state);
    Win32_API *api = &WINAPI;

    int numfn = 0;

    LOAD_KERNEL_FN(GetModuleHandleA);
    LOAD_KERNEL_FN(GetCommandLineA);
    LOAD_KERNEL_FN(LoadLibraryA);
    LOAD_KERNEL_FN(GetProcAddress);
    LOAD_KERNEL_FN(GetModuleFileNameA);
    LOAD_KERNEL_FN(AddDllDirectory);
    LOAD_KERNEL_FN(GetLastError);
    LOAD_KERNEL_FN(CreateFileA);
    LOAD_KERNEL_FN(CloseHandle);
    LOAD_KERNEL_FN(GetFileSizeEx);
    LOAD_KERNEL_FN(ReadFile);
    LOAD_KERNEL_FN(WriteFile);
    LOAD_KERNEL_FN(DeleteFileA);
    LOAD_KERNEL_FN(CreateDirectoryA);
    LOAD_KERNEL_FN(RemoveDirectoryA);
    LOAD_KERNEL_FN(QueryPerformanceCounter);
    LOAD_KERNEL_FN(QueryPerformanceFrequency);
    LOAD_KERNEL_FN(GetLocalTime);


    ptr lib_shlwapi = api->LoadLibraryA("shlwapi.dll");
    if(!lib_shlwapi)
    {
        PL_Log(LOG_FATAL, "Failed to load shlwapi.dll");
        return 0;
    }

    ptr lib_user32 = api->LoadLibraryA("user32.dll");
    if(!lib_user32)
    {
        PL_Log(LOG_FATAL, "Failed to load user32.dll");
        return 0;
    }

    ptr lib_gdi32 = api->LoadLibraryA("gdi32.dll");
    if(!lib_gdi32)
    {
        PL_Log(LOG_FATAL, "Failed to load gdi32.dll");
        return 0;
    }

    ptr lib_opengl32 = api->LoadLibraryA("opengl32.dll");
    if(!lib_opengl32)
    {
        PL_Log(LOG_FATAL, "Failed to load opengl32.dll");
        return 0;
    }

    LOAD_WIN_FN(lib_shlwapi, PathFileExistsA);

    LOAD_WIN_FN(lib_user32, TranslateMessage);
    LOAD_WIN_FN(lib_user32, DispatchMessageA);
    LOAD_WIN_FN(lib_user32, PeekMessageA);
    LOAD_WIN_FN(lib_user32, DefWindowProcA);
    LOAD_WIN_FN(lib_user32, RegisterClassA);
    LOAD_WIN_FN(lib_user32, CreateWindowExA);
    LOAD_WIN_FN(lib_user32, DestroyWindow);
    LOAD_WIN_FN(lib_user32, MoveWindow);
    LOAD_WIN_FN(lib_user32, SetWindowPos);
    LOAD_WIN_FN(lib_user32, GetWindowPlacement);
    LOAD_WIN_FN(lib_user32, SetWindowPlacement);
    LOAD_WIN_FN(lib_user32, GetDC);
    LOAD_WIN_FN(lib_user32, ReleaseDC);
    LOAD_WIN_FN(lib_user32, SetWindowTextA);
    LOAD_WIN_FN(lib_user32, MessageBoxA);
    LOAD_WIN_FN(lib_user32, GetCursorPos);
    LOAD_WIN_FN(lib_user32, ScreenToClient);
    LOAD_WIN_FN(lib_user32, GetWindowLongA);
    LOAD_WIN_FN(lib_user32, SetWindowLongA);
    LOAD_WIN_FN(lib_user32, MonitorFromWindow);
    LOAD_WIN_FN(lib_user32, GetMonitorInfoA);
    LOAD_WIN_FN(lib_user32, GetClientRect);
    LOAD_WIN_FN(lib_user32, GetWindowRect);

    LOAD_WIN_FN(lib_gdi32, ChoosePixelFormat);
    LOAD_WIN_FN(lib_gdi32, DescribePixelFormat);
    LOAD_WIN_FN(lib_gdi32, SetPixelFormat);
    LOAD_WIN_FN(lib_gdi32, SwapBuffers);

    LOAD_WIN_FN(lib_opengl32, wglCreateContext);
    LOAD_WIN_FN(lib_opengl32, wglDeleteContext);
    LOAD_WIN_FN(lib_opengl32, wglGetProcAddress);
    LOAD_WIN_FN(lib_opengl32, wglMakeCurrent);

    PL_Log(LOG_INFO, "Win32 API: %d functions loaded", numfn);
    return 1;
}

u64 Win32_GetPerfCount(void)
{
    u64 result = 0;
    Win32_LARGE_INTEGER perf;
    QueryPerformanceCounter(&perf);
    result = (u64)perf.QuadPart;
    return result;
}

u32 NotLoaded_XInputGetState(u32 user_index, Win32_XINPUT_STATE *state)
{
    return 1167L; //ERROR_DEVICE_NOT_CONNECTED
}

u32 NotLoaded_XInputSetState(u32 user_index, Win32_XINPUT_VIBRATION *vib)
{
    return 1167L; //ERROR_DEVICE_NOT_CONNECTED
}

bool Win32_LoadXInput(void)
{
    Assert(G_win32_state);
    Win32_XInput *api = &XINPUT_API;
    
    ptr xinput_dll = WINAPI.LoadLibraryA("xinput1_4.dll");

    if(xinput_dll)
    {
        api->version = XINPUT_1_4;
    }

    else
    {
        xinput_dll = WINAPI.LoadLibraryA("xinput9_1_0.dll");

        if(xinput_dll)
        {
            api->version = XINPUT_9_1_0;
        }

        else
        {
            xinput_dll = WINAPI.LoadLibraryA("xinput1_3.dll");

            if(xinput_dll)
            {
                api->version = XINPUT_1_3;
            }

            else
            {
                PL_Log(LOG_ERROR, "LoadXInput: failed to load xinput");
                api->XInputGetState = &NotLoaded_XInputGetState;
                api->XInputSetState = &NotLoaded_XInputSetState;
                return 0;
            }
        }
    }

    return 1;
}

int NotLoaded_XAudio2Create(IXAudio2 **ppXAudio2, u32 Flags, u32 XAudio2Processor)
{
    return ((int)0x88960003); //XAUDIO2_E_XAPO_CREATION_FAILED
}

int NotLoaded_XAudio2CreateWithVersionInfo(IXAudio2** ppXAudio2, u32 Flags, u32 XAudio2Processor, u32 ntddiVersion)
{
    return ((int)0x88960003); //XAUDIO2_E_XAPO_CREATION_FAILED
}

int Intercept_XAudio2Create(IXAudio2 **ppXAudio2, u32 Flags, u32 XAudio2Processor)
{
    const u32 ntddi[] =
    {
        0x06010000, 0x06020000, 0x06030000,
        0x0A000000, 0x0A000000, 0x0A000001,
        0x0A000002, 0x0A000003, 0x0A000004,
        0x0A000005, 0x0A000006, 0x0A000007,
        0x0A000008, 0x0A000009, 0x0A00000A,
        0x0A00000B, 0x0A00000C
    };
    
    for(int i=ArrayCount(ntddi)-1; i>=0; i--)
    {
        if(XAUDIO_API.XAudio2CreateWithVersionInfo(ppXAudio2, Flags, XAudio2Processor, ntddi[i]) == 0)
        {
            return 0;
        }
    }

    return XAUDIO_API.XAudio2CreateInternal(ppXAudio2, Flags, XAudio2Processor);
}

bool Win32_LoadXAudio(void)
{
    Assert(G_win32_state);
    Win32_XAudio *api = &XAUDIO_API;
    api->version = XAUDIO_NOT_LOADED;
    ptr xaudio_dll = WINAPI.LoadLibraryA("xaudio2_9.dll");
    
    if(xaudio_dll)
    {
        api->version = XAUDIO_2_9;
    }
    
    else
    {
        xaudio_dll = LoadLibraryA("xaudio2_9d.dll");
        
        if(xaudio_dll)
        {
            api->version = XAUDIO_2_9D;
        }
        
        else
        {
            xaudio_dll = LoadLibraryA("xaudio2_8.dll");
            
            if(xaudio_dll)
            {
                api->version = XAUDIO_2_8;
            }
            
            else
            {
                xaudio_dll = LoadLibraryA("xaudio2_7.dll");
                
                if(xaudio_dll)
                {
                    api->version = XAUDIO_2_7;
                }
                
                else
                {
                    //NOTE: if xaudio doesnt load, audio just wont play
                    api->version = XAUDIO_NOT_LOADED;
                    PL_Log(LOG_ERROR, "LoadXAudio: failed to load xaudio");
                    XAUDIO_API.XAudio2Create = &NotLoaded_XAudio2Create;
                    XAUDIO_API.XAudio2CreateInternal = &NotLoaded_XAudio2Create;
                    XAUDIO_API.XAudio2CreateWithVersionInfo = &NotLoaded_XAudio2CreateWithVersionInfo;
                    return 0;
                }
            }
        }
    }

    XAUDIO_API.XAudio2Create = &Intercept_XAudio2Create;
    XAUDIO_API.XAudio2CreateInternal = (pfn_XAudio2Create)WINAPI.GetProcAddress(xaudio_dll, "XAudio2Create");
    XAUDIO_API.XAudio2CreateWithVersionInfo = (pfn_XAudio2CreateWithVersionInfo)WINAPI.GetProcAddress(xaudio_dll, "XAudio2CreateWithVersionInfo");

    return 1;
}
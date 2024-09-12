
/*============================================================
 * Project: Ascencia
 * File: win32_api.c
 * Author: Phraggers
 * Date: Thu Sep 12 2024
============================================================*/

#include <win32/win32_state.h>
#include <win32/win32_api.h>
#include <platform/alloc.h>
#include <platform/logging.h>

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

#define LOAD_WIN_FN(lib, name) {api->##name = (pfn_##name)api->GetProcAddress(lib, #name); if(!api->##name) {PL_Log(LOG_FATAL, "failed to load DLL FN: %s", #name); return 0;}}

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
    Win32_API *api = &G_win32_state->api;

    api->GetModuleHandleA = &GetModuleHandleA;
    api->GetCommandLineA = &GetCommandLineA;
    api->LoadLibraryA = &LoadLibraryA;
    api->GetProcAddress = &GetProcAddress;
    api->GetModuleFileNameA = &GetModuleFileNameA;
    api->AddDllDirectory = &AddDllDirectory;
    api->GetLastError = &GetLastError;
    api->CreateFileA = &CreateFileA;
    api->CloseHandle = &CloseHandle;
    api->GetFileSizeEx = &GetFileSizeEx;
    api->ReadFile = &ReadFile;
    api->WriteFile = &WriteFile;
    api->DeleteFileA = &DeleteFileA;
    api->CreateDirectoryA = &CreateDirectoryA;
    api->RemoveDirectoryA = &RemoveDirectoryA;

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

    PL_Log(LOG_DEBUG, "Win32 API functions loaded");
    return 1;
}
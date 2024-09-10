/*============================================================
 * Project: Ascencia [EXE]
 * File: interface.c
 * Author: Phraggers
 * Date: Sat Sep 07 2024
============================================================*/

#include <util/types.h>
#include <win32/shared.h>

/* WINDOWS TYPES -> MY TYPES
ATOM        u16
BOOL        i32
BOOLEAN     u8
BYTE        u8
CCHAR       char
CHAR        char
COLORREF    u32
DWORD       u32
DWORDLONG   u64
DWORD_PTR   u64
DWORD32     u32
DWORD64     u64
FLOAT       r32
HACCEL      ptr
HALF_PTR    i32
HANDLE      ptr
HBITMAP     ptr
HBRUSH      ptr
HCOLORSPACE ptr
HCONV       ptr
HCONVLIST   ptr
HCURSOR     ptr
HDC         ptr
HDDEDATA    ptr
HDESK       ptr
HDROP       ptr
HDWP        ptr
HENHMETAFILE ptr
HFILE       i32
HFONT       ptr
HGDIOBJ     ptr
HGLOBAL     ptr
HHOOK       ptr
HICON       ptr
HINSTANCE   ptr
HKEY        ptr
HKL         ptr
HLOCAL      ptr
HMENU       ptr
HMETAFILE   ptr
HMODULE     ptr
HMONITOR    ptr
HPALETTE    ptr
HPEN        ptr
HRESULT     i32
HRGN        ptr
HRSRC       ptr
HSZ         ptr
HWINSTA     ptr
HWND        ptr
INT         i32
INT_PTR     i64
INT8        i8
INT16       i16
INT32       i32
INT64       i64
LANGID      u16
LCID        u32
LCTYPE      u32
LGRPID      u32
LONG        i32
LONGLONG    i64
LONG_PTR    i64
LONG32      i32
LONG64      i64
LPARAM      i64
LPBOOL      i32*
LPBYTE      u8*
LPCOLORREF  u32*
LPCSTR      const cstr
LPCTSTR     if(utf) {const wcstr} else {const cstr}
LPCVOID     const ptr
LPCWSTR     const wcstr
LPDWORD     u32*
LPHANDLE    ptr*
LPINT       i32*
LPLONG      int32*
LPSTR       cstr
LPTSTR      if(utf) {wcstr} else {cstr}
LPVOID      ptr
LPWORD      u16*
LPWSTR      wcstr
LRESULT     i64
PBOOL       i32*
PBOOLEAN    u8*
PBYTE       u8*
PCHAR       char*
PCSTR       const cstr
PCTSTR      if(utf) {const wcstr} else {const cstr}
PCWSTR      const wcstr
PDWORD      u32*
PDWORDLONG  u64*
PDWORD_PTR  u64*
PDWORD32    u32*
PDWORD64    u64*
PFLOAT      r32*
PHALF_PTR   i32*
PHANDLE     ptr*
PHKEY       ptr*
PINT        i32*
PINT_PTR    i32**
PINT8       i8*
PINT16      i16*
PINT32      i32*
PINT64      i64*
PLCID       u32*
PLONG       i32*
PLONGLONG   i64*
PLONG_PTR   i32**
PLONG32     i32*
PLONG64     i64*
POINTER_32  i32*
POINTER_64  i64*
POINTER_SIGNED i32*
POINTER_UNSIGNED ptr
PSHORT      i16*
PSIZE_T     u64*
PSSIZE_T    i64*
PSTR        cstr
PTBYTE      if(utf) {u16*} else {u8*}
PTCHAR      if(utf) {u16*} else {char*}
PTSTR       if(utf) {const wcstr} else {const cstr}
PUCHAR      u8*
PUHALF_PTR  u32*
PUINT       u32*
PUINT_PTR   u32**
PUINT8      u8*
PUINT16     u16*
PUINT32     u32*
PUINT64     u64*
PULONG      u32*
PULONGLONG  u64*
PULONG_PTR  u32**
PULONG32    u32*
PULONG64    u64*
PUSHORT     u16*
PVOID       ptr
PWCHAR      wcstr
PWORD       u16*
PWSTR       wcstr
QWORD       u64
SC_HANDLE   ptr
SC_LOCK     ptr
SERVICE_STATUS_HANDLE ptr
SHORT       i16
SIZE_T      u64
SSIZE_T     i64
TBYTE       if(utf) {u16} else {u8}
TCHAR       if(utf) {u16} else {char}
UCHAR       u8
UHALF_PTR   u32
UINT        u32
UINT_PTR    u64
UINT8       u8
UINT16      u16
UINT32      u32
UINT64      u64
ULONG       u32
ULONGLONG   u64
ULONG_PTR   u64
ULONG32     u32
ULONG64     u64

UNICODE_STRING 
typedef struct sUnicodeStr 
{
    u16  length;
    u16  max_length;
    wcstr  buffer;
} UnicodeStr;
typedef UnicodeStr *PUNICODE_STRING;
typedef const UnicodeStr *PCUNICODE_STRING;

USHORT      u16
USN         i64
VOID        void
WCHAR       u16
WORD        u16
WPARAM      u64
*/

/* kernel32.lib */
extern ptr GetModuleHandleA(const cstr module_name);
extern cstr GetCommandLineA(void);
extern ptr LoadLibraryA(const cstr lib_name);
extern ptr GetProcAddress(ptr module, const cstr proc_name);
extern ptr GetProcessHeap(void);
extern ptr HeapAlloc(ptr heap, u32 flags, u64 bytes);
extern ptr HeapReAlloc(ptr heap, u32 flags, ptr old_mem, u64 bytes);
extern i32 HeapFree(ptr heap, u32 flags, ptr mem);
extern u32 GetModuleFileNameA(ptr module, cstr filename, u32 size);
extern ptr AddDllDirectory(cstr path);
extern u32 GetLastError(void);
extern ptr CreateFileA(cstr path, u32 access, u32 smode, ptr sec, u32 creation, u32 flags, ptr ftemplate);
extern b32 CloseHandle(ptr handle);
extern b32 GetFileSizeEx(ptr handle, LARGE_INTEGER *size);
extern b32 ReadFile(ptr handle, ptr buffer, u32 bytes_to_read, u32 *bytes_read, ptr overlapped);
extern b32 WriteFile(ptr handle, const ptr buffer, u32 bytes_to_write, u32 *bytes_written, ptr overlapped);
extern b32 DeleteFileA(cstr path);
extern b32 CreateDirectoryA(cstr path, ptr sec);
extern b32 RemoveDirectoryA(cstr path);

/*==============================
   Platform Interface
  ============================*/

bool WIN_InterfaceInit(WIN_Data *win32_data)
{

    win32_data = PL_Alloc0(sizeof(WIN_Data));
    if(!win32_data)
    {
        return 0;
    }
    G_win32_data = win32_data;

    return 1;
}

ptr WIN_LoadLibrary(const cstr lib_name)
{
    if(!lib_name)
    {
        return 0;
    }

    return LoadLibraryA(lib_name);
}

ptr WIN_GetProcAddress(ptr module, const cstr proc_name)
{
    if(module == 0 || proc_name == 0)
    {
        return 0;
    }

    return GetProcAddress(module, proc_name);
}

/*==============================
   Memory Allocation Functions
  ============================*/

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

/*===================
  Filesystem
====================*/

void WIN_SetBasePath(void)
{
    Assert(G_win32_data != 0);

    if(G_win32_data->win_config.base_path[0] == 0)
    {
        GetModuleFileNameA(0, G_win32_data->win_config.base_path, STRING_LEN);

        for(cstr cp = &G_win32_data->win_config.base_path[STRING_LEN];
            *cp != '\\' && cp > &G_win32_data->win_config.base_path[0];
            cp--)
        {
            *cp = 0;
        }
    }
}

cstr PL_GetBasePath(void)
{
    Assert(G_win32_data != 0);

    if(G_win32_data->win_config.base_path[0] == 0)
    {
        WIN_SetBasePath();
    }

    return (cstr)(&G_win32_data->win_config.base_path[0]);
}

cstr PL_GetPrefPath(void)
{
    Assert(G_win32_data != 0);
    return (cstr)(&G_win32_data->win_config.pref_path[0]);
}

bool PL_DoesFileExist(cstr path)
{
    Assert(G_win32_data != 0);
    
    if(!path)
    {
        PL_Log(LOG_DEBUG, "DoesFileExist: no path given");
        return 0;
    }

    bool result = (WINAPI.PathFileExistsA(path)) ? 1:0;
    if(result)
    {
        PL_Log(LOG_DEBUG, "DoesFileExist [true]: %s", path);
    }
    else
    {
        PL_Log(LOG_DEBUG, "DoesFileExist [false]: ,%s", path);
    }
    return result;
}

bool PL_DeleteFile(cstr path)
{
    if(!path)
    {
        PL_Log(LOG_DEBUG, "DeleteFile: no path given");
        return 0;
    }

    bool result = (DeleteFileA(path)) ? 1:0;
    if(result)
    {
        PL_Log(LOG_DEBUG, "DeleteFile [success]: %s", path);
    }
    else
    {
        PL_Log(LOG_DEBUG, "DeleteFile [failed]: %s", path);
    }
    return result;
}

bool PL_CreateDirectory(cstr path)
{
    if(!path)
    {
        PL_Log(LOG_DEBUG, "CreateDirectory: no path given");
        return 0;
    }

    bool result = (CreateDirectoryA(path, 0)) ? 1:0;
    if(result)
    {
        PL_Log(LOG_DEBUG, "CreateDirectory [success]: %s", path);
    }
    else
    {
        PL_Log(LOG_DEBUG, "CreateDirectory [failed]: %s", path);
    }
    return result;
}

bool PL_DeleteDirectory(cstr path)
{
    if(!path)
    {
        PL_Log(LOG_DEBUG, "DeleteDirectory: no path given");
        return 0;
    }

    bool result = (RemoveDirectoryA(path)) ? 1:0;
    if(result)
    {
        PL_Log(LOG_DEBUG, "DeleteDirectory [success]: %s", path);
    }
    else
    {
        PL_Log(LOG_DEBUG, "DeleteDirectory [failed]: %s", path);
    }
    return result;
}

/* file handle */
ptr PL_OpenFileHandleR(cstr path)
{
    if(!path)
    {
        PL_Log(LOG_DEBUG, "OpenFileHandleR: no path given");
        return 0;
    }

    ptr handle = CreateFileA(path, 0x80000000L, 1, 0, 3, 0, 0);

    if(handle == 0 || handle == ((ptr)(i64)-1))
    {
        PL_Log(LOG_ERROR, "OpenFileHandleR: failed to open file: %s", path);
        return 0;
    }

    PL_Log(LOG_DEBUG, "OpenFileHandleR: file opened: %s", path);
    return handle;
}

ptr PL_OpenFileHandleW(cstr path)
{
    if(!path)
    {
        PL_Log(LOG_DEBUG, "OPenFileHandleW: no path given");
        return 0;
    }

    ptr handle = CreateFileA(path, 0x40000000L, 0, 0, 2, 0, 0);

    if(handle == 0 || handle == ((ptr)(i64)-1))
    {
        PL_Log(LOG_ERROR, "OpenFileHandleW: failed to open file: %s", path);
        return 0;
    }

    PL_Log(LOG_DEBUG, "OpenFileHandleW: file opened: %s", path);
    return handle;
}

void PL_CloseFile(ptr handle)
{
    if(!handle)
    {
        PL_Log(LOG_DEBUG, "CloseFile: no handle given");
        return;
    }

    PL_Log(LOG_DEBUG, "CloseFile: 0x%I64x", handle);

    CloseHandle(handle);
}

u64 PL_GetFileSize(ptr handle)
{
    if(!handle)
    {
        PL_Log(LOG_DEBUG, "GetFileSize: no handle given");
        return 0;
    }

    LARGE_INTEGER size;
    b32 success = GetFileSizeEx(handle, &size);
    if(!success)
    {
        PL_Log(LOG_ERROR, "GetFileSize: failed to get file size: 0x%I64x", handle);
        return 0;
    }

    u64 result = (u64)size.QuadPart;

    PL_Log(LOG_DEBUG, "GetFileSize: 0x%I64x : %lld bytes", handle, result);
    return result;
}

/* file data */
ASC_FileData *PL_FileRead(cstr path)
{
    if(!path)
    {
        PL_Log(LOG_DEBUG, "FileRead: no path given");
        return 0;
    }

    u64 file_size;

    ptr handle = PL_OpenFileHandleR(path);
    if(!handle)
    {
        PL_Log(LOG_DEBUG, "FileRead: failed to get handle: %s", path);
        return 0;
    }

    file_size = PL_GetFileSize(handle);
    if(!file_size)
    {
        PL_Log(LOG_DEBUG, "FileRead: failed to get size: %s", path);
        PL_CloseFile(handle);
        return 0;
    }

    ASC_FileData *result = PL_Alloc0(sizeof(ASC_FileData) + file_size);
    if(!result)
    {
        PL_Log(LOG_ERROR, "FileRead: malloc error: %s", path);
        PL_CloseFile(handle);
        return 0;
    }

    result->buffer = (ptr)((u64)result + (u64)(sizeof(ASC_FileData)));
    result->size = file_size;

    u32 bytes_read;
    b32 read_result = ReadFile(handle, result->buffer, (u32)file_size, &bytes_read, 0);
    if(read_result == 0 || (u32)file_size != bytes_read)
    {
        PL_Log(LOG_ERROR, "FileRead: failed to read file: 0x%I64x", handle);
        PL_CloseFile(handle);
        PL_Free(result);
        return 0;
    }

    PL_Log(LOG_DEBUG, "FileRead: success: %s", path);
    PL_CloseFile(handle);
    return result;
}

bool PL_FileWrite(cstr path, ASC_FileData *data)
{
    if(path == 0 || data == 0 || data->buffer == 0 || data->size == 0)
    {
        PL_Log(LOG_DEBUG, "FileWrite: invalid parameters");
        return 0;
    }

    ptr handle = PL_OpenFileHandleW(path);
    if(!handle)
    {
        PL_Log(LOG_DEBUG, "FileWrite: failed to get handle: %s", path);
        return 0;
    }

    u32 bytes_written;
    b32 write_result = WriteFile(handle, data->buffer, (u32)data->size, &bytes_written, 0);
    
    if(write_result == 0 || (u32)data->size != bytes_written)
    {
        PL_Log(LOG_ERROR, "FileWrite: failed to write file: %s", path);
        PL_CloseFile(handle);
        return 0;
    }

    PL_Log(LOG_DEBUG, "FileWrite: success: %s", path);
    PL_CloseFile(handle);
    return 1;
}

bool PL_FileAppend(cstr path, ASC_FileData *data)
{
    if(path == 0 || data == 0 || data->buffer == 0 || data->size == 0)
    {
        PL_Log(LOG_DEBUG, "FileAppend: empty parameters");
        return 0;
    }

    //TODO: write function

    return 0;
}

void PL_FileFree(ASC_FileData *data)
{
    if(!data)
    {
        PL_Log(LOG_DEBUG, "FileFree: empty parameters");
        return;
    }

    data->buffer = 0;
    data->size = 0;

    PL_Log(LOG_DEBUG, "FileFree: 0x%I64x", data);
    PL_Free(data);
}

/*===================
  WinAPI Init
====================*/

local bool _LoadWinAPI_shlwapi(void);

bool WIN_WinAPIInit(void)
{
    Assert(G_win32_data != 0);

    if(!_LoadWinAPI_shlwapi())
    {
        return 0;
    }

    return 1;
}

#define LOAD_WIN_FN(name) {WINAPI.##name = (pWin32_##name)WIN_GetProcAddress(lib, #name); if(!WINAPI.##name) return 0;}

local bool _LoadWinAPI_shlwapi(void)
{
    ptr lib = WIN_LoadLibrary("shlwapi.dll");

    if(!lib)
    {
        return 0;
    }

    LOAD_WIN_FN(PathFileExistsA);

    return 1;
}

/*===================
   Logging
====================*/

local bool _LogFileInit(void)
{
    if(!PL_DoesFileExist("logs"))
    {
        PL_CreateDirectory("logs");
    }

    strcpy(G_win32_data->logfile_path, PL_GetBasePath());
    strcat(G_win32_data->logfile_path, "logs\\");
    //TODO: get timestamp
    strcat(G_win32_data->logfile_path, "yymmddhhmmss_");
    strcat(G_win32_data->logfile_path, "log.txt");

    return 1;
}

bool PL_SetLogLevel(LOG_LEVEL console, LOG_LEVEL logfile)
{
    Assert(G_win32_data != 0);

    if(G_win32_data->logfile_path[0] == 0)
    {
        if(!_LogFileInit())
        {
            return 0;
        }
    }

    G_win32_data->console_loglevel = console;
    G_win32_data->logfile_loglevel = logfile;

    return 1;
}

bool PL_Log(LOG_LEVEL level, cstr string, ...)
{
    if(G_win32_data == 0 || 
       string == 0)
    {
        return 0;
    }

    if(level > G_win32_data->console_loglevel &&
       level > G_win32_data->logfile_loglevel)
    {
        return 1;
    }

    cstr logstring = PL_Alloc0(STRING_LEN);
    if(!logstring)
    {
        return 0;
    }

    //TODO: timestamp
    strcat(logstring, "[hh:mm:ss]");

    switch(level)
    {
        case LOG_FATAL:
        {
            strcat(logstring, " [FATAL] ");
        } break;
        case LOG_ERROR:
        {
            strcat(logstring, " [ERROR] ");
        } break;
        case LOG_WARN:
        {
            strcat(logstring, " [WARN]  ");
        } break;
        case LOG_INFO:
        {
            strcat(logstring, " [INFO]  ");
        } break;
        case LOG_DEBUG:
        {
            strcat(logstring, " [DEBUG] ");
        } break;
        case LOG_NONE: break;
    }

    cstr fstring = PL_Alloc0(STRING_LEN);
    if(!fstring)
    {
        PL_Free(logstring);
        return 0;
    }
    va_list args;
    va_start(args, string);
    vsnprintf(fstring, STRING_LEN, string, args);
    va_end(args);

    snprintf(logstring+strlen(logstring), STRING_LEN, fstring);
    PL_Free(fstring);

    if(level <= G_win32_data->console_loglevel)
    {
        puts(logstring);
    }

    if(level <= G_win32_data->logfile_loglevel)
    {
        strcat(logstring, "\n");
        //TODO: write log file
    }

    PL_Free(logstring);

    return 1;
}
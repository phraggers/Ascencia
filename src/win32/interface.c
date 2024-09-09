/*============================================================
 * Project: Ascencia [EXE]
 * File: interface.c
 * Author: Phraggers
 * Date: Sat Sep 07 2024
============================================================*/

#include <util/types.h>
#include <win32/shared.h>

/* local functions */
local bool _InitStdLib(void);

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

/*==============================
   kernel32.lib
  ============================*/
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
extern ptr CreateFileA(cstr path, u32 access, u32 smode, ptr sec, u32 creation, u32 flags, ptr template);
extern b32 CloseHandle(ptr handle);
typedef union _LARGE_INTEGER { struct {u32 LowPart; i32 HighPart;} s; 
struct {u32 LowPart; i32 HighPart;} u; i64 QuadPart;} LARGE_INTEGER;
extern b32 GetFileSizeEx(ptr handle, LARGE_INTEGER *size);
extern b32 ReadFile(ptr handle, ptr buffer, u32 bytes_to_read, u32 *bytes_read, ptr overlapped);
extern b32 WriteFile(ptr handle, const ptr buffer, u32 bytes_to_write, u32 *bytes_written, ptr overlapped);

/*==============================
   Platform Interface
  ============================*/

bool WIN_InterfaceInit(WIN_Data *win32_data)
{
    win32_data = PL_Alloc0(sizeof(WIN_Data));
    if(!win32_data)
    {
        //TODO: error: malloc error
        return 0;
    }
    G_win32_data = win32_data;

    return 1;
}

ptr WIN_LoadLibrary(const cstr lib_name)
{
    return LoadLibraryA(lib_name);
}

ptr WIN_GetProcAddress(ptr module, const cstr proc_name)
{
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
    if(G_win32_data->base_path[0] == 0)
    {
        GetModuleFileNameA(0, G_win32_data->base_path, ASC_MAX_PATH);

        for(cstr cp = &G_win32_data->base_path[ASC_MAX_PATH];
            *cp != '\\' && cp > &G_win32_data->base_path[0];
            cp--)
        {
            *cp = 0;
        }
    }
}

cstr PL_GetBasePath(void)
{
    if(G_win32_data->base_path[0] == 0)
    {
        WIN_SetBasePath();
    }

    return (cstr)(&G_win32_data->base_path[0]);
}

cstr PL_GetPrefPath(void)
{
    return (cstr)(&G_win32_data->pref_path[0]);
}

/* file handle */
ptr PL_OpenFileHandleR(cstr path)
{
    if(!path)
    {
        return 0;
    }

    ptr handle = CreateFileA(path, 0x80000000L, 1, 0, 3, 0, 0);

    if(handle == 0 || handle == ((ptr)(i64)-1))
    {
        //TODO: error: failed to open file
        return 0;
    }

    return handle;
}

ptr PL_OpenFileHandleW(cstr path)
{
    if(!path)
    {
        return 0;
    }

    ptr handle = CreateFileA(path, 0x40000000L, 0, 0, 2, 0, 0);

    if(handle == 0 || handle == ((ptr)(i64)-1))
    {
        //TODO: error: failed to open file
        return 0;
    }

    return handle;
}

void PL_CloseFile(ptr handle)
{
    if(!handle)
    {
        return;
    }

    CloseHandle(handle);
}

u64 PL_GetFileSize(ptr handle)
{
    if(!handle)
    {
        return 0;
    }

    LARGE_INTEGER size;
    b32 success = GetFileSizeEx(handle, &size);
    if(!success)
    {
        //TODO: error: failed to get file size
        return 0;
    }

    u64 result = (u64)size.QuadPart;
    return result;
}

/* file data */
ASC_FileData *PL_FileRead(cstr path)
{
    if(!path)
    {
        return 0;
    }

    u64 file_size;

    ptr handle = PL_OpenFileHandleR(path);
    if(!handle)
    {
        return 0;
    }

    file_size = PL_GetFileSize(handle);
    if(!file_size)
    {
        PL_CloseFile(handle);
        return 0;
    }

    ASC_FileData *result = PL_Alloc0(sizeof(ASC_FileData) + file_size);
    if(!result)
    {
        //TODO: error: malloc error
        PL_CloseFile(handle);
        return 0;
    }

    result->buffer = (ptr)((u64)result + (u64)(sizeof(ASC_FileData)));
    result->size = file_size;

    u32 bytes_read;
    b32 read_result = ReadFile(handle, result->buffer, (u32)file_size, &bytes_read, 0);
    if(read_result == 0 || (u32)file_size != bytes_read)
    {
        //TODO: error: failed to read file
        PL_CloseFile(handle);
        PL_Free(result);
        return 0;
    }

    PL_CloseFile(handle);
    return result;
}

bool PL_FileWrite(cstr path, ASC_FileData *data)
{
    if(path == 0 || data == 0 || data->buffer == 0 || data->size == 0)
    {
        return 0;
    }

    ptr handle = PL_OpenFileHandleW(path);
    if(!handle)
    {
        return 0;
    }

    u32 bytes_written;
    b32 write_result = WriteFile(handle, data->buffer, (u32)data->size, &bytes_written, 0);
    
    if(write_result == 0 || (u32)data->size != bytes_written)
    {
        //TODO: error: failed to write file
        PL_CloseFile(handle);
        return 0;
    }

    PL_CloseFile(handle);
    return 1;
}

bool PL_FileAppend(cstr path, ASC_FileData *data)
{
    if(path == 0 || data == 0 || data->buffer == 0 || data->size == 0)
    {
        return 0;
    }

    //TODO:

    return 0;
}

void PL_FileFree(ASC_FileData *data)
{
    if(!data)
    {
        return;
    }

    data->buffer = 0;
    data->size = 0;
    PL_Free(data);
}

/*===================
  StdLib Init
====================*/
bool WIN_StdLibInit(STD_interface *stdlib_interface)
{
    stdlib_interface = PL_Alloc0(sizeof(STD_interface));
    if(!stdlib_interface)
    {
        //TODO: error: malloc error
        return 0;
    }
    G_stdlib_interface = stdlib_interface;

    if(!_InitStdLib())
    {
        return 0;
    }

    return 1;
}

#define LOAD_DLLFN(name) {name = (p##name)WIN_GetProcAddress(lib, #name ); if(!name) return 0;}

local bool _InitStdLib(void)
{
    ptr lib = WIN_LoadLibrary(ASC_STDLIB_DLL);

    if(!lib)
    {
        //TODO: error: failed to load asc_stdlib.dll
        return 0;
    }

    LOAD_DLLFN(STD_puts);
    LOAD_DLLFN(STD_memcpy);
    LOAD_DLLFN(STD_strlen);
    LOAD_DLLFN(STD_strcpy);
    LOAD_DLLFN(STD_strcat);

    return 1;
}
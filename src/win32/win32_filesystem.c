/*============================================================
 * Project: Ascencia
 * File: win32_filesystem.c
 * Author: Phraggers
 * Date: Fri Sep 13 2024
============================================================*/

#include <util/string_helpers.h>
#include <platform/filesystem.h>
#include <platform/alloc.h>
#include <win32/win32_state.h>

cstr PL_GetBasePath(void)
{
    return Win32_GetBasePath();
}

cstr PL_GetPrefPath(void)
{
    return Win32_GetPrefPath();
}

bool PL_DoesFileExist(cstr path)
{
    if((!path) || 
       (!strlen(path)) ||
       (!G_win32_state) || 
       (!WINAPI.PathFileExistsA))
    {
        PL_Log(LOG_DEBUG, "DoesFileExist: invalid parameters");
        return 0;
    }

    b32 result = WINAPI.PathFileExistsA(path);

    if(result)
    {
        PL_Log(LOG_DEBUG, "DoesFileExist: %s : true", path);
        return 1;
    }
    else
    {
        PL_Log(LOG_DEBUG, "DoesFileExist: %s : false", path);
        return 0;
    }
}

bool PL_DeleteFile(cstr path)
{
    if((!path) || 
       (!strlen(path)) ||
       (!G_win32_state) || 
       (!WINAPI.DeleteFileA))
    {
        PL_Log(LOG_DEBUG, "DeleteFile: invalid parameters");
        return 0;
    }

    bool result = (WINAPI.DeleteFileA(path)) ? 1:0;
    if(result)
    {
        PL_Log(LOG_INFO, "DeleteFile: %s : success", path);
        return 1;
    }
    else
    {
        PL_Log(LOG_ERROR, "DeleteFile: %s : fail", path);
        return 0;
    }
}

bool PL_CreateDirectory(cstr path)
{
    if((!path) || 
       (!strlen(path)) ||
       (!G_win32_state) || 
       (!WINAPI.CreateDirectoryA))
    {
        PL_Log(LOG_DEBUG, "CreateDirectory: invalid parameters");
        return 0;
    }

    bool result = (WINAPI.CreateDirectoryA(path, 0)) ? 1:0;
    if(result)
    {
        PL_Log(LOG_INFO, "CreateDirectory: %s: success", path);
        return 1;
    }
    else
    {
        PL_Log(LOG_ERROR, "CreateDirectory: %s: fail", path);
        return 0;
    }
}

bool PL_DeleteDirectory(cstr path)
{
    if((!path) || 
       (!strlen(path)) ||
       (!G_win32_state) || 
       (!WINAPI.RemoveDirectoryA))
    {
        PL_Log(LOG_DEBUG, "CreateDirectory: invalid parameters");
        return 0;
    }

    bool result = (WINAPI.RemoveDirectoryA(path)) ? 1:0;
    if(result)
    {
        PL_Log(LOG_INFO, "DeleteDirectory: %s: success", path);
        return 1;
    }
    else
    {
        PL_Log(LOG_ERROR, "DeleteDirectory: %s: fail", path);
        return 0;
    }
}

cstr PL_PlatformFilePath(cstr path)
{
    if((!path) || (!strlen(path)))
    {
        PL_Log(LOG_DEBUG, "PlatformFilePath: invalid parameters");
        return 0;
    }

    for(cstr char_index = path;
        char_index < (cstr)((u64)path + (u64)STRING_LEN-1);
        char_index++)
    {
        if(*char_index == '/')
        {
            *char_index = '\\';
        }
    }

    return path;
}

cstr PL_PlatformDirectoryPath(cstr path)
{
    if((!path) || (!strlen(path)))
    {
        PL_Log(LOG_DEBUG, "PlatformDirectoryPath: invalid parameters");
        return 0;
    }

    PL_PlatformFilePath(path);

    if(path[strlen(path)-1] != '\\')
    {
        path[strlen(path)] = '\\';
    }

    return path;
}


ptr PL_OpenFileHandleR(cstr path)
{
    if((!path) || 
       (!strlen(path)) ||
       (!G_win32_state) || 
       (!WINAPI.CreateFileA))
    {
        PL_Log(LOG_DEBUG, "OpenFileHandleR: invalid parameters");
        return 0;
    }

    ptr handle = WINAPI.CreateFileA(path, 0x80000000L, 1, 0, 3, 0, 0);

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
    if((!path) || 
       (!strlen(path)) ||
       (!G_win32_state) || 
       (!WINAPI.CreateFileA))
    {
        PL_Log(LOG_DEBUG, "OpenFileHandleW: invalid parameters");
        return 0;
    }

    ptr handle = WINAPI.CreateFileA(path, 0x40000000L, 0, 0, 2, 0, 0);

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
    if((!handle) ||
       (!WINAPI.CloseHandle))
    {
        PL_Log(LOG_DEBUG, "CloseFile: invalid parameters");
        return;
    }

    PL_Log(LOG_DEBUG, "CloseFile: 0x%I64x", handle);
    WINAPI.CloseHandle(handle);
}

u64 PL_GetFileSize(ptr handle)
{
    if((!handle) ||
       (!WINAPI.GetFileSizeEx))
    {
        PL_Log(LOG_DEBUG, "GetFileSize: invalid parameters");
        return 0;
    }

    Win32_LARGE_INTEGER size;
    b32 success = WINAPI.GetFileSizeEx(handle, &size);
    if(!success)
    {
        PL_Log(LOG_ERROR, "GetFileSize: failed to get file size: 0x%I64x", handle);
        return 0;
    }

    u64 result = (u64)size.QuadPart;

    #ifdef DEBUG
    cstr sizestr = PL_String_New();
    PL_String_ShortFileSize(sizestr, result);
    PL_Log(LOG_DEBUG, "GetFileSize: 0x%I64x : %s", handle, sizestr);
    PL_Free(sizestr);
    #endif

    return result;
}


PL_FileData *PL_FileRead(cstr path)
{
    if((!path) || (!strlen(path)))
    {
        PL_Log(LOG_DEBUG, "FileRead: invalid parameters");
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

    PL_FileData *result = PL_Alloc0(sizeof(PL_FileData) + file_size);
    if(!result)
    {
        PL_Log(LOG_ERROR, "FileRead: malloc error: %s", path);
        PL_CloseFile(handle);
        return 0;
    }

    result->buffer = (ptr)((u64)result + (u64)(sizeof(PL_FileData)));
    result->size = file_size;

    u32 bytes_read = 0;
    b32 read_result = WINAPI.ReadFile(handle, result->buffer, (u32)file_size, &bytes_read, 0);

    if(read_result == 0 || (u32)file_size != bytes_read)
    {
        cstr sizestr = PL_String_New();
        PL_String_ShortFileSize(sizestr, bytes_read);
        PL_Log(LOG_ERROR, "FileRead: failed to read file: 0x%I64x : read %s", handle, sizestr);
        PL_CloseFile(handle);
        PL_Free(sizestr);
        PL_Free(result);

        return 0;
    }

    PL_Log(LOG_DEBUG, "FileRead: success: %s", path);
    PL_CloseFile(handle);
    return result;
}

bool PL_FileWrite(cstr path, PL_FileData *data)
{
    if(path == 0 || 
       data == 0 || 
       data->buffer == 0 || 
       data->size == 0)
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

    u32 bytes_written = 0;
    b32 write_result = WINAPI.WriteFile(handle, data->buffer, (u32)data->size, &bytes_written, 0);
    
    if(write_result == 0 || (u32)data->size != bytes_written)
    {
        PL_CloseFile(handle);

        cstr sizestr = PL_String_New();
        PL_String_ShortFileSize(sizestr, bytes_written);
        PL_Log(LOG_ERROR, "FileWrite: failed to write file: %s: read %s", path, sizestr);
        PL_Free(sizestr);
        return 0;
    }

    PL_Log(LOG_DEBUG, "FileWrite: success: %s", path);
    PL_CloseFile(handle);
    return 1;
}

bool PL_FileAppend(cstr path, PL_FileData *data)
{
    if(path == 0 || 
       data == 0 || 
       data->buffer == 0 || 
       data->size == 0)
    {
        PL_Log(LOG_DEBUG, "FileAppend: empty parameters");
        return 0;
    }

    FILE *outfile = 0;
    outfile = fopen(path, "ab");

    if(!outfile)
    {
        PL_Log(LOG_ERROR, "FileAppend: failed to open file: %s", path);
        return 0;
    }

    u64 bytes_written = (u64)fwrite(data->buffer, 1, data->size, outfile);
    fclose(outfile);

    cstr size1 = PL_String_New();
    PL_String_ShortFileSize(size1, bytes_written);
    cstr size2 = PL_String_New();
    PL_String_ShortFileSize(size2, data->size);

    if(bytes_written != data->size)
    {
        PL_Log(LOG_ERROR, "FileAppend: failed to write data: %s (wrote %s, expected %s)", path, size1, size2);
        PL_Free(size1);
        PL_Free(size2);
        return 0;
    }

    PL_Log(LOG_INFO, "FileAppend: wrote %s to %s", size1, path);
    PL_Free(size1);
    PL_Free(size2);

    return 1;
}

void PL_FileFree(PL_FileData *data)
{
    if(!data)
    {
        PL_Log(LOG_DEBUG, "FileFree: invalid parameters");
        return;
    }

    #ifdef DEBUG
    cstr sizestr = PL_String_New();
    PL_String_ShortFileSize(sizestr, data->size);
    PL_Log(LOG_DEBUG, "FileFree: 0x%I64x : %s", data, sizestr);
    PL_Free(sizestr);
    #endif

    data->buffer = 0;
    data->size = 0;

    PL_Free(data);
}

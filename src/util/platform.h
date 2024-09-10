/*============================================================
 * Project: Ascencia [SHARED]
 * File: platform.h
 * Author: Phraggers
 * Date: Mon Sep 09 2024
============================================================*/

#ifndef ASCENCIA_UTIL_PLATFORM_H
#define ASCENCIA_UTIL_PLATFORM_H

#include <util/types.h>
#include <util/stdlib_interface.h>

typedef enum
{
    LOG_NONE = 0,
    LOG_FATAL,
    LOG_ERROR,
    LOG_WARN,
    LOG_INFO,
    LOG_DEBUG
} LOG_LEVEL;

typedef struct sASC_FileData
{
    ptr buffer;
    u64 size;
} ASC_FileData;

/* heap allocation */
ptr PL_Alloc(u64 size);
ptr PL_Alloc0(u64 size);
ptr PL_ReAlloc(ptr old_mem, u64 new_size);
ptr PL_ReAlloc0(ptr old_mem, u64 new_size);
b32 PL_Free(ptr mem);

/* filesystem */
cstr PL_GetBasePath(void);
cstr PL_GetPrefPath(void);
bool PL_DoesFileExist(cstr path);
bool PL_DeleteFile(cstr path);
bool PL_CreateDirectory(cstr path);
bool PL_DeleteDirectory(cstr path);

/* file handle */
ptr PL_OpenFileHandleR(cstr path); //read handle
ptr PL_OpenFileHandleW(cstr path); //write handle
void PL_CloseFile(ptr handle);
u64 PL_GetFileSize(ptr handle);

/* file data */
ASC_FileData *PL_FileRead(cstr path); //read whole file
bool PL_FileWrite(cstr path, ASC_FileData *data); //overwrite file
bool PL_FileAppend(cstr path, ASC_FileData *data);
void PL_FileFree(ASC_FileData *data);

/* logging */
bool PL_SetLogLevel(LOG_LEVEL console, LOG_LEVEL logfile);
bool PL_Log(LOG_LEVEL level, cstr string);
bool PL_Log2(LOG_LEVEL level, cstr string1, cstr string2);
bool PL_Log3(LOG_LEVEL level, cstr string1, cstr string2, cstr string3);
bool PL_Log4(LOG_LEVEL level, cstr string1, cstr string2, cstr string3, cstr string4);

#endif /* ASCENCIA_UTIL_PLATFORM_H */
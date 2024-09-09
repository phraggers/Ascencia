/*============================================================
 * Project: Ascencia [SHARED]
 * File: platform.h
 * Author: Phraggers
 * Date: Mon Sep 09 2024
============================================================*/

#ifndef ASCENCIA_PLATFORM_H
#define ASCENCIA_PLATFORM_H

#include <util/types.h>
#include <util/stdlib_interface.h>

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

#endif /* ASCENCIA_PLATFORM_H */
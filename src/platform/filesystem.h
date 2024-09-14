/*============================================================
 * Project: Ascencia
 * File: filesystem.h
 * Author: Phraggers
 * Date: Fri Sep 13 2024
============================================================*/

#ifndef ASCENCIA_FILESYSTEM_H
#define ASCENCIA_FILESYSTEM_H

#include <util/types.h>

typedef struct
{
    ptr buffer;
    u64 size;
} PL_FileData;

/* filesystem */
cstr PL_GetBasePath(void);
cstr PL_GetPrefPath(void);
bool PL_DoesFileExist(cstr path);
bool PL_DeleteFile(cstr path);
bool PL_CreateDirectory(cstr path);
bool PL_DeleteDirectory(cstr path);

cstr PL_PlatformFilePath(cstr path); //returns path with platform correct slashes
cstr PL_PlatformDirectoryPath(cstr path); //ensures correct slashes and trailing slash

/* file handle */
ptr PL_OpenFileHandleR(cstr path); //read handle
ptr PL_OpenFileHandleW(cstr path); //write handle
void PL_CloseFile(ptr handle);
u64 PL_GetFileSize(ptr handle);

/* file data */
PL_FileData *PL_FileRead(cstr path); //read whole file
bool PL_FileWrite(cstr path, PL_FileData *data); //overwrite file
bool PL_FileAppend(cstr path, PL_FileData *data);
void PL_FileFree(PL_FileData *data);

#endif /* ASCENCIA_FILESYSTEM_H */
#ifndef WIN32_DIR_H
/* ==============================================================
   Project: Ascencia
   File:    win32_dir.h
   Author:  Phraggers
   Date:    21-11-2024
   ============================================================== */

#include <util/types.h>

// DIR/PATH HELPERS

// adds '\\' to end of path if not there
inline void PL_DirAddSlash(char *path);

// deletes chars from end of path until '\\' found
inline void PL_DirUpDirectory(char *path);

// converts path from '\\' to '/'
inline void PL_DirToFwdSlash(char *path);

// converts path from '/' to '\\'
inline void PL_DirToBackSlash(char *path);

// returns true if path is file
b32 PL_PathIsFile(char *path);

// returns true if path is directory
b32 PL_PathIsDirectory(char *path);

typedef struct sDirEntry_
{
    char path[MAX_PATH];
    struct sDirEntry_ *next;
    struct sDirEntry_ *prev;
} sDirEntry;

// create DirEntry list
sDirEntry *PL_NewDirList(void);
// delete and free DirEntry list
void PL_DeleteDirList(sDirEntry *list);
// adds DirEntry to back of list
void PL_PushDirEntry(sDirEntry *list, char *path);
// remove DirEntry from list
void PL_RemoveDirEntry(sDirEntry *entry);
// remove last DirEntry from back of list
void PL_PopDirEntry(sDirEntry *list);
// returns last entry in dir list
sDirEntry *PL_GetLastDirEntry(sDirEntry *list);

typedef struct sFileEntry_
{
    char full_path[MAX_PATH]; //fullpath & filename, '\\' path sep
    char relative_path[MAX_PATH]; //relative path (from dirlist root) & filename, '/' path sep
    u32 size;
    struct sFileEntry_ *next;
    struct sFileEntry_ *prev;
} sFileEntry;

// creates new FileEntry list
sFileEntry *PL_NewFileList(void);
// delete and free FileEntry list
void PL_DeleteFileList(sFileEntry *list);
// adds FileEntry to back of list
void PL_PushFileEntry(sFileEntry *list, char *full_path, char *relative_path, u64 size);
// remove FileEntry from list
void PL_RemoveFileEntry(sFileEntry *entry);
// remove last FileEntry from back of list
void PL_PopFileEntry(sFileEntry *list);
// returns last entry in file list
sFileEntry *PL_GetLastFileEntry(sFileEntry *list);

typedef struct
{
    sDirEntry *dir_list;
    sFileEntry *file_list;
    u64 dir_count;
    u64 file_count;
} sDirList;

// create and populate dir list
sDirList *PL_GetDirList(char *path);

#define WIN32_DIR_H
#endif

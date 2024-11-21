#ifndef WIN32_DIR_H
/* ==============================================================
   Project: Ascencia
   File:    win32_dir.h
   Author:  Phraggers
   Date:    21-11-2024
   ============================================================== */

#include <util/types.h>

typedef struct {char str[MAX_PATH];} sPathStr, *PathStr;
// alloc new path string
PathStr PathStr_New(char *path);
// free path string
void PathStr_Free(PathStr path);
// add c-string to end of path string
void PathStr_CatC(PathStr dst, char *src);
// add path string to end of path string
void PathStr_CatP(PathStr dst, PathStr src);
// returns strlen of path
u64 PathStr_Len(PathStr path);

typedef struct sDirEntry_
{
    sPathStr path;
    struct sDirEntry_ *next;
    struct sDirEntry_ *prev;
} sDirEntry;

// adds '\\' to end of path if not there
inline void PL_DirAddSlash(PathStr path);
// deletes chars from end of path until '\\' found
inline void PL_DirUpDirectory(PathStr path);
// converts path from '\\' to '/'
inline void PL_DirToFwdSlash(PathStr path);
// converts path from '/' to '\\'
inline void PL_DirToBackSlash(PathStr path);

// creates new DirEntry list
sDirEntry *PL_NewDirList(void);
// delete and free DirEntry list
void PL_DeleteDirList(sDirEntry *list);
// adds DirEntry to back of list
void PL_PushDirEntry(sDirEntry *list, sDirEntry *entry);
// remove DirEntry from list
void PL_RemoveDirEntry(sDirEntry *entry);
// remove last DirEntry from back of list
void PL_PopDirEntry(sDirEntry *list);

typedef struct sFileEntry_
{
    sPathStr path; //fullpath & filename, prefer '\\' path sep
    sPathStr name; //relative path & filename, prefer '/' path sep
    u32 size;
    struct sFileEntry_ *next;
    struct sFileEntry_ *prev;
} sFileEntry;

// creates new FileEntry list
sFileEntry *PL_NewFileList(void);
// delete and free FileEntry list
void PL_DeleteFileList(sFileEntry *list);
// adds FileEntry to back of list
void PL_PushFileEntry(sFileEntry *list, sFileEntry *entry);
// remove FileEntry from list
void PL_RemoveFileEntry(sFileEntry *entry);
// remove last FileEntry from back of list
void PL_PopFileEntry(sFileEntry *list);

#define WIN32_DIR_H
#endif

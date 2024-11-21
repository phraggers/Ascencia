/* ==============================================================
   Project: Ascencia
   File:    win32_dir.c
   Author:  Phraggers
   Date:    21-11-2024
   ============================================================== */

#include <win32/win32_dir.h>
#include <win32/win32_alloc.h>

PathStr PathStr_New(char *path)
{
    PathStr result = PL_Alloc0(sizeof(sPathStr));
    return result;
}

void PathStr_Free(PathStr path)
{
    if(path) PL_Free(path);
}

void PathStr_CatC(PathStr dst, char *src)
{
    if(dst && src &&
       (strlen(dst->str)+strlen(src) < MAX_PATH))
    {
        strcat(dst->str, src);
    }
}

void PathStr_CatP(PathStr dst, PathStr src)
{
    if(dst && src &&
       (strlen(dst->str)+strlen(src->str) < MAX_PATH))
    {
        strcat(dst->str, src->str);
    }
}

u64 PathStr_Len(PathStr path)
{
    if(path)
    {
        u64 result = strlen(path->str);
        return result;
    }

    return 0;
}

inline void PL_DirAddSlash(PathStr path)
{
    if(path->str[(PathStr_Len(path))-1] != '\\')
    {
        path->str[PathStr_Len(path)] = '\\';
    }
}

inline void PL_DirUpDirectory(PathStr path)
{
    for(char *c = &path->str[(PathStr_Len(path))-1];
        *c != '\\';
        c--)
    {
        if(c == path->str) return;
        *c = 0;
    }
}

inline void PL_DirToFwdSlash(PathStr path)
{
    if(path)
    {
        for(char *c = path->str;
            c < &path->str[(PathStr_Len(path))-1];
            c++)
        {
            if(*c == '\\')
            {
                *c = '/';
            }
        }
    }
}

inline void PL_DirToBackSlash(PathStr path)
{
    if(path)
    {
        for(char *c = path->str;
            c < &path->str[(PathStr_Len(path))-1];
            c++)
        {
            if(*c == '/')
            {
                *c = '\\';
            }
        }
    }
}

sDirEntry *PL_NewDirList(void)
{
    sDirEntry *result = PL_Alloc0(sizeof(sDirEntry));
    return result;
}

void PL_DeleteDirList(sDirEntry *list)
{
    if(list)
    {
        PL_Free(list);
    }
}

void PL_PushDirEntry(sDirEntry *list, sDirEntry *entry)
{
    if(list && entry)
    {
        sDirEntry *pentry = list;
        while(pentry->next)
        {
            pentry = pentry->next;
        }

        pentry->next = PL_Alloc0(sizeof(sDirEntry));
        pentry->next->prev = pentry;
        pentry = pentry->next;
        PathStr_CatP(&pentry->path, &entry->path);
    }
}

void PL_RemoveDirEntry(sDirEntry *entry)
{
    if(entry)
    {
        sDirEntry *pentry = entry;
        sDirEntry *next = entry->next;
        sDirEntry *prev = entry->prev;
        if(next) next->prev = prev;
        if(prev) prev->next = next;
        PL_Free(entry);
    }
}

void PL_PopDirEntry(sDirEntry *list)
{
    if(list)
    {
        sDirEntry *pentry = list;
        while(pentry->next)
        {
            pentry = pentry->next;
        }

        PL_RemoveDirEntry(pentry);
    }
}

sFileEntry *PL_NewFileList(void)
{
    sFileEntry *result = PL_Alloc0(sizeof(sFileEntry));
    return result;
}

void PL_DeleteFileList(sFileEntry *list)
{
    if(list)
    {
        PL_Free(list);
    }
}

void PL_PushFileEntry(sFileEntry *list, sFileEntry *entry)
{
    if(list && entry)
    {
        sFileEntry *pentry = list;
        while(pentry->next)
        {
            pentry = pentry->next;
        }

        pentry->next = PL_Alloc0(sizeof(sFileEntry));
        pentry->next->prev = pentry;
        pentry = pentry->next;
        PathStr_CatP(&pentry->path, &entry->path);
        PathStr_CatP(&pentry->name, &entry->name);
        pentry->size = entry->size;
    }
}

void PL_RemoveFileEntry(sFileEntry *entry)
{
    if(entry)
    {
        sFileEntry *pentry = entry;
        sFileEntry *next = entry->next;
        sFileEntry *prev = entry->prev;
        if(next) next->prev = prev;
        if(prev) prev->next = next;
        PL_Free(entry);
    }
}

void PL_PopFileEntry(sFileEntry *list)
{
    if(list)
    {
        sFileEntry *pentry = list;
        while(pentry->next)
        {
            pentry = pentry->next;
        }

        PL_RemoveFileEntry(pentry);
    }
}

/* ==============================================================
   Project: Ascencia
   File:    win32_dir.c
   Author:  Phraggers
   Date:    21-11-2024
   ============================================================== */

#include <win32/win32_dir.h>
#include <win32/win32_alloc.h>

inline void PL_DirAddSlash(char *path)
{
    if(path[(strlen(path))-1] != '\\')
    {
        path[strlen(path)] = '\\';
    }
}

inline void PL_DirUpDirectory(char *path)
{
    for(char *c = &path[(strlen(path))-1];
        *c != '\\';
        c--)
    {
        if(c == path) return;
        *c = 0;
    }
}

inline void PL_DirToFwdSlash(char *path)
{
    if(path)
    {
        for(char *c = path;
            c < &path[(strlen(path))-1];
            c++)
        {
            if(*c == '\\')
            {
                *c = '/';
            }
        }
    }
}

inline void PL_DirToBackSlash(char *path)
{
    if(path)
    {
        for(char *c = path;
            c < &path[(strlen(path))-1];
            c++)
        {
            if(*c == '/')
            {
                *c = '\\';
            }
        }
    }
}

b32 PL_PathIsFile(char *path)
{
    DWORD win32_result = GetFileAttributesA(path);
    if(win32_result != INVALID_FILE_ATTRIBUTES)
    {
        if(win32_result & FILE_ATTRIBUTE_DIRECTORY) return 0;
        else return 1;
    }
    else return 0;
}

b32 PL_PathIsDirectory(char *path)
{
    DWORD win32_result = GetFileAttributesA(path);
    if(win32_result != INVALID_FILE_ATTRIBUTES)
    {
        if(win32_result & FILE_ATTRIBUTE_DIRECTORY) return 1;
        else return 0;
    }
    else return 0;
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
        sDirEntry *pentry = list;
        while(pentry->next)
        {
            pentry = pentry->next;
        }

        while(pentry->prev)
        {
            sDirEntry *prev = pentry->prev;
            PL_Free(pentry);
            pentry = prev;
        }

        PL_Free(list);
    }
}

void PL_PushDirEntry(sDirEntry *list, char *path)
{
    if(list && path)
    {
        sDirEntry *pentry = list;
        while(pentry->next)
        {
            pentry = pentry->next;
        }

        pentry->next = PL_Alloc0(sizeof(sDirEntry));
        pentry->next->prev = pentry;
        pentry = pentry->next;
        strcpy(pentry->path, path);
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
        sDirEntry *last = PL_GetLastDirEntry(list);
        if(last)
        {
            //NOTE: we don't want to remove the first list entry
            if(last->prev)
            {
                PL_RemoveDirEntry(last);
            }
        }
    }
}

sDirEntry *PL_GetLastDirEntry(sDirEntry *list)
{
    sDirEntry *result = 0;

    if(list)
    {
        result = list;
        while(result->next)
        {
            result = result->next;
        }
    }

    return result;
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
        sFileEntry *pentry = list;
        while(pentry->next)
        {
            pentry = pentry->next;
        }

        while(pentry->prev)
        {
            sFileEntry *prev = pentry->prev;
            PL_Free(pentry);
            pentry = prev;
        }

        PL_Free(list);
    }
}

void PL_PushFileEntry(sFileEntry *list, char *full_path, char *relative_path, u64 size)
{
    if(list && full_path && relative_path && size)
    {
        sFileEntry *pentry = list;
        while(pentry->next)
        {
            pentry = pentry->next;
        }

        pentry->next = PL_Alloc0(sizeof(sFileEntry));
        pentry->next->prev = pentry;
        pentry = pentry->next;

        strcpy(pentry->full_path, full_path);
        strcpy(pentry->relative_path, relative_path);
        pentry->size = size;
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
        sFileEntry *last = PL_GetLastFileEntry(list);
        if(last)
        {
            //NOTE: we don't want to remove the first list entry
            if(last->prev)
            {
                PL_RemoveFileEntry(last);
            }
        }
    }
}

sFileEntry *PL_GetLastFileEntry(sFileEntry *list)
{
    sFileEntry *result = 0;

    if(list)
    {
        result = list;
        while(result->next)
        {
            result = result->next;
        }
    }

    return result;
}

//TODO: this method is all wrong, need to rethink.
// probably just need to get a filelist, or just totally rewrite this

sDirList *PL_GetDirList(char *path)
{
    sDirList *result = 0;

    if(PL_PathIsDirectory(path))
    {
        result = (sDirList*)PL_Alloc0(sizeof(sDirList));
        result->dir_list = PL_NewDirList();
        result->file_list = PL_NewFileList();

        PL_PushDirEntry(result->dir_list, path);
        result->dir_count++;
        sDirEntry *current_dir = PL_GetLastDirEntry(result->dir_list);

        while(current_dir)
        {
            WIN32_FIND_DATA find_data;
            HANDLE find = FindFirstFileA(current_dir->path, &find_data);

            do
            {
                if(find_data.dwFileAttributes == INVALID_FILE_ATTRIBUTES)
                {
                    continue;
                }

                else if(find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                {
                    if(!strcmp(find_data.cFileName, ".") ||
                       !strcmp(find_data.cFileName, ".."))
                    {
                        continue;
                    }

                    char *found_dir = (char*)PL_Alloc0(MAX_PATH);
                    strcpy(found_dir, current_dir->path);
                    PL_DirAddSlash(found_dir);
                    strcat(found_dir, find_data.cFileName);
                    printf("DIR FOUND: %s\n", found_dir);
                    PL_PushDirEntry(result->dir_list, found_dir);
                    result->dir_count++;
                    PL_Free(found_dir);
                }

                else
                {
                    LARGE_INTEGER filesize;
                    filesize.LowPart = find_data.nFileSizeLow;
                    filesize.HighPart = find_data.nFileSizeHigh;
                    char *full_path = (char*)PL_Alloc0(MAX_PATH);
                    char *relative_path_tmp = (char*)PL_Alloc0(MAX_PATH);
                    char *relative_path = (char*)PL_Alloc0(MAX_PATH);
                    strcpy(full_path, current_dir->path);
                    PL_DirAddSlash(full_path);
                    strcat(full_path, find_data.cFileName);
                    strcpy(relative_path_tmp, full_path);
                    strcpy(relative_path, relative_path_tmp + strlen(path));
                    PL_DirToFwdSlash(relative_path);
                    PL_PushFileEntry(result->file_list, full_path, relative_path, (u64)filesize.QuadPart);

                    printf("FILE FOUND: FULL: %s RELATIVE: %s SIZE: %llu\n",
                           full_path, relative_path, (u64)filesize.QuadPart);

                    PL_Free(full_path);
                    PL_Free(relative_path);
                    PL_Free(relative_path_tmp);
                } //TODO continue

            }
            while(FindNextFileA(find, &find_data));

            PL_RemoveDirEntry(current_dir);
            current_dir = PL_GetLastDirEntry(result->dir_list);
        }
    }

    return result;
}

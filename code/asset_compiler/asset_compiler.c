/* ==============================================================
   Project: Ascencia
   File:    asset_compiler.c
   Author:  Phraggers
   Date:    20-11-2024
   ============================================================== */

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <util/types.h>

#if 0
local b32 AC_GetDirList(char *input_dir)
{
    AC_SetTrailingSlash(input_dir);
    AC_PushDir(input_dir);
    strcat(input_dir, "*");

    while(g_dirlist_count)
    {
        WIN32_FIND_DATA ffd;
        char *current_dir = (char*)malloc(MAX_PATH);
        memset(current_dir, 0, MAX_PATH);
        struct sDir *dir_entry = AC_GetLastDir();
        if(dir_entry) strcpy(current_dir, dir_entry->path);
        else
        {
            printf("ERROR: Expected dir but list is empty\n");
            return 0;
        }
        AC_PopDir();
        printf("DIR_ENTRY: %s\n", current_dir);

        strcat(current_dir, "*");
        HANDLE find = FindFirstFileA(current_dir, &ffd);
        current_dir[strlen(current_dir)-1] = 0;
        if(find == INVALID_HANDLE_VALUE)
        {
            printf("FindFirstFile Error: %d\n", GetLastError());
            return 0;
        }

        do
        {
            if(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                if(!strcmp(ffd.cFileName, ".") || !strcmp(ffd.cFileName, "..")) continue;
                char *found_dir = (char*)malloc(MAX_PATH);
                memset(found_dir, 0, MAX_PATH);
                strcpy(found_dir, current_dir);
                AC_SetTrailingSlash(found_dir);
                strcat(found_dir, ffd.cFileName);
                AC_SetTrailingSlash(found_dir);
                AC_PushDir(found_dir);
                free(found_dir);
            }
            else
            {
                LARGE_INTEGER filesize;
                filesize.LowPart = ffd.nFileSizeLow;
                filesize.HighPart = ffd.nFileSizeHigh;
                char *fullpath = (char*)malloc(MAX_PATH);
                memset(fullpath, 0, MAX_PATH);
                strcpy(fullpath, current_dir);
                strcat(fullpath, ffd.cFileName);
                AC_PushFile(fullpath, &fullpath[strlen(input_dir)-1], (u32)filesize.QuadPart);
                free(fullpath);
            }
        } while(FindNextFileA(find, &ffd));

        free(current_dir);
    }

    DWORD err = GetLastError();
    if(err && err != ERROR_NO_MORE_FILES)
    {
        printf("FindFirstFile (Post) Error: %d\n", err);
        return 0;
    }

    return 1;
}

local b32 AC_CreateDat(char *output_dir)
{
    AC_SetTrailingSlash(output_dir);
    char *output_filepath = (char*)malloc(MAX_PATH);
    memset(output_filepath, 0, MAX_PATH);
    strcpy(output_filepath, output_dir);
    strcat(output_filepath, "data.dat");

    FILE *datfile = fopen(output_filepath, "wb");
    u32 datfilesize = 0;
    if(!datfile)
    {
        printf("ERROR: opening %s to write\n", output_filepath);
        return 0;
    }

    //TODO: cmp file contents/address list

    while(g_filelist_count)
    {
        u64 timer = AC_QueryTimer();
        struct sFile *file_entry = AC_GetLastFile();

        FILE *ifile = fopen(file_entry->path, "rb");
        if(!ifile)
        {
            printf("ERROR: opening %s to read\n", file_entry->path);
            return 0;
        }

        void *ifile_data = malloc((u64)file_entry->filesize);
        fread(ifile_data, 1, (u64)file_entry->filesize, ifile);
        fclose(ifile);

        //LZ77_result cmp_file = LZ77_Compress(ifile_data, file_entry->filesize);

        free(ifile_data);
        //datfilesize += (u32)fwrite(cmp_file.data, 1, cmp_file.size, datfile);
        //if(cmp_file.data) free(cmp_file.data);

        r32 elapsed = AC_TimeElapsed(timer, AC_QueryTimer());
        //printf("FILE: %s: [%u bytes uncompressed] [%u bytes compressed] [compress time: %.02f sec]\n",
        //file_entry->name, file_entry->filesize, cmp_file.size, elapsed);

        AC_PopFile();
    }

    fclose(datfile);
    printf("OUTPUT: %s (%u bytes)", output_filepath, datfilesize);

    return 1;
}
#endif

int main(int argc, char **argv)
{
    printf("\nASSET_COMPILER: %s\n", argv[0]);
#if 0
    if(argc < 3 || argc > 3)
    {
        printf("ERROR: Invalid arg count: %d\n", argc);
        return -1;
    }

    printf("INPUT:  %s\nOUTPUT: %s\n", argv[1], argv[2]);

    AC_TimerInit();

    char *input_dir = (char*)malloc(MAX_PATH);
    char *output_dir = (char*)malloc(MAX_PATH);
    memset(input_dir, 0, MAX_PATH);
    memset(output_dir, 0, MAX_PATH);
    strcpy(input_dir, argv[1]);
    strcpy(output_dir, argv[2]);

    if(!AC_GetDirList(input_dir)) return -1;
    if(!AC_CreateDat(output_dir)) return -1;

    free(input_dir);
    free(output_dir);
#endif
    return 0;
}

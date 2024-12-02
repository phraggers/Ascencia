/* ==============================================================
   Project: Ascencia
   File:    logic_thread.c
   Author:  Phraggers
   Date:    18-11-2024
   ============================================================== */

#include <win32/platform.h>

DWORD WINAPI LogicThread(LPVOID param)
{
    g_state->logic_thread.mutex = CreateMutexA(0, 0, 0);
    WaitForSingleObject(g_state->logic_thread.mutex, INFINITE);
    ReleaseMutex(g_state->logic_thread.mutex);

#if 0 //TEST: zip test
    struct zip_t *zip = zip_open("test.zip", 0, 'r');
    if(zip)
    {
        for(int entry_index = 0;
            entry_index < zip_entries_total(zip);
            entry_index++)
        {
            zip_entry_openbyindex(zip, entry_index);

            const char *entryname = zip_entry_name(zip);
            int isdir = zip_entry_isdir(zip);
            u64 size = zip_entry_size(zip); //i think this is unzipped size
            u32 crc32 = zip_entry_crc32(zip);

            printf("Zip Entry: name[%s]  isdir[%d] size[%llu] crc32[%u]",
                    entryname, isdir, size, crc32);

            void *unzipped_data;
            u64 unzipped_size;
            i64 bytes_read = zip_entry_read(zip, &unzipped_data, &unzipped_size);

            printf("Read %lld bytes, data[%x], size[%llu]",
                    bytes_read, unzipped_data, unzipped_size);
            }

            zip_entry_close(zip);

            free(unzipped_data);
        }

        zip_close(zip);
    }
    else
    {
        printf("failed to open file: %s", data_path);
        return 0;
    }
#endif

#if 0 //TEST: lz77 test

    char *base_path = PL_Alloc0(MAX_PATH);
    char *test_path = PL_Alloc0(MAX_PATH);
    char *cmp_path = PL_Alloc0(MAX_PATH);
    char *dcmp_path = PL_Alloc0(MAX_PATH);
    GetModuleFileNameA(0, base_path, MAX_PATH);
    for(char *c=&base_path[strlen(base_path)]; *c!='\\'; c--) *c=0;
    strcpy(test_path, base_path);
    strcpy(cmp_path, base_path);
    strcpy(dcmp_path, base_path);
    strcat(test_path, "test.txt");
    strcat(cmp_path, "test_cmp.bin");
    strcat(dcmp_path, "test_dcmp.txt");

    {
        OutputDebugStringA("STARTING COMPRESSION\n");
        u64 cmp_timer = PL_QueryTimer();

        FILE *ftest = fopen(test_path, "rb");
        fseek(ftest, 0, SEEK_END);
        u32 fsize = (u32)ftell(ftest);
        fseek(ftest, 0, SEEK_SET);
        void *fdata = malloc((u64)fsize);
        fread(fdata, 1, fsize, ftest);
        fclose(ftest);
        LZ77_result test = LZ77_Compress(fdata, fsize);
        free(fdata);

        FILE *cmp = fopen(cmp_path, "wb");
        fwrite(test.data, 1, (u64)test.size, cmp);
        fclose(cmp);

        free(test.data);
        r32 cmp_elapsed = PL_TimeElapsed(cmp_timer, PL_QueryTimer());
        char logtmp[0xff] = {0};
        snprintf(logtmp, 0xff, "COMPRESSION COMPLETE in %.02f sec\n", cmp_elapsed);
        OutputDebugStringA(logtmp);
    }

    {
        OutputDebugStringA("STARTING DECOMPRESSION\n");
        u64 dcmp_timer = PL_QueryTimer();

        FILE *ftest = fopen(cmp_path, "rb");
        fseek(ftest, 0, SEEK_END);
        u32 fsize = (u32)ftell(ftest);
        fseek(ftest, 0, SEEK_SET);
        void *fdata = malloc((u64)fsize);
        fread(fdata, 1, fsize, ftest);
        fclose(ftest);
        LZ77_result test = LZ77_Decompress(fdata);
        free(fdata);

        FILE *dcmp = fopen(dcmp_path, "wb");
        fwrite(test.data, 1, (u64)test.size, dcmp);
        fclose(dcmp);

        free(test.data);
        r32 dcmp_elapsed = PL_TimeElapsed(dcmp_timer, PL_QueryTimer());
        char logtmp[0xff] = {0};
        snprintf(logtmp, 0xff, "DECOMPRESSION COMPLETE in %.02f sec\n", dcmp_elapsed);
        OutputDebugStringA(logtmp);
    }

    PL_Free(base_path);
    PL_Free(test_path);
    PL_Free(cmp_path);
    PL_Free(dcmp_path);

#endif

    r32 target_ms = 1000.0f / (r32)PL_LOGIC_THREAD_HZ;
    u64 timer = PL_QueryTimer();
    r32 elapsed_ms = 0.0f;

    while(g_state->running)
    {
        timer = PL_QueryTimer();


        elapsed_ms = (PL_TimeElapsed(timer, PL_QueryTimer()) * 1000.0f);
        if((u32)elapsed_ms < target_ms)
        {
            Sleep((u32)(target_ms - elapsed_ms));
        }
    }

    return 0;
}

b32 PL_StartLogicThread(void)
{
    sThread *thread = &g_state->logic_thread;
    thread->handle = CreateThread(0, 0, LogicThread, 0, 0, &thread->id);
    if(!thread->handle) return 0;
    return 1;
}

b32 PL_QuitLogicThread(void)
{
    sThread *thread = &g_state->logic_thread;
    WaitForSingleObject(thread->handle, 0);
    DWORD ecode = 0;
    if(!GetExitCodeThread(thread->handle, &ecode)) return 0;

    int attempts = 100;
    while(ecode == 259 && attempts >= 0)
    {
        WaitForSingleObject(thread->handle, 15);
        if(!GetExitCodeThread(thread->handle, &ecode)) return 0;
    }

    if(ecode)
    {
        if(attempts < 0) return 0; //thread hanging
        else return 0; //error
    }

    CloseHandle(g_state->logic_thread.mutex);
    return 1;
}

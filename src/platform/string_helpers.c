/*============================================================
 * Project: Ascencia
 * File: string_helpers.c
 * Author: Phraggers
 * Date: Thu Sep 12 2024
============================================================*/

#include <platform/string_helpers.h>
#include <time.h>

// print string as smallest name (eg size = 1024 + 512 = "1.5 KB")
void PL_String_ShortFileSize(cstr string, u64 size)
{
    r32 fsize = 0.0f;

    if(size >= GB(1))
    {
        fsize = (r32)size / (r32)GB(1);
        snprintf(string, STRING_LEN, "%.02f GB", fsize);
    }
    
    else if(size >= MB(1))
    {
        fsize = (r32)size / (r32)MB(1);
        snprintf(string, STRING_LEN, "%.02f MB", fsize);
    }
    
    else if(size >= KB(1))
    {
        fsize = (r32)size / (r32)KB(1);
        snprintf(string, STRING_LEN, "%.02f KB", fsize);
    }

    else
    {
        snprintf(string, STRING_LEN, "%lld bytes", size);
    }
}

void PL_String_TimeStampNow(cstr buffer, cstr format)
{
    time_t time_now;
    struct tm *tm_info;
    time_now = time(0);
    tm_info = localtime(&time_now);
    strftime(buffer, STRING_LEN, format, tm_info);
}
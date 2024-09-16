/*============================================================
 * Project: Ascencia
 * File: string_helpers.c
 * Author: Phraggers
 * Date: Thu Sep 12 2024
============================================================*/

#include <util/string_helpers.h>
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

void PL_String_Hex(cstr buffer, u8 *bp, u64 size)
{
    const cstr hex = "0123456789abcdef";
    cstr cp = buffer;

    for(int byte_index = 0;
        byte_index < size-1; 
        ++byte_index)
    {
        *cp++ = hex[(*bp>>4)&0xF];
        *cp++ = hex[(*bp++)&0xF];
    }

    *cp++ = hex[(*bp>>4)&0xF];
    *cp++ = hex[(*bp)&0xF];
    *cp = 0;
}
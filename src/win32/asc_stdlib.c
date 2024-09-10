
/*============================================================
 * Project: Ascencia [STDLIB DLL]
 * File: asc_stdlib.c
 * Author: Phraggers
 * Date: Sun Sep 08 2024
============================================================*/

#include <util/types.h>

#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

/*===================
    stdio.h
=====================*/
int STD_puts(const cstr cstring)
#ifdef DEBUG
{
    int result = puts(cstring);
    return result;
}
#else
{
    return 0;
}
#endif

/*===================
    string.h
=====================*/

ptr STD_memcpy(ptr dst, ptr src, u64 size)
{
    if(dst == 0 || src == 0 || size == 0)
    {
        return 0;
    }

    ptr result = memcpy(dst, (const ptr)src, size);

    return result;
}

u64 STD_strlen(cstr cstring)
{
    if(!cstring)
    {
        return 0;
    }

    u64 result = (u64)strlen(cstring);
    return result;
}

cstr STD_strcpy(cstr dst, const cstr src)
{
    if(dst == 0 || src == 0)
    {
        return 0;
    }
    
    //just cheese it for now
    //most of my strings are path length len anyway
    strcpy_s(dst, MAX_PATH_LENGTH, src);
    return dst;
}

cstr STD_strcat(cstr dst, cstr src)
{
    if(dst == 0 || src == 0)
    {
        return 0;
    }

    //just cheese it for now
     //most of my strings are path length len anyway
    strcat_s(dst, MAX_PATH_LENGTH, src);
    return dst;
}
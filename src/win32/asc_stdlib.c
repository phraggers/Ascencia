
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
{
    int result = puts(cstring);
    return result;
}

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
    //most of my strings are 0xff len anyway
    strcpy_s(dst, 0xff, src);
    return dst;
}

cstr STD_strcat(cstr dst, cstr src)
{
    if(dst == 0 || src == 0)
    {
        return 0;
    }

    //just cheese it for now
    //most of my strings are 0xff len anyway
    strcat_s(dst, 0xff, src);
    return dst;
}
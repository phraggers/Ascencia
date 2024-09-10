
/*============================================================
 * Project: Ascencia [SHARED]
 * File: stdlib_interface.h
 * Author: Phraggers
 * Date: Sun Sep 08 2024
============================================================*/
/* 
- This is to keep cstdlib separate from other namespaces
- MSVC cstdlib is not compatible with other platforms
   (ie '_s' versions of many stdlib functions) 
- MSVC cstdlib won't build with -WX and -Wall, which I want
    for main build
*/

#ifndef ASCENCIA_UTIL_STDLIB_INTERFACE_H
#define ASCENCIA_UTIL_STDLIB_INTERFACE_H

typedef int (*pSTD_puts)(const cstr cstring);
typedef ptr (*pSTD_memcpy)(ptr dst, ptr src, u64 size);
typedef u64 (*pSTD_strlen)(cstr cstring);
typedef cstr (*pSTD_strcpy)(cstr dst, const cstr src);
typedef cstr (*pSTD_strcat)(cstr dst, cstr src);

typedef struct sSTD_interface
{
    pSTD_puts fnSTD_puts;
    pSTD_memcpy fnSTD_memcpy;
    pSTD_strlen fnSTD_strlen;
    pSTD_strcpy fnSTD_strcpy;
    pSTD_strcat fnSTD_strcat;
} STD_interface;

#ifdef ASCENCIA_STDLIB_INSTANCE
STD_interface *G_stdlib_interface;
#else
extern STD_interface *G_stdlib_interface;
#endif

#define STD_puts G_stdlib_interface->fnSTD_puts
#define STD_memcpy G_stdlib_interface->fnSTD_memcpy
#define STD_strlen G_stdlib_interface->fnSTD_strlen
#define STD_strcpy G_stdlib_interface->fnSTD_strcpy
#define STD_strcat G_stdlib_interface->fnSTD_strcat

#endif /* ASCENCIA_UTIL_STDLIB_INTERFACE_H */
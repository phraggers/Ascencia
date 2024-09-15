/*============================================================
 * Project: Ascencia
 * File: linux_api.c
 * Author: Phraggers
 * Date: Sat Sep 14 2024
============================================================*/

#include <util/types.h>
#include <platform/alloc.h>
#include <platform/filesystem.h>

ptr PL_Alloc(u64 size)
{
    if(!size)
    {
        return 0;
    }

    ptr result = 0;
    result = malloc(size);
    return result;
}

ptr PL_Alloc0(u64 size)
{
    if(!size)
    {
        return 0;
    }

    ptr result = 0;
    result = malloc(size);
    if(result)
    {
        memset(result, 0, size);
    }
    return result;
}

ptr PL_ReAlloc(ptr old_mem, u64 new_size)
{
    if((!old_mem) || (!new_size))
    {
        return 0;
    }

    ptr result = 0;
    result = realloc(old_mem, new_size);
    return result;
}

ptr PL_ReAlloc0(ptr old_mem, u64 new_size)
{
    //TODO
    return PL_ReAlloc(old_mem, new_size);
}

b32 PL_Free(ptr mem)
{
    if(!mem)
    {
        return 0;
    }

    free(mem);
    return 1;
}

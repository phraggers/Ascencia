/* ==============================================================
   File:    alloc.c
   Author:  Phraggers
   Date:    21-04-2025
   ============================================================== */

void *PL_Alloc(u64 size)
{
    void *result = 0;
    result = (void*)HeapAlloc(GetProcessHeap(), 0, size);
    return result;
}

void *PL_Alloc0(u64 size)
{
    void *result = 0;
    result = (void*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, size);
    return result;
}

void *PL_ReAlloc(void *old, u64 size)
{
    void *result = 0;
    result = (void*)HeapReAlloc(GetProcessHeap(), 0, old, size);
    return result;
}

void *PL_ReAlloc0(void *old, u64 size)
{
    void *result = 0;
    result = (void*)HeapReAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, old, size);
    return result;
}

b32 PL_Free(void *mem)
{
    BOOL win32_result = HeapFree(GetProcessHeap(), 0, mem);
    b32 result = (b32)((win32_result) ? 1 : 0);
    return result;
}

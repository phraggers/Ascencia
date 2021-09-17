#ifndef MEMORY_H
/* ========================================================================
   $File:    Memory [Header] $
   $Project: Ascencia $
   $Date:    16-09-2021 $
   $Author:  Phil Bagshaw $
   $Notice:  (c)Phragware 2021 $
   ======================================================================== */

enum
Asc_MemoryType
{
  ASC_MEM_STATIC, // mem can only be assigned once
  ASC_MEM_ROLLOVER, // when memory runs out the pointer wraps back to 0 and continues
  ASC_MEM_DYNAMIC //when mem is freed all existing get shifted down (slow, pointer unfriendly)
};

struct
Asc_MemoryBlock
{
    int8 Initialized;
    uint64 Size; //bytes reserved for block
    uint64 Pointer; //pointer within block to assign new variables
    Asc_MemoryType Type;
    void *Memory;
};

static struct
Asc_Memory
{
    uint64 Size;
    uint64 Pointer;
    void *Memory;
} Memory;

static Asc_MemoryBlock*
ASC_NewMemoryBlock(uint64 Size, Asc_MemoryType Type)
{
    Asc_MemoryBlock *Result = 0;

    if(Memory.Pointer + sizeof(Asc_MemoryBlock) + Size <= (size_t)Memory.Memory + Memory.Size)
    {
        Result = (Asc_MemoryBlock*)Memory.Pointer;
        Memory.Pointer += (sizeof(Asc_MemoryBlock) + Size);
        SDL_memset((void*)Result, 0, sizeof(Asc_MemoryBlock));

        Result->Initialized = 1;
        Result->Size = Size;
        Result->Type = Type;
        Result->Memory = (void*)((size_t)Result + sizeof(Asc_MemoryBlock));
        Result->Pointer = (uint64)Result->Memory;
    }

    else
    {
        SDL_SetError("Ran out of Memory!");
        SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, SDL_GetError());
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Fatal Error!",
                                 SDL_GetError(), 0);
        Application.Running = 0;
        Result = 0;
    }

    return Result;
}

static void*
ASC_Alloc(Asc_MemoryBlock *Block, size_t Size)
{
    void* Result = 0;

    if(Block->Initialized)
    {
        if((Block->Pointer - (size_t)Block->Memory) + Size <= Block->Size)
        {
            Result = (void*)Block->Pointer;
            Block->Pointer += Size;
        }

        else
        {
            switch(Block->Type)
            {
              case ASC_MEM_STATIC:
              case ASC_MEM_DYNAMIC:
              {
                  SDL_SetError("MemBlock out of Memory! (%d Bytes / %d Bytes)", (Block->Pointer - (size_t)Block->Memory) + sizeof(void*), Block->Size);
                  SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, SDL_GetError());
              } break;

              case ASC_MEM_ROLLOVER:
              {
                  Block->Pointer = (uint64)Block->Memory;
                  Result = (void*)Block->Pointer;
                  Block->Pointer += Size;
              } break;
            }
        }
    }

    else
    {
        SDL_SetError("Attempted to write to uninitialized memory! 0x%#016X", (size_t)Block);
        SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, SDL_GetError());
    }

    return Result;
}

static int8
ASC_Free(Asc_MemoryBlock *Block, void *Pointer, size_t Size)
{
    int8 Result = 0;

    if((uint64)Pointer >= (uint64)Block->Memory &&
       (uint64)Pointer < (uint64)Block->Memory + Block->Size)
    {
        switch(Block->Type)
        {
          case ASC_MEM_STATIC:
          {
              SDL_memset(Pointer, 0, Size);
          } break;

          case ASC_MEM_DYNAMIC:
          {

          } break;

          case ASC_MEM_ROLLOVER:
          {
              SDL_memset(Pointer, 0, Size);
          } break;
        }
    }

    else
    {
        SDL_SetError("Attempted to free memory from invalid MemBlock! 0x%#016X", (size_t)Pointer);
        SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, SDL_GetError());
    }

    return Result;
}

#define MEMORY_H
#endif

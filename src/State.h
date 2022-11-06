
#ifndef _ASC_APPSTATE_H_
#define _ASC_APPSTATE_H_

// Allocation macros
#define ASC_Alloc(TYPE, NAME) TYPE* NAME = (TYPE*)State_Alloc(sizeof(TYPE), 0)
//#define ASC_AllocAlign(TYPE, NAME, ALIGN) TYPE* NAME = (TYPE*)State_Alloc(sizeof(TYPE), ALIGN)
#define ASC_AllocAlign(TYPE, NAME) TYPE* NAME = (TYPE*)State_Alloc(sizeof(TYPE), 8)
#define ASC_AllocArray(TYPE, COUNT, NAME) TYPE* NAME = (TYPE*)State_Alloc(sizeof(TYPE) * COUNT, 0)
//#define ASC_AllocArrayAlign(TYPE, COUNT, NAME, ALIGN) TYPE* NAME = (TYPE*)State_Alloc(sizeof(TYPE) * COUNT, ALIGN)
#define ASC_AllocArrayAlign(TYPE, COUNT, NAME) TYPE* NAME = (TYPE*)State_Alloc(sizeof(TYPE) * COUNT, 8)

#define ASC_ReAlloc(PTR, NEWSIZE) State_ReAlloc(PTR, NEWSIZE, 0)
#define ASC_ReAllocAlign(PTR, NEWSIZE, ALIGN) State_ReAlloc(PTR, NEWSIZE, ALIGN)
#define ASC_ReAllocAutoAlign(PTR, NEWSIZE) State_ReAlloc(PTR, NEWSIZE, NEWSIZE)

#define ASC_Free(PTR) State_Free(PTR)

// malloc/realloc/free syntax
#define ASC_malloc(SIZE) State_Alloc(SIZE, 8)
#define ASC_realloc(PTR, NEWSIZE) State_ReAlloc(PTR, NEWSIZE, 8)
#define ASC_free(PTR) State_Free(PTR)

typedef struct
{
	u64 MemSize; // total memsize
	u64 UsedMem; // currently allocated mem

	void* Data; // byte where user data starts
	void* DataEnd; // last byte of usable memory (do not access beyond this byte)

	SDL_version AppVersion;
	SDL_version SDLVersion;
	SDL_version MixVersion;
	SDL_version NetVersion;
	SDL_version GLVersion;

	char* PrefPath;
	char* BasePath;
	int ArgC;
	char** ArgV;

	ASC_S_Log Log;
	ASC_Timer Timer;
	ASC_Input Input;
	ASC_Window Window;
	ASC_Audio Audio;

	bool Running;
	GAMESTATE GameState;
} ASC_AppState;

// Pass 0 for default (sizeof ASC_AppState + DEF_ALLOCSIZE)
void* State_Init(u64 _Size);
void State_Quit(void);

void* State_Alloc(u64 _Size, u64 _Align);
void* State_ReAlloc(void* _Ptr, u64 _NewSize, u64 _Align);
void State_Free(void* _Ptr);

#endif //_ASC_APPSTATE_H_
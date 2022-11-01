
#ifndef _ASC_APPSTATE_H_
#define _ASC_APPSTATE_H_

#define ASC_DEFAULTUSERSIZE MB(256)

#define ASC_Alloc(TYPE, NAME) TYPE* NAME = (TYPE*)State_Alloc(sizeof(TYPE), 0)
#define ASC_AllocAlign(TYPE, NAME, ALIGN) TYPE* NAME = (TYPE*)State_Alloc(sizeof(TYPE), ALIGN)
#define ASC_AllocArray(TYPE, COUNT, NAME) TYPE* NAME = (TYPE*)State_Alloc(sizeof(TYPE) * COUNT, 0)
#define ASC_AllocArrayAlign(TYPE, COUNT, NAME, ALIGN) TYPE* NAME = (TYPE*)State_Alloc(sizeof(TYPE) * COUNT, ALIGN)
#define ASC_Free(NAME) State_Free(NAME)

typedef struct
{
	u64 MemSize; // total memsize
	void* Data; // byte where user data starts
	void* DataEnd; // last byte of usable memory (do not access beyond this byte)
	char AppName[32];
	char AppOrg[32];
	char LoginName[32];
	SDL_version AppVersion;
	SDL_version SDLVersion;
	SDL_version MixVersion;
	SDL_version NetVersion;
	SDL_version GLVersion;
	char* PrefPath;
	char* BasePath;
	bool Running;
	ASC_Timer Timer;
	ASC_Input Input;
	ASC_Window Window;
} ASC_AppState;

// Pass 0 for default (sizeof ASC_AppState + ASC_DEFAULTUSERSIZE)
void* State_Init(u64 _Size);
void State_Quit();

void* State_Alloc(u64 _Size, u64 _Align);
void State_Free(void* _Ptr);

#endif //_ASC_APPSTATE_H_
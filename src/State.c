#include "Ascencia.h"

void* State_Init(u64 _Size)
{
	u64 Size = _Size;

	if ((Size < sizeof(ASC_AppState) + DEF_ALLOCSIZE) ||
		((Size % 8) != 0))
	{
		if (Size < sizeof(ASC_AppState) + DEF_ALLOCSIZE)
		{
			Size = sizeof(ASC_AppState) + DEF_ALLOCSIZE;
		}

		while ((Size % 8) != 0)
		{
			Size++;
		}
	}

	void* Result = SDL_malloc(Size);

	if (!Result)
	{
		return 0;
	}

	else
	{
		ASC_AppState* StateResult = (ASC_AppState*)Result;
		SDL_memset(Result, 0, Size);

		StateResult->MemSize = Size;
		StateResult->Data = (void*)((u64)StateResult + (u64)sizeof(ASC_AppState));
		StateResult->DataEnd = (void*)((u64)StateResult + Size - 1);
		StateResult->UsedMem = 0;
	}

	return Result;
}

void State_Quit(void)
{
	if (State)
	{
		if (State->PrefPath) SDL_free(State->PrefPath);
		if (State->BasePath) SDL_free(State->BasePath);
		SDL_free(State);
	}
}

void* State_Alloc(u64 _Size, u64 _Align)
{
	if (!State) return 0;

	void* Result = State->Data;
	u64 RequiredSizeCounter = sizeof(u64) + _Size;
	void* PreviousRegionEnd = State->Data;

	for (char* c = (char*)State->Data; (u64)c < (u64)State->DataEnd; c++)
	{
		u64* iSize = (u64*)c;

		bool Unaligned = 0;

		if (_Align > 0)
		{
			if ((u64)Result % _Align != 0) // TODO: double check this is the correct check
			{
				Unaligned = 1;
			}

			if (!Unaligned)
			{
				if ((u64)((u64)Result - (u64)sizeof(u64)) <= (u64)PreviousRegionEnd)
				{
					Unaligned = 1;
				}
			}
		}

		if ( (*iSize != 0) || Unaligned)
		{
			RequiredSizeCounter = sizeof(u64) + _Size; // reset counter

			u64 IncSize = *iSize;
			for (u64 i = 0; i < IncSize; i++) c++;
			Result = (void*)c;
			if(IncSize > 0) c--; // for() will increment c again
			PreviousRegionEnd = (void*)c;

			continue;
		}

		RequiredSizeCounter--;
		
		if(RequiredSizeCounter == 0)
		{
			if (_Align > 0)
			{
				// shift back to put marker before aligned byte
				char* ptr = (char*)Result;
				for (int i = 0; i < sizeof(u64); i++) ptr--;
				Result = (void*)ptr;
			}

			u64 SizeMarker = sizeof(u64) + _Size;
			SDL_memcpy(Result, (void*)&SizeMarker, sizeof(u64));

			// shift fwd to return result
			char* ptr = (char*)Result;
			for (int i = 0; i < sizeof(u64); i++) ptr++;
			Result = (void*)ptr;

			State->UsedMem += _Size + sizeof(u64);

			ASC_Log(LOGLEVEL_DEBUG, "STATE: Allocated Memory Block [0x%x] (%u Bytes) (%u Bytes Remaining)",
					Result, _Size, State->MemSize - State->UsedMem);
			return Result;
		}
	}

	ASC_Log(LOGLEVEL_FATAL, "STATE: Allocator Out of Memory!");
	return 0;
}

void* State_ReAlloc(void* _Ptr, u64 _NewSize, u64 _Align)
{
	u64 OldSize;
	{
		char* Ptr = (char*)_Ptr;
		Ptr -= sizeof(u64);
		u64* iPtr = (u64*)Ptr;

		if (Ptr < (char*)State->Data || Ptr > (char*)State->DataEnd)
		{
			ASC_Log(LOGLEVEL_FATAL, "STATE: Failed to realloc mem block! (0x%x Out of Bounds!)", _Ptr);
			return 0;
		}

		OldSize = *iPtr;
	}

	void* Result = State_Alloc(_NewSize, _Align);

	if (!Result)
	{
		ASC_Log(LOGLEVEL_FATAL, "STATE: Failed to realloc mem block! (%u Bytes From 0x%x To %u Bytes)",
				OldSize, _Ptr, _NewSize);
		return 0;
	}

	if(_NewSize > OldSize) SDL_memcpy(Result, _Ptr, (size_t)OldSize);
	else SDL_memcpy(Result, _Ptr, _NewSize);

	State_Free(_Ptr);

	ASC_Log(LOGLEVEL_DEBUG, "STATE: Reallocated Memory Block [0x%x] (%u Bytes) to [0x%x] (%u Bytes)",
			_Ptr, OldSize, Result, _NewSize);

	return Result;
}

void State_Free(void* _Ptr)
{
	if (!State ||
		_Ptr < State->Data ||
		_Ptr >= State->DataEnd)
	{
		return;
	}

	void* Ptr = _Ptr;
	{
		// Move pointer back to size variable (ptr - sizeof(u64))
		char* dec = (char*)_Ptr;
		dec -= sizeof(u64);
		Ptr = (void*)dec;
	}

	u64* iPtr = (u64*)Ptr;
	u64 Size = *iPtr;
	char* cPtr = (char*)Ptr;

	for (u64 i = 0; i < Size; i++)
	{
		*cPtr = 0;
		cPtr++;
	}

	State->UsedMem -= Size;

	ASC_Log(LOGLEVEL_DEBUG, "STATE: Freed Memory Block [0x%x] (%u Bytes) (%u Bytes Remaining)",
			_Ptr, Size - sizeof(u64), State->MemSize - State->UsedMem);
}
#include "Ascencia.h"

void* State_Init(u64 _Size)
{
	u64 Size = _Size;

	if ((Size < sizeof(ASC_AppState) + ASC_DEFAULTUSERSIZE) ||
		((Size % 8) != 0))
	{
		if (Size < sizeof(ASC_AppState) + ASC_DEFAULTUSERSIZE)
		{
			Size = sizeof(ASC_AppState) + ASC_DEFAULTUSERSIZE;
		}

		while ((Size % 8) != 0)
		{
			Size++;
		}
	}

	if (_Size != Size)
	{
		ASC_Log(LOGLEVEL_DEBUG, "STATE: Requested %u Bytes, Requesting: %u Bytes",
				_Size, Size);
	}
	else
	{
		ASC_Log(LOGLEVEL_DEBUG, "STATE: Requested size: %u Bytes", _Size);
	}

	void* Result = SDL_malloc(Size);

	if (!Result)
	{
		ASC_Log(LOGLEVEL_FATAL, "STATE: Error allocating Memory");
		return 0;
	}
	else
	{
		ASC_AppState* StateResult = (ASC_AppState*)Result;
		ASC_Log(LOGLEVEL_DEBUG, "STATE: Clearing Memory to 0");
		SDL_memset(Result, 0, Size);
		ASC_Log(LOGLEVEL_DEBUG, "STATE: Memory Cleared");

		StateResult->MemSize = Size;
		StateResult->Data = (void*)((u64)StateResult + (u64)sizeof(ASC_AppState));
		StateResult->DataEnd = (void*)((u64)StateResult + Size - 1);

		ASC_Log(LOGLEVEL_DEBUG, "STATE: Alloc AppState[0x%x] Size[%u], Data[%u] DataEnd[%u]",
				Result, Size, StateResult->Data, StateResult->DataEnd);
	}

	return Result;
}

void State_Quit()
{
	if (State)
	{
		ASC_Log(LOGLEVEL_DEBUG, "STATE: Freeing Memory");
		if (State->PrefPath) SDL_free(State->PrefPath);
		if (State->BasePath) SDL_free(State->BasePath);
		SDL_free(State);
	}
}

void* State_Alloc(u64 _Size, u64 _Align)
{
	if (!State) return 0;

	void* Result = State->Data;
	u64* iResult = (u64*)Result; // result cast to u64 to read Size tag
	char* cResult = (char*)iResult; // byte pointer & iterator
	u64 PrevResult = *iResult; // Size tag at prev block (before shifting)

	// alignment (a u64 is placed before actual returned mem block, so we shift up by sizeof u64)
	if (_Align)
	{
		while ((((u64)cResult + sizeof(u64)) % _Align) != 0) cResult++;
		iResult = (u64*)cResult;
		Result = (void*)cResult;
	}

	while (cResult < (char*)State->DataEnd)
	{
		if (PrevResult == 0)
		{
			bool Free = 1;

			for (u64 i = 0; i < sizeof(u64) + _Size; i++)
			{
				cResult += i;

				if (cResult >= (char*)State->DataEnd)
				{
					ASC_Log(LOGLEVEL_FATAL, "STATE: Out of Memory!");
					return 0;
				}

				if (*cResult != 0) // used mem block found, skip to next
				{
					Free = 0;
					u64 PrevResult = *iResult;

					// re-align before continuing
					if (_Align)
					{
						while ((((u64)cResult + sizeof(u64)) % _Align) != 0) cResult++;
						iResult = (u64*)cResult;
						Result = (void*)cResult;
					}

					break;
				}
			}

			if (Free)
			{
				*iResult = _Size + sizeof(u64);

				// move pointer forward to make space for size variable
				char* inc = (char*)Result;
				inc += sizeof(u64);
				Result = (void*)inc;

				ASC_Log(LOGLEVEL_DEBUG, "STATE: Allocated Memory Block [0x%x], %u Bytes", Result, _Size);
				return Result;
			}
		}

		else
		{
			cResult = (char*)((u64)cResult + PrevResult);
			iResult = (u64*)cResult;
			PrevResult = *iResult;

			// re-align before continuing
			if (_Align)
			{
				while ((((u64)cResult + sizeof(u64)) % _Align) != 0) cResult++;
				iResult = (u64*)cResult;
				Result = (void*)cResult;
			}

			if (cResult >= (char*)State->DataEnd)
			{
				ASC_Log(LOGLEVEL_FATAL, "STATE: Out of Memory!");
				return 0;
			}
		}
	}

	return 0;
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

	ASC_Log(LOGLEVEL_DEBUG, "STATE: Freed Memory Block [0x%x] (%u Bytes)", _Ptr, Size - sizeof(u64));
}
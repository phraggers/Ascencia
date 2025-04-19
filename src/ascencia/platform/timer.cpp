// Ascencia
// platform/timer.cpp

#include <ascencia/platform/core.h>

cTimer::cTimer()
{
	
}

bool cTimer::Init(void)
{
	return 1;
}

bool cTimer::Add(std::string ID)
{
	for (auto i : Timers)
	{
		if (i.first == ID)
		{
			return 0;
		}
	}

	Timers[ID] = SDL_GetTicksNS();
	return 1;
}

f32 cTimer::Query(std::string ID)
{
	std::map<std::string, u64>::iterator Iterator = Timers.find(ID);
	if (Iterator != Timers.end())
	{
		u64 Elapsed = SDL_GetTicksNS() - Timers[ID];
		f32 Result = (f32)NANOSECONDS_TO_SECONDS((f32)Elapsed);
		return Result;
	}

	return 0.0f;
}

f32 cTimer::Remove(std::string ID)
{
	f32 Result = 0.0f;

	std::map<std::string, u64>::iterator Iterator = Timers.find(ID);
	if (Iterator != Timers.end())
	{
		u64 Elapsed = SDL_GetTicksNS() - Timers[ID];
		Result = (f32)NANOSECONDS_TO_SECONDS((f32)Elapsed);
		Timers.erase(Iterator);
	}	

	return Result;
}

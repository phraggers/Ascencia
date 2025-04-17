// Ascencia
// ascencia/platform/timer.cpp

#include <ascencia/platform/application.h>

sTimer::sTimer()
{
	
}

bool sTimer::Init(void)
{
	return 1;
}

bool sTimer::Add(std::string ID)
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

f32 sTimer::Query(std::string ID)
{
	std::map<std::string, u64>::iterator Iterator = Timers.find(ID);
	if (Iterator != Timers.end())
	{
		u64 Elapsed = SDL_GetTicksNS() - Timers[ID];
		f32 Result = NANOSECONDS_TO_SECONDS((f32)Elapsed);
		return Result;
	}

	return 0.0f;
}

f32 sTimer::Remove(std::string ID)
{
	f32 Result = 0.0f;

	std::map<std::string, u64>::iterator Iterator = Timers.find(ID);
	if (Iterator != Timers.end())
	{
		u64 Elapsed = SDL_GetTicksNS() - Timers[ID];
		Result = NANOSECONDS_TO_SECONDS((f32)Elapsed);
		Timers.erase(Iterator);
	}	

	return Result;
}

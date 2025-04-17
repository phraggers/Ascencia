// Ascencia
// ascencia/platform/timer.h

#pragma once

struct sTimer
{
	std::map<std::string, u64> Timers;

	sTimer();
	bool Init(void);
	bool Add(std::string ID);
	f32 Query(std::string ID);
	f32 Remove(std::string ID);
};
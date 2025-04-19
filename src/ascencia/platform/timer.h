// Ascencia
// platform/timer.h

#pragma once

class cTimer
{
	std::map<std::string, u64> Timers;

	public:
	cTimer();
	bool Init(void);
	bool Add(std::string ID);
	f32 Query(std::string ID);
	f32 Remove(std::string ID);
};
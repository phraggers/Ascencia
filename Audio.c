
#include "Ascencia.h"

r32 Audio_GetMusicVolume(void)
{
	if (!State) return 0;

	int Volume = Mix_VolumeMusic(-1);
	return (r32)((r32)State->Audio.MusicVolume / 128.0f);
}

void Audio_SetMusicVolume(r32 _Volume)
{
	if (!State) return;

	int Volume;

	if (_Volume <= 0.0f) Volume = 0;
	else if (_Volume >= 1.0f) Volume = 128;
	else Volume = (int)((r32)128.0f * (r32)_Volume);

	State->Audio.MusicVolume = (r32)Volume / 128.0f;
	Mix_VolumeMusic(Volume);
}

// chunk settings
//Mix_Volume(_Channel, _Volume); //channel -1 for all (int, int)
//Mix_SetPanning(_Channel, _Left, _Right); //(int, Uint8, Uint8) _Channel = MIX_CHANNEL_POST means pan happens last
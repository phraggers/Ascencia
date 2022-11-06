#ifndef _ASC_AUDIO_H_
#define _ASC_AUDIO_H_

typedef struct
{
	r32 MusicVolume; // 0.0f - 1.0f
} ASC_Audio;

r32 Audio_GetMusicVolume(void);
void Audio_SetMusicVolume(r32 _Volume);

#endif //_ASC_AUDIO_H_
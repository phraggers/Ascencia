/*============================================================
 * Project: Ascencia
 * File: audio.h
 * Author: Phraggers
 * Date: Sun Sep 15 2024
============================================================*/

#ifndef ASCENCIA_AUDIO_H
#define ASCENCIA_AUDIO_H

#include <util/types.h>

#define AUDIO_SAMPLE_RATE 48000
#define AUDIO_BYTES_PER_SAMPLE 4 // sizeof(i16)*2
#define AUDIO_BUFFER_SECONDS 1 // audio buffer size in seconds
#define AUDIO_BYTES_PER_SEC (AUDIO_SAMPLE_RATE * AUDIO_BYTES_PER_SAMPLE)
#define AUDIO_BUFFER_SIZE (AUDIO_BYTES_PER_SEC * AUDIO_BUFFER_SECONDS)

typedef struct
{
    bool is_init;
    r32 volume;
    u32 play_cursor; // sound driver last reported play pos within buffer
    u8 buffer[AUDIO_BUFFER_SIZE]; // circular play buffer
} PL_Audio;

bool PL_AudioInit(void);
PL_Audio* PL_GetAudio(void);
ptr PL_GetAudioBuffer(void);
void PL_AudioFrame(void);

void PL_AudioTestSquare(int hz, r32 vol);
void PL_AudioTestSine(int hz, r32 vol);

#endif /* ASCENCIA_AUDIO_H */
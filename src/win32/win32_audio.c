/*============================================================
 * Project: Ascencia
 * File: win32_audio.c
 * Author: Phraggers
 * Date: Sun Sep 15 2024
============================================================*/

#include <platform/audio.h>
#include <platform/alloc.h>
#include <win32/win32_api.h>
#include <win32/win32_state.h>

#include <dsound.h>

typedef int (*pfn_DirectSoundCreate)(LPCGUID guid, LPDIRECTSOUND *lpds, LPUNKNOWN puo);

typedef struct
{
    pfn_DirectSoundCreate Create;
    LPDIRECTSOUND dsound;
    WAVEFORMATEX wavefmt;
    LPDIRECTSOUNDBUFFER primary_buffer;
    LPDIRECTSOUNDBUFFER secondary_buffer;
} DSoundState;
global DSoundState *dsstate;

local bool InitDSound(void)
{
    if(!dsstate)
    {
        dsstate = PL_Alloc0(sizeof(DSoundState));
        if(!dsstate)
        {
            PL_Log(LOG_FATAL, "DSoundInit: malloc error");
            return 0;
        }
    }

    ptr dsound_dll = WIN_API.LoadLibraryA("dsound.dll");

    if(!dsound_dll)
    {
        PL_Log(LOG_ERROR, "InitDSound: failed to load dsound.dll");
        return 0;
    }

    dsstate->Create = (pfn_DirectSoundCreate)WIN_API.GetProcAddress(dsound_dll, "DirectSoundCreate");

    if(!dsstate->Create)
    {
        PL_Log(LOG_ERROR, "InitDSound: failed to load DirectSoundCreate function");
        return 0;
    }
    
    PL_Log(LOG_INFO, "InitDSound: Loaded dsound.dll");

    
    int ecode = dsstate->Create(0, &dsstate->dsound, 0);
    if(ecode < 0)
    {
        PL_Log(LOG_ERROR, "InitDSound: DirectSoundCreate failed: %d", ecode);
        return 0;
    }

    ecode = dsstate->dsound->lpVtbl->SetCooperativeLevel(dsstate->dsound, G_win32_state->window.handle, DSSCL_PRIORITY);
    if(ecode < 0)
    {
        PL_Log(LOG_ERROR, "InitDSound: SetCooperativeLevel failed: %d", ecode);
        return 0;
    }

    DSBUFFERDESC primary_buffer_desc = {sizeof(primary_buffer_desc)};
    primary_buffer_desc.dwFlags = DSBCAPS_PRIMARYBUFFER;
    ecode = dsstate->dsound->lpVtbl->CreateSoundBuffer(dsstate->dsound, &primary_buffer_desc, &dsstate->primary_buffer, 0);
    if(ecode < 0)
    {
        PL_Log(LOG_ERROR, "InitDSound: CreateSoundBuffer(primary) failed: %d", ecode);
        return 0;
    }

    dsstate->wavefmt.wFormatTag = WAVE_FORMAT_PCM;
    dsstate->wavefmt.nChannels = 2;
    dsstate->wavefmt.nSamplesPerSec = AUDIO_SAMPLE_RATE;
    dsstate->wavefmt.wBitsPerSample = 16;
    dsstate->wavefmt.nBlockAlign = (dsstate->wavefmt.nChannels * dsstate->wavefmt.wBitsPerSample) / 8;
    dsstate->wavefmt.nAvgBytesPerSec = dsstate->wavefmt.nSamplesPerSec * dsstate->wavefmt.nBlockAlign;
    
    ecode = dsstate->primary_buffer->lpVtbl->SetFormat((IDirectSoundBuffer*)dsstate->primary_buffer, &dsstate->wavefmt);
    if(ecode < 0)
    {
        PL_Log(LOG_ERROR, "InitDSound: PrimaryBuffer:SetFormat failed: %d", ecode);
        return 0;
    }

    DSBUFFERDESC secondary_buffer_desc = {sizeof(secondary_buffer_desc)};
    secondary_buffer_desc.dwBufferBytes = AUDIO_BUFFER_SIZE;
    secondary_buffer_desc.lpwfxFormat = &dsstate->wavefmt;
    ecode = dsstate->dsound->lpVtbl->CreateSoundBuffer(dsstate->dsound, &secondary_buffer_desc, &dsstate->secondary_buffer, 0);
    if(ecode < 0)
    {
        PL_Log(LOG_ERROR, "InitDSound: CreateSoundBuffer(secondary) failed: %d", ecode);
        return 0;
    }

    ecode = dsstate->secondary_buffer->lpVtbl->Play((IDirectSoundBuffer*)dsstate->secondary_buffer, 0, 0, DSBPLAY_LOOPING);
    if(ecode < 0)
    {
        PL_Log(LOG_ERROR, "InitDSound: Play failed: %d", ecode);
        return 0;
    }

    PL_GetAudio()->is_init = 1;
    return 1;
}

bool PL_AudioInit(void)
{
    if(!InitDSound())
    {
        PL_Log(LOG_ERROR, "AudioInit: failed to init DSound");
        return 0;
    }

    return 1;
}

PL_Audio* PL_GetAudio(void)
{
    PL_Audio *result = 0;
    
    if(G_win32_state)
    {
        result = &G_win32_state->audio;
    }

    return result;
}

ptr PL_GetAudioBuffer(void)
{
    ptr result = 0;

    if(PL_GetAudio())
    {
        result = PL_GetAudio()->buffer;
    }

    return result;
}

void PL_AudioFrame(void)
{
    if((!PL_GetAudio()) || 
       (!PL_GetAudio()->is_init))
    {
        return;
    }
    
    PL_AudioTestSquare(440, 0.05f);
}

void PL_AudioTestSquare(int hz, r32 vol)
{
    persist u32 square_index = 0;
    int period = AUDIO_SAMPLE_RATE / hz;

    u32 play_cursor, write_cursor;

    int ecode = dsstate->secondary_buffer->lpVtbl->GetCurrentPosition((IDirectSoundBuffer*)dsstate->secondary_buffer, (LPDWORD)&play_cursor, (LPDWORD)&write_cursor);
    if(ecode >= 0)
    {
        u32 lock_byte = (square_index * AUDIO_BYTES_PER_SAMPLE) % AUDIO_BUFFER_SIZE;
        u32 bytes;

        if(lock_byte == play_cursor)
        {
            bytes = AUDIO_BUFFER_SIZE;
        }
        else if(lock_byte > play_cursor)
        {
            bytes = (AUDIO_BUFFER_SIZE - lock_byte);
            bytes += play_cursor;
        }
        else
        {
            bytes = play_cursor - lock_byte;
        }
        
        ptr regions[2];
        u32 regionsize[2];

        ecode = dsstate->secondary_buffer->lpVtbl->Lock((IDirectSoundBuffer*)dsstate->secondary_buffer, lock_byte, bytes, &regions[0], (LPDWORD)&regionsize[0], &regions[1], (LPDWORD)&regionsize[1], 0);
        if(ecode >= 0)
        {
            i16 vol16 = (i16)((r32)INT16_MAX * vol);
            
            for(int i=0;i<2;i++)
            {
                u32 region_count = regionsize[i] / AUDIO_BYTES_PER_SAMPLE;
                i16 *sample = (i16*)regions[i];

                for(u32 sample_index = 0;
                sample_index < region_count;
                sample_index++)
                {
                    i16 sample_value = ((square_index++ / (period/2))%2) ? vol16 : -vol16;
                    *sample++ = sample_value;
                    *sample++ = sample_value;
                }
            }

            dsstate->secondary_buffer->lpVtbl->Unlock((IDirectSoundBuffer*)dsstate->secondary_buffer, regions[0], regionsize[0], regions[1], regionsize[1]);
        }
    }
}

void PL_AudioTestSine(int hz, r32 vol)
{

}
/* ==============================================================
   Project: Ascencia
   File:    audio_thread.c
   Author:  Phraggers
   Date:    18-11-2024
   ============================================================== */

#include <win32/platform.h>

DWORD WINAPI AudioThread(LPVOID param)
{
    g_state->audio_thread.mutex = CreateMutexA(0, 0, 0);
    WaitForSingleObject(g_state->audio_thread.mutex, INFINITE);
    ReleaseMutex(g_state->audio_thread.mutex);

    if(!PL_InitDSound()) return -1;
    PL_ClearSoundBuffer();
    PL_AudioPlay();
    if(!g_state->audio.is_init) return 0;

    i16 *samples = (i16*)PL_Alloc0(g_state->audio.dsbuffer_size);
    DWORD latency_bytes = 0;
    r32 latency_sec = 0;

    //TEST: square tune
    #if 0
    {
        i16 *tgt_sample = samples;
        u32 sample_index = 0;
        u32 vol = 500;
        u32 freq = 256;
        for(u32 i=0; i<g_state->audio.dsbuffer_size; i+=sizeof(i16)*2)
        {
            i16 sample1 = ((sample_index++ / ((48000/freq)/2)) %2) ? vol:-vol;
            *tgt_sample++ = sample1;
            *tgt_sample++ = sample1;

            if(i%128==0)vol--;
            if(vol==0)
            {
                vol = 500;
                freq -= 32;
                if(freq <= 64) freq = 256;
            }
        }
    }
    #endif

    r32 target_ms = 1000.0f / (r32)PL_AUDIO_THREAD_HZ;
    u64 timer = PL_QueryTimer();
    r32 elapsed_ms = 0.0f;

    b32 sound_started = 0;

    while(g_state->running)
    {
        timer = PL_QueryTimer();

        DWORD play_cursor, write_cursor;
        HRESULT result = IDirectSoundBuffer_GetCurrentPosition(g_state->audio.dsbuffer,
                                                               &play_cursor, &write_cursor);
        if(result == DS_OK)
        {
            if(!sound_started)
            {
                g_state->audio.sample_index = write_cursor / g_state->audio.bytes_per_sample;
                sound_started = 1;
            }

            DWORD byte_to_lock = ((g_state->audio.sample_index * g_state->audio.bytes_per_sample) %
                                  g_state->audio.dsbuffer_size);
            DWORD bytes_per_frame = (int)((r32)(g_state->audio.sample_rate * g_state->audio.bytes_per_sample)
                                          / PL_AUDIO_THREAD_HZ);
            DWORD frame_boundary = play_cursor + bytes_per_frame;
            DWORD safe_write_cursor = write_cursor;
            if(safe_write_cursor < play_cursor) safe_write_cursor += g_state->audio.dsbuffer_size;
            safe_write_cursor += g_state->audio.margin_bytes;
            b32 low_latency = (safe_write_cursor < frame_boundary) ? 1:0;

            DWORD target_cursor = 0;
            if(low_latency)
            {
                target_cursor = (frame_boundary + bytes_per_frame);
            }
            else
            {
                target_cursor = (write_cursor + bytes_per_frame + g_state->audio.margin_bytes);
            }
            target_cursor = (target_cursor % g_state->audio.dsbuffer_size);

            DWORD bytes_to_write = 0;
            if(byte_to_lock > target_cursor)
            {
                bytes_to_write = (g_state->audio.dsbuffer_size - byte_to_lock);
                bytes_to_write += target_cursor;
            }
            else
            {
                bytes_to_write = target_cursor - byte_to_lock;
            }

            int sample_count = bytes_to_write / g_state->audio.bytes_per_sample;
            bytes_to_write = sample_count * g_state->audio.bytes_per_sample;

            //TODO: get samples

            PL_FillSoundBuffer(byte_to_lock, bytes_to_write, sample_count, samples);
        }
        else
        {
            sound_started = 0;
        }

        elapsed_ms = (PL_TimeElapsed(timer, PL_QueryTimer()) * 1000.0f);
        if((u32)elapsed_ms < target_ms)
        {
            Sleep((u32)(target_ms - elapsed_ms));
        }
    }

    return 0;
}

b32 PL_StartAudioThread(void)
{
    sThread *thread = &g_state->audio_thread;
    thread->handle = CreateThread(0, 0, AudioThread, 0, 0, &thread->id);
    if(!thread->handle) return 0;
    return 1;
}

b32 PL_QuitAudioThread(void)
{
    sThread *thread = &g_state->audio_thread;
    WaitForSingleObject(thread->handle, 0);
    DWORD ecode = 0;
    if(!GetExitCodeThread(thread->handle, &ecode)) return 0;

    int attempts = 100;
    while(ecode == 259 && attempts >= 0)
    {
        WaitForSingleObject(thread->handle, 15);
        if(!GetExitCodeThread(thread->handle, &ecode)) return 0;
    }

    if(ecode)
    {
        if(attempts < 0) return 0; //thread hanging
        else return 0; //error
    }

    CloseHandle(g_state->audio_thread.mutex);
    return 1;
}

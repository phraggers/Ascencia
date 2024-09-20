/*============================================================
 * Project: Ascencia
 * File: win32_audio.c
 * Author: Phraggers
 * Date: Sun Sep 15 2024
============================================================*/

#include <platform/audio.h>
#include <win32/win32_api.h>
#include <win32/win32_state.h>

/* xaudio2.h */

// Calculate the argument to SetVolume from a decibel value
float Win32_XAudio2DecibelsToAmplitudeRatio(float Decibels)
{
    return powf(10.0f, Decibels / 20.0f);
}

// Recover a volume in decibels from an amplitude factor
float XAudio2AmplitudeRatioToDecibels(float Volume)
{
    if (Volume == 0)
    {
        return -3.402823466e+38f; // Smallest float value (-FLT_MAX)
    }
    return 20.0f * log10f(Volume);
}

// Calculate the argument to SetFrequencyRatio from a semitone value
float XAudio2SemitonesToFrequencyRatio(float Semitones)
{
    // FrequencyRatio = 2 ^ Octaves
    //                = 2 ^ (Semitones / 12)
    return powf(2.0f, Semitones / 12.0f);
}

// Recover a pitch in semitones from a frequency ratio
float XAudio2FrequencyRatioToSemitones(float FrequencyRatio)
{
    // Semitones = 12 * log2(FrequencyRatio)
    //           = 12 * log2(10) * log10(FrequencyRatio)
    return 39.86313713864835f * log10f(FrequencyRatio);
}

// Convert from filter cutoff frequencies expressed in Hertz to the radian
// frequency values used in XAUDIO2_FILTER_PARAMETERS.Frequency, state-variable
// filter types only.  Use XAudio2CutoffFrequencyToOnePoleCoefficient() for one-pole filter types.
// Note that the highest CutoffFrequency supported is SampleRate/6.
// Higher values of CutoffFrequency will return XAUDIO2_MAX_FILTER_FREQUENCY.
float XAudio2CutoffFrequencyToRadians(float CutoffFrequency, u32 SampleRate)
{
    if ((u32)(CutoffFrequency * 6.0f) >= SampleRate)
    {
        return XAUDIO2_MAX_FILTER_FREQUENCY;
    }
    return 2.0f * sinf((float)pi32() * CutoffFrequency / (float)SampleRate);
}

// Convert from radian frequencies back to absolute frequencies in Hertz
float XAudio2RadiansToCutoffFrequency(float Radians, float SampleRate)
{
    return SampleRate * asinf(Radians / 2.0f) / (float)pi32();
}

// Convert from filter cutoff frequencies expressed in Hertz to the filter
// coefficients used with XAUDIO2_FILTER_PARAMETERS.Frequency,
// LowPassOnePoleFilter and HighPassOnePoleFilter filter types only.
// Use XAudio2CutoffFrequencyToRadians() for state-variable filter types.
float XAudio2CutoffFrequencyToOnePoleCoefficient(float CutoffFrequency, u32 SampleRate)
{
    if ((u32)CutoffFrequency >= SampleRate)
    {
        return XAUDIO2_MAX_FILTER_FREQUENCY;
    }
    return ( 1.0f - powf(1.0f - 2.0f * CutoffFrequency / (float)SampleRate, 2.0f) );
}
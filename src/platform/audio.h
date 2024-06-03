/*============================================================
 * Project: Ascencia
 * File: audio.h
 * Author: Phraggers
 * Date: Mon May 13 2024
============================================================*/

#ifdef ASC_HEAD

#include <sndfile.h>
#include <AL/alc.h>
#include <AL/al.h>

struct ASC_AudioSource
{
    ALuint id;
    ALuint buffer;
    r32 pitch, gain;
    r32 position[3];
    r32 velocity[3];
    bool loop;
};

struct ASC_Audio
{
    ALCdevice *alcdevice;
    ALCcontext *alccontext;
    ALuint sfxBuffers[AUDIO_SFX_BUFFERS];
    ASC_AudioSource sources[AUDIO_SFX_BUFFERS];
};

static bool ASC_AudioInit(void);

/** @returns index to sfxBuffers array */
static u32 ASC_AudioAddSFX(ASC_VirtualFileHandle *file);

/** @param sfxID: index into sfxBuffers array 
 * @returns success */
static bool ASC_AudioRemoveSFX(u32 sfxID);

static bool ASC_AudioAddSource(void);
static bool ASC_AudioRemoveSource(u32 ID);

static void ASC_AudioPlaySource(u32 ID);

// virtual io functions
sf_count_t _VIO_get_filelen(void* user_data);
sf_count_t _VIO_seek(sf_count_t offset, int whence, void* user_data);
sf_count_t _VIO_read(void* sfptr, sf_count_t count, void* user_data);
sf_count_t _VIO_write(const void* sfptr, sf_count_t count, void* user_data);
sf_count_t _VIO_tell(void* user_data);

#endif

/*====================================================================================*/

#ifdef ASC_IMPL

static bool ASC_AudioInit(void)
{
    ASC_Audio *audio = &gApp->audio;

    audio->alcdevice = alcOpenDevice(0);
    if(!audio->alcdevice)
    {
        ASC_Error("ASC_AudioInit: Failed to open audio device");
        return 0;
    }

    audio->alccontext = alcCreateContext(audio->alcdevice, 0);
    if(!audio->alccontext)
    {
        ASC_Error("ASC_AudioInit: Failed to create audio context");
        return 0;
    }

    if(!alcMakeContextCurrent(audio->alccontext))
    {
        ASC_Error("ASC_AudioInit: Failed to set audio context");
        return 0;
    }

    const ALCchar *name = 0;
    
    if(alcIsExtensionPresent(audio->alcdevice, "ALC_ENUMERATE_ALL_EXT"))
    {
        name = alcGetString(audio->alcdevice, ALC_ALL_DEVICES_SPECIFIER);
    }
    
    if(!name || alcGetError(audio->alcdevice) != ALC_NO_ERROR)
    {
        name = alcGetString(audio->alcdevice, ALC_DEVICE_SPECIFIER);
    }

    ASC_InfoLog("ASC_AudioInit: Opened device: %s", name);

    return 1;
}

static u32 ASC_AudioAddSFX(ASC_VirtualFileHandle *file)
{
    ASC_Audio *audio = &gApp->audio;

    u32 result = 0;
    alGetError(); // clear any existing error
    bool slotFound = 0;

    for(u32 sfxBufferIndex = 0;
    sfxBufferIndex < AUDIO_SFX_BUFFERS;
    sfxBufferIndex++)
    {
        if(audio->sfxBuffers[sfxBufferIndex] == 0)
        {
            alGenBuffers(1, &audio->sfxBuffers[sfxBufferIndex]);

            if(alGetError() == AL_INVALID_VALUE)
            {
                ASC_Error("ASC_AudioAddSFX: Error: buffer array isn't large enough to hold the number of buffers requested.");
                return 0;
            }

            if(alGetError() == AL_OUT_OF_MEMORY)
            {
                ASC_Error("ASC_AudioAddSFX: Error: there is not enough memory available to generate all the buffers requested.");
                return 0;
            }

            result = sfxBufferIndex;
            slotFound = 1;
            break;
        }
    }

    if(!slotFound)
    {
        ASC_Error("ASC_AudioAddSFX: Error: SFXBuffer array is full.");
        return 0;
    }

    //TODO CHECK all the stuff, this is unchanged copy paste

    SF_VIRTUAL_IO vio = {0};
    vio.get_filelen = _VIO_get_filelen;
    vio.read = _VIO_read;
    vio.seek = _VIO_seek;
    vio.tell = _VIO_tell;
    vio.write = _VIO_write;
    SF_INFO sfinfo = {0};
    file->position = 0;
    SNDFILE *sndfile = 0;
    sndfile = sf_open_virtual(&vio, SFM_READ, &sfinfo, (ptr)file);

    ALenum err, format;
	ALuint buffer;
	short* membuf;
	sf_count_t num_frames;
	ALsizei num_bytes;

    if(!sndfile)
    {
        ASC_Error("ASC_AudioAddSFX: failed to open audio: %s", sf_strerror(sndfile));
        return 0;
    }

    if(sfinfo.frames < 1)
    {
        ASC_Error("ASC_AudioAddSFX: Bad sample count");
        sf_close(sndfile);
        return 0;
    }

    /* Get the sound format, and figure out the OpenAL format */
	format = AL_NONE;
	if (sfinfo.channels == 1)
		format = AL_FORMAT_MONO16;
	else if (sfinfo.channels == 2)
		format = AL_FORMAT_STEREO16;
	if (!format)
	{
		ASC_Error("ASC_AudioAddSFX: Unsupported channel count: %d", sfinfo.channels);
		sf_close(sndfile);
		return 0;
	}

	/* Decode the whole audio file to a buffer. */
	membuf = static_cast<short*>(malloc((size_t)(sfinfo.frames * sfinfo.channels) * sizeof(short)));

	num_frames = sf_readf_short(sndfile, membuf, sfinfo.frames);
	if (num_frames < 1)
	{
		free(membuf);
		sf_close(sndfile);
		ASC_Error("ASC_AudioAddSFX: Failed to read samples");
		return 0;
	}
	num_bytes = (ALsizei)(num_frames * sfinfo.channels) * (ALsizei)sizeof(short);

	/* Buffer the audio data into a new buffer object, then free the data and
	 * close the file.
	 */
	buffer = 0;
	alGenBuffers(1, &buffer);
	alBufferData(buffer, format, membuf, num_bytes, sfinfo.samplerate);

	free(membuf);
	sf_close(sndfile);

	/* Check if an error occured, and clean up if so. */
	err = alGetError();
	if (err != AL_NO_ERROR)
	{
		ASC_Error("OpenAL Error: %s", alGetString(err));
		if (buffer && alIsBuffer(buffer))
			alDeleteBuffers(1, &buffer);
		return 0;
	}

	ASC_InfoLog("ASC_AudioAddSFX: SFX[%d] loaded", result);
    return result;
}

static bool ASC_AudioRemoveSFX(u32 sfxID)
{
    ASC_Audio *audio = &gApp->audio;
    alDeleteBuffers(1, &audio->sfxBuffers[sfxID]);
    audio->sfxBuffers[sfxID] = 0;
    return 1;
}

static bool ASC_AudioAddSource(void)
{
    //TODO get free slot ....

//TODO rename all the stuff, this is unchanged copy paste
    // TODO rename/link to slot
    /*
    pitch = 1.f;
	gain = 1.f;
	Position[3] = { 0,0,0 };
	Velocity[3] = { 0,0,0 };
	LoopSound = false;
	buffer = 0;

    alGenSources(1, &p_Source);
	alSourcef(p_Source, AL_PITCH, p_Pitch);
	alSourcef(p_Source, AL_GAIN, p_Gain);
	alSource3f(p_Source, AL_POSITION, p_Position[0], p_Position[1], p_Position[2]);
	alSource3f(p_Source, AL_VELOCITY, p_Velocity[0], p_Velocity[1], p_Velocity[2]);
	alSourcei(p_Source, AL_LOOPING, p_LoopSound);
	alSourcei(p_Source, AL_BUFFER, p_Buffer);
    */
   return 1;
}

static bool ASC_AudioRemoveSource(u32 ID)
{
    ASC_Audio *audio = &gApp->audio;
    
    //TODO check this
    //TODO rename all the stuff, this is unchanged copy paste
    alDeleteSources(1, &audio->sources[ID].id);
    return 1;
}

static void ASC_AudioPlaySource(u32 ID)
{
    /*
    //TODO rename all the stuff, this is unchanged copy paste
	if (ID != p_Buffer)
	{
			p_Buffer = ID;
			alSourcei(p_Source, AL_BUFFER, (ALint)p_Buffer);
	}

	alSourcePlay(p_Source);


	ALint state = AL_PLAYING;
	std::cout << "playing sound";
	while (state == AL_PLAYING && alGetError() == AL_NO_ERROR)
	{
		std::cout << "currently playing sound";
		alGetSourcei(p_Source, AL_SOURCE_STATE, &state);
	}
	std::cout << "done playing sound";
    */
}

/**-------------------------------------------
 * libSndFile Virtual IO Function Definitions
 ---------------------------------------------*/

sf_count_t _VIO_get_filelen(void* user_data)
{
    ASC_VirtualFileHandle *filehandle = (ASC_VirtualFileHandle*)user_data;
    return (sf_count_t)filehandle->size;
}

sf_count_t _VIO_seek(sf_count_t offset, int whence, void* user_data)
{
    ASC_VirtualFileHandle *filehandle = (ASC_VirtualFileHandle*)user_data;

    switch(whence)
    {
        case SEEK_SET:
        {
            filehandle->position = (u64)offset;
        } break;

        case SEEK_CUR:
        {
            filehandle->position += (u64)offset;
        } break;

        case SEEK_END:
        {
            filehandle->position =  filehandle->size + (u64)offset;
        } break;

        default: 
        {
            return -1;
        }
    }

    return (sf_count_t)filehandle->position;
}

sf_count_t _VIO_read(void* sfptr, sf_count_t count, void* user_data)
{
    ASC_VirtualFileHandle *filehandle = (ASC_VirtualFileHandle*)user_data;
    sf_count_t bytesRemaining = (sf_count_t)(filehandle->size - filehandle->position);
    sf_count_t bytesToRead = (bytesRemaining < count) ? bytesRemaining : count;

    const ptr srcptr = (const ptr)((u64)filehandle->data + (u64)filehandle->position);
    memcpy(sfptr, srcptr, (size_t)bytesToRead);

    filehandle->position += (u64)bytesToRead;

    return bytesToRead;
}

sf_count_t _VIO_write(const void* ptr, sf_count_t count, void* user_data)
{
    //TODO for writing (not needed while we are just reading)
    return 0;
}

sf_count_t _VIO_tell(void* user_data)
{
    ASC_VirtualFileHandle *filehandle = (ASC_VirtualFileHandle*)user_data;
    return (sf_count_t)filehandle->position;
}

#endif //AUDIO_H_
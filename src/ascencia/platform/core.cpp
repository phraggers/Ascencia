// Ascencia
// platform/core.cpp

#include <ascencia/platform/core.h>

bool cCore::Init(int argc, char** argv)
{
	memset(&Core->State, 0, sizeof(sCoreState));
	strcpy(State.AppName, CFG_APP_NAME);
	strcpy(State.AppOrg, CFG_APP_ORG);
	State.Version.DevStage = CFG_APP_VER_DEVSTAGE;
	State.Version.Major = CFG_APP_VER_MAJ;
	State.Version.Minor = CFG_APP_VER_MIN;

	{
		std::stringstream ss;
		ss << State.AppName << " [" << CFG_DevStageToString(State.Version.DevStage)
			<< " " << State.Version.Major << "." << State.Version.Minor << "]";
		LOG_PRINT("Core::Init", ss.str());
	}

	if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_GAMEPAD | SDL_INIT_EVENTS))
	{
		LOG_FATAL("Core::Init", std::string("SDL Init failed: ", SDL_GetError()));
		return 0;
	}
	else
	{
		std::stringstream ss;
		ss << "SDL "
			<< SDL_VERSIONNUM_MAJOR(SDL_VERSION) << "."
			<< SDL_VERSIONNUM_MINOR(SDL_VERSION) << "."
			<< SDL_VERSIONNUM_MICRO(SDL_VERSION) << " Loaded";
		LOG_PRINT("Core::Init", ss.str());
	}

	if (!Timer.Init())
	{
		return 0;
	}

	if (!Log.Init())
	{
		return 0;
	}

	if (!Config.Init())
	{
		return 0;
	}

	if (!SDLNet_Init())
	{
		LOG_FATAL("Core::Init", std::string("SDLNet Init failed: ", SDL_GetError()));
		return 0;
	}
	else
	{
		std::stringstream ss;
		ss << "SDLNet "
			<< SDL_VERSIONNUM_MAJOR(SDL_NET_VERSION) << "."
			<< SDL_VERSIONNUM_MINOR(SDL_NET_VERSION) << "."
			<< SDL_VERSIONNUM_MICRO(SDL_NET_VERSION) << " Loaded";
		LOG_PRINT("Core::Init", ss.str());
	}

	if (!PHYSFS_init(argv[0]))
	{
		LOG_FATAL("Core::Init", std::string("PHYSFS failed to initialize: ", PHYSFS_getLastError()));
		return 0;
	}
	else
	{
		std::stringstream ss;
		ss << "PHYSFS " << PHYSFS_VER_MAJOR << "." << PHYSFS_VER_MINOR << "." << PHYSFS_VER_PATCH << " loaded";
		LOG_PRINT("Core::Init", ss.str());
	}

	if (!TTF_Init())
	{
		LOG_FATAL("Core::Init", std::string("SDL_ttf Init failed: ", SDL_GetError()));
		return 0;
	}
	else
	{
		std::stringstream ss;
		ss << "SDL_ttf "
			<< SDL_VERSIONNUM_MAJOR(SDL_TTF_VERSION) << "."
			<< SDL_VERSIONNUM_MINOR(SDL_TTF_VERSION) << "."
			<< SDL_VERSIONNUM_MICRO(SDL_TTF_VERSION) << " Loaded";
		LOG_PRINT("Core::Init", ss.str());
	}

	if (!Window.Init())
	{
		return 0;
	}

	// Temp: test OpenAL
	{
		// Open the default audio device
		ALCdevice* AudioDevice = alcOpenDevice(0);
		if (!AudioDevice)
		{
			LOG_FATAL("Core::Init", "OpenAL failed to open device");
			return 0;
		}

		// Create a context
		ALCcontext* ALContext = alcCreateContext(AudioDevice, 0);
		if (!ALContext)
		{
			LOG_FATAL("Core::Init", "OpenAL failed to create context");
			return 0;
		}

		// Make the context current
		if (!alcMakeContextCurrent(ALContext))
		{
			LOG_FATAL("Core::Init", "OpenAL failed to make context current");
			return 0;
		}

		// Print OpenAL version
		const char* OpenALVersion = (const char*)alGetString(AL_VERSION);
		std::stringstream ss;
		ss << "OpenAL: " << OpenALVersion << " loaded";
		LOG_PRINT("Core::Init", ss.str());

		// Cleanup
		alcDestroyContext(ALContext);
		alcCloseDevice(AudioDevice);
	}

	{
		std::stringstream ss;
		ss << "Startup : " << NANOSECONDS_TO_SECONDS((f32)SDL_GetTicksNS()) << " seconds";
		LOG_INFO("Core::Init", ss.str());
	}

	return 1;
}

bool cCore::Run(void)
{
#if 0
	//PHYSFS TEST
	std::string main_dat_path = std::string(DATA_PATH) + "main.dat";
	if (!PHYSFS_mount(main_dat_path.c_str(), 0, 0))
	{
		std::cerr << "PHYSFS: error loading " << main_dat_path << " : " << PHYSFS_getLastError() << std::endl;
	}
	else
	{
		std::cout << "PHYSFS: Loaded " << main_dat_path << std::endl;
		char** rc = PHYSFS_enumerateFiles("/");
		char** i;
		for (i = rc; *i != 0; i++)
		{
			PHYSFS_file* file = PHYSFS_openRead(*i);
			PHYSFS_sint64 file_size = PHYSFS_fileLength(file);
			std::cout << " * PHYSFS file: " << *i << " : " << file_size << " bytes" << std::endl;
			char* file_buffer = new char[file_size];
			int file_read = PHYSFS_read(file, file_buffer, 1, file_size);
			// do file stuff...
			delete[] file_buffer;
			PHYSFS_close(file);
		}
		PHYSFS_freeList(rc);
		std::cout << "Listing done" << std::endl;
	}
#endif

	SDL_Event Event;
	State.Running = 1;
	while (State.Running)
	{
		SDL_PollEvent(&Event);
		if (Event.type == SDL_EVENT_QUIT)
		{
			State.Running = 0;
		}

		if (Event.type == SDL_EVENT_KEY_UP)
		{
			if (Event.key.scancode == SDL_SCANCODE_F11)
			{
				Window.ToggleFullscreen();
			}
		}

		Window.Frame();
	}

	return 1;
}

void cCore::Quit(void)
{
	Config.Save();
	Window.Quit();
	TTF_Quit();
	SDLNet_Quit();
	SDL_Quit();
	PHYSFS_deinit();
	LOG_PRINT("Core::Quit", "Quitting");

#if(BUILD_DEBUG==1)
	LOG_PRINT("Core::Quit", "Log Print Test");
	LOG_DEBUG("Core::Quit", "Log Debug Test");
	LOG_INFO("Core::Quit", "Log Info Test");
	LOG_WARNING("Core::Quit", "Log Warning Test");
	LOG_ERROR("Core::Quit", "Log Error Test");
	LOG_FATAL("Core::Quit", "Log Fatal Test");
#endif

	Log.Quit();
}

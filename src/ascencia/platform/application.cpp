// Ascencia
// ascencia/platform/application.cpp

#include <ascencia/platform/application.h>

bool sApplication::Init(int argc, char** argv)
{
	memset(&App->State, 0, sizeof(sApplicationState));
	strcpy(State.AppName, CFG_APP_NAME);
	strcpy(State.AppOrg, CFG_APP_ORG);
	State.Version.DevStage = CFG_APP_VER_DEVSTAGE;
	State.Version.Major = CFG_APP_VER_MAJ;
	State.Version.Minor = CFG_APP_VER_MIN;

	std::cout << State.AppName << " [" << CFG_DevStageToString(State.Version.DevStage)
		<< " " << State.Version.Major << "." << State.Version.Minor << "]" << std::endl;

	if (!SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_GAMEPAD | SDL_INIT_EVENTS))
	{
		std::cerr << "SDL init failed : " << SDL_GetError() << std::endl;
		return 0;
	}
	else
	{
		std::cout << "SDL "
			<< SDL_VERSIONNUM_MAJOR(SDL_VERSION) << "."
			<< SDL_VERSIONNUM_MINOR(SDL_VERSION) << "."
			<< SDL_VERSIONNUM_MICRO(SDL_VERSION) << " Loaded"
			<< std::endl;
	}

	if (!Timer.Init())
	{
		return 0;
	}

	if (!Config.Init())
	{
		return 0;
	}

	if (!SDLNet_Init())
	{
		std::cerr << "SDLNet init failed : " << SDL_GetError() << std::endl;
		return 0;
	}
	else
	{
		std::cout << "SDLNet "
			<< SDL_VERSIONNUM_MAJOR(SDL_NET_VERSION) << "."
			<< SDL_VERSIONNUM_MINOR(SDL_NET_VERSION) << "."
			<< SDL_VERSIONNUM_MICRO(SDL_NET_VERSION) << " Loaded"
			<< std::endl;
	}

	if (!PHYSFS_init(argv[0]))
	{
		std::cerr << "PHYSFS failed to initialize : " << PHYSFS_getLastError() << std::endl;
		return 0;
	}
	else
	{
		std::cout << "PHYSFS " << PHYSFS_VER_MAJOR << "." << PHYSFS_VER_MINOR << "." << PHYSFS_VER_PATCH << " loaded" << std::endl;
	}

	if (!TTF_Init())
	{
		std::cerr << "SDL_ttf init failed : " << SDL_GetError() << std::endl;
		return 0;
	}
	else
	{
		std::cout << "SDL_ttf "
			<< SDL_VERSIONNUM_MAJOR(SDL_TTF_VERSION) << "."
			<< SDL_VERSIONNUM_MINOR(SDL_TTF_VERSION) << "."
			<< SDL_VERSIONNUM_MICRO(SDL_TTF_VERSION) << " Loaded"
			<< std::endl;
	}

	if (!Window.Init())
	{
		return 0;
	}

	std::cout << "[APP] Startup : " << NANOSECONDS_TO_SECONDS((f32)SDL_GetTicksNS()) << " seconds" << std::endl;

	return 1;
}

bool sApplication::Run(void)
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

void sApplication::Quit(void)
{
	Config.Save();
	SDL_DestroyWindow(Window.Handle);
	TTF_Quit();
	SDLNet_Quit();
	SDL_Quit();
	PHYSFS_deinit();
}

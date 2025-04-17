// Ascencia
// ascencia/platform/window.cpp

#include <ascencia/platform/application.h>

static void Window_SetIcon(void)
{
	//TODO wrap this in a nicer API
	std::string MainDatPath = std::string(DATA_PATH) + "main.dat";
	if (!PHYSFS_mount(MainDatPath.c_str(), 0, 0))
	{
		std::cerr << "PHYSFS: error loading " << MainDatPath << " : " << PHYSFS_getLastError() << std::endl;
	}
	else
	{
		PHYSFS_file* file = PHYSFS_openRead("Icon.png");
		PHYSFS_sint64 file_size = PHYSFS_fileLength(file);
		char* file_buffer = new char[file_size];
		int file_read = PHYSFS_read(file, file_buffer, 1, file_size);

		SDL_IOStream* IconFileSDLIO = SDL_IOFromMem(file_buffer, file_size);
		SDL_Surface* Icon = IMG_Load_IO(IconFileSDLIO, 1);
		SDL_SetWindowIcon(App->Window.Handle, Icon);

		delete[] file_buffer;
		PHYSFS_close(file);
		PHYSFS_unmount(MainDatPath.c_str());
	}
}

static void Window_SetDefaultTitle(void)
{
	std::stringstream ss;
	ss << App->State.AppName << " [" << CFG_DevStageToString(App->State.Version.DevStage)
		<< " " << App->State.Version.Major << "." << App->State.Version.Minor << "]";
	App->Window.Title = ss.str();
}

sWindow::sWindow()
{
	memset(&State, 0, sizeof(sWindowState));
	Handle = 0;
	GLRC = 0;
	memset(&Dimension, 0, sizeof(SDL_Rect));
}

bool sWindow::Init(void)
{
	Window_SetIcon();
	Window_SetDefaultTitle();

	if (!SDL_GL_LoadLibrary(0))
	{
		std::cerr << "SDL GL failed : " << SDL_GetError() << std::endl;
		return 0;
	}
	else
	{
		if (!SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1)) std::cerr << "SDLGL Error : " << SDL_GetError() << std::endl;
		if (!SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE)) std::cerr << "SDLGL Error : " << SDL_GetError() << std::endl;
		if (!SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, CFG_WINDOW_GL_VERSION_MAJ)) std::cerr << "SDLGL Error : " << SDL_GetError() << std::endl;
		if (!SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, CFG_WINDOW_GL_VERSION_MIN)) std::cerr << "SDLGL Error : " << SDL_GetError() << std::endl;
		if (!SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1)) std::cerr << "SDLGL Error : " << SDL_GetError() << std::endl;
		if (!SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24)) std::cerr << "SDLGL Error : " << SDL_GetError() << std::endl;
	}

	Handle = SDL_CreateWindow(Title.c_str(), CFG_WINDOW_W, CFG_WINDOW_H, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	if (!Handle)
	{
		std::cerr << "Failed to create SDL window! " << SDL_GetError() << std::endl;
		return 0;
	}
	SDL_SetWindowPosition(Handle, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

	GLRC = SDL_GL_CreateContext(Handle);
	if (!GLRC)
	{
		std::cerr << "Failed to create GLContext : " << SDL_GetError() << std::endl;
		return 0;
	}
	
	gladLoadGL(SDL_GL_GetProcAddress);
	SDL_GL_SetSwapInterval(CFG_WINDOW_VSYNC);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glViewport(0, 0, CFG_WINDOW_W, CFG_WINDOW_H);

	std::cout << "GLAD loaded GL " << CFG_WINDOW_GL_VERSION_MAJ << "." << CFG_WINDOW_GL_VERSION_MIN << std::endl;
	std::cout << "" << "OpenGL Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "" << "OpenGL Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "" << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "" << "OpenGL Shading Language Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

	if (App->Config.IsConfigNew)
	{
		UpdateState();
		App->Config.Save();
	}
	else
	{
		ApplyState();
		UpdateState();
	}

	return 1;
}

bool sWindow::Frame(void)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	SDL_GL_SwapWindow(Handle);
	UpdateState();

	return 1;
}

void sWindow::SetFullscreen(bool Fullscreen)
{
	SDL_WindowFlags Flags = SDL_GetWindowFlags(Handle);
	
	if (Fullscreen)
	{
		if (Flags & SDL_WINDOW_FULLSCREEN)
		{
			UpdateState();
		}
		else
		{
			UpdateState();
			ToggleFullscreen();
		}
	}
	else
	{
		if (Flags & SDL_WINDOW_FULLSCREEN)
		{
			UpdateState();
			ToggleFullscreen();
		}
		else
		{
			UpdateState();
		}
	}
}

void sWindow::ToggleFullscreen(void)
{
	if (State.IsFullscreen)
	{
		State.IsFullscreen = 0;
		SDL_SetWindowFullscreen(Handle, 0);
		SDL_SetWindowSize(Handle, State.WindowedWH[0], State.WindowedWH[1]);
		SDL_SetWindowPosition(Handle, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
	}
	else
	{
		State.IsFullscreen = 1;
		SDL_GetWindowSize(Handle, &State.WindowedWH[0], &State.WindowedWH[1]);
		SDL_SetWindowFullscreen(Handle, 1);
	}
}

void sWindow::ApplyState(void)
{
	if (State.VSync)
	{
		SDL_GL_SetSwapInterval(1);
	}
	else
	{
		SDL_GL_SetSwapInterval(0);
	}

	if (State.IsFullscreen)
	{
		SetFullscreen(1);
	}
	else
	{
		int x = State.WindowedWH[0];
		int y = State.WindowedWH[1];
		SetFullscreen(0);
		SDL_SetWindowSize(Handle, x, y);
		UpdateState();
	}
}

void sWindow::UpdateState(void)
{
	SDL_GetWindowPosition(Handle, &Dimension.x, &Dimension.y);
	SDL_GetWindowSize(Handle, &Dimension.w, &Dimension.h);

	SDL_WindowFlags Flags = SDL_GetWindowFlags(Handle);

	if (Flags & SDL_WINDOW_FULLSCREEN)
	{
		State.IsFullscreen = 1;
	}
	else
	{
		State.IsFullscreen = 0;
		State.WindowedWH[0] = Dimension.w;
		State.WindowedWH[1] = Dimension.h;
	}

	if (Flags & SDL_WINDOW_MAXIMIZED)
	{
		State.IsMaximized = 1;
	}
	else
	{
		State.IsMaximized = 0;
	}

	if (Flags & SDL_WINDOW_MINIMIZED)
	{
		State.IsMinimized = 1;
	}
	else
	{
		State.IsMinimized = 0;
	}

	if (Flags & SDL_WINDOW_INPUT_FOCUS)
	{
		State.IsFocussed = 1;
	}
	else
	{
		State.IsFocussed = 0;
	}
}

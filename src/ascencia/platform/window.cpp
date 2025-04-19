// Ascencia
// platform/window.cpp

#include <ascencia/platform/core.h>

static void Window_SetIcon(void)
{
	//TODO wrap this in a nicer API
	std::string MainDatPath = std::string(DATA_PATH) + "main.dat";
	if (!PHYSFS_mount(MainDatPath.c_str(), 0, 0))
	{
		std::stringstream ss;
		ss << "PHYSFS: error loading " << MainDatPath << " : " << PHYSFS_getLastError();
		LOG_ERROR("Window::SetIcon", ss.str());
	}
	else
	{
		PHYSFS_file* file = PHYSFS_openRead("Icon.png");
		PHYSFS_sint64 file_size = PHYSFS_fileLength(file);
		char* file_buffer = new char[file_size];
		int file_read = PHYSFS_read(file, file_buffer, 1, file_size);

		SDL_IOStream* IconFileSDLIO = SDL_IOFromMem(file_buffer, file_size);
		SDL_Surface* Icon = IMG_Load_IO(IconFileSDLIO, 1);
		SDL_SetWindowIcon(Core->Window.Handle, Icon);

		delete[] file_buffer;
		PHYSFS_close(file);
		PHYSFS_unmount(MainDatPath.c_str());
	}
}

static void Window_SetDefaultTitle(void)
{
	std::stringstream ss;
	ss << Core->State.AppName << " [" << CFG_DevStageToString(Core->State.Version.DevStage)
		<< " " << Core->State.Version.Major << "." << Core->State.Version.Minor << "]";
	Core->Window.Title = ss.str();
}

cWindow::cWindow()
{
	memset(&State, 0, sizeof(sWindowState));
	Handle = 0;
	memset(&Dimension, 0, sizeof(SDL_Rect));
}

bool cWindow::Init(void)
{
	Window_SetIcon();
	Window_SetDefaultTitle();
	Renderer.Setup();

	Handle = SDL_CreateWindow(Title.c_str(), CFG_WINDOW_W, CFG_WINDOW_H, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	if (!Handle)
	{
		LOG_FATAL("Window::Init", std::string("SDL CreateWindow Error: ", SDL_GetError()));
		return 0;
	}
	
	SDL_SetWindowPosition(Handle, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
	Renderer.Start();

	if (Core->Config.IsConfigNew)
	{
		UpdateState();
		Core->Config.Save();
	}
	else
	{
		ApplyState();
		UpdateState();
	}

	return 1;
}

void cWindow::Quit(void)
{
	Renderer.Quit();
	SDL_DestroyWindow(Handle);
}

bool cWindow::Frame(void)
{
	UpdateState();

	//TODO implement proper timer
	SDL_Delay(17);

	return 1;
}

void cWindow::SetFullscreen(bool Fullscreen)
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

void cWindow::ToggleFullscreen(void)
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

void cWindow::ApplyState(void)
{
	if (State.VSync)
	{
		Renderer.SetVSync(1);
	}
	else
	{
		Renderer.SetVSync(0);
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

void cWindow::UpdateState(void)
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

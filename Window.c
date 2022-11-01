
#include "Ascencia.h"

bool Window_Init()
{
	SDL_snprintf(State->Window.Title, 64, "%s [%d.%d.%d]", State->AppName, State->AppVersion.major,
				 State->AppVersion.minor, State->AppVersion.patch);

	State->Window.Handle = SDL_CreateWindow(State->Window.Title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
											State->Window.Dimensions.w, State->Window.Dimensions.h,
											SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN |
											SDL_WINDOW_RESIZABLE);

	if (!State->Window.Handle)
	{
		ASC_Log(LOGLEVEL_FATAL, "WINDOW: Failed to create window: %s", SDL_GetError());
		return 0;
	}

	ASC_Log(LOGLEVEL_INFO, "WINDOW: Window Created");

	State->Window.GLContext = SDL_GL_CreateContext(State->Window.Handle);

	if (!State->Window.GLContext)
	{
		ASC_Log(LOGLEVEL_FATAL, "WINDOW: Failed to create GLContext: %s", SDL_GetError());
		return 0;
	}

	ASC_Log(LOGLEVEL_INFO, "WINDOW: GLContext Created");

	SDL_ShowCursor(SDL_DISABLE);
	State->Window.MouseShown = 0;

	return 1;
}

void Window_Resize()
{
	SDL_Rect dim = { 0 };
	SDL_GetWindowSize(State->Window.Handle, &dim.w, &dim.h);
	SDL_GetWindowPosition(State->Window.Handle, &dim.x, &dim.y);

	if (dim.w != State->Window.Dimensions.w ||
		dim.h != State->Window.Dimensions.h ||
		dim.x != State->Window.Dimensions.x ||
		dim.y != State->Window.Dimensions.y)
	{
		ASC_Log(LOGLEVEL_DEBUG, "WINDOW: Resize X[%d] Y[%d] W[%d] H[%d]", dim.x, dim.y, dim.w, dim.h);
		
		State->Window.Dimensions.w = dim.w;
		State->Window.Dimensions.h = dim.h;
		State->Window.Dimensions.x = dim.x;
		State->Window.Dimensions.y = dim.y;

		glViewport(0, 0, State->Window.Dimensions.w, State->Window.Dimensions.h);
	}	
}

bool Window_ToggleFullscreen()
{
	if (!State->Window.Fullscreen)
	{
		SDL_SetWindowFullscreen(State->Window.Handle, SDL_WINDOW_FULLSCREEN_DESKTOP);
		State->Window.Fullscreen = 1;
		ASC_Log(LOGLEVEL_DEBUG, "WINDOW: Fullscreen[%d]", State->Window.Fullscreen);
	}

	else
	{
		SDL_SetWindowFullscreen(State->Window.Handle, 0);
		State->Window.Fullscreen = 0;
		ASC_Log(LOGLEVEL_DEBUG, "WINDOW: Fullscreen[%d]", State->Window.Fullscreen);
	}

	return State->Window.Fullscreen;
}

bool Window_Screenshot(char* _Path)
{
	SDL_Surface* image = SDL_CreateRGBSurface(SDL_SWSURFACE, State->Window.Dimensions.w,
											  State->Window.Dimensions.h, 24, 0x000000FF,
											  0x0000FF00, 0x00FF0000, 0);

	glReadPixels(0, 0, State->Window.Dimensions.w, State->Window.Dimensions.h,
				 GL_RGB, GL_UNSIGNED_BYTE, image->pixels);

	char sshotpath[128] = { 0 };
	{
		char sshotdir[128] = { 0 };
		char datestr[128] = { 0 };
		SDL_snprintf(sshotdir, 128, "%sScreenshots", State->PrefPath);
		if(!Util_DirExists(sshotdir)) Util_MkDir(sshotdir);
		ASC_Log(LOGLEVEL_DEBUG, "Screenshots Dir [%s]", sshotdir);
		Util_TimeFormat(datestr, "%yyy-%M-%D_%h-%m-%s_Screenshot.bmp");
		SDL_snprintf(sshotpath, 128, "%sScreenshots/%s", State->PrefPath, datestr);
	}

	SDL_LockSurface(image);

	int pitch = image->pitch; // row size
	ASC_AllocArray(char, pitch, temp); // intermediate buffer
	char* pixels = (char*)image->pixels;

	for (int i = 0; i < image->h / 2; ++i)
	{
		// get pointers to the two rows to swap
		char* row1 = pixels + i * pitch;
		char* row2 = pixels + (image->h - i - 1) * pitch;

		// swap rows
		SDL_memcpy(temp, row1, pitch);
		SDL_memcpy(row1, row2, pitch);
		SDL_memcpy(row2, temp, pitch);
	}

	ASC_Free(temp);

	SDL_UnlockSurface(image);
	SDL_SaveBMP(image, sshotpath);
	SDL_FreeSurface(image);

	if(_Path) SDL_snprintf(_Path, 128, sshotpath);
	ASC_Log(LOGLEVEL_INFO, "WINDOW: Screenshot saved [%s]", sshotpath);

	return 0;
}

bool Window_ToggleMouse()
{
	if (!State->Window.MouseShown)
	{
		SDL_ShowCursor(SDL_ENABLE);
		State->Window.MouseShown = 1;
		return 1;
	}

	else
	{
		SDL_ShowCursor(SDL_DISABLE);
		State->Window.MouseShown = 0;
		return 0;
	}
}
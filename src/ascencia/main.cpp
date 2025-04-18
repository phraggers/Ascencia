// Ascencia
// ascencia/main.cpp

/*

Tohdoh List

libcurl
SDL_mixer OR OpenAL OR SDL_sound, 
framerate timer for non-vsync
threads
Logging module
Input (keyboard, mouse, controller)
Asset manager
Audio
OpenGL
GameState
Intro Splash & Main Menu

*/

#define ASC_IMPLEMENTATION
#include <ascencia/platform/application.h>
#include <SDL3/SDL_main.h>

bool ASC_Init(int argc, char** argv)
{
	App = new sApplication;
	if (!App)
	{
		std::cerr << "App: FATAL allocation error" << std::endl;
		return 0;
	}

	if (!App->Init(argc, argv))
	{
		return 0;
	}

	return 1;
}

bool ASC_Run(void)
{
	if (!App->Run())
	{
		return 0;
	}

	return 1;
}

void ASC_Quit(void)
{
	App->Quit();
}

int main(int argc, char** argv)
{
	if (!ASC_Init(argc, argv)) return -1;
	if (!ASC_Run()) return -1;
	ASC_Quit();
	return 0;
}
// Ascencia
// main.cpp

/*

Tohdoh List

framerate timer for non-vsync
threads
Input (keyboard, mouse, controller)
Asset manager
Audio
OpenGL
GameState
Intro Splash & Main Menu

*/

#define ASC_IMPLEMENTATION
#include <ascencia/platform/core.h>
#include <SDL3/SDL_main.h>

bool ASC_Init(int argc, char** argv)
{
	Core = new cCore;
	if (!Core)
	{
		std::cerr << "FATAL allocation error" << std::endl;
		return 0;
	}

	if (!Core->Init(argc, argv))
	{
		return 0;
	}

	return 1;
}

bool ASC_Run(void)
{
	if (!Core->Run())
	{
		return 0;
	}

	return 1;
}

void ASC_Quit(void)
{
	Core->Quit();
}

int main(int argc, char** argv)
{
	if (!ASC_Init(argc, argv)) return -1;
	if (!ASC_Run()) return -1;
	ASC_Quit();
	return 0;
}

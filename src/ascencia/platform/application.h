// Ascencia
// ascencia/platform/application.h

#pragma once
#include <shared/types.h>
#include <shared/stdlibs.h>

#include <glad/glad.h>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <glm/glm.hpp>
#include <physfs.h>

#include <ascencia/util/helper_defs.h>
#include <ascencia/platform/window.h>
#include <ascencia/platform/timer.h>
#include <ascencia/platform/config.h>

struct sApplicationState
{
	bool Running;
	char AppName[0xff];
	char AppOrg[0xff];
	sAppVersion Version;
};

struct sApplication
{
	sApplicationState State;
	sConfig Config;
	sWindow Window;
	sTimer Timer;

	bool Init(int argc, char **argv);
	bool Run(void);
	void Quit(void);
};

#ifndef ASC_IMPLEMENTATION
extern sApplication* App;
#else
sApplication* App;
#endif
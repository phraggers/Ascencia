// Ascencia
// platform/core.h

#pragma once
#include <shared/types.h>
#include <shared/stdlibs.h>

#ifdef ASC_IMPLEMENTATION
#include <glad/glad.c>
#endif
#include <glad/glad.h>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_net/SDL_net.h>
#include <glm/glm.hpp>
#include <physfs.h>
#include <AL/alc.h>
#include <AL/al.h>
#if(BUILD_DEBUG==1)
#include <termcolor/termcolor.hpp>
#endif

#include <ascencia/util/helper_defs.h>
#include <ascencia/platform/log.h>
#include <ascencia/video/renderer.h>
#include <ascencia/platform/window.h>
#include <ascencia/platform/timer.h>
#include <ascencia/platform/config.h>

struct sCoreState
{
	bool Running;
	char AppName[0xff];
	char AppOrg[0xff];
	sAppVersion Version;
};

class cCore
{
	public:
	sCoreState State;
	cConfig Config;
	cWindow Window;
	cTimer Timer;
	cLog Log;

	bool Init(int argc, char **argv);
	bool Run(void);
	void Quit(void);
};

#ifndef ASC_IMPLEMENTATION
extern cCore* Core;
#else
cCore* Core;
#endif
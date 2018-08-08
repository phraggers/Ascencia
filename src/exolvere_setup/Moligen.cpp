//=============
// Exolvere
// Moligen.cpp
//=============
#include "Headers.h"
#include "Media.h"
#include "Moligen.h"
#include "MoligenMainLoop.hpp"
#include "MoligenHandleEvents.hpp"

//=========
// initSDL
//=========
void Moligen::initSDL()
{
  std::stringstream ss;
  ss << gl.appName << " v" << gl.appVerMaj << "." << gl.appVerMin;
  gl.appTitle = ss.str();

  // Initialize SDL
  if(SDL_Init(SDL_INIT_EVERYTHING))
    {
      error.fatal(1, "0");
    }

  if(IMG_Init(0))
    {
      error.fatal(2, "0");
    }

  if(TTF_Init())
    {
      error.fatal(3, "0");
    }

  if(Mix_Init(0))
    {
      error.fatal(4, "0");
    }

  if(SDLNet_Init())
    {
      error.fatal(5, "0");
    }
}

//============
// initWindow
//============
void Moligen::initWindow()
{
  // Create Window
  gl.mWindow = SDL_CreateWindow(gl.appTitle.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, gl.appWidth, gl.appHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_BORDERLESS);

  // Error check window
  if(!gl.mWindow)
    {
      error.fatal(6, "0");
    }
}

//==============
// initRenderer
//==============
void Moligen::initRenderer()
{
  // Create Renderer
  gl.mRenderer = SDL_CreateRenderer(gl.mWindow, -1, SDL_RENDERER_ACCELERATED);

  // Error check renderer
  if(!gl.mRenderer)
    {
      error.fatal(7, "0");
    }

  SDL_SetRenderDrawColor(gl.mRenderer, 0, 0, 0, 255);
}

//===========
// loopStart
//===========
void Moligen::loopStart()
{
  handleEvents();

  // Update Window Position
  SDL_GetWindowPosition(gl.mWindow, &gl.appXPos, &gl.appYPos);

  // Check for SDL Errors
  const char *SDLError = SDL_GetError();
  if(*SDLError)
    {
     error.warning(8, SDL_GetError());
     SDL_ClearError();
    }

  SDL_RenderClear(gl.mRenderer);
}

//=========
// loopEnd
//=========
void Moligen::loopEnd()
{
  SDL_RenderPresent(gl.mRenderer);
}

//================
// closeRenderer
//================
void Moligen::closeRenderer()
{
  SDL_DestroyRenderer(gl.mRenderer);
  gl.mRenderer = NULL;
}

//===============
// closeWindow
//===============
void Moligen::closeWindow()
{
  SDL_DestroyWindow(gl.mWindow);
  gl.mWindow = NULL;
}

//===========
// closeSDL
//===========
void Moligen::closeSDL()
{
  SDLNet_Quit();
  Mix_Quit();
  TTF_Quit();
  IMG_Quit();
  SDL_Quit();
}

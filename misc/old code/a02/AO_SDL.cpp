//===================
// Ascencia: Origins
// AO_SDL.cpp
//===================
#include "AO_Headers.h"
#include "AO_SDL.h"

//=============
// Constructor
//=============
AO_SDL::AO_SDL(int winW, int winH)
{

  // App Details
  _appName = "Ascencia: Origins";
  _appVerMaj = 0;
  _appVerMin = 2;

  // SDL Variable Definitions
  _AOWindow = NULL;
  _AORenderer = NULL;
  _AOEvent = new SDL_Event();

  // Engine Variable Definitions
  _quit = false;
  _winW = winW;
  _winH = winH;
  _frameCount = 0;

  // Initialize
  AO_Launch_Console();
  AO_SDL_Init();
  AO_TTF_Init();

}

//============
// Destructor
//============
AO_SDL::~AO_SDL()
{

  // Cleanup
  SDL_DestroyWindow(_AOWindow);
  SDL_DestroyRenderer(_AORenderer);
  delete _AOEvent;

  // Quit SDL
  SDL_Quit();
  IMG_Quit();
  TTF_Quit();
}

//==================
// AO_Lauch_Console
//==================
void AO_SDL::AO_Launch_Console()
{
  // Debug Window Header
  for(int i = 0; i < 4; i++)
    std::cout << std::endl;

  std::cout << "============================" << std::endl;
  std::cout << "     " << _appName << " "
            << "v" << _appVerMaj << "." << _appVerMin << std::endl;
  std::cout << "  written by Molivos (2018)" << std::endl;
  std::cout << "============================" << std::endl;
  std::cout << "       DEBUG CONSOLE" << std::endl;
  std::cout << " Please pay no attention to" << std::endl;
  std::cout << " the man behind the curtain!" << std::endl;
  std::cout << "============================" << std::endl;
  std::cout << " LOADING! Please Wait..." << std::endl;
}

//=============
// AO_SDL_Init
//=============
void AO_SDL::AO_SDL_Init()
{

  if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
      std::cout << "SDL failed to initialize! Error: "
                << SDL_GetError() << std::endl;
    }

  else
    {
      std::cout << "SDL started successfully!" << std::endl;

      // Create SDL Window
      _AOWindow = SDL_CreateWindow("Ascencia: Origins",
                                   SDL_WINDOWPOS_UNDEFINED,
                                   SDL_WINDOWPOS_UNDEFINED,
                                   _winW, _winH,
                                   SDL_WINDOW_SHOWN);

      // Error Check Window
      if(_AOWindow == NULL)
        {
          std::cout << "SDL Failed to create a window! Error: "
                    << SDL_GetError() << std::endl;
          _quit = true;
        }

      else
        {
          std::cout << "SDL Window created successfully!" << std::endl;

          // Create SDL Renderer
          _AORenderer = SDL_CreateRenderer(_AOWindow, -1, SDL_RENDERER_ACCELERATED);

          // Error Check Renderer
          if(_AORenderer == NULL)
            {
              std::cout << "SDL failed to create a renderer! Error: "
                        << SDL_GetError() << std::endl;
              _quit = true;
            }

          else
            {
              std::cout << "SDL Renderer created successfully!" << std::endl;
            }
        }
    }

}

//=============
// AO_TTF_Init
//=============
void AO_SDL::AO_TTF_Init()
{
  if(!TTF_WasInit())
    {
      if(TTF_Init() != 0)
        {
          std::cout << "TTF Failed to initialize! Error: "
                    << TTF_GetError() << std::endl;
        }

      else
        {
          std::cout << "TTF initialized successfully!" << std::endl;
        }
    }
}

//===========
// loopStart
//===========
void AO_SDL::loopStart()
{
  // FPS Limiter
  while(SDL_GetTicks() % 33 != 0) { /* WAIT */}
  _frameCount++;

  // Handle Event Queue
  while(SDL_PollEvent(_AOEvent) != 0)
    {
      // Check for QUIT
      if(_AOEvent->type == SDL_QUIT)
        _quit = true;
    }

  // Clear Screen
  SDL_RenderClear(_AORenderer);
}

//=========
// loopEnd
//=========
void AO_SDL::loopEnd()
{
  SDL_RenderPresent(_AORenderer);
}

//=========
// Getters
//=========
std::string AO_SDL::get_appName()
{
  return _appName;
}

int AO_SDL::get_appVerMaj()
{
  return _appVerMaj;
}

int AO_SDL::get_appVerMin()
{
  return _appVerMin;
}

SDL_Window* AO_SDL::get_AOWindow()
{
  return _AOWindow;
}

SDL_Renderer* AO_SDL::get_AORenderer()
{
  return _AORenderer;
}

SDL_Event* AO_SDL::get_AOEvent()
{
  return _AOEvent;
}

int AO_SDL::get_frameCount()
{
  return _frameCount;
}

int AO_SDL::get_winW()
{
  return _winW;
}

int AO_SDL::get_winH()
{
  return _winH;
}

bool AO_SDL::get_quit()
{
  return _quit;
}

//=========
// Setters
//=========
void AO_SDL::set_frameCount(int frameCount)
{
  _frameCount = frameCount;
}

void AO_SDL::set_winW(int setWinW)
{
  _winW = setWinW;
}

void AO_SDL::set_winH(int setWinH)
{
  _winH = setWinH;
}

void AO_SDL::set_quit(bool setQuit)
{
  _quit = setQuit;
}

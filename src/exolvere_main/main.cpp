//============================
// Exolvere
// main.cpp
//============================
// Written by Molivos (2018)
//============================
#define COMPANY "Molivos Software"
#define SOFTWARE "Exolvere"

#include "Headers.h"
#include "Media.h"
#include "Moligen.h"

//=========
// Globals
//=========
// App Strings
std::string Globals::appCompany = COMPANY;
std::string Globals::appName = SOFTWARE;
std::string Globals::appTitle = SOFTWARE;
std::string Globals::appBasePath = "";
std::string Globals::appPrefPath = "";

// App Variables
int Globals::appVerMaj = 0;
int Globals::appVerMin = 15;
int Globals::appControl = CS_BEGIN;
bool Globals::appQuit = false;

// Main Window Variables
int Globals::appWidth = 960;
int Globals::appHeight = 540;
bool Globals::appFullscreen = false;
int Globals::appXPos = 0;
int Globals::appYPos = 0;

// SDL Variables
SDL_Window *Globals::mWindow = NULL;
SDL_Renderer *Globals::mRenderer = NULL;
SDL_Event Globals::mEvent;

// ErrorHandler
std::map<unsigned int, std::string> ErrorHandler::errorCodesMap;

//===================
// MAIN Entry Point
//===================
int main(int argc, char* argv[])
{
  // Pre-Init
  Globals::appBasePath = SDL_GetBasePath();
  Globals::appPrefPath = SDL_GetPrefPath(Globals::appCompany.c_str(), Globals::appName.c_str());

  // Engine
  ErrorHandler error;
  Moligen moligen;

  // Init
  error.init();
  moligen.initSDL();
  moligen.initWindow();
  moligen.initRenderer();

  SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "BaseLocation", Globals::appBasePath.c_str(), NULL);
  SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "PrefLocation", Globals::appPrefPath.c_str(), NULL);

  // Run
  moligen.mainLoop();

  // Exit
  moligen.closeRenderer();
  moligen.closeWindow();
  moligen.closeSDL();

  return 0;
}

// TODO:
// - Fix file paths (using Globals::appBasePath and Globals::appPrefPath)
// - test with sound and fonts
// - test new draw() and play() etc functions on ALL media in all dats.
// - create launcher screen
// - create main menu as it was in Ascencia Origins v0.9
// - get volume slider working

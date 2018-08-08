//=============
// exolvere
// Globals.h
//=============
#ifndef _GLOBALS_H_
#define _GLOBALS_H_

//=====================
// Control State Enum
//=====================
enum CS
  {
    CS_BEGIN,
    CS_QUITSTATE,
    CS_LAUNCHER_INIT,
    CS_LAUNCHER_LOOP,
    CS_LAUNCHER_CLEANUP,
    CS_LOGOSPLASH,
    CS_MAINMENU_INIT,
    CS_MAINMENU_LOOP,
    CS_END
  };

//=========
// Globals
//=========

struct Globals
{
  // App Strings
  static std::string appCompany;
  static std::string appName;
  static std::string appTitle;
  static std::string appBasePath;
  static std::string appPrefPath;

  // App Variables
  static int appVerMaj;
  static int appVerMin;
  static int appControl;
  static bool appQuit;

  // Main Window Variables
  static int appWidth;
  static int appHeight;
  static bool appFullscreen;
  static int appXPos;
  static int appYPos;

  // SDL Variables
  static SDL_Window *mWindow;
  static SDL_Renderer *mRenderer;
  static SDL_Event mEvent;

};

#endif

//===================
// Ascencia: Origins
// AO_Engine.h
//===================
#ifndef _AO_ENGINE_H_
#define _AO_ENGINE_H_
#include "AO_Headers.h"
#include "AO_SDL.h"
#include "AO_Sprite.h"
#include "AO_TTF.h"

class AO_Engine
{

 public:

  AO_Engine(int winW, int winH);
  ~AO_Engine();

  // Game Loops
  void AO_LogoLoop();
  void AO_MainMenuLoop();
  void AO_GameLoop();

 private:

  // Load SDL
  AO_SDL* AOSDL;

  // Logo
  unsigned int logoA;
  int logoStep;
  int logoTimer;

  // Main Menu
  bool mainMenu;

  // Fonts
  AO_TTF* font_title_screen_footer;

  // Sprites
  AO_Sprite* sprite_molivos_logo;
  AO_Sprite* sprite_title_screen_background;
  AO_Sprite* sprite_title_screen_title;

  AO_Sprite* sprite_mainMenu_NewGame;
  AO_Sprite* sprite_mainMenu_LoadGame;
  AO_Sprite* sprite_mainMenu_Options;
  AO_Sprite* sprite_mainMenu_Quit;

  AO_Sprite* sprite_mainMenu_NewGame_hover;
  AO_Sprite* sprite_mainMenu_LoadGame_hover;
  AO_Sprite* sprite_mainMenu_Options_hover;
  AO_Sprite* sprite_mainMenu_Quit_hover;

  AO_Sprite* sprite_mainMenu_NewGame_down;
  AO_Sprite* sprite_mainMenu_LoadGame_down;
  AO_Sprite* sprite_mainMenu_Options_down;
  AO_Sprite* sprite_mainMenu_Quit_down;

};

#endif

//===================
// Ascencia: Origins
// AO_Engine.cpp
//===================
#include "AO_Engine.h"

//============
//Constructor
//============
AO_Engine::AO_Engine(int winW, int winH)
{

  // Initialize SDL
  AOSDL = new AO_SDL(winW, winH);

  // Logo Render
  logoA = 0;
  logoStep = 0;
  logoTimer = 0;

  // Main Menu
  mainMenu = true;

  //-----------
  // TTF Fonts
  //-----------
  font_title_screen_footer = new AO_TTF
    (AOSDL->get_AORenderer(),
     "../data/ttf/liberation-serif/regular.ttf",
     30, 515, 700, 250, 30);

  //---------
  // Sprites
  //---------

  sprite_molivos_logo = new AO_Sprite
    (AOSDL->get_AORenderer(),
     "../data/img/molivos_software",
     352, 209, 576, 350);
  SDL_SetTextureAlphaMod(sprite_molivos_logo->getTexture(), 0);

  sprite_title_screen_background = new AO_Sprite(AOSDL->get_AORenderer(),
                                                 "../data/img/title_screen_background",
                                                 0, 0,
                                                 AOSDL->get_winW(),
                                                 AOSDL->get_winH());

  sprite_title_screen_title = new AO_Sprite(AOSDL->get_AORenderer(),
                                            "../data/img/title_screen_title",
                                            280, 50, 725, 123);

  // Main Menu Plates

  sprite_mainMenu_NewGame = new AO_Sprite(AOSDL->get_AORenderer(),
                                       "../data/img/mainMenu_NewGame",
                                       450, 220, 400, 66);

  sprite_mainMenu_LoadGame = new AO_Sprite(AOSDL->get_AORenderer(),
                                          "../data/img/mainMenu_LoadGame",
                                          450, 300, 400, 66);

  sprite_mainMenu_Options = new AO_Sprite(AOSDL->get_AORenderer(),
                                          "../data/img/mainMenu_Options",
                                          450, 380, 400, 66);

  sprite_mainMenu_Quit = new AO_Sprite(AOSDL->get_AORenderer(),
                                          "../data/img/mainMenu_Quit",
                                          450, 460, 400, 66);

  // Hover

  sprite_mainMenu_NewGame_hover = new AO_Sprite(AOSDL->get_AORenderer(),
                                          "../data/img/mainMenu_NewGame_hover",
                                          450, 220, 400, 66);

  sprite_mainMenu_LoadGame_hover = new AO_Sprite(AOSDL->get_AORenderer(),
                                           "../data/img/mainMenu_LoadGame_hover",
                                           450, 300, 400, 66);

  sprite_mainMenu_Options_hover = new AO_Sprite(AOSDL->get_AORenderer(),
                                          "../data/img/mainMenu_Options_hover",
                                          450, 380, 400, 66);

  sprite_mainMenu_Quit_hover = new AO_Sprite(AOSDL->get_AORenderer(),
                                       "../data/img/mainMenu_Quit_hover",
                                       450, 460, 400, 66);

  // Down

  sprite_mainMenu_NewGame_down = new AO_Sprite(AOSDL->get_AORenderer(),
                                          "../data/img/mainMenu_NewGame_down",
                                          450, 220, 400, 66);

  sprite_mainMenu_LoadGame_down = new AO_Sprite(AOSDL->get_AORenderer(),
                                           "../data/img/mainMenu_LoadGame_down",
                                           450, 300, 400, 66);

  sprite_mainMenu_Options_down = new AO_Sprite(AOSDL->get_AORenderer(),
                                          "../data/img/mainMenu_Options_down",
                                          450, 380, 400, 66);

  sprite_mainMenu_Quit_down = new AO_Sprite(AOSDL->get_AORenderer(),
                                       "../data/img/mainMenu_Quit_down",
                                       450, 460, 400, 66);



}

//============
// Destructor
//============
AO_Engine::~AO_Engine()
{
  // Delete Sprites
  delete sprite_title_screen_background;
  delete sprite_title_screen_title;

  delete sprite_mainMenu_NewGame;
  delete sprite_mainMenu_LoadGame;
  delete sprite_mainMenu_Options;
  delete sprite_mainMenu_Quit;

  delete sprite_mainMenu_NewGame_hover;
  delete sprite_mainMenu_LoadGame_hover;
  delete sprite_mainMenu_Options_hover;
  delete sprite_mainMenu_Quit_hover;

  delete sprite_mainMenu_NewGame_down;
  delete sprite_mainMenu_LoadGame_down;
  delete sprite_mainMenu_Options_down;
  delete sprite_mainMenu_Quit_down;

  delete AOSDL;
}

//=============
// AO_LogoLoop
//=============
void AO_Engine::AO_LogoLoop()
{

  while(logoStep < 3 && !AOSDL->get_quit())
    {

      AOSDL->loopStart();

      //------------------------------
      // Molivos Software logo splash
      //------------------------------

      if(logoStep >= 0 && logoStep < 3)
        {

          sprite_molivos_logo->Draw();
          SDL_SetTextureAlphaMod(sprite_molivos_logo->getTexture(), logoA);

          // Fade in Molivos Logo
          if(logoStep == 0)
            {
              if(logoA < 255)
                logoA++;
              else
                {
                  logoTimer = SDL_GetTicks() + 3000;
                  logoStep++;
                }
            }

          // Hold Logo
          if(logoStep == 1)
            {
              if(logoTimer < SDL_GetTicks())
                logoStep++;
            }

          // Fade out Molivos Logo
          if(logoStep == 2)
            {
              if(logoA > 0)
                logoA--;
              else
                logoStep++;
            }

          if(logoStep == 3)
            delete sprite_molivos_logo;

        } // logo

      AOSDL->loopEnd();

    }
}

//=================
// AO_MainMenuLoop
//=================
void AO_Engine::AO_MainMenuLoop()
{
  while(mainMenu && !AOSDL->get_quit())
    {
      AOSDL->loopStart();

      sprite_title_screen_background->Draw();
      sprite_title_screen_title->Draw();

      sprite_mainMenu_NewGame->Draw();
      sprite_mainMenu_LoadGame->Draw();
      sprite_mainMenu_Options->Draw();
      sprite_mainMenu_Quit->Draw();

      font_title_screen_footer->set_text("Molivos 2018");
      font_title_screen_footer->Draw();

      AOSDL->loopEnd();
    }
}

//=================
// AO_GameLoop
//=================
void AO_Engine::AO_GameLoop()
{
  while(mainMenu && !AOSDL->get_quit())
    {
      AOSDL->loopStart();



      AOSDL->loopEnd();
    }
}

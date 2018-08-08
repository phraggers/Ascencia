//===================
// Ascencia: Origins
// main.cpp
//===================
// Written by Molivos (2018)

#include "AO_Headers.h"
#include "AO_Engine.h"

//==================
// Main Entry Point
//==================
int main(int argc, char* argv[])
{
  // Initialize Game Engine
  AO_Engine engine(1280, 768);

  // Game Run
  engine.AO_LogoLoop();
  engine.AO_MainMenuLoop();
  engine.AO_GameLoop();


  return EXIT_SUCCESS;
}

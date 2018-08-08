// =================
// ASCENCIA ORIGINS
// main.cpp
// =================

// =================
// C++ DEPENDENCIES
// =================

#include <iostream>
#include <string>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <cstddef>
//#include <algorithm> // stoi() - see if there are alternatives for WIN?

// =================
// CUSTOM HEADERS
// =================

#include "AOsys.h"
#include "Player.h"
#include "Print.h"
#include "globals.h"

// =================
// ENTRY POINT MAIN
// =================

int main(int argc, char* argv[])
{
  // objects
  Sys sys;
  Player player;
  Print print;

  // main variables
  unsigned short mainMenuSelect = 0; // main menu selection
  bool returnTocc = 0; // return to character creation, skips main menu select
  bool gameInit = 0; // confirm game start at end of main menu loop

  sys.cls();
  print.header();

  // END PROGRAM
  { // tell sys destructor to clean up
    std::ofstream tmpFile("ascencia.tmp");
    tmpFile << "TERMINATE" << std::flush;
    tmpFile.close();
  }
  return 0;
}

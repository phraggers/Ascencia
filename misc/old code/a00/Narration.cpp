// ===================
// ASCENCIA NARRATION
// Narration.cpp
// ===================

// =================
// C++ DEPENDENCIES
// =================

#include <iostream>
#include <iomanip>

// =================
// CUSTOM DEPENDENCIES
// =================

#include "include/Globals.h"
#include "include/Print.h"
#include "include/Narration.h"

// =================
// PRINT VARIABLES
// =================

void Narration::n01()
{

  Globals gl;
  std::string pad = gl.getPad();

  std::cout << pad << "As your head clears, you realize that you are definitely no longer\n"
            << pad << "sitting at home, as is evident by the strange sound of birdsong sung\n"
            << pad << "by unfamiliar birds.\n\n"
            << pad << "You spit out a tuft of grass that somehow made its way into your\n"
            << pad << "mouth, and attempt to rub the blurriness from your eyes.\n\n"
            << pad << "You see grassy hills stretching out in all directions, although you\n"
            << pad << "can make out what appears to be a large body of water some distance\n"
            << pad << "away in one direction. Other than that, you also note that it seems to\n"
            << pad << "be rather a pleasant day; There are a few fluffy white clouds in the sky\n"
            << pad << "and the sun rays on your face make you feel quite at ease.\n"
            << std::endl;

}

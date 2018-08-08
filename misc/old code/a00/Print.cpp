// =========================
// ASCENCIA PRINT FUNCTIONS
// Print.cpp
// =========================


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


// =================
// HEADER
// =================

void Print::header()
{ // PRINT HEADER

  Globals gl;
  std::string pad = gl.getPad();

  std::cout << "      _____________________________________________________________________________" << std::endl;
  std::cout << "     |    _______  _______  _______  _______  _        _______ _________ _______   |" << std::endl;
  std::cout << "     |   (  ___  )(  ____ \\(  ____ \\(  ____ \\( (    /|(  ____ \\\\__   __/(  ___  )  |" << std::endl;
  std::cout << "     |   | (   ) || (    \\/| (    \\/| (    \\/|  \\  ( || (    \\/   ) (   | (   ) |  |" << std::endl;
  std::cout << "     |   | (___) || (_____ | |      | (__    |   \\ | || |         | |   | (___) |  |" << std::endl;
  std::cout << "     |   |  ___  |(_____  )| |      |  __)   | (\\ \\) || |         | |   |  ___  |  |" << std::endl;
  std::cout << "     |   | (   ) |      ) || |      | (      | | \\   || |         | |   | (   ) |  |" << std::endl;
  std::cout << "     |   | )   ( |/\\____) || (____/\\| (____/\\| )  \\  || (____/\\___) (___| )   ( |  |" << std::endl;
  std::cout << "     |   |/     \\|\\_______)(_______/(_______/|/    )_)(_______/\\_______/|/     \\|  |" << std::endl;
  std::cout << "     |_____________________________________________________________________________|" << std::endl;
  std::cout << std::endl;
  std::cout << "          **** VERSION: ";
  /*Print::version();*/
  std::cout << " **** COPYRIGHT MOLIVOS 2017 ****" << std::endl;
  /*Print::line();*/
} // END HEADER

// =================
// VERSION
// =================

void Print::version()
{   // print version:

  Globals gl;
  std::string pad = gl.getPad();

  std::cout << gl.getVersionStates(gl.getVersion(0))
            << " " << gl.getVersion(1) << "." << gl.getVersion(2)
            << " : " << gl.getVersion(3)
            << "-" << std::setfill('0') << std::setw(2) << gl.getVersion(4)
            << "-" << std::setfill('0') << std::setw(2) << gl.getVersion(5);
} // END PRINT VERSION

// =================
// MAIN MENU
// =================

void Print::mainMenu()
{ // PRINT MAIN MENU

  Globals gl;
  std::string pad = gl.getPad();

  std::cout << pad << " ------------------------ MAIN MENU ------------------------ " <<std::endl;
  std::cout << pad << "(1) Start New Game" << std::endl;
  std::cout << pad << "(2) Load Existing Game" << std::endl;
  std::cout << pad << "(3) Options" << std::endl;
  std::cout << pad << "(0) EXIT" << std::endl << std::endl;
} // END MAIN MENU

// =================
// LINE
// =================

void Print::line()
{
  std::cout << "      " << std::setfill('=') << std::setw(77) << "=" << std::endl;
}

// =================
// PLAYER TOTAL XP
// =================

void Print::playerTotalXP()   // print Player's TOTAL XP
// Let me explain: This may seem RIDICULOUSLY over complicated,
// but over level 60 ish, the total XP gained by a player is too large
// for an unsigned long long int to hold, and it's hard to read without commas.
// This function uses a short int array to hold numbers up to 999,
// then shifts +1 to the next element in the array.
// It uses array[0] as the 0-999 column, so needs to print in reverse order
// (as in, array[1] is thousand units and array[2] is ten thousands, etc)
{/*
  // XP Table
  unsigned long int xpTable[gl.getMaxLevel()+1];
  xpTable[0] = 0;
  xpTable[1] = 0;
  xpTable[2] = gl.getxpTablelvl2();
  for(int i = 3; i <= gl.getMaxLevel(); i++)
    { xpTable[i] = ((xpTable[i-1]) * gl.getxpTableInc() ); }
  // variables required to print total xp gained
  unsigned long int playerXPtotali = 0; // iterator
  short playerXPtotal[9] = {0,0,0,0,0,0,0,0,0}; // 9 comma separated segments
  playerXPtotali = player.getxp(); // put current XP into iterator
  // TODO: fix this ^... player object in this file is not the same as main.cpp

  for(int x = player.getLevel(); x > 0; x--)
    {
      while(playerXPtotali > 0)
        {
          playerXPtotali--;
          playerXPtotal[0]++;
          if(playerXPtotal[0] == 1000)
            {
              for(int i=0; i<9; i++)
                {
                  if(playerXPtotal[i] == 1000)
                    {
                      playerXPtotal[i] = 0;
                      playerXPtotal[i+1]++;
                    }
                }
            }
        }
      playerXPtotali = xpTable[x];
    }

  // The Print:
  if(playerXPtotal[8] > 0)
    {
      std::cout << playerXPtotal[8] << ",";
      for(int i = 7; i > 0; i--)
        { std::cout << std::setfill('0') << std::setw(3) << playerXPtotal[i] << ","; }
      std::cout << std::setfill('0') << std::setw(3) << playerXPtotal[0];
    }

  else if(playerXPtotal[7] > 0)
    {
      std::cout << playerXPtotal[7] << ",";
      for(int i = 6; i > 0; i--)
        { std::cout << std::setfill('0') << std::setw(3) << playerXPtotal[i] << ","; }
      std::cout << std::setfill('0') << std::setw(3) << playerXPtotal[0];
    }

  else if(playerXPtotal[6] > 0)
    {
      std::cout << playerXPtotal[6] << ",";
      for(int i = 5; i > 0; i--)
        { std::cout << std::setfill('0') << std::setw(3) << playerXPtotal[i] << ","; }
      std::cout << std::setfill('0') << std::setw(3) << playerXPtotal[0];
    }

  else if(playerXPtotal[5] > 0)
    {
      std::cout << playerXPtotal[5] << ",";
      for(int i = 4; i > 0; i--)
        { std::cout << std::setfill('0') << std::setw(3) << playerXPtotal[i] << ","; }
      std::cout << std::setfill('0') << std::setw(3) << playerXPtotal[0];
    }

  else if(playerXPtotal[4] > 0)
    {
      std::cout << playerXPtotal[4] << ",";
      for(int i = 3; i > 0; i--)
        { std::cout << std::setfill('0') << std::setw(3) << playerXPtotal[i] << ","; }
      std::cout << std::setfill('0') << std::setw(3) << playerXPtotal[0];
    }

  else if(playerXPtotal[3] > 0)
    {
      std::cout << playerXPtotal[3] << ",";
      for(int i = 2; i > 0; i--)
        { std::cout << std::setfill('0') << std::setw(3) << playerXPtotal[i] << ","; }
      std::cout << std::setfill('0') << std::setw(3) << playerXPtotal[0];
    }

  else if(playerXPtotal[2] > 0)
    {
      std::cout << playerXPtotal[2] << ",";
      std::cout << std::setfill('0') << std::setw(3) << playerXPtotal[1] << ",";
      std::cout << std::setfill('0') << std::setw(3) << playerXPtotal[0];
    }
  else if(playerXPtotal[1] > 0)
    {
      std::cout << playerXPtotal[1] << ",";
      std::cout << std::setfill('0') << std::setw(3) << playerXPtotal[0];
    }

  else std::cout << playerXPtotal[0];
  /**/
} // END printing Player XP */

// =================
// PLAYER CHARACTER SHEET
// =================

void Print::playerCharacterSheet()
{/* // Print Player Character Sheet

  // XP Table
  unsigned long int xptable[gl.getMaxLevel() + 1];
  xptable[0] = 0;
  xptable[1] = 0;
  xptable[2] = gl.getxpTablelvl2();
  for(int i = 3; i <= gl.getMaxLevel(); i++)
    { xptable[i] = ( (xptable[i-1] ) * gl.getxpTableInc() ); }

  // Print sheet
  std::cout << pad << "Name: " << player.getName() << std::endl
            << pad << "Gender: " << player.getGender() << std::endl
            << pad << "Level: " << player.getLevel() << std::endl
            << pad << "XP: " << player.getxp() << "/"
            << xptable[player.getLevel() + 1] - player.getxp() << std::endl
            << pad << "Total XP: ";
  /*Print::playerTotalXP();*/
  std::cout << std::endl;
  /**/
} // END print player character sheet

// =================
// SYS COMMANDS
// =================

void Print::sysCommands()
{

  Globals gl;
  std::string pad = gl.getPad();

  std::cout << std::endl << pad
            << "SYSTEM COMMANDS: save, quit, character, inventory, help\n";
}

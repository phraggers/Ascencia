// =========================
// ASCENCIA PRINT FUNCTIONS
// Print.cpp
// =========================


// =================
// C++ DEPENDENCIES
// =================

#include <iostream>
#include <iomanip>
#include <string>

// =================
// CUSTOM DEPENDENCIES
// =================

#include "AOsys.h"
#include "Player.h"
#include "Print.h"

// =================
// HEADER
// =================

void Print::header()
{ // PRINT HEADER
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
  version();
  std::cout << " **** COPYRIGHT MOLIVOS 2017 ****" << std::endl;
  line();
} // END HEADER

// =================
// VERSION
// =================

void Print::version()
{   // print version:
  std::cout << sysPrint.getVersionStates(sysPrint.getVersion(0))
            << " " << sysPrint.getVersion(1) << "." << sysPrint.getVersion(2)
            << " : " << sysPrint.getVersion(3)
            << "-" << std::setfill('0') << std::setw(2) << sysPrint.getVersion(4)
            << "-" << std::setfill('0') << std::setw(2) << sysPrint.getVersion(5);
} // END PRINT VERSION

// =================
// MAIN MENU
// =================

void Print::mainMenu()
{ // PRINT MAIN MENU
  std::string pad = sysPrint.getPad();
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
{
  // variables required to print total xp gained
  unsigned long int playerXPtotali = 0; // iterator
  short playerXPtotal[9] = {0,0,0,0,0,0,0,0,0}; // 9 comma separated segments
  playerXPtotali = playerPrint.getxp(); // put current XP into iterator

  for(int x = playerPrint.getLevel(); x > 0; x--)
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
      playerXPtotali = sysPrint.getxpTable(x);
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
} // END printing Player XP

// =================
// PLAYER CHARACTER SHEET
// =================

void Print::playerCharacterSheet()
{ // Print Player Character Sheet
  std::string pad = sysPrint.getPad();
  std::cout << pad << "Name: " << playerPrint.getName() << std::endl
            << pad << "Gender: " << playerPrint.getGender() << std::endl
            << pad << "Level: " << playerPrint.getLevel() << std::endl
            << pad << "XP: " << playerPrint.getxp() << "/"
            << sysPrint.getxpTable(playerPrint.getLevel() + 1) - playerPrint.getxp() << std::endl
            << pad << "Total XP: ";
  playerTotalXP();
  std::cout << std::endl;
} // END print player character sheet

// =================
// SYS COMMANDS
// =================

void Print::sysCommands()
{
  std::string pad = sysPrint.getPad();
  std::cout << std::endl << pad
            << "SYSTEM COMMANDS: save, quit, character, inventory, help\n";
}

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
#include <algorithm>

// =================
// CUSTOM HEADERS
// =================

#include "include/Globals.h"
#include "include/Print.h"
#include "include/Narration.h"
#include "include/CharAttributes.h"

// =================
// NOTES
// =================
/* RANDOM
   -------
   srand((int) time(0));
   int rand1 = (rand() % 100);

   SAVE FILE FORMAT
   ----------------
   versionState (version[0])
   versionMajor (version[1])
   versionMinor (version[2])
   player.Name
   player.Gender
   player.Level
   player.xp
   player.savebookmark

   XP TABLE
   --------
   unsigned long int xpTable[gl.getMaxLevel() + 1];
   xpTable[0] = 0;
   xpTable[1] = 0;
   xpTable[2] = gl.getxpTablelvl2();
   for(int i = 3; i <= gl.getMaxLevel(); i++)
   { xpTable[i] = ((xpTable[i-1]) * gl.xpTableInc ); }

*/

// Function Declarations
void sys(int call);
void waits(int sec);

// =================
// ENTRY POINT MAIN
// =================

int main(int argc, char* argv[])
{

  Narration narr;

  // XP Table
  unsigned long int xpTable[playerMaxLevel + 1];
  xpTable[0] = 0;
  xpTable[1] = 0;
  xpTable[2] = xpTablelvl2;
  for(int i = 3; i <= playerMaxLevel; i++)
    { xpTable[i] = ((xpTable[i-1]) * xpTableInc() ); }

  int mainMenuSelect = 0; // main menu selection
  int badInput = 0; // menu selection bad
  bool returnTocc = 0; // return to character creation, skips main menu select
  bool gameInit = 0; // confirm game start at end of main menu loop

  do
    {
      sys(0);
      print.header();
      print.mainMenu();

      if(badInput == 1)
        {
          std::cout << pad << "Bad Input! Please choose one of the options above." << std::endl;
          badInput = 0;
        }

      if(returnTocc == 0)
        {
          std::cout << pad << "Please type selection and hit ENTER: ";
          std::cin >> mainMenuSelect;
          std::cout << std::endl;
        }
      if(returnTocc == 1)
        { mainMenuSelect = 1; }

      // (1) Start New Game
      if(mainMenuSelect == 1)
        {

          bool overwrite = 0;
          bool returnToMenu = 0;

          while(overwrite == 0)
            {
              sys(0);
              print.header();
              std::cout << pad <<
                " ------------------------ NEW GAME -------------------------\n" << std::endl;
              std::cout << pad << "Please enter a character name (NO SPACES!)" << std::endl;
              std::cout << pad << "Name: ";
              std::string sname;
              std::cin >> sname;
              player.setName(sname);

              { // Check for existing SaveFile with that name
                std::ifstream savefile(player.getName() + ".sav");
                if(savefile.is_open())
                  {
                    savefile.close();
                    std::cout << pad << "Character already exists! Overwrite?"
                              << " (y/n) (or q to return to main menu): ";
                    char confirm;
                    std::cin >> confirm;
                    while(!(confirm == 'y' || confirm == 'Y'
                            || confirm == 'n' || confirm == 'N' || confirm == 'q' || confirm == 'Q'))
                      {
                        std::cout << pad << "Bad Input! Type 'y', 'n' or 'q' and hit ENTER: ";
                        std::cin >> confirm;
                      }
                    if(confirm == 'y' || confirm == 'Y') { overwrite = 1; }
                    if(confirm == 'n' || confirm == 'N') { overwrite = 0; }
                    if(confirm == 'q' || confirm == 'Q') { returnToMenu = 1; break; }
                  }

                else if(!(savefile.is_open()))
                  {
                    returnToMenu = 0;
                    overwrite = 1;
                  }

              } // END: check save file
            } // END: while overwrite loop

          if(returnToMenu == 0)
            { // Character Creation
              sys(0);
              print.header();
              std::cout << pad <<
                " --------------------- CHARACTER CREATION ------------------\n" << std::endl;
              std::cout << pad << "Name: " << player.getName() << std::endl;
              std::cout << pad << "Gender (Type 'M' or 'F' and hit ENTER): ";
              char sg;
              std::cin >> sg;
              player.setGenderc(sg);
              while(!(player.getGenderc() == 'm' || player.getGenderc() == 'M'
                      || player.getGenderc() == 'f' || player.getGenderc() == 'F'))
                {
                  std::cout << pad
                            << "Invalid input! Please type 'M' or 'F' and hit ENTER: ";
                  std::cin >> sg;
                  player.setGenderc(sg);
                }

              // set Gender choice from char playerGenderc to string playerGender
              switch(player.getGenderc())
                {
                case 'm':
                case 'M':
                  player.setGender("Male");
                  break;
                case 'f':
                case 'F':
                  player.setGender("Female");
                  break;
                default:
                  std::cerr << "An Error ocurred, playerGenderc!";
                }

              // set starting character stats
              player.setLevel(1);
              player.setxp(0);
              player.setSaveBookmark(1);

              // display character sheet
              sys(0);
              print.header();
              std::cout << pad <<
                " --------------------- CHARACTER CREATION ------------------\n" << std::endl;
              print.playerCharacterSheet();

              // check if correct
              std::cout << pad << "Is this correct? (y/n) (or q to return to main menu): ";
              char confirm;
              std::cin >> confirm;
              while(!(confirm == 'y' || confirm == 'Y'
                      || confirm == 'n' || confirm == 'N' || confirm == 'q' || confirm == 'Q'))
                {
                  std::cout << pad << "Bad Input! Please type 'y', 'n' or 'q' and hit ENTER: ";
                  std::cin >> confirm;
                }
              returnTocc = 0;
              if(confirm == 'y' || confirm == 'Y') { overwrite = 1; }
              if(confirm == 'n' || confirm == 'N')
                { overwrite = 0; returnToMenu = 1; returnTocc = 1; }
              if(confirm == 'q' || confirm == 'Q')
                { overwrite = 0; returnToMenu = 1; returnTocc = 0; }

              if(overwrite == 1)
              { // Create SaveFile
                std::ofstream savefile;
                savefile.open(player.getName() + ".sav");
                savefile << "versionState=" << gl.getVersionStates(gl.getVersion(0)) << std::endl;
                savefile << "versionMajor=" << gl.getVersion(1) << std::endl;
                savefile << "versionMinor=" << gl.getVersion(2) << std::endl;
                savefile << "playerName=" << player.getName() << std::endl;
                savefile << "playerGender=" << player.getGenderc() << std::endl;
                savefile << "playerLevel=" << player.getLevel() << std::endl;
                savefile << "playerXP=" << player.getxp() << std::endl;
                savefile << "savebookmark=" << player.getSaveBookmark() << std::endl;
                savefile.close();
                std::cout << pad << "Saved: " << player.getName() << ".sav" << std::endl;

                waits(3);

                gameInit = 1; //confirm start new game

              } // END: Create SaveFile

            } // END: Character Creation
        } // END (1) Start New Game

      // (2) Load Existing Game
      if(mainMenuSelect == 2)
        {
          bool returnToMenu = 0;
          bool loadc = 0;
          while(returnToMenu == 0 && loadc == 0)
            {
              sys(0);
              print.header();
              std::cout << pad <<
                " ----------------------- LOAD GAME -------------------------\n" << std::endl;
              std::cout << pad << "Enter existing character name (NO SPACES!)" << std::endl;
              std::cout << pad << "Name: ";
              std::string sname;
              std::cin >> sname;
              player.setName(sname);

              { // Check for existing SaveFile with that name
                std::ifstream savefile(player.getName() + ".sav");
                if(savefile.is_open())
                  {
                    savefile.close();
                    std::cout << pad << "File found. Load game?"
                              << " (y/n) (or q to return to main menu): ";
                    char confirm;
                    std::cin >> confirm;
                    while(!(confirm == 'y' || confirm == 'Y'
                            || confirm == 'n' || confirm == 'N' || confirm == 'q' || confirm == 'Q'))
                      {
                        std::cout << pad << "Bad Input! Type 'y', 'n' or 'q' and hit ENTER: ";
                        std::cin >> confirm;
                      }
                    if(confirm == 'y' || confirm == 'Y') { returnToMenu = 0; loadc = 1; }
                    if(confirm == 'n' || confirm == 'N') { returnToMenu = 0; loadc = 0; }
                    if(confirm == 'q' || confirm == 'Q') { returnToMenu = 1; loadc = 0; }
                  }

                else if(!(savefile.is_open()))
                  {
                    std::cout << pad << "Savefile with that name not found!" << std::endl;
                    waits(3);
                    returnToMenu = 0;
                    loadc = 0;
                  }

              } // END: check save file
            } // END: load file while(returnToMenu == 0 && loadc == 0)

          if(loadc == 1)
            { // Load game

              std::string str1; // reads variable where value goes
              std::string str2; // reads value

              std::ifstream savefile(player.getName() + ".sav");
              std::cout << pad << "Loading File. Please wait!" << std::endl;

              while(getline(savefile,str2))
                {
                  str1 = str2; // put line into these 2 strings

                  { // str1: replace everything after = with =
                    bool j = 0; // when = is read, this is 1, meaning replace with =
                    for(int i = 0; i < str1.length(); i++)
                      {
                        if(str1[i] == '=') { j = 1; }
                        if(j) { str1[i] = '='; }
                      }
                  } // end replace str1 unwanted =

                  { // str2: replace everything before = with =
                    bool j = 1; // when = is read, this is 0, meaning stop replacing
                    for(int i = 0; i < str2.length(); i++)
                      {
                        if(str2[i] == '=') { j = 0; }
                        if(j) { str2[i] = '='; }
                      }
                  } // end replace str2 unwanted =

                  // remove all '='
                  str1.erase(std::remove(str1.begin(), str1.end(), '='),
                                    str1.end());
                  str2.erase(std::remove(str2.begin(), str2.end(), '='),
                                    str2.end());

                  { // place variables and values into active variables
                    if(str1 == "versionState")
                      { for(int i = 0; i < 4; i++)
                          { if(str2 == gl.getVersionStates(i))
                              { player.setSaveVersion(0,i); }}}
                    if(str1 == "versionMajor") {player.setSaveVersion(1,stoi(str2)); }
                    if(str1 == "versionMinor") {player.setSaveVersion(2,stoi(str2)); }
                    if(str1 == "playerName") { player.setName(str2); }
                    if(str1 == "playerGender")
                      {
                        player.setGenderc(str2[0]);
                        // set Gender choice from char playerGenderc to string playerGender
                        switch(str2[0])
                          {
                          case 'm':
                          case 'M':
                            player.setGender("Male");
                            break;
                          case 'f':
                          case 'F':
                            player.setGender("Female");
                            break;
                          default:
                            std::cerr << "An Error ocurred, playerGenderc!";
                          }
                      }
                    if(str1 == "playerLevel") { player.setLevel(stoi(str2)); }
                    if(str1 == "playerXP") { player.setxp(stoi(str2)); }
                    if(str1 == "savebookmark") { player.setSaveBookmark(stoi(str2)); }
                  } // END: place variables and values into active variables
                } // END: while reading lines
              savefile.close();
              gameInit = 1;
              std::cout << pad << "File Loaded!" << std::endl;

              { // warn if saveversion is not current version
                if(!(player.getSaveVersion(0) == gl.getVersion(0)
                     && player.getSaveVersion(1) == gl.getVersion(1)
                     && player.getSaveVersion(2) == gl.getVersion(2)))
                  {
                    std::cout << pad
                              << "Save File Version: " << player.getSaveVersion(0) << " "
                              << player.getSaveVersion(1) << "." << player.getSaveVersion(2) << "\n"
                              << pad << "Game version: "
                              << gl.getVersionStates(gl.getVersion(0)) << " "
                              << gl.getVersion(1) << "." << gl.getVersion(2) << std::endl << pad
                              << "Warning! Save File version is different from current\n" << pad
                              << "game version. File must be updated first! Continue? (y/n): ";
                    char confirm;
                    std::cin >> confirm;
                  }
              } // END: warn if save version different

              // TODO: IMPLEMENT SAVE FILE UPGRADE FUNCTION

            } // END load game

        } // END (2) Load Existing Game

      // (3) Options
      if(mainMenuSelect == 3)
        {

        } // END (3) Options

      // Bad Input
      if (mainMenuSelect > 3) { badInput = 1; }

      // END OF MAIN MENU

      // GAME INITIALIZE
      while(gameInit == 1)
        {
          std::string sinput; // player input
          unsigned short doAt = 0; // count input attempts

          // CHAPTER 1: INTRO
          if(player.getSaveBookmark() == 1)
            {
              sys(0);
              print.header();
              print.playerCharacterSheet();
              print.sysCommands();
              print.line();
              narr.n01();
              std::cout << pad << "Tutorial: After 'Do:', type an action and hit ENTER.\n";
              std::cout << pad << "(Note you can type in the SYSTEM COMMANDS here)\n";
              print.line();
              std::cout << pad << "Do: ";
              std::getline(std::cin, sinput);
              doAt++;
              if(sinput == "quit") { gameInit = 0; break; }

            } // END: CHAPTER 1: INTRO
        } // END: while gameInit 1


    } // END GameLoop Do

  // (0) EXIT
  while(mainMenuSelect != 0);
  if(mainMenuSelect == 0) { return 0; } // Successful Exit

  return 1; // Unexpected Exit
}

// =================
// END OF MAIN
// =================

void sys(int call)
{ // Function: System Calls (0:WIN, 1:LIN, 2:OSX)
  Globals gl;
  switch(call)
    {
    case 0:
      if(gl.getos() == 0) { system("cls"); }
      if(gl.getos() == 1 || gl.getos() ==2) { system("clear"); }
      break;
    default:
      std::cerr << "Invalid System Call: " << call << std::endl;
    }

} // END: sys call function

void waits(int sec)
{
  double pauseTimer = (time(0) + sec);
  while(pauseTimer != time(0)) { /* waiting... */ }
}

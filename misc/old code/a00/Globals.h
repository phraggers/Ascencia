// ========================
// ASCENCIA GLOBALS HEADER
// Globals.h
// ========================

#ifndef _GLOBALS_H_
#define _GLOBALS_H_

// Globals
// System Variables
extern int os; // WIN=0 LIN=1 OSX=2 // This is used for system() commands

// VERSION:
extern std::string versionStates[4];

// 0:versionState, 1:majorVersion, 2:minorVersion, 3:year, 4:month, 5:date
extern unsigned int version[6];

// XP Table Set
extern int xpTablelvl2; // xp required to level up for the first time
extern double xpTableInc; // how much more xp required every level
extern int playerMaxLevel;

// Default Padding
extern std::string pad;

// Character
// Player Character Details
extern std::string playerName;
extern char playerGenderc;
extern std::string playerGender;
extern int playerLevel;
extern int playerxp;

// Player Savefile variables
extern unsigned int saveBookmark;
extern unsigned int saveVersion[3]; // no need for date


/* DISABLING THE CLASSES while testing globals

// =================
// CLASS: Globals
// =================

class Globals
{

 private:

  // System Variables
  const int os = 1; // WIN=0 LIN=1 OSX=2 // This is used for system() commands

  // VERSION:
  std::string versionStates[4] = {"inDev","alpha","beta","release"};

  // 0:versionState, 1:majorVersion, 2:minorVersion, 3:year, 4:month, 5:date
  const unsigned int version[6] = {0,0,3,2017,11,06};

  // XP Table Set
  const int xpTablelvl2 = 10; // xp required to level up for the first time
  const double xpTableInc = 1.3; // how much more xp required every level
  const int maxLevel = 100;

  // Default Padding
  std::string pad = "          ";

 public:

  // GETTERS
  int getos();
  std::string getVersionStates(int state);
  unsigned int getVersion(int ver);
  int getxpTablelvl2();
  double getxpTableInc();
  int getMaxLevel();
  std::string getPad();

};

// =================
// CLASS: Player
// =================

class Player
{

 private:

  // Player Character Details
  std::string name;
  char genderc;
  std::string gender;
  int level;
  int xp;

  // Player Savefile variables
  unsigned int saveBookmark;
  unsigned int saveVersion[3]; // no need for date

 public:

  // GETTERS
  std::string getName();
  char getGenderc();
  std::string getGender();
  int getLevel();
  int getxp();

  unsigned int getSaveBookmark();
  unsigned int getSaveVersion(int ver);

  // SETTERS
  void setName(std::string sname);
  void setGenderc(char sgenderc);
  void setGender(std::string sgender);
  void setLevel(int slevel);
  void setxp(int sxp);
  void setSaveBookmark(unsigned int ssaveBookmark);
  void setSaveVersion(unsigned int ssetSaveVersion, int ver);

};

*/

#endif

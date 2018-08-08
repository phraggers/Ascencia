// ========================
// ASCENCIA PLAYER HEADER
// Player.h
// ========================

#ifndef _PLAYER_H_
#define _PLAYER_H_

// ======================
// CLASS: PLAYER GLOBALS
// ======================

class PlayerGlobals
{

 public:

  // Globally Accessible Variables
  static std::string name;
  static char genderc;
  static std::string gender;
  static unsigned int level;
  static unsigned int xp;

};

// ======================
// CLASS: PLAYER
// ======================

class Player
{

 private:

  // Global Object
  PlayerGlobals playerG;
  Sys sysPlayer;

  // auto setters
  void setGender(char genderc);
  void levelUp();

  // internal getters
  char getGenderc();

 public:

  // Setters
  void setName(std::string sname);
  void setGenderc(char sgenderc);
  void setLevel(unsigned int slevel);
  void setxp(unsigned int sxp);
  void addxp(unsigned int axp);

  // Getters
  std::string getName();
  std::string getGender();
  unsigned int getLevel();
  unsigned int getxp();

};

#endif

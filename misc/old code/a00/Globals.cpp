// =================
// ASCENCIA GLOBALS
// Globals.cpp
// =================

// =================
// C++ DEPENDENCIES
// =================

#include <iostream>
#include <string>

// =================
// CUSTOM DEPENDENCIES
// =================

#include "include/Globals.h"

// ==============
// DEFINE Globals
// ==============

// System Variables
int os = 1; // WIN=0 LIN=1 OSX=2 // This is used for system() commands

// VERSION:
std::string versionStates[4] = {"inDev","alpha","beta","release"};

// 0:versionState, 1:majorVersion, 2:minorVersion, 3:year, 4:month, 5:date
unsigned int version[6] = {0,0,3,2017,11,06};

// XP Table Set
int xpTablelvl2 = 10; // xp required to level up for the first time
double xpTableInc = 1.3; // how much more xp required every level
int playerMaxLevel = 100;

// Default Padding
std::string pad = "          ";



/* DISABLING FUNCTIONS while testing Globals

// =================
// Globals GETTERS
// =================

int Globals::getos() { return os; }
std::string Globals::getVersionStates(int state) { return versionStates[state]; }
unsigned int Globals::getVersion(int ver) { return version[ver]; }
int Globals::getxpTablelvl2() { return xpTablelvl2; }
double Globals::getxpTableInc() { return xpTableInc; }
int Globals::getMaxLevel() { return maxLevel; }
std::string Globals::getPad() { return pad; }

// =================
// Player GETTERS
// =================

std::string Player::getName() { return name; }
char Player::getGenderc() { return genderc; }
std::string Player::getGender() { return gender; }
int Player::getLevel() { return level; }
int Player::getxp() { return xp; }
unsigned int Player::getSaveBookmark() { return saveBookmark; }
unsigned int Player::getSaveVersion(int ver) { return saveVersion[ver]; }

// =================
// Player SETTERS
// =================

void Player::setName(std::string sname) { name = sname; }
void Player::setGenderc(char sgenderc) { genderc = sgenderc; }
void Player::setGender(std::string sgender) { gender = sgender; }
void Player::setLevel(int slevel) { level = slevel; }
void Player::setxp(int sxp) { xp = sxp; }
void Player::setSaveBookmark(unsigned int ssaveBookmark) { saveBookmark = ssaveBookmark; }
void Player::setSaveVersion(unsigned int sver, int ver) { saveVersion[sver] = ver; }

*/

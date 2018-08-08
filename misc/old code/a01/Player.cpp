// ========================
// ASCENCIA PLAYER SOURCE
// Player.cpp
// ========================

// ============
// C++ HEADERS
// ============

#include <iostream>
#include <string>

// ==============
// CUSTOM HEADERS
// ==============

#include "AOsys.h"
#include "Player.h"

// ---------
// Contents:
// ---------
// Auto Setters
// Internal Getters
// Setters
// Getters

// =============
// AUTO SETTERS
// =============

// SET GENDER
void Player::setGender(char genderc)
{
  if(genderc == 'm' || genderc == 'M') { playerG.gender = "Male"; }
  if(genderc == 'f' || genderc == 'F') { playerG.gender = "Female"; }
}

// LEVEL UP
void Player::levelUp()
{
  // check xp req for next level vs current xp
  if(getxp() >= sysPlayer.getxpTable(getLevel() + 1))
    {
      setxp(getxp() - sysPlayer.getxpTable(getLevel() + 1));
      setLevel(getLevel() + 1);
      // TODO: need proper system here to announce level up
      std::cout << "\nLEVEL UP!\n" << std::flush;
    }
}

// =================
// INTERNAL GETTERS
// =================

// GET GENDER CHAR
char Player::getGenderc()
{
  return playerG.genderc;
}

// ========
// SETTERS
// ========

// SET NAME
void Player::setName(std::string sname)
{
  playerG.name = sname;
}

// SET GENDER CHAR
void Player::setGenderc(char sgenderc)
{
  playerG.genderc = sgenderc;
  setGender(sgenderc);
}

// SET LEVEL
void Player::setLevel(unsigned int slevel)
{
  playerG.level = slevel;
}

// SET XP
void Player::setxp(unsigned int sxp)
{
  playerG.xp = sxp;
}

// ADD XP
void Player::addxp(unsigned int axp)
{
  playerG.xp += axp;
  levelUp();
}

// ========
// GETTERS
// ========

std::string Player::getName() { return playerG.name; }
std::string Player::getGender() { return playerG.gender; }
unsigned int Player::getLevel() { return playerG.level; }
unsigned int Player::getxp() { return playerG.xp; }

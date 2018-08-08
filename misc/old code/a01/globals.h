// ========================
// ASCENCIA GLOBALS HEADER
// globals.h
// ========================
// To assign memory space for variables
// shared between objects of the same class

#ifndef _GLOBALS_H_
#define _GLOBALS_H_

// ========
// Globals
// ========

short SysGlobals::os;
int SysGlobals::sysTimer;

std::string SysGlobals::versionStates[4];
int SysGlobals::version[6];

unsigned int SysGlobals::xpTablelvl2;
double SysGlobals::xpTableInc;
unsigned int SysGlobals::maxLevel;
unsigned long int SysGlobals::xpTable[101];

std::string SysGlobals::pad;

// ===============
// PLAYER GLOBALS
// ===============

std::string PlayerGlobals::name;
char PlayerGlobals::genderc;
std::string PlayerGlobals::gender;
unsigned int PlayerGlobals::level;
unsigned int PlayerGlobals::xp;


#endif

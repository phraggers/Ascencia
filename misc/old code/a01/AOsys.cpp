// ========================
// ASCENCIA SYSTEMS SOURCE
// AOsys.cpp
// ========================

// ============
// C++ HEADERS
// ============

#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
#include <ctime>
#include <cstdlib>

// ==============
// CUSTOM HEADERS
// ==============

#include "AOsys.h"

// ---------
// Contents:
// ---------
// Constructors
// Destructors
// Initialization
// Auto Setters
// Setters
// Getters
// Sys Funcs
// XP Table Funcs

// ============
// CONSTRUCTORS
// ============

SysGlobals::SysGlobals()
{
  bool init = 0;
  { // check if init happened yet
    int count = 0;
    {
      std::string tmpFileStr;
      std::ifstream tmpFile("ascencia.tmp");
      while(getline(tmpFile,tmpFileStr)) { count++; };
      tmpFile.close();
    }
    {
      std::string tmpFileStr[count+1];
      std::ifstream tmpFile("ascencia.tmp");
      for(int i = 0; i <= count; i++)
        {
          getline(tmpFile,tmpFileStr[i]);
        }
      tmpFile.close();
      for(int i = 0; i <= count; i++)
        {
          if(tmpFileStr[i] == "INIT=TRUE") { init = 1; }
        }
    }
    if(init == 0)
      {
        std::cout << "================================================" << std::endl;
        std::cout << "ASCENCIA ORIGINS IS INITIALIZING... PLEASE WAIT!" << std::endl;
        std::cout << "================================================" << std::endl;
        testRWAccess();
      }
  }
}

Sys::Sys()
{
  bool init = 0;
  { // check if init happened yet
    int count = 0;
    {
      std::string tmpFileStr;
      std::ifstream tmpFile("ascencia.tmp");
      while(getline(tmpFile,tmpFileStr)) { count++; };
      tmpFile.close();
    }
    {
      std::string tmpFileStr[count];
      std::ifstream tmpFile("ascencia.tmp");
      for(int i = 0; i <= count; i++)
        {
          getline(tmpFile,tmpFileStr[i]);
        }
      tmpFile.close();
      for(int i = 0; i <= count; i++)
        {
          if(tmpFileStr[i] == "INIT=TRUE") { init = 1; }
        }
    }
    if(init == 0)
      { // INITIALIZE GLOBALS
        setos();
        setSysTimer();
        setVersionStates();
        setVersion();
        setxpTablelvl2();
        setxpTableInc();
        setMaxLevel();
        setxpTable();
        setPad();
        setInitTrue();
        // SPLASH
        std::cout << "Initialization Successful." << std::endl;
        if(getos() == 0)
          { std::cout << "OS: 0 (Windows Command Prompt)" << std::endl; }
        if(getos() == 1)
          { std::cout << "OS: 1 (GNU Bash Shell)" << std::endl; }
        std::cout << "TPS: " << getSysTimer() << " Ticks per second" << std::endl;
        double i = clock() + (2 * (getSysTimer()));
        while(i > clock()){}
      }
  }
}

// ============
// DESTRUCTORS
// ============

SysGlobals::~SysGlobals()
{
  { // check if main ended
    std::string tmpFileStr;
    std::ifstream tmpFile("ascencia.tmp");
    getline(tmpFile,tmpFileStr);
    tmpFile.close();
    if(tmpFileStr == "TERMINATE")
      {
        removetmpFile();
      }
  }
}

// ===============
// INITIALIZATION
// ===============

// SYSGLOBALS: TEST RW ACCESS
void SysGlobals::testRWAccess()
{
  std::ofstream testFile("test.tmp");
  try {
    if(testFile.is_open()) { testFile.close(); std::remove("test.tmp"); }
    else if(!(testFile.is_open())) { throw 1; } }
  catch(int e)
    {
      if(e == 1)
        {
          std::cerr << "Error while attempting to write test file. "
                    << "Move Ascencia Origins to somewhere with read/write permissions.";
          std::terminate();
        }
    }
}

// SYSGLOBALS::REMOVETMPFILE
void SysGlobals::removetmpFile()
{
  std::remove("ascencia.tmp");
}

// ===============
// GLOBAL SETTERS
// ===============

// SYS::SETOS
void Sys::setos()
{

  system("echo \"os\" > ascencia.tmp");

  int count = 0;

  {
    std::string tmpFileStr;
    std::ifstream tmpFile("ascencia.tmp");
    while(getline(tmpFile,tmpFileStr)) { count++; };
    tmpFile.close();
  }

  {
    std::string tmpFileStr[count+1];
    std::ifstream tmpFile("ascencia.tmp");
    for(int i = 0; i <= count; i++) { getline(tmpFile,tmpFileStr[i]); }
    tmpFile.close();

    bool set = 0;
    for(int i = 0; i <= count; i++)
      {
        if(tmpFileStr[i] == "\"os\" ") { sysG.os = 0; set = 1; }
        if(tmpFileStr[i] == "os") { sysG.os = 1; set = 1; }
      }

    if(set == 0)
      {
        int manual;
        std::cout << std::endl << "Failed to detect OS! Please input OS manually.\n";
        std::cout << "(type 0 for WINDOWS or 1 for Linux/OSX and hit ENTER): ";
        std::cin >> manual;

        while(!(manual == 0 || manual == 1))
          {
            std::cout << "Invalid input! (type 0 for WINDOWS ";
            std::cout << "or 1 for Linux/OSX and hit ENTER): ";
            std::cin >> manual;
          }
        sysG.os = manual;
      }
  }
}

// SYS::SETSYSTIMER
void Sys::setSysTimer()
{
  clock_t c = time(0) + 1;
  while(c > time(0)){}
  clock_t t = clock();
  c = time(0) + 1;
  while(c > time(0)){}
  t = clock() - t;
  sysG.sysTimer = t;
}

// SET VERSION STATES
void Sys::setVersionStates()
{
  sysG.versionStates[0] = "inDev";
  sysG.versionStates[1] = "alpha";
  sysG.versionStates[2] = "beta";
  sysG.versionStates[3] = "release";
}

// SET CURRENT VERSION
// 0:versionState, 1:VersionMajor, 2:VersionMinor, 3:year, 4:month, 5:date
void Sys::setVersion()
{
  sysG.version[0] = 0;
  sysG.version[1] = 1;
  sysG.version[2] = 1;
  sysG.version[3] = 2017;
  sysG.version[4] = 11;
  sysG.version[5] = 9;
}

// SET XP TABLE LVL 2
// xp required to level up for the first time
void Sys::setxpTablelvl2()
{
  sysG.xpTablelvl2 = 10;
}

// SET XP TABLE INC
// xp required every level multiplier
void Sys::setxpTableInc()
{
  sysG.xpTableInc = 1.3;
}

// SET MAX LEVEL
// max level a player can reach
void Sys::setMaxLevel()
{
  sysG.maxLevel = 100;
}

// SET XP TABLE
void Sys::setxpTable()
{ // this should be sxpTable[maxLevel+1], but maxLevel isn't const now
  unsigned long int sxpTable[101];
  sxpTable[0] = 0;
  sxpTable[1] = 0;
  sxpTable[2] = sysG.xpTablelvl2;
  for(int i = 3; i <= sysG.maxLevel; i++)
    { sxpTable[i] = ((sxpTable[i-1]) * sysG.xpTableInc); }
  for(int i = 0; i <= sysG.maxLevel; i++)
    { sysG.xpTable[i] = sxpTable[i]; }
}

// SET PAD
// default text padding
void Sys::setPad()
{
  sysG.pad = "          ";
}

// SET INIT TRUE
void Sys::setInitTrue()
{ // write "INIT=TRUE" to ascencia.tmp so constructor only runs once
  std::ofstream tmpFile("ascencia.tmp");
  tmpFile << "INIT=TRUE" << std::flush;
  tmpFile.close();
}

// ========
// GETTERS
// ========

short Sys::getos() { return sysG.os; }
int Sys::getSysTimer() { return sysG.sysTimer; }
std::string Sys::getVersionStates(int state) { return sysG.versionStates[state]; }
int Sys::getVersion(int i) { return sysG.version[i]; }
unsigned int Sys::getxpTablelvl2() { return sysG.xpTablelvl2; }
double Sys::getxpTableInc() { return sysG.xpTableInc; }
unsigned int Sys::getMaxLevel() { return sysG.maxLevel; }
std::string Sys::getPad() { return sysG.pad; }

// ===========
// SYS FUNCS
// ===========

// SYS::CLS
void Sys::cls()
{
  if(sysG.os == 0) { system("cls"); }
  if(sysG.os == 1) { system("clear"); }
}

// ===============
// XP TABLE FUNCS
// ===============

unsigned int Sys::getxpTable(unsigned int xlevel)
{
  return sysG.xpTable[xlevel];
}

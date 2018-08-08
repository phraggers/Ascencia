// ========================
// ASCENCIA SYSTEMS HEADER
// AOsys.h
// ========================

#ifndef _AOSYS_H_
#define _AOSYS_H_

// =================
// CLASS: SYSGLOBALS
// =================

class SysGlobals
{

 private:

  // Constructors/Destructors
  void testRWAccess();
  void removetmpFile();

 public:

  // Constructor
  SysGlobals();

  // Destructor
  ~SysGlobals();

  // Global Variables
  static short os;
  static int sysTimer;

  static std::string versionStates[4];
  static int version[6];
  // XP Table Set
  static unsigned int xpTablelvl2;
  static double xpTableInc;
  static unsigned int maxLevel;
  static unsigned long int xpTable[101];
  // Default Padding
  static std::string pad;

};

// =================
// CLASS: SYS
// =================

class Sys
{

 private:

  // Global Object
  SysGlobals sysG;

  // Global Setters (Only happens at startup)
  void setos();
  void setSysTimer();
  void setVersionStates();
  void setVersion();
  void setxpTablelvl2();
  void setxpTableInc();
  void setMaxLevel();
  void setxpTable();
  void setPad();

  // After init, write in tmp that init happened
  void setInitTrue();

 public:

  // Constructor
  Sys();

  // Getters
  short getos();
  int getSysTimer();
  std::string getVersionStates(int state);
  int getVersion(int i);
  unsigned int getxpTablelvl2();
  double getxpTableInc();
  unsigned int getMaxLevel();
  std::string getPad();

  // Sys funcs
  void cls();

  // XP Table Funcs
  unsigned int getxpTable(unsigned int xlevel);

};


#endif

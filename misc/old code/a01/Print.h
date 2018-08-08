// =========================
// ASCENCIA PRINT HEADER
// Print.h
// =========================

#ifndef _PRINT_H_
#define _PRINT_H_

// =============
// CLASS: PRINT
// =============

class Print
{

 private:

  // global objects
  Sys sysPrint;
  Player playerPrint;

 public:

  void header();
  void version();
  void mainMenu();
  void line();
  void playerTotalXP();
  void playerCharacterSheet();
  void sysCommands();
};

#endif

//===========
// Exolvere
// Moligen.h
//===========

#ifndef _MOLIGEN_H_
#define _MOLIGEN_H_

struct Moligen
{
  // Functions
  void initSDL();
  void initWindow();
  void initRenderer();

  void handleEvents();
  void mainLoop();
  void loopStart();
  void loopEnd();

  void closeRenderer();
  void closeWindow();
  void closeSDL();

  // Variables
  Globals gl;
  Media media;
  ErrorHandler error;
};

#endif

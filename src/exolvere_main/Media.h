//===========
// Exolvere
// Media.h
//===========

#ifndef _MEDIA_H_
#define _MEDIA_H_

#include "Dat.h"
#include "Sprite.h"

struct Media
{
  // Functions
  void loadingscreen();
  void loadDat(std::string fileName);
  void copyToMap(std::string datname, std::string mapname, std::string mapstring);
  void unloadDat(std::string datName);

  // Sprites
  void spriteNew(std::string id, std::string dat, std::string file);
  void spriteDraw(std::string id);
  void spriteResize(std::string id, int x, int y, int w, int h);
  void spriteCrop(std::string id, int x, int y, int w, int h);
  void spriteDelete(std::string id);

  // Data Maps
  std::map<std::string, Dat> datMap;
  std::map<std::string, Sprite*> sprites;
};

#endif

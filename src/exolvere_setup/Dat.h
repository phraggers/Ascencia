//==========
// Exolvere
// Dat.h
//==========
#ifndef _DAT_H_
#define _DAT_H_

struct Dat
{
  // Data Storage
  //[filename, file contents]
  std::map<std::string, std::string> fonts;
  std::map<std::string, std::string> sprites;
  std::map<std::string, std::string> music;
  std::map<std::string, std::string> chunks;
};

#endif

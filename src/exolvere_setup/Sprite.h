//===========
// Exolvere
// Sprite.h
//===========
#ifndef _SPRITE_H_
#define _SPRITE_H_

//================
// POS enum
//================
enum POS
  {
    POS_NO_CHANGE,
    POS_SOURCE,
    POS_CENTERED,
    POS_LEFT_0,
    POS_LEFT_1,
    POS_LEFT_2,
    POS_LEFT_3,
    POS_LEFT_4,
    POS_RIGHT_0,
    POS_RIGHT_1,
    POS_RIGHT_2,
    POS_RIGHT_3,
    POS_RIGHT_4,
    POS_TOP_0,
    POS_TOP_1,
    POS_TOP_2,
    POS_TOP_3,
    POS_TOP_4,
    POS_BOTTOM_0,
    POS_BOTTOM_1,
    POS_BOTTOM_2,
    POS_BOTTOM_3,
    POS_BOTTOM_4
  };

//===============
// Sprite Struct
//===============
struct Sprite
{
  // functions
  Sprite(std::string &fileContents);
  ~Sprite();
  void draw();

  // variables
  SDL_RWops *rw;
  SDL_Surface *surface;
  SDL_Texture *texture;
  SDL_Rect source;
  SDL_Rect srect;
  SDL_Rect drect;
  Uint32 format;
  int access;
};

#endif

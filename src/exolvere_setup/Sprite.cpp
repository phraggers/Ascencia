//============
// Exolvere
// Sprite.cpp
//============
#include "Headers.h"
#include "Sprite.h"

//=============
// Constructor
//=============
Sprite::Sprite(std::string &fileContents)
{
  size_t buffersize = fileContents.length() + 1;
  char buffer[buffersize];
  for(int i=0; i<=buffersize; i++)
    buffer[i] = fileContents[i];

  rw = SDL_RWFromMem(buffer, sizeof(buffer));
  surface = IMG_Load_RW(rw, 1);
  texture = SDL_CreateTextureFromSurface(Globals::mRenderer, surface);

  //NOTE source: as source file
  SDL_QueryTexture(texture, &format, &access, &source.w, &source.h);
  source.x = 0;
  source.y = 0;

  //NOTE srect: in regards to source pixels from file
  srect.x = 0;
  srect.y = 0;
  srect.w = source.w;
  srect.h = source.h;

  //NOTE drect: in regards to destination pixels on screen
  drect.x = 0;
  drect.y = 0;
  drect.w = source.w;
  drect.h = source.h;
}

//============
// Destructor
//============
Sprite::~Sprite()
{
  SDL_DestroyTexture(texture);
}

//=======
// draw
//=======
void Sprite::draw()
{
  SDL_RenderCopy(Globals::mRenderer, texture, &srect, &drect);
}

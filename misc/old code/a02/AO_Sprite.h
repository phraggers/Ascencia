//===================
// Ascencia: Origins
// AO_Sprite.h
//===================
#ifndef _AO_SPRITE_H_
#define _AO_SPRITE_H_
#include "AO_Headers.h"

class AO_Sprite
{

 public:

  AO_Sprite(SDL_Renderer* ptr_AORenderer,
            std::string filePath,
            int x, int y, int w, int h);
  ~AO_Sprite();

  // Functions
  void Draw();

  // Getters
  int getWidth();
  int getHeight();
  SDL_Texture* getTexture();

  // Setters
  void setWidth(int w);
  void setHeight(int h);

 private:

  // SDL Variables
  SDL_Texture* _imageTexture;
  SDL_Rect _imageRect;
  SDL_Rect _imageCrop;
  SDL_Renderer* _imageRenderer;

  // Image Variables
  int _imageWidth;
  int _imageHeight;

};

#endif

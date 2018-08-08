//===================
// Ascencia: Origins
// AO_Sprite.cpp
//===================
#include "AO_Sprite.h"

//=============
// Constructor
//=============
AO_Sprite::AO_Sprite(SDL_Renderer* ptr_AORenderer,
                     std::string filePath,
                     int x, int y, int w, int h)
{

  // Renderer
  _imageRenderer = NULL;
  _imageRenderer = ptr_AORenderer;

  // Texture
  _imageTexture = NULL;
  _imageTexture = IMG_LoadTexture(_imageRenderer, filePath.c_str());

  // Image Rect
  _imageRect.x = x;
  _imageRect.y = y;
  _imageRect.w = w;
  _imageRect.h = h;

  // Crop Rect
  SDL_QueryTexture(_imageTexture, NULL, NULL, &_imageWidth, &_imageHeight);
  _imageCrop.x = 0;
  _imageCrop.y = 0;
  _imageCrop.w = _imageWidth;
  _imageCrop.h = _imageHeight;


  // Error Check
  if(_imageTexture == NULL)
    {
      std::cout << "SDL IMG: " << filePath
                << " failed to load! Error: "
                << IMG_GetError() << std::endl;
    }

  else
    {
      std::cout << "SDL IMG: " << filePath
                << " loaded successfully!" << std::endl;
    }

}

//============
// Destructor
//============
AO_Sprite::~AO_Sprite()
{
  SDL_DestroyTexture(_imageTexture);
}

//======
// Draw
//======
void AO_Sprite::Draw()
{
  SDL_RenderCopy(_imageRenderer, _imageTexture, &_imageCrop, &_imageRect);
}

//=========
// Getters
//=========

int AO_Sprite::getWidth()
{
  return _imageWidth;
}

int AO_Sprite::getHeight()
{
  return _imageHeight;
}

SDL_Texture* AO_Sprite::getTexture()
{
  return _imageTexture;
}

//=========
// Setters
//=========

void AO_Sprite::setWidth(int w)
{
  _imageWidth = w;
}

void AO_Sprite::setHeight(int h)
{
  _imageHeight = h;
}

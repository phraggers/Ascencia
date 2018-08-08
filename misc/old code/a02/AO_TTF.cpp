//===================
// Ascencia: Origins
// AO_TTF.cpp
//===================
#include "AO_TTF.h"

//=============
// Constructor
//=============
AO_TTF::AO_TTF(SDL_Renderer* ptr_AORenderer,
               std::string filePath,
               int pt, int x, int y, int w, int h)
{
  // Passed Variables
  _textRenderer = ptr_AORenderer;
  _fontPath = filePath;

  // Rect
  _textRect.x = x;
  _textRect.y = y;
  _textRect.w = w;
  _textRect.h = h;

  // Crop
  _textCrop.x = 0;
  _textCrop.y = 0;
  _textCrop.w = w;
  _textCrop.h = h;

  // Variables
  _ptSize = pt;
  _text = "undefined";

  // Color
  _colorRed = 255;
  _colorGreen = 255;
  _colorBlue = 255;
  _color = {_colorRed,_colorGreen,_colorBlue};

  // Set Font
  _AOFont = NULL;
  _textSurface = NULL;
  _textTexture = NULL;

  _AOFont = TTF_OpenFont(_fontPath.c_str(), _ptSize);

  // Error Check
  if(_AOFont == NULL)
    {
      std::cout << "SDL TTF: " << _fontPath
                << " failed to load! Error: "
                << TTF_GetError() << std::endl;
    }
  else
    {
      std::cout << "SDL TTF: " << _fontPath
                << " loaded successfully!" << std::endl;

      // Load TTF Surface
      _textSurface = TTF_RenderText_Blended(_AOFont, _text.c_str(), _color);

      if(_textSurface == NULL)
        {
          std::cout << "SDL Surface for TTF: " << _fontPath
                    << " failed to initialize! Error: "
                    << SDL_GetError() << std::endl;
        }

      else
        {
          std::cout << "SDL Surface for TTF: " << _fontPath
                    << " initialized successfully!" << std::endl;

          // Load SDL Texture
          _textTexture = SDL_CreateTextureFromSurface
            (_textRenderer, _textSurface);

          if(_textTexture == NULL)
            {
              std::cout << "SDL TTF: " << _fontPath
                        << " failed to load texture from surface! Error: "
                        << SDL_GetError() << std::endl;
            }

          else
            {
              std::cout << "SDL TTF: " << _fontPath
                        << " loaded texture from surface." << std::endl;
            }
        }
    }
}


//============
// Destructor
//============
AO_TTF::~AO_TTF()
{
  TTF_CloseFont(_AOFont);
  _AOFont = NULL;
  _textSurface = NULL;
  _textTexture = NULL;
}

//======
// Draw
//======
void AO_TTF::Draw()
{
  SDL_RenderCopy(_textRenderer, _textTexture, &_textCrop, &_textRect);
}

//========
// Update
//========
void AO_TTF::Update()
{
  _textSurface = TTF_RenderText_Solid(_AOFont, _text.c_str(), _color);
  _textTexture = SDL_CreateTextureFromSurface(_textRenderer, _textSurface);
}

//=========
// Getters
//=========
int AO_TTF::get_width()
{
  return _textWidth;
}

int AO_TTF::get_height()
{
  return _textHeight;
}

int AO_TTF::get_ptSize()
{
  return _ptSize;
}

std::string AO_TTF::get_text()
{
  return _text;
}

Uint8 AO_TTF::get_colorRed()
{
  return _colorRed;
}

Uint8 AO_TTF::get_colorGreen()
{
  return _colorGreen;
}

Uint8 AO_TTF::get_colorBlue()
{
  return _colorBlue;
}

//=========
// Setters
//=========
void AO_TTF::set_width(int w)
{
  if(_textWidth != w)
    {
      _textWidth = w;
      Update();
    }
}

void AO_TTF::set_height(int h)
{
  if(_textHeight != h)
    {
      _textHeight = h;
      Update();
    }
}

void AO_TTF::set_ptSize(int setPtSize)
{
  if(_ptSize != setPtSize)
    {
      _ptSize = setPtSize;
      Update();
    }
}

void AO_TTF::set_text(std::string setText)
{
  if(_text != setText)
    {
      _text = setText;
      Update();
    }
}

void AO_TTF::set_color(Uint8 red, Uint8 green, Uint8 blue)
{
  if(_colorRed != red ||
     _colorGreen != green ||
     _colorBlue != blue)
    {
      _colorRed = red;
      _colorGreen = green;
      _colorBlue = blue;
      _color = {_colorRed,_colorGreen,_colorBlue};
      Update();
    }
}

void AO_TTF::set_colorRed(Uint8 red)
{
  if(_colorRed != red)
    {
      _colorRed = red;
      _color = {_colorRed,_colorGreen,_colorBlue};
      Update();
    }
}

void AO_TTF::set_colorGreen(Uint8 green)
{
  if(_colorGreen != green)
    {
      _colorGreen = green;
      _color = {_colorRed,_colorGreen,_colorBlue};
      Update();
    }
}

void AO_TTF::set_colorBlue(Uint8 blue)
{
  if(_colorBlue != blue)
    {
      _colorBlue = blue;
      _color = {_colorRed,_colorGreen,_colorBlue};
      Update();
    }
}

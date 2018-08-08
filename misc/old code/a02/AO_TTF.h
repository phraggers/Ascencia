//===================
// Ascencia: Origins
// AO_TTF.h
//===================
#ifndef _AO_TTF_H_
#define _AO_TTF_H_
#include "AO_Headers.h"

class AO_TTF
{

 public:

  AO_TTF(SDL_Renderer* ptr_appRenderer,
         std::string filePath,
         int pt, int x, int y, int w, int h);
  ~AO_TTF();

  // Functions
  void Draw();
  void Update();

  // Getters
  int get_width();
  int get_height();
  int get_ptSize();
  std::string get_text();
  Uint8 get_colorRed();
  Uint8 get_colorGreen();
  Uint8 get_colorBlue();

  // Setters
  void set_width(int w);
  void set_height(int h);
  void set_ptSize(int setPtSize);
  void set_text(std::string setText);
  void set_color(Uint8 red, Uint8 green, Uint8 blue);
  void set_colorRed(Uint8 red);
  void set_colorGreen(Uint8 green);
  void set_colorBlue(Uint8 blue);

 private:

  // SDL Variables
  SDL_Renderer* _textRenderer;
  TTF_Font* _AOFont;
  SDL_Surface* _textSurface;
  SDL_Texture* _textTexture;
  SDL_Color _color;
  SDL_Rect _textRect;
  SDL_Rect _textCrop;

  // Text Variables
  std::string _fontPath;
  int _ptSize;
  Uint8 _colorRed;
  Uint8 _colorGreen;
  Uint8 _colorBlue;
  std::string _text;
  int _textWidth;
  int _textHeight;

};

#endif

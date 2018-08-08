//===================
// Ascencia: Origins
// AO_SDL.h
//===================
#ifndef _AO_SDL_H_
#define _AO_SDL_H_

class AO_SDL
{

 public:

  AO_SDL(int winW, int winH);
  ~AO_SDL();

  // Initializers
  void AO_Launch_Console();
  void AO_SDL_Init();
  void AO_TTF_Init();

  // Functions
  void loopStart();
  void loopEnd();

  // Getters
  std::string get_appName();
  int get_appVerMaj();
  int get_appVerMin();

  SDL_Window* get_AOWindow();
  SDL_Renderer* get_AORenderer();
  SDL_Event* get_AOEvent();

  int get_frameCount();
  int get_winW();
  int get_winH();
  bool get_quit();

  // Setters
  void set_quit(bool setQuit);
  void set_winW(int setWinW);
  void set_winH(int setWinH);
  void set_frameCount(int setFrameCount);

 private:

  // App Details
  std::string _appName;
  int _appVerMaj;
  int _appVerMin;

  // SDL Variables
  SDL_Window* _AOWindow;
  SDL_Renderer* _AORenderer;
  SDL_Event* _AOEvent;

  // Engine Variables
  bool _quit;
  int _winW;
  int _winH;
  int _frameCount;

};

#endif

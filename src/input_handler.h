//================================
//          ASCENCIA CLIENT
//          input.h
//================================
//        (c) Phragware 2019
//================================

#ifndef ASC_CLIENT_INPUTHANDLER_H_
#define ASC_CLIENT_INPUTHANDLER_H_

#include <headers.h>

enum InputHandler_Keystate
{
    ASC_KEYDOWN,
    ASC_KEYUP
};

struct InputHandler_Mouse
{
    void Init();
    int x;
    int y;
    std::map<Uint8, bool> button;
};

struct InputHandler_Window
{
    void Init();
    void Update();
    int ID;
    int x;
    int y;
    int w;
    int h;
    bool shown;
    bool minimized;
    bool maximized;
    bool mouse_focus;
    bool kb_focus;
};

enum InputHandler_Keypress
{
    ASC_KEYREPEAT,
    ASC_ONKEYDOWN,
    ASC_ONKEYUP
};

struct InputHandler_Keytimer
{
    InputHandler_Keytimer(SDL_Keycode _key);
};

struct InputHandler
{
    void Init();
    void PostFrameCleanup();
    void Keyboard(SDL_Keycode _keycode, InputHandler_Keystate _state);
    void MouseMotion();
    void MouseButton(Uint8 _button, InputHandler_Keystate _state);
    void WindowEvent(Uint32 _ID, Sint32 _data1, Sint32 _data2, Uint8 _event);
    bool Keypress(SDL_Keycode _key, InputHandler_Keypress _press);
    bool Mousepress(Uint8 _button, InputHandler_Keypress _press);

    std::map<SDL_Keycode, bool> Keys;
    std::map<SDL_Keycode, bool> Keydowns;
    std::map<SDL_Keycode, bool> Keyups;
    std::map<Uint8, bool> Mousedowns;
    std::map<Uint8, bool> Mouseups;
    InputHandler_Mouse mouse;
    InputHandler_Window window;
};

#endif // ASC_CLIENT_INPUTHANDLER_H_
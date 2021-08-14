//================================
//          ASCENCIA CLIENT
//          input_handler.cpp
//================================
//        (c) Phragware 2019
//================================

#define ASC_INPUTHANDLER_SYSTEMS
#include <systems.h>
#undef ASC_INPUTHANDLER_SYSTEMS

//====================
// InputHandler Init
//====================
void InputHandler::Init()
{
    window.Init();
    mouse.Init();
}

//==========
// Keyboard
//==========
void InputHandler::Keyboard(SDL_Keycode _keycode, InputHandler_Keystate _state)
{
    bool KeyDown;

    if(_state == ASC_KEYDOWN)
    {
        KeyDown = true;
    }

    else
    {
        KeyDown = false;
        Keyups[_keycode] = 1;
    }

    Keys[_keycode] = KeyDown;

    int keydowns = 0;

    for(auto const &x : Keys)
    {
        if(x.second)
        {
            keydowns++;
        }
    }

    if(keydowns == 0)
    {
        Keys.clear();
        Keydowns.clear();
    }
}

//====================
// Mouse Init
//====================
void InputHandler_Mouse::Init()
{
    SDL_GetMouseState(&x, &y);
    button[SDL_BUTTON_LEFT] = false;
    button[SDL_BUTTON_RIGHT] = false;
    button[SDL_BUTTON_MIDDLE] = false;
    button[SDL_BUTTON_X1] = false;
    button[SDL_BUTTON_X2] = false;
}

//===============
// MouseMotion
//===============
void InputHandler::MouseMotion()
{
    SDL_GetMouseState(&mouse.x, &mouse.y);
}

//==============
// MouseButton
//==============
void InputHandler::MouseButton(Uint8 _button, InputHandler_Keystate _state)
{
    bool KeyDown;

    if(_state == ASC_KEYDOWN)
    {
        KeyDown = true;
    }

    else
    {
        KeyDown = false;
        Mouseups[_button] = 1;
    }

    mouse.button[_button] = KeyDown;

    int mousedowns = 0;

    for(auto const &x : mouse.button)
    {
        if(x.second)
        {
            mousedowns++;
        }
    }

    if(mousedowns == 0)
    {
        Mousedowns.clear();
    }
}

//================
// Window Init
//================
void InputHandler_Window::Init()
{
    ID, x, y, w, h = 0;
    shown, minimized, maximized, mouse_focus, kb_focus = false;
    Update();
}

//=================
// Window Update
//=================
void InputHandler_Window::Update()
{
    ID = SDL_GetWindowID(ASC_WINDOW);
    SDL_GetWindowPosition(ASC_WINDOW, &x, &y);
    SDL_GetWindowSize(ASC_WINDOW, &w, &h);
    Uint32 Flags = SDL_GetWindowFlags(ASC_WINDOW);
    if(Flags | SDL_WINDOW_SHOWN) shown = true;
    if(Flags | SDL_WINDOW_MINIMIZED) minimized = true;
    if(Flags | SDL_WINDOW_MAXIMIZED) maximized = true;
    if(Flags | SDL_WINDOW_MOUSE_FOCUS) mouse_focus = true;
    if(Flags | SDL_WINDOW_INPUT_FOCUS) kb_focus = true;
}

//==============
// WindowEvent
//==============
void InputHandler::WindowEvent(Uint32 _ID, Sint32 _data1, Sint32 _data2, Uint8 _event)
{
    window.Update();
}

//==============
// Keypress
//==============
bool InputHandler::Keypress(SDL_Keycode _key, InputHandler_Keypress _press)
{
    if(_press == ASC_KEYREPEAT)
    {
        return Keys[_key];
    }

    if(_press == ASC_ONKEYDOWN)
    {
        if( Keydowns.count(_key)==0 && Keys.count(_key)>0 )
        {
            Keydowns[_key] = 1;
            return true;
        }

        if(Keydowns.count(_key)>0)
        {
            return false;
        }
    }

    if(_press == ASC_ONKEYUP)
    {
        if( Keyups.count(_key)>0 )
        {
            Keyups.erase(_key);
            return true;
        }
    }

    return false;
}

//============
// Mousepress
//============
bool InputHandler::Mousepress(Uint8 _button, InputHandler_Keypress _press)
{
    if(_press == ASC_KEYREPEAT)
    {
        return mouse.button[_button];
    }

    if(_press == ASC_ONKEYDOWN)
    {
        if( Mousedowns.count(_button)==0 && mouse.button[_button] )
        {
            Mousedowns[_button] = 1;
            return true;
        }

        if(Mousedowns.count(_button)>0)
        {
            return false;
        }
    }

    if(_press == ASC_ONKEYUP)
    {
        if( Mouseups.count(_button)>0 )
        {
            Mouseups.erase(_button);
            return true;
        }
    }

    return false;
}


//========
// Frame
//========
void InputHandler::PostFrameCleanup()
{
    // clear keyups which are older than 10 frames
	std::map<SDL_Keycode, bool>::iterator it = Keyups.begin();
	while (it != Keyups.end())
    {
        it->second = it->second + 1;
		if (it->second >= 10) it = Keyups.erase(it);
        else it++;
	}

    std::map<Uint8, bool>::iterator it2 = Mouseups.begin();
	while (it2 != Mouseups.end())
    {
        it2->second = it2->second + 1;
		if (it2->second >= 10) it2 = Mouseups.erase(it2);
        else it2++;
	}
}
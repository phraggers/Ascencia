/*============================================================
 * Project: Ascencia
 * File: window.h
 * Author: Phraggers
 * Date: Sun May 12 2024
============================================================*/

#ifdef ASC_HEAD

#define ASC_NUM_MOUSE_BUTTONS 5
#define ASC_NUM_GAMEPADS 4
#define ASC_NUM_GAMEPAD_BUTTONS 14

typedef struct {r32 x,y;} rv2;
typedef struct {i32 x,y;} iv2;

typedef struct
{
    bool isDown;
    bool wasDown;
    bool downTick;
    bool upTick;
    u64 perfDown;
} ASC_ButtonState;

struct ASC_Mouse
{
    rv2 pos;
    r32 wheel; // positive = up, negative = down

    union
    {
        struct {ASC_ButtonState left, middle, right, x1, x2;};
        ASC_ButtonState buttons[ASC_NUM_MOUSE_BUTTONS];
    } buttons;
};

struct ASC_Gamepad
{
    bool isConnected;
    SDL_Gamepad *handle;

    union
    {
        struct {ASC_ButtonState a, b, x, y, ls, rs, l3, r3, 
                start, back, up, down, left, right;};
        ASC_ButtonState buttons[ASC_NUM_GAMEPAD_BUTTONS];
    } buttons;

    r32 lt, rt;
    rv2 lstick, rstick;
};

struct ASC_Input
{
    ASC_ButtonState keyboard[SDL_NUM_SCANCODES];
    ASC_Mouse mouse;
    ASC_Gamepad gamepads[ASC_NUM_GAMEPADS];
};

struct ASC_Window
{
    ASC_Input input;

    SDL_Window *handle;
    SDL_GLContext glcontext;

    bool hasFocus;
    bool isFullscreen;
};

static bool ASC_WindowInit(void);
static bool ASC_WindowHandleEvents(void);
static bool ASC_WindowUpdateEvents(void);
static void ASC_ProcessGamepadButton(SDL_Gamepad *gamepad, ASC_ButtonState *button, SDL_GamepadButton sdlbutton);
static bool ASC_WindowUpdateRender(void);
static bool ASC_WindowSetFullscreen(bool fullscreen);
static bool ASC_WindowToggleFullscreen(void);
static bool ASC_WindowSetVSync(bool vsync);
static bool ASC_WindowToggleVSync(void);
static void ASC_WindowSaveConfig(void);

#endif

/*===================================================================================*/

#ifdef ASC_IMPL

static bool ASC_WindowInit(void)
{
    ASC_Config *config = &gApp->config;
    ASC_Window *window = &gApp->window;

    if(SDL_Init(SDL_INIT_EVENTS | SDL_INIT_GAMEPAD | SDL_INIT_JOYSTICK | SDL_INIT_TIMER | SDL_INIT_VIDEO))
    {
        ASC_Error("ASC_WindowInit: SDL_Init error: %s", SDL_GetError());
        return 0;
    }
    else
    {
        SDL_Version ver = {0};
        SDL_GetVersion(&ver);
        ASC_InfoLog("ASC_WindowInit: Loaded SDL [%d.%d.%d]", ver.major, ver.minor, ver.patch);
    }

    if(!IMG_Init(IMG_INIT_PNG))
    {
        ASC_Error("ASC_WindowInit: IMG_Init error: %s", IMG_GetError());
        return 0;
    }
    else
    {
        SDL_Version ver = {0};
        ver.major = SDL_IMAGE_MAJOR_VERSION;
        ver.minor = SDL_IMAGE_MINOR_VERSION;
        ver.patch = SDL_IMAGE_MICRO_VERSION;
        ASC_InfoLog("ASC_WindowInit: Loaded IMG [%d.%d.%d]", ver.major, ver.minor, ver.patch);
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, config->gl_ver_maj);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, config->gl_ver_min);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, config->vsync);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, config->gl_multisampling);

    window->handle = SDL_CreateWindow(config->app_name, config->window_width, config->window_height, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
    if(!window->handle)
    {
        ASC_Error("ASC_WindowInit: Create Window error: %s", SDL_GetError());
        return 0;
    }
    else
    {
        ASC_DebugLog("ASC_WindowInit: Window created");
    }

    window->glcontext = SDL_GL_CreateContext(window->handle);
    if(!window->glcontext)
    {
        ASC_Error("ASC_WindowInit: Error creating GLContext: %s", SDL_GetError());
        return 0;
    }
    else
    {
        ASC_DebugLog("ASC_WindowInit: SDLGLContext Created");
    }

    if(SDL_GL_MakeCurrent(window->handle, window->glcontext))
    {
        ASC_Error("ASC_WindowInit: Error setting GLContext: %s", SDL_GetError());
        return 0;
    }
    else
    {
        ASC_DebugLog("ASC_WindowInit: SDLGLContext set to window");
    }

    if(!ASC_LoadGLFunctions(config->gl_ver_maj, config->gl_ver_min))
    {
        return 0;
    }
    else
    {
        ASC_DebugLog("ASC_WindowInit: OpenGL functions loaded");
    }

    SDL_GL_SetSwapInterval(config->vsync);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    window->hasFocus = 1;

    if(config->fullscreen)
    {
        SDL_SetWindowSize(window->handle, CONFIG_DEFAULT_WINDOW_WIDTH, CONFIG_DEFAULT_WINDOW_HEIGHT);
        SDL_SetWindowPosition(window->handle, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
        ASC_WindowSetFullscreen(1);
    }

    return 1;
}

static bool ASC_WindowHandleEvents(void)
{
    ASC_Window *window = &gApp->window;
    SDL_Event event;
    
    while(SDL_PollEvent(&event))
    {
        switch(event.type)
        {
            case SDL_EVENT_QUIT:
            case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
            case SDL_EVENT_WINDOW_DESTROYED:
            {
                ASC_InfoLog("ASC_WindowHandleEvents: Window Closed");
                return 0;
            } break;

            case SDL_EVENT_KEY_DOWN:
            {
                ASC_ButtonState *key = &window->input.keyboard[event.key.keysym.scancode];
                key->isDown = 1;
                if(!key->wasDown)
                {
                    key->downTick = 1;
                    key->perfDown = SDL_GetPerformanceCounter();
                    ASC_DebugLog("KeyDownEvent: KeyCode[%u]", event.key.keysym.scancode);
                }
            } break;

            case SDL_EVENT_KEY_UP:
            {
                ASC_ButtonState *key = &window->input.keyboard[event.key.keysym.scancode];
                ASC_DebugLog("KeyUpEvent: KeyCode[%u] Duration[%.04fs]", event.key.keysym.scancode, ASC_TimerPerfDelta(key->perfDown));
                key->isDown = 0;
                key->upTick = 1;
                key->perfDown = 0;
            } break;

            case SDL_EVENT_MOUSE_BUTTON_DOWN:
            {
                ASC_ButtonState *btn = 0;

                switch(event.button.button)
                {
                    case SDL_BUTTON_LEFT:
                    btn = &window->input.mouse.buttons.left; break;
                    case SDL_BUTTON_MIDDLE:
                    btn = &window->input.mouse.buttons.middle; break;
                    case SDL_BUTTON_RIGHT:
                    btn = &window->input.mouse.buttons.right; break;
                    case SDL_BUTTON_X1:
                    btn = &window->input.mouse.buttons.x1; break;
                    case SDL_BUTTON_X2:
                    btn = &window->input.mouse.buttons.x2; break;
                }

                if(btn)
                {
                    btn->isDown = 1;
                    if(!btn->wasDown)
                    {
                        btn->downTick = 1;
                        btn->perfDown = SDL_GetPerformanceCounter();
                        ASC_DebugLog("MouseBtnDownEvent: Button[%u]", event.button.button);
                    }
                }

            } break;

            case SDL_EVENT_MOUSE_BUTTON_UP:
            {
                ASC_ButtonState *btn = 0;

                switch(event.button.button)
                {
                    case SDL_BUTTON_LEFT:
                    btn = &window->input.mouse.buttons.left; break;
                    case SDL_BUTTON_MIDDLE:
                    btn = &window->input.mouse.buttons.middle; break;
                    case SDL_BUTTON_RIGHT:
                    btn = &window->input.mouse.buttons.right; break;
                    case SDL_BUTTON_X1:
                    btn = &window->input.mouse.buttons.x1; break;
                    case SDL_BUTTON_X2:
                    btn = &window->input.mouse.buttons.x2; break;
                }

                if(btn)
                {
                    ASC_DebugLog("MouseBtnUpEvent: Button[%u] Duration[%.04fs]", event.button.button, ASC_TimerPerfDelta(btn->perfDown));
                    btn->isDown = 0;
                    btn->upTick = 1;
                    btn->perfDown = 0;
                }
            } break;

            case SDL_EVENT_MOUSE_MOTION:
            {
                SDL_GetMouseState(&window->input.mouse.pos.x, &window->input.mouse.pos.y);
                ASC_DebugLog("MouseMotionEvent: Position[%g,%g]", window->input.mouse.pos.x, window->input.mouse.pos.y);
            } break;

            case SDL_EVENT_MOUSE_WHEEL:
            {
                ASC_DebugLog("MouseWheelEvent: %g", event.wheel.y);
                window->input.mouse.wheel = event.wheel.y;
            } break;

            case SDL_EVENT_WINDOW_EXPOSED:
            case SDL_EVENT_WINDOW_FOCUS_GAINED:
            case SDL_EVENT_WINDOW_RESTORED:
            case SDL_EVENT_WINDOW_SHOWN:
            case SDL_EVENT_WINDOW_TAKE_FOCUS:
            {
                ASC_DebugLog("WindowEvent: Window has focus");
                window->hasFocus = 1;
            } break;

            case SDL_EVENT_WINDOW_HIDDEN:
            case SDL_EVENT_WINDOW_MINIMIZED:
            case SDL_EVENT_WINDOW_FOCUS_LOST:
            case SDL_EVENT_WINDOW_OCCLUDED:
            {
                ASC_DebugLog("WindowEvent: Window does not have focus");
                window->hasFocus = 0;
            } break;
        }
     }

     // a new SDL joystickID is applied to a gamepad that is connected,
     // even if that same gamepad was previously connected.
     // what I'm doing here is storing my own GamepadID (as index into the gamepad array),
     // checking if the gamepad is assigned, else it is disconnected so can be cleared,
     // then when a gamepad is connected, assign the first available slot, as if it was a new player.
     // If I wanted to actually handle multiple players, eg split-screen local multiplayer,
     // then this would need more robust handling and logic.

     // tbh using Xinput is much easier than this, but hey-ho SDL is great at everything else
     // (and xinput is windows only)
     
     bool gamepadHandled[ASC_NUM_GAMEPADS]; // using this to check if it is connected
     
     int numJoysticks;
     u32 *joystickIDs = SDL_GetJoysticks(&numJoysticks); //array of connected IDs

     if(joystickIDs)
     {
         for(int gamepadIndex = 0;
         gamepadIndex < numJoysticks;
         gamepadIndex++)
         {
            gamepadHandled[gamepadIndex] = 1;
            ASC_Gamepad *gamepad = &window->input.gamepads[gamepadIndex];

             if(!gamepad->handle)
             {
                SDL_Joystick *joystick = 0;
                joystick = SDL_OpenJoystick(gamepadIndex);
                gamepad->handle = SDL_OpenGamepad(joystickIDs[gamepadIndex]);
                gamepad->isConnected = 1;
                SDL_SetGamepadPlayerIndex(gamepad->handle, gamepadIndex);
                ASC_DebugLog("Gamepad Connected: %d", gamepadIndex);
             }

             gamepad->lt = ASC_norm32((r32)SDL_GetGamepadAxis(gamepad->handle, SDL_GAMEPAD_AXIS_LEFT_TRIGGER), 0.0f, 32767.0f);
             gamepad->rt = ASC_norm32((r32)SDL_GetGamepadAxis(gamepad->handle, SDL_GAMEPAD_AXIS_RIGHT_TRIGGER), 0.0f, 32767.0f);

             gamepad->lstick.x = (ASC_norm32((r32)SDL_GetGamepadAxis(gamepad->handle, SDL_GAMEPAD_AXIS_LEFTX), -32768.0f, 32767.0f) * 2.0f) -1.0f;
             gamepad->lstick.y = (ASC_norm32((r32)SDL_GetGamepadAxis(gamepad->handle, SDL_GAMEPAD_AXIS_LEFTY), -32768.0f, 32767.0f) * 2.0f) -1.0f;

             gamepad->rstick.x = (ASC_norm32((r32)SDL_GetGamepadAxis(gamepad->handle, SDL_GAMEPAD_AXIS_RIGHTX), -32768.0f, 32767.0f) * 2.0f) -1.0f;
             gamepad->rstick.y = (ASC_norm32((r32)SDL_GetGamepadAxis(gamepad->handle, SDL_GAMEPAD_AXIS_RIGHTY), -32768.0f, 32767.0f) * 2.0f) -1.0f;

             ASC_ProcessGamepadButton(gamepad->handle, &gamepad->buttons.a, SDL_GAMEPAD_BUTTON_SOUTH);
             ASC_ProcessGamepadButton(gamepad->handle, &gamepad->buttons.b, SDL_GAMEPAD_BUTTON_EAST);
             ASC_ProcessGamepadButton(gamepad->handle, &gamepad->buttons.x, SDL_GAMEPAD_BUTTON_WEST);
             ASC_ProcessGamepadButton(gamepad->handle, &gamepad->buttons.y, SDL_GAMEPAD_BUTTON_NORTH);
             ASC_ProcessGamepadButton(gamepad->handle, &gamepad->buttons.ls, SDL_GAMEPAD_BUTTON_LEFT_SHOULDER);
             ASC_ProcessGamepadButton(gamepad->handle, &gamepad->buttons.rs, SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER);
             ASC_ProcessGamepadButton(gamepad->handle, &gamepad->buttons.l3, SDL_GAMEPAD_BUTTON_LEFT_STICK);
             ASC_ProcessGamepadButton(gamepad->handle, &gamepad->buttons.r3, SDL_GAMEPAD_BUTTON_RIGHT_STICK);
             ASC_ProcessGamepadButton(gamepad->handle, &gamepad->buttons.start, SDL_GAMEPAD_BUTTON_START);
             ASC_ProcessGamepadButton(gamepad->handle, &gamepad->buttons.back, SDL_GAMEPAD_BUTTON_BACK);
             ASC_ProcessGamepadButton(gamepad->handle, &gamepad->buttons.up, SDL_GAMEPAD_BUTTON_DPAD_UP);
             ASC_ProcessGamepadButton(gamepad->handle, &gamepad->buttons.down, SDL_GAMEPAD_BUTTON_DPAD_DOWN);
             ASC_ProcessGamepadButton(gamepad->handle, &gamepad->buttons.left, SDL_GAMEPAD_BUTTON_DPAD_LEFT);
             ASC_ProcessGamepadButton(gamepad->handle, &gamepad->buttons.right, SDL_GAMEPAD_BUTTON_DPAD_RIGHT);
         }

         SDL_free(joystickIDs);
     }

     // disconnected gamepads
     for(int gamepadIndex=0;
     gamepadIndex < ASC_NUM_GAMEPADS;
     gamepadIndex++)
     {
        if(!gamepadHandled[gamepadIndex] && window->input.gamepads[gamepadIndex].isConnected)
        {
            SDL_CloseJoystick(SDL_GetGamepadJoystick(window->input.gamepads[gamepadIndex].handle));
            SDL_CloseGamepad(window->input.gamepads[gamepadIndex].handle);
            memset(&window->input.gamepads[gamepadIndex], 0, sizeof(ASC_Gamepad));
            ASC_DebugLog("Gamepad Disconnected: %d", gamepadIndex);
        }
     }
        
     return 1;
}

static bool ASC_WindowUpdateEvents(void)
{
    ASC_Window *window = &gApp->window;

    for(int keyIndex = 0;
    keyIndex < SDL_NUM_SCANCODES;
    keyIndex++)
    {
        ASC_ButtonState *key = &window->input.keyboard[keyIndex];

        if(key->isDown)
        {
            key->wasDown = 1;
        }
        else
        {
            if(key->wasDown)
            {
                key->wasDown = 0;
            }
        }

        key->downTick = 0;
        key->upTick = 0;
    }

    for(int btnIndex = 0;
    btnIndex < ASC_NUM_MOUSE_BUTTONS;
    btnIndex++)
    {
        ASC_ButtonState *btn = &window->input.mouse.buttons.buttons[btnIndex];

        if(btn->isDown)
        {
            btn->wasDown = 1;
        }
        else
        {
            if(btn->wasDown)
            {
                btn->wasDown = 0;
            }
        }

        btn->downTick = 0;
        btn->upTick = 0;
    }

    window->input.mouse.wheel = 0.0f;

    for(int gamepadIndex = 0;
    gamepadIndex < ASC_NUM_GAMEPADS;
    gamepadIndex++)
    {
        ASC_Gamepad *gamepad = &window->input.gamepads[gamepadIndex];

        if(gamepad->isConnected)
        {
            for(int buttonIndex = 0;
            buttonIndex < ASC_NUM_GAMEPAD_BUTTONS;
            buttonIndex++)
            {
                if(gamepad->buttons.buttons[buttonIndex].isDown)
                {
                    gamepad->buttons.buttons[buttonIndex].wasDown = 1;
                }
                else
                {
                    if(gamepad->buttons.buttons[buttonIndex].wasDown)
                    {
                        gamepad->buttons.buttons[buttonIndex].wasDown = 0;
                    }
                }
                
                gamepad->buttons.buttons[buttonIndex].downTick = 0;
                gamepad->buttons.buttons[buttonIndex].upTick = 0;
            }
        }
    }

    return 1;
}

static void ASC_ProcessGamepadButton(SDL_Gamepad *gamepad, ASC_ButtonState *button, SDL_GamepadButton sdlbutton)
{
    button->isDown = SDL_GetGamepadButton(gamepad, sdlbutton);

    if(button->isDown)
    {
        if(!button->wasDown) 
        {
            button->downTick = 1;
            button->perfDown = SDL_GetPerformanceCounter();
            ASC_DebugLog("GamepadButtonEvent: GamepadID[%d] Button[%u] : isDown", SDL_GetGamepadPlayerIndex(gamepad), sdlbutton);
        }
    }
    else
    {
        if(button->wasDown)
        {
            ASC_DebugLog("GamepadButtonEvent: GamepadID[%d] Button[%u] Duration[%.04fs] : isUp", SDL_GetGamepadPlayerIndex(gamepad), sdlbutton, ASC_TimerPerfDelta(button->perfDown));
            button->upTick = 1;
            button->perfDown = 0;
        }
    }
}

static bool ASC_WindowUpdateRender(void)
{
    SDL_GL_SwapWindow(gApp->window.handle);
    return 1;
}

static bool ASC_WindowSetFullscreen(bool fullscreen)
{
    ASC_Window *window = &gApp->window;

    if(fullscreen)
    {
        if(window->isFullscreen)
        {
            return 1;
        }

        else
        {
            SDL_SetWindowFullscreen(window->handle, 1);
            window->isFullscreen = 1;
            return 1;
        }
    }

    else
    {
        if(window->isFullscreen)
        {
            SDL_SetWindowFullscreen(window->handle, 0);
            window->isFullscreen = 0;
            return 0;
        }

        else
        {
            return 0;
        }
    }
}

static bool ASC_WindowToggleFullscreen(void)
{
    ASC_Window *window = &gApp->window;

    if(window->isFullscreen)
    {
        SDL_SetWindowFullscreen(window->handle, 0);
        window->isFullscreen = 0;
        return 0;
    }

    else
    {
        SDL_SetWindowFullscreen(window->handle, 1);
        window->isFullscreen = 1;
        return 1;
    }
}

static bool ASC_WindowSetVSync(bool vsync)
{
    if(vsync)
    {
        if(gApp->config.vsync)
        {
            return 1;
        }

        else
        {
            SDL_GL_SetSwapInterval(1);
            gApp->config.vsync = 1;
            return 1;
        }
    }

    else
    {
        if(gApp->config.vsync)
        {
            SDL_GL_SetSwapInterval(0);
            gApp->config.vsync = 0;
            return 0;
        }

        else
        {
            return 0;
        }
    }
}

static bool ASC_WindowToggleVSync(void)
{
    if(gApp->config.vsync)
    {
        SDL_GL_SetSwapInterval(0);
        gApp->config.vsync = 0;
        return 0;
    }

    else
    {
        SDL_GL_SetSwapInterval(1);
        gApp->config.vsync = 1;
        return 1;
    }
}

static void ASC_WindowSaveConfig(void)
{
    ASC_Window *window = &gApp->window;
    ASC_Config *config = &gApp->config;
    SDL_GetWindowSize(window->handle, &config->window_width, &config->window_height);
    config->fullscreen = window->isFullscreen;
}

#endif //WINDOW_H_
/* ========================================================================
   $File:    Ascencia [Source] $
   $Project: Ascencia $
   $Date:    08-09-2021 $
   $Author:  Phil Bagshaw $
   $Notice:  (c)Phragware 2021 $
   ======================================================================== */

#include <glad.c>
#include <SDL/SDL.h>
#include "Ascencia.h"
#include "Events.cpp"

//TODO: https://raw.githubusercontent.com/gabomdq/SDL_GameControllerDB/master/gamecontrollerdb.txt
// SDL_GameControllerAddMappingsFromFile("gamecontrollerdb.txt"); (alt: from char*, from RWops)
// Community sourced SDL Controller config, maybe use Curl to download it (it gets updated)

int
main(int argc, char **argv)
{
    int ExitCode = 0;
    
    Application = {};
    Application.Dimension.x = SDL_WINDOWPOS_UNDEFINED;
    Application.Dimension.y = SDL_WINDOWPOS_UNDEFINED;
    Application.Dimension.w = 1280;
    Application.Dimension.h = 720;

    if(!SDL_Init(SDL_INIT_EVERYTHING))
    {
        SDL_version SDLVersion_Loaded;
        SDL_version SDLVersion_Compiled;
        SDL_GetVersion(&SDLVersion_Loaded);
        SDL_VERSION(&SDLVersion_Compiled);
        SDL_Log("Loaded SDL Version: %d.%d.%d", SDLVersion_Loaded.major, SDLVersion_Loaded.minor, SDLVersion_Loaded.patch);
        SDL_Log("Compiled SDL Version: %d.%d.%d", SDLVersion_Compiled.major, SDLVersion_Compiled.minor, SDLVersion_Compiled.patch);

        if(SDLVersion_Loaded.major < SDLVersion_Compiled.major ||
           SDLVersion_Loaded.minor < SDLVersion_Compiled.minor ||
           SDLVersion_Loaded.patch < SDLVersion_Compiled.patch - 5)
        {
            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING, "Warning!", "An older version of sdl2.dll is loaded.\nApplication may behave unexpectedly!", 0);
        }

        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
        SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

        Application.Window = SDL_CreateWindow("Ascencia",
                                              Application.Dimension.x,
                                              Application.Dimension.y,
                                              Application.Dimension.w,
                                              Application.Dimension.h,
                                              SDL_WINDOW_RESIZABLE |
                                              SDL_WINDOW_SHOWN |
                                              SDL_WINDOW_OPENGL |
                                              SDL_WINDOW_ALLOW_HIGHDPI);

        if(Application.Window)
        {
            SDL_SetWindowMinimumSize(Application.Window, 16*30, 9*30); //16:9
            Application.GLContext = SDL_GL_CreateContext(Application.Window);
            SDL_GL_MakeCurrent(Application.Window, Application.GLContext);
            SDL_GL_SetSwapInterval(1);

            if(gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
            {
                SDL_Log("OpenGL Version Loaded: %d.%d", GLVersion.major, GLVersion.minor);
                
                Memory = {};
                Memory.PersistantSize = Megabytes(64);
                Memory.TransientSize = Gigabytes(1);
                Memory.Persistant = SDL_malloc((size_t)(Memory.PersistantSize +
                                                        Memory.TransientSize));
                Memory.Transient = ((uint8*)Memory.Persistant + Memory.PersistantSize);

                if(Memory.Persistant && Memory.Transient)
                {
                    Memory.Initialized = 1;
                    Application.Running = 1;

                    SDL_GameController *Controller = 0;                    

                    //NOTE: FrameTiming
                    uint32 Timer_TotalFrameTicks = 0;
                    uint32 Timer_TotalFrames = 0;
                    uint32 Timer_StartTicks = SDL_GetTicks();
                    uint32 Timer_10SecTotalFrameTicks = 0;
                    uint32 Timer_10SecTotalFrames = 0;
                    uint32 Timer_10SecStartTicks = SDL_GetTicks();

                    while(Application.Running)
                    {
                        Timer_10SecTotalFrames++;
                        Timer_TotalFrames++;

                        uint32 Cap_StartTicks = SDL_GetTicks();
                        uint64 Timer_StartPerf = SDL_GetPerformanceCounter();

                        ASC_HandleEvents();

                        //NOTE: Reset Controller if disconnected & reconnected
                        if(!SDL_GameControllerGetAttached(Controller))
                        {
                            SDL_GameControllerClose(Controller);
                            Controller = 0;
                        }

                        if(!Controller)
                        {
                            for(int iJoystick = 0; iJoystick < SDL_NumJoysticks(); ++iJoystick)
                            {
                                if(SDL_IsGameController(iJoystick))
                                {
                                    Controller = SDL_GameControllerOpen(iJoystick);
                                    if(!Controller)
                                    {
                                        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
                                                                 "Failed to open Controller",
                                                                 SDL_GetError(), 0);
                                    }
                                }
                            }
                        }

                        //NOTE: Controller
                        if(SDL_GameControllerGetButton(Controller, SDL_CONTROLLER_BUTTON_DPAD_UP))
                        {
                            // button pressed
                        }
                            

                        //NOTE: Keyboard
                        if(ASC_Keydown('w') || ASC_Keydown(SDLK_UP))
                        {
                            // key pressed
                        }

                        if(ASC_KeySingle(SDLK_F12))
                        {
                            SDL_SetError("SDL_Version(Compiled): %d.%d.%d\nSDL_Version(Loaded): %d.%d.%d\nOpenGL_Version(Loaded): %d.%d\n\n(c)Phragware 2021\nAll Rights Reserved.", SDLVersion_Compiled.major, SDLVersion_Compiled.minor, SDLVersion_Compiled.patch, SDLVersion_Loaded.major, SDLVersion_Loaded.minor, SDLVersion_Loaded.patch, GLVersion.major, GLVersion.minor);
                            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "About Ascencia", SDL_GetError(), 0);
                            SDL_ClearError();
                            
                        }

                        //NOTE: Mouse
                        if(Application.MouseInWindow)
                        {
                            if(Application.MouseLeft)
                            {
                                // left clicked
                            }

                            else if(Application.MouseRight)
                            {
                                // right clicked
                            }
                        }

                        // NOTE: Render
                        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

                        // push draws to backbuffer ...
                        

                        SDL_GL_SwapWindow(Application.Window);

                        // NOTE: FrameTiming
                        uint32 Timer_FrameTicks = SDL_GetTicks() - Timer_StartTicks;
                        Timer_TotalFrameTicks += Timer_FrameTicks;
                        real32 Timer_AvgFPS = Timer_TotalFrames / (Timer_FrameTicks / 1000.0f);

                        uint32 Timer_10SecFrameTicks = SDL_GetTicks() - Timer_10SecStartTicks;
                        Timer_10SecTotalFrameTicks += Timer_10SecFrameTicks;
                        real32 Timer_10SecAvgFPS = Timer_10SecTotalFrames / (Timer_10SecFrameTicks
                                                                             / 1000.0f);

                        uint64 Timer_EndPerf = SDL_GetPerformanceCounter();
                        uint64 Timer_FramePerf = Timer_EndPerf - Timer_StartPerf;

                        if(Timer_10SecTotalFrameTicks >= 10000)
                        {
                            Timer_10SecTotalFrameTicks = 0;
                            Timer_10SecTotalFrames = 0;
                            Timer_10SecStartTicks = SDL_GetTicks();
                        }

                        uint32 Cap_FrameTicks = SDL_GetTicks() - Cap_StartTicks;
                        real32 Cap_DesiredTicksPerFrame = 1000.0f/60.0f;
                        if(Cap_FrameTicks < Cap_DesiredTicksPerFrame)
                        {
                            SDL_Delay((uint32)(Cap_DesiredTicksPerFrame - Cap_FrameTicks));
                        }


                    } // while(running)
                } // if(memory)

                else //NOTE: !Memory.Persistant || !Memory.Transient
                {
                    SDL_SetError("Failed to reserve Application Memory! (PersistantSize: %d, TransientSize: %d)", Memory.PersistantSize, Memory.TransientSize);
                    SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, SDL_GetError());
                    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Fatal Error!",
                                             SDL_GetError(), 0);
                    ExitCode = 4;
                }
            } // if(gladLoadGLLoader)

            else //NOTE: Glad Load error
            {
                SDL_SetError("Failed to Load OpenGL!");
                SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, "gladLoadGLLoader failed!");
                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Fatal Error!",
                                         "GLAD Failed to initialize OpenGL Context!", 0);

                ExitCode = 3;
            }
        }

        else //NOTE: SDL_CreateWindow error
        {
            SDL_SetError("Failed to create Window! %s", SDL_GetError());
            SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, SDL_GetError());
            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Fatal Error!",
                                     SDL_GetError(), 0);
            ExitCode = 2;
        }
    }

    else //NOTE: SDL_Init error
    {
        SDL_SetError("Failed to Init SDL! %s", SDL_GetError());
        SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, SDL_GetError());
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Fatal Error!",
                                 SDL_GetError(), 0);
        ExitCode = 1;
    }

    SDL_Quit();
    return 0;
}

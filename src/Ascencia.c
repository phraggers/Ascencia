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
#include "Memory.h"
#include "Events.h"

//TODO: https://raw.githubusercontent.com/gabomdq/SDL_GameControllerDB/master/gamecontrollerdb.txt
// SDL_GameControllerAddMappingsFromFile("gamecontrollerdb.txt"); (alt: from char*, from RWops)
// Community sourced SDL Controller config, maybe use Curl to download it (it gets updated)

int
main(int argc, char **argv)
{
    int ExitCode = 0;

    SDL_memset(&Application, 0, sizeof(Application));
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
                glViewport(0,0, Application.Dimension.w, Application.Dimension.h);
                SDL_Log("OpenGL Version Loaded: %d.%d", GLVersion.major, GLVersion.minor);

                SDL_memset(&Memory, 0, sizeof(Memory));
                Memory.Size = Gigabytes(2);
                Memory.Memory = SDL_malloc((size_t)Memory.Size);
                Memory.Pointer = (uint64)Memory.Memory;

                if(Memory.Memory)
                {
                    struct Asc_MemoryBlock *PMem = ASC_NewMemoryBlock(Megabytes(64),
                                                                      ASC_MEM_STATIC);
                    Assert(PMem);
                    struct Asc_MemoryBlock *DMem = ASC_NewMemoryBlock(Megabytes(64),
                                                                      ASC_MEM_DYNAMIC);
                    Assert(DMem);
                    struct Asc_MemoryBlock *TMem = ASC_NewMemoryBlock(Gigabytes(1),
                                                                      ASC_MEM_ROLLOVER);
                    Assert(TMem);

                    int *Test = (int*)ASC_Alloc(DMem, sizeof(int));
                    Assert(Test);
                    *Test = 0x12345678;
                    ASC_Free(DMem, Test, sizeof(int));

                    // TEST: OPENGL
                    const char *vertexShaderSource = "#version 330 core\n"
                        "layout (location = 0) in vec3 aPos;\n"
                        "void main()\n"
                        "{\n"
                        "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                        "}\0";
                    const char *fragmentShaderSource = "#version 330 core\n"
                        "out vec4 FragColor;\n"
                        "void main()\n"
                        "{\n"
                        "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
                        "}\n\0";

                    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
                    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
                    glCompileShader(vertexShader);
                    int success;
                    char infoLog[512];
                    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
                    if (!success)
                    {
                        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
                        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Shader Error!", infoLog, 0);
                    }
                    // fragment shader
                    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
                    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
                    glCompileShader(fragmentShader);
                    // check for shader compile errors
                    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
                    if (!success)
                    {
                        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
                        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Shader Error!", infoLog, 0);
                    }

                    unsigned int shaderProgram = glCreateProgram();
                    glAttachShader(shaderProgram, vertexShader);
                    glAttachShader(shaderProgram, fragmentShader);
                    glLinkProgram(shaderProgram);
                    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
                    if (!success) {
                        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
                        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Shader Error!", infoLog, 0);
                    }
                    glDeleteShader(vertexShader);
                    glDeleteShader(fragmentShader);

                    float vertices[] = {
                      -0.5f, -0.5f, 0.0f, // left
                      0.5f, -0.5f, 0.0f, // right
                      0.0f,  0.5f, 0.0f  // top
                    };

                    unsigned int VBO, VAO;
                    glGenVertexArrays(1, &VAO);
                    glGenBuffers(1, &VBO);
                    glBindVertexArray(VAO);
                    glBindBuffer(GL_ARRAY_BUFFER, VBO);
                    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
                    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
                    glEnableVertexAttribArray(0);
                    glBindBuffer(GL_ARRAY_BUFFER, 0);
                    glBindVertexArray(0);

                    Application.Running = 1;
                    SDL_GameController *Controller = 0;
                    SDL_memset(&Timer, 0, sizeof(struct Asc_Timer));
                    Timer.Total.StartTicks = SDL_GetTicks();
                    Timer.Avg.StartTicks = SDL_GetTicks();
                    Timer.Frame.DesiredTicksPerFrame = 1000.0f/60.0f;
                    Timer.Avg.SampleTicks = 10000;

                    while(Application.Running)
                    {
                        Timer.Total.Frames++;
                        Timer.Avg.Frames++;
                        Timer.Frame.StartTicks = SDL_GetTicks();
                        Timer.Frame.StartPerf = SDL_GetPerformanceCounter();

                        ASC_HandleEvents();

                        //NOTE: Reset Controller if disconnected & reconnected
                        if(Controller && !SDL_GameControllerGetAttached(Controller))
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
                        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
                        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

                        glUseProgram(shaderProgram);
                        glBindVertexArray(VAO);
                        glDrawArrays(GL_TRIANGLES, 0, 3);

                        SDL_GL_SwapWindow(Application.Window);

                        // NOTE: FrameTiming
                        Timer.Frame.Ticks = SDL_GetTicks() - Timer.Frame.StartTicks;
                        Timer.Total.Ticks += SDL_GetTicks() - Timer.Total.StartTicks;
                        Timer.Avg.Ticks += SDL_GetTicks() - Timer.Avg.StartTicks;
                        Timer.Frame.EndPerf = SDL_GetPerformanceCounter() - Timer.Frame.StartPerf;

                        Timer.Total.AvgFPS = (real32)Timer.Total.Frames /
                            (((real32)SDL_GetTicks() - (real32)Timer.Total.StartTicks) / 1000.0f);
                        Timer.Avg.AvgFPS = (real32)Timer.Avg.Frames /
                            (((real32)SDL_GetTicks() - (real32)Timer.Avg.StartTicks) / 1000.0f);

                        if(Timer.Avg.Ticks >= Timer.Avg.SampleTicks)
                        {
                            Timer.Avg.Ticks = 0;
                            Timer.Avg.Frames = 0;
                            Timer.Avg.StartTicks = SDL_GetTicks();
                            SDL_Log("AvgFps(Total): %.02f, AvgFPS(Last10kFrames): %.02f",
                                    Timer.Total.AvgFPS, Timer.Avg.AvgFPS);
                        }

                        //NOTE: Framerate Limiter
                        if(Timer.Frame.Ticks < Timer.Frame.DesiredTicksPerFrame)
                        {
                            SDL_Delay((uint32)(Timer.Frame.DesiredTicksPerFrame -
                                               (real32)Timer.Frame.Ticks));
                        }


                    } // while(running)
                } // if(memory)

                else //NOTE: !Memory
                {
                    SDL_SetError("Failed to reserve Application Memory!");
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
    return ExitCode;
}

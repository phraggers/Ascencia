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
#include "Timer.h"
#include "Memory.h"
#include "Events.h"

//TODO: https://raw.githubusercontent.com/gabomdq/SDL_GameControllerDB/master/gamecontrollerdb.txt
// SDL_GameControllerAddMappingsFromFile("gamecontrollerdb.txt"); (alt: from char*, from RWops)
// Community sourced SDL Controller config, maybe use Curl to download it (it gets updated)

int
main(int argc, char **argv)
{
    int ExitCode = 0;

    ASC_TimerInit(60);
    ASC_TimerTStart(0);

    ZeroMem(Application);
    Application.Dimension.x = SDL_WINDOWPOS_UNDEFINED;
    Application.Dimension.y = SDL_WINDOWPOS_UNDEFINED;
    Application.Dimension.w = 1280;
    Application.Dimension.h = 720;
    Application.Running = 1;

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
            SDL_GL_SetSwapInterval(0);

            if(gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
            {
                glViewport(0,0, Application.Dimension.w, Application.Dimension.h);
                SDL_Log("OpenGL Version Loaded: %d.%d", GLVersion.major, GLVersion.minor);

                ZeroMem(Memory);
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
                    struct Asc_MemoryBlock *TMem = ASC_NewMemoryBlock(Megabytes(256),
                                                                      ASC_MEM_ROLLOVER);
                    Assert(TMem);

                    //TEST: mem test (stops compiler removing unused stuff)
                    int *Test = (int*)ASC_Alloc(DMem, sizeof(int));
                    Assert(Test);
                    *Test = 0x12345678;
                    ASC_Free(DMem, Test, sizeof(int));

                    // TEST: OPENGL
                    uint32 ShaderProgram = UINT32_MAX;
                    {
                        char *VertexShaderSource = "#version 330 core\n"
                            "layout (location = 0) in vec3 aPos;\n"
                            "void main()\n"
                            "{\n"
                            "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                            "}\0";
                        char *FragmentShaderSource = "#version 330 core\n"
                            "out vec4 FragColor;\n"
                            "void main()\n"
                            "{\n"
                            "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
                            "}\n\0";

                        uint32 VertexShader = glCreateShader(GL_VERTEX_SHADER);
                        glShaderSource(VertexShader, 1, &VertexShaderSource, 0);
                        glCompileShader(VertexShader);

                        int Success;
                        char InfoLog[512];
                        glGetShaderiv(VertexShader, GL_COMPILE_STATUS, &Success);

                        if(Success)
                        {
                            uint32 FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
                            glShaderSource(FragmentShader, 1, &FragmentShaderSource, 0);
                            glCompileShader(FragmentShader);
                            glGetShaderiv(FragmentShader, GL_COMPILE_STATUS, &Success);

                            if(Success)
                            {
                                ShaderProgram = glCreateProgram();
                                glAttachShader(ShaderProgram, VertexShader);
                                glAttachShader(ShaderProgram, FragmentShader);
                                glLinkProgram(ShaderProgram);
                                glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);

                                if(Success)
                                {
                                    glDeleteShader(VertexShader);
                                    glDeleteShader(FragmentShader);

                                }

                                else // shader program compile error
                                {
                                    glGetProgramInfoLog(ShaderProgram, 512, 0, InfoLog);
                                    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
                                                             "Shader Error!", InfoLog, 0);
                                }
                            }

                            else // fragment shader error
                            {
                                glGetShaderInfoLog(FragmentShader, 512, 0, InfoLog);
                                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
                                                         "Shader Error!", InfoLog, 0);
                            }
                        }

                        else //vertex shader error
                        {
                            glGetShaderInfoLog(VertexShader, 512, 0, InfoLog);
                            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
                                                     "Shader Error!", InfoLog, 0);
                        }
                    }

                    real32 Vertices[] =
                        {
                          0.5f, 0.5f, 0.0f, // top right
                          0.5f, -0.5f, 0.0f, // bottom right
                          -0.5f, -0.5f, 0.0f,  // bottom left
                          -0.5f, 0.5f, 0.0f // top left
                        };

                    uint32 Indices[] =
                        {
                          0,1,3, // first triangle
                          1,2,3 // second triangle
                        };

                    uint32 VBO, VAO, EBO;
                    glGenVertexArrays(1, &VAO);
                    glGenBuffers(1, &VBO);
                    glGenBuffers(1, &EBO);
                    glBindVertexArray(VAO);
                    glBindBuffer(GL_ARRAY_BUFFER, VBO);
                    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
                    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
                    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);
                    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
                    glEnableVertexAttribArray(0);
                    glBindBuffer(GL_ARRAY_BUFFER, 0);
                    glBindVertexArray(0);
                    GLenum PolygonModes[2] = {GL_FILL, GL_LINE};//, GL_POINT};
                    uint8 PolygonMode = 0;

                    SDL_GameController *Controller = 0;

                    SDL_Log("Startup took %.02f ms", ASC_TimerTEnd(0));

                    while(Application.Running)
                    {
                        ASC_TimerNewFrame();
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

                        if(ASC_KeySingle(SDLK_F10))
                        {
                            PolygonMode = ((PolygonMode+1 > ArrayCount(PolygonModes)-1)
                                           ? 0 : PolygonMode+1);
                            glPolygonMode(GL_FRONT_AND_BACK, PolygonModes[PolygonMode]);
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

                        if(ShaderProgram != UINT32_MAX) glUseProgram(ShaderProgram);
                        glBindVertexArray(VAO);
                        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
                        glBindVertexArray(0);

                        SDL_GL_SwapWindow(Application.Window);
                        ASC_TimerEndFrame();
                        ASC_TimerFrameLimit();

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

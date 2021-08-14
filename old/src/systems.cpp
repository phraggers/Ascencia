//================================
//          ASCENCIA CLIENT
//          systems.cpp
//================================
//        (c) Phragware 2019
//================================

#include <systems.h>

//==============
// Constructor
//==============
Systems::Systems()
{
    if(ASC_RELEASE == 0x00) Error.Set(1, "client_systems/Systems", "invalid G_RELEASE");
    if(ASC_VERSION == 0x00) Error.Set(1, "client_systems/Systems", "invalid G_VERSION");
    isInit = false;
    isQuit = false;
}

//===================
//  Destructor
//===================
Systems::~Systems()
{
    Quit();
}

//=======
// Start
//=======
void Systems::Start(int _argc, char **_argv)
{
    if(isInit) return;
    ProcessArgs(_argc, _argv);
    InitSDL();
    InitWindow();
    InitSDLRender();
    InitOpenGLRender();
    Input.Init();
    isInit = true;

    Game.Run();
}

//=======
// Quit
//=======
void Systems::Quit()
{
    if(isQuit) return;
    SDL_GL_DeleteContext(ASC_GLContext);
    SDL_DestroyRenderer(ASC_RENDERER);
    SDL_DestroyWindow(ASC_WINDOW);
    SDLNet_Quit();
    Mix_Quit();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

//===============
// ProcessArgs
//===============
void Systems::ProcessArgs(int _argc, char **_argv)
{
    for(int i=0; i<_argc; i++)
    {
        if(_argv[i] == "DEBUG_MODE") ASC_DEBUGMODE = 1;
    }
}

//================================
//    InitSDL
//================================
void Systems::InitSDL()
{
    if(isInit) return;

    if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        Error.Set(1, "client_systems/start", "SDL2_Main failed to initialize", SDL_GetError());
        return;
    }

    int IMG_Init_Flags = IMG_INIT_PNG;
    int IMG_Init_Code = IMG_Init(IMG_Init_Flags);

    if(IMG_Init_Code == 0)
    {
        Error.Set(1, "client_systems/start", "SDL2_IMG failed to initialize (startup error)", IMG_GetError());
        return;
    }

    if(IMG_Init_Code & IMG_Init_Flags != IMG_Init_Flags)
    {
        Error.Set(1, "client_systems/start", "SDL2_IMG failed to init requested filetype support", IMG_GetError());
        return;
    }

    if(TTF_Init() != 0)
    {
        Error.Set(1, "client_systems/start", "SDL2_TTF failed to initialize", TTF_GetError());
        return;
    }

    int Mix_Init_Flags = MIX_INIT_MP3;
    int Mix_Init_Code = Mix_Init(Mix_Init_Flags);

    if(Mix_Init_Code == 0)
    {
        Error.Set(1, "client_systems/start", "SDL2_Mix failed to initialize (startup error)", Mix_GetError());
        return;
    }

    if(Mix_Init_Code & Mix_Init_Flags != Mix_Init_Flags)
    {
        Error.Set(1, "client_systems/start", "SDL2_Mix failed to init requested filetype support", Mix_GetError());
        return;
    }

    if(SDLNet_Init() != 0)
    {
        Error.Set(1, "client_systems/start", "SDL2_Net failed to initialize", SDL_GetError());
        return;
    }
}

//================================
//    InitWindow
//================================
void Systems::InitWindow()
{
    if(isInit) return;

    ASC_WINDOW = SDL_CreateWindow(ASC_WINDOWTITLE.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, ASC_WINWIDTH, ASC_WINHEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

    if(ASC_WINDOW == NULL)
    {
        Error.Set(1, "client_systems/start", "SDL failed to create window", SDL_GetError());
        return;
    }
}

//================================
//    InitSDLRender
//================================
void Systems::InitSDLRender()
{
    if(isInit) return;

    ASC_RENDERER = SDL_CreateRenderer(ASC_WINDOW, -1, SDL_RENDERER_ACCELERATED);

    if(ASC_RENDERER == NULL)
    {
        Error.Set(1, "client_systems/start", "SDL failed to create renderer", SDL_GetError());
        return;
    }

    SDL_SetRenderDrawColor(ASC_RENDERER, 25, 50, 125, 255);
    SDL_RenderClear(ASC_RENDERER);
    SDL_RenderPresent(ASC_RENDERER);
}

//================================
//    InitOpenGLRender
//================================
void Systems::InitOpenGLRender()
{
    if(isInit) return;

    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, ASC_OGLMAJ );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, ASC_OGLMIN );
    ASC_GLContext = SDL_GL_CreateContext( ASC_WINDOW );

    if( ASC_GLContext == NULL )
    {
        Error.Set(1, "client_systems/start", "OpenGL context could not be created", SDL_GetError() );
        return;
    }

    // SDL_GL VSync: attempt -1 (adaptive) else 1 (synchronized), else error
    if( SDL_GL_SetSwapInterval( -1 ) == -1 )
    {
        if( SDL_GL_SetSwapInterval( 1 ) == -1 )
        {
            Error.Set(1, "client_systems/start", "OpenGL VSync failed", SDL_GetError());
        }
    }

    glClearColor(0.04f, 0.08f, 0.25f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    SDL_GL_SwapWindow(ASC_WINDOW);
}

//===============
// DebugMessage
//===============
void Systems::DebugMessage(std::string message1)
{
    if(!ASC_DEBUGMODE) return;
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Debug Message", message1.c_str(), NULL);
}

void Systems::DebugMessage(std::string message1, std::string message2)
{
    if(!ASC_DEBUGMODE) return;
    std::string message = message1;
    message += "\n";
    message += message2;
    DebugMessage(message);
}

void Systems::DebugMessage(std::string message1, std::string message2, std::string message3)
{
    if(!ASC_DEBUGMODE) return;
    std::string message = message1;
    message += "\n";
    message += message2;
    message += "\n";
    message += message3;
    DebugMessage(message);
}

void Systems::DebugMessageV(void *dump1)
{
    if(!ASC_DEBUGMODE) return;
    std::ostringstream o;
    o << dump1;
    DebugMessage(o.str());
}

void Systems::DebugMessageV(void *dump1, void *dump2)
{
    if(!ASC_DEBUGMODE) return;
    std::ostringstream o;
    o << dump1 << "\n" << dump2;
    DebugMessage(o.str());
}

void Systems::DebugMessageV(void *dump1, void *dump2, void *dump3)
{
    if(!ASC_DEBUGMODE) return;
    std::ostringstream o;
    o << dump1 << "\n" << dump2 << "\n" << dump3;
    DebugMessage(o.str());
}
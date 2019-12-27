//================================
//          ASCENCIA CLIENT
//          gamelogic.cpp
//================================
//        (c) Phragware 2019
//================================

#define ASC_GAMELOGIC_SYSTEMS
#include <systems.h>
#undef ASC_GAMELOGIC_SYSTEMS

//================================
//    Constructor
//================================
GameLogic::GameLogic()
{
   ChangeState = 0;
}

//================================
//    Destructor
//================================
GameLogic::~GameLogic()
{
    std::vector<std::string> vStates;

	if(States.size())
    {
        std::map<std::string, GameState*>::iterator it = States.begin();
        while (it != States.end())
        {
            vStates.push_back(it->first);
            it++;
        }
    }

    if(vStates.size())
    {
        for(int i=0; i<vStates.size(); i++)
        {
            delete States[vStates[i]];
            States.erase(vStates[i]);
        }
    }
}

//================================
//    Run
//================================
void GameLogic::Run()
{
    ASC_APPSTATE = "start";
    States[ASC_APPSTATE] = new GameState(ASC_APPSTATE, "TOP");

    while(ASC_APPSTATE != "quit")
    {
        FrameRate();
        HandleEvents();
        SetWindowTitle();
        RenderPreFrame();
        RenderFrame();
        RenderPostFrame();
        Input.PostFrameCleanup();
        CheckNewState();
    }
}

//============
// SetState
//============
void GameLogic::SetState(std::string _state)
{
    if(_state != ASC_APPSTATE)
    {
        NewState = _state;
        ChangeState = 1;
    }
    else return;
}

//=================
// FrameRate
//=================
void GameLogic::FrameRate()
{
    int Ticks = SDL_GetTicks();
    while ((SDL_GetTicks() - Ticks) < (1000 / ASC_FRAMERATE)) {}
}

//=================
// HandleEvents
//=================
void GameLogic::HandleEvents()
{
    SDL_Event SDLEvent;
    while (SDL_PollEvent(&SDLEvent))
    {
        if(SDLEvent.type == SDL_QUIT)
        {ASC_APPSTATE = "quit";
        return;}

        if(SDLEvent.type == SDL_KEYDOWN)
        Input.Keyboard(SDLEvent.key.keysym.sym, ASC_KEYDOWN);

        if(SDLEvent.type == SDL_KEYUP)
        Input.Keyboard(SDLEvent.key.keysym.sym, ASC_KEYUP);

        if(SDLEvent.type == SDL_MOUSEMOTION)
        Input.MouseMotion();

        if(SDLEvent.type == SDL_MOUSEBUTTONDOWN)
        Input.MouseButton(SDLEvent.button.button, ASC_KEYDOWN);

        if(SDLEvent.type == SDL_MOUSEBUTTONUP)
        Input.MouseButton(SDLEvent.button.button, ASC_KEYUP);

        if(SDLEvent.type == SDL_WINDOWEVENT)
        Input.WindowEvent(SDLEvent.window.windowID, SDLEvent.window.data1, SDLEvent.window.data2, SDLEvent.window.event);
    }
}

//=================
// SetWindowTitle
//=================
void GameLogic::SetWindowTitle()
{
    std::string window_title = ASC_APPNAME;
    window_title += " ";

    if(ASC_RELEASE == 0x01) window_title += "indev_";
    else if(ASC_RELEASE == 0x02) window_title += "alpha_";
    else if(ASC_RELEASE == 0x03) window_title += "beta_";
    else if(ASC_RELEASE == 0x04) window_title += "release_";
    else window_title += "version_";

    std::ostringstream o;
    o << std::setw(3) << std::setfill('0') << ASC_VERSION;
    window_title += o.str();

    if(ASC_RENDERENGINE == "SDL") window_title += " [SDL]";
    else if(ASC_RENDERENGINE == "OGL") window_title += " [OGL]";

    if(ASC_DEBUGMODE) window_title += " DEBUG_MODE";

    window_title += " (";
    window_title += ASC_APPSTATE;
    window_title += ")";

    SDL_SetWindowTitle(ASC_WINDOW, window_title.c_str());
}

//================================
//    RenderPreFrame
//================================
void GameLogic::RenderPreFrame()
{
    if(ASC_RENDERENGINE == "SDL")
    {
        SDL_RenderClear(ASC_RENDERER);
    }

    if(ASC_RENDERENGINE == "OGL")
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
}

//================================
//    RenderFrame
//================================
void GameLogic::RenderFrame()
{
    States[ASC_APPSTATE]->Run();
}

//================================
//    RenderPostFrame
//================================
void GameLogic::RenderPostFrame()
{
    if(ASC_RENDERENGINE == "SDL")
    {
        SDL_RenderPresent(ASC_RENDERER);
    }

    if(ASC_RENDERENGINE == "OGL")
    {
        SDL_GL_SwapWindow(ASC_WINDOW);
    }
}

//================================
//    CheckNewState
//================================
void GameLogic::CheckNewState()
{
    if(ChangeState)
    {
        std::string prev_state = ASC_APPSTATE;
        ASC_APPSTATE = NewState;

        delete States[prev_state];
        States.erase(prev_state);

        States[ASC_APPSTATE] = new GameState(ASC_APPSTATE, "TOP");

        ChangeState = 0;
    }
    else return;
}
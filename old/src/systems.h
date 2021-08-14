//================================
//          ASCENCIA CLIENT
//          systems.h
//================================
//        (c) Phragware 2019
//================================

#ifndef ASC_SYSTEMS_H_
#define ASC_SYSTEMS_H_

#include <headers.h>
#include <helpers.h>
#include <error_handler.h>
#include <input_handler.h>
#include <gamestate.h>
#include <gamelogic.h>
#include <asset_manager.h>

#ifndef ASC_INIT
extern std::string ASC_APPNAME;
extern std::string ASC_DEVNAME;
extern uint32_t ASC_VERSION;
extern uint8_t ASC_RELEASE;
extern uint32_t ASC_WINWIDTH;
extern uint32_t ASC_WINHEIGHT;
extern uint16_t ASC_FRAMERATE;
extern std::string ASC_RENDERENGINE;
extern std::string ASC_WINDOWTITLE;
extern uint8_t ASC_OGLMAJ;
extern uint8_t ASC_OGLMIN;
extern bool ASC_FULLSCREEN;
extern bool ASC_DEBUGMODE;
extern SDL_Window *ASC_WINDOW;
extern SDL_Renderer *ASC_RENDERER;
extern SDL_GLContext ASC_GLContext;
extern std::string ASC_PREFPATH;
extern std::string ASC_BASEPATH;
extern std::string ASC_APPSTATE;
extern std::string ASC_SUBSTATE;
#endif // ASC_INIT

#ifndef ASC_ERRORHANDLER_SYSTEMS
extern ErrorHandler Error;
#endif // ndef ASC_ERRORHANDLER_SYSTEMS
#ifdef ASC_ERRORHANDLER_SYSTEMS
ErrorHandler Error;
#endif // def ASC_ERRORHANDLER_SYSTEMS

#ifndef ASC_INPUTHANDLER_SYSTEMS
extern InputHandler Input;
#endif // ndef ASC_INPUTHANDLER_SYSTEMS
#ifdef ASC_INPUTHANDLER_SYSTEMS
InputHandler Input;
#endif // def ASC_INPUTHANDLER_SYSTEMS

#ifndef ASC_GAMELOGIC_SYSTEMS
extern GameLogic Game;
#endif // ndef ASC_GAMELOGIC_SYSTEMS
#ifdef ASC_GAMELOGIC_SYSTEMS
GameLogic Game;
#endif // def ASC_GAMELOGIC_SYSTEMS

#ifndef ASC_ASSETMANAGER_SYSTEMS
extern AssetManager Assets;
#endif // ndef ASC_ASSETMANAGER_SYSTEMS
#ifdef ASC_ASSETMANAGER_SYSTEMS
AssetManager Assets;
#endif // def ASC_ASSETMANAGER_SYSTEMS

//==========
// Systems
//==========
struct Systems
{
    Systems();
    ~Systems();
    void Start(int _argc, char **_argv);
    void Quit();
    void ProcessArgs(int _argc, char **_argv);
    void InitSDL();
    void InitWindow();
    void InitSDLRender();
    void InitOpenGLRender();
    void DebugMessage(std::string message1);
    void DebugMessage(std::string message1, std::string message2);
    void DebugMessage(std::string message1, std::string message2, std::string message3);
    void DebugMessageV(void *dump1);
    void DebugMessageV(void *dump1, void *dump2);
    void DebugMessageV(void *dump1, void *dump2, void *dump3);

    bool isInit;
    bool isQuit;
};

#ifndef ASC_INIT
extern Systems System;
#endif

#endif // ASC_SYSTEMS_H_
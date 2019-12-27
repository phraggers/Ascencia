//================================
//          ASCENCIA CLIENT
//            client.cpp
//================================
//        (c) Phragware 2019
//================================

#define G_APPNAME "Ascencia"
#define G_DEVNAME "Phragware"
#define G_WINWIDTH 800
#define G_WINHEIGHT 600
#define G_FRAMERATE 60
#define G_RENDERENGINE "OGL" // SDL or OGL
#define G_OGLMAJ 3
#define G_OGLMIN 3
#define G_FULLSCREEN 0

#ifndef DEBUG_MODE
#define G_DEBUGMODE 0
#else
#define G_DEBUGMODE 1
#endif

#ifndef G_RELEASE
#define G_RELEASE 0x00
#endif
#ifndef G_VERSION
#define G_VERSION 0x00
#endif

#include <headers.h>
#define ASC_INIT
#include <systems.h>

//==================
// GetPrefPath
//==================
std::string ASC_GetPrefPath()
{
    std::string str = SDL_GetPrefPath(G_DEVNAME, G_APPNAME);

    if (str.back() != '\\')
    {
        str.push_back('\\');
    }

    return str;
}

//==================
// GetBasePath
//==================
std::string ASC_GetBasePath()
{
    std::string str = SDL_GetBasePath();

    if (str.back() != '\\')
    {
        str.push_back('\\');
    }

    return str;
}

//==========
// Globals
//==========
std::string ASC_APPNAME = G_APPNAME;
std::string ASC_DEVNAME = G_DEVNAME;
uint32_t ASC_VERSION = G_VERSION;
uint8_t ASC_RELEASE = G_RELEASE;
uint32_t ASC_WINWIDTH = G_WINWIDTH;
uint32_t ASC_WINHEIGHT = G_WINHEIGHT;
uint16_t ASC_FRAMERATE = G_FRAMERATE;
std::string ASC_RENDERENGINE = G_RENDERENGINE;
std::string ASC_WINDOWTITLE = ASC_APPNAME;
uint8_t ASC_OGLMAJ = G_OGLMAJ;
uint8_t ASC_OGLMIN = G_OGLMIN;
bool ASC_FULLSCREEN = G_FULLSCREEN;
bool ASC_DEBUGMODE = G_DEBUGMODE;
SDL_Window *ASC_WINDOW;
SDL_Renderer *ASC_RENDERER;
SDL_GLContext ASC_GLContext;
std::string ASC_PREFPATH = ASC_GetPrefPath();
std::string ASC_BASEPATH = ASC_GetBasePath();
std::string ASC_APPSTATE = ASC_APPNAME;
std::string ASC_SUBSTATE = ASC_APPNAME;
Systems System;

#undef ASC_INIT

int main(int argc, char **argv)
{
    System.Start(argc, argv);
    System.Quit();
    return 0;
}

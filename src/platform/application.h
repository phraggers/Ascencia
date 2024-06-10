/*============================================================
 * Project: Ascencia
 * File: application.h
 * Author: Phraggers
 * Date: Mon Jun 03 2024
============================================================*/
#ifndef APPLICATION_H_
#define APPLICATION_H_

#include <util/types.h>

#ifdef ASC_WINDOWS
    #include <WinSock2.h>
#else // ASC_LINUX / ASC_MACOS
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <fcntl.h>
#endif

#include <SDL3/SDL.h>
#include <SDL3/SDL_thread.h>
#include <archive.h>
#include <archive_entry.h>

#define ASC_HEAD
#include <util/modules.h>
#undef ASC_HEAD

struct ASC_Application
{
    bool running;
    SDL_DateTime clock;
    ASC_Timer timer;
    ASC_Config config;
    ASC_Network network;
    ASC_Window window;
    ASC_Audio audio;
    ASC_Assets assets;
    ASC_Logic logic;
};

static ASC_Application *gApp;

static bool ASC_Init(int argc, char **argv);
static bool ASC_Run(void);
static void ASC_Quit();
static bool ASC_ProcessKeybind(ASC_Keybind *keybind);
static void ASC_ProcessKeybinds(void);
static void ASC_UpdateClock(void);

#define ASC_IMPL
#include <util/modules.h>

/*============================================================*/

static bool ASC_Init(int argc, char **argv)
{
    gApp = (ASC_Application*)malloc(sizeof(*gApp));
    memset(gApp, 0, sizeof(*gApp));
    ASC_UpdateClock();

    if(!ASC_ConfigInit(argc, argv)) ASC_Fatal("Config Initialization Error");
    if(!ASC_LogInit()) ASC_Fatal("Log Initialization Error");
    if(!ASC_NetInit()) ASC_Fatal("Network Initialization Error");
    if(!ASC_WindowInit()) ASC_Fatal("Window Initialization Error");
    if(!ASC_AudioInit()) ASC_Fatal("Audio Initialization Error");
    if(!ASC_TimerInit()) ASC_Fatal("Timer Initialization Error");
    if(!ASC_AssetsInit()) ASC_Fatal("Assets Initialization Error");
    if(!ASC_LogicInit()) ASC_Fatal("Logic Initialization Error");

    ASC_InfoLog("ASC_Init: %s %s [%d.%d.%d]", gApp->config.app_org, gApp->config.app_name, gApp->config.app_ver[0], gApp->config.app_ver[1], gApp->config.app_ver[2]);

    gApp->running = 1;
    return 1;
}

static bool ASC_Run(void)
{
    while(gApp->running)
    {
        ASC_TimerFrameStart();

        // clock doesn't need to updated every frame
        if(gApp->timer.ticks % 10 == 0) ASC_UpdateClock();

        if(!ASC_WindowHandleEvents())
        {
            gApp->running = 0;
            break;
        }

        // net frame
        ASC_NetFrame();

        ASC_ProcessKeybinds();

        // game logic

        if(!ASC_WindowUpdateEvents())
        {
            gApp->running = 0;
            break;
        }
        
        // update audio
        
        // update net send
        
        if(!ASC_WindowUpdateRender())
        {
            gApp->running = 0;
            break;
        }
        
        ASC_TimerFrameEnd();
    }

    return 1;
}

static void ASC_Quit()
{
    ASC_NetQuit();
    ASC_WindowSaveConfig();
    ASC_ConfigQuit();
    ASC_LogQuit();
}

static bool ASC_ProcessKeybind(ASC_Keybind *keybind)
{
    bool result = 0;
    ASC_Input *input = &gApp->window.input;

    if(keybind->state == ASC_KEYBIND_DOWNTICK && 
    input->keyboard[keybind->key].downTick)
    {
        result = 1;
    }
    else if(keybind->state == ASC_KEYBIND_UPTICK && 
    input->keyboard[keybind->key].upTick)
    {
        result = 1;
    }
    else if(keybind->state == ASC_KEYBIND_ISDOWN &&
    input->keyboard[keybind->key].isDown)
    {
        result = 1;
    }

    if(!result) return 0;
    if(keybind->modifiers == 0) return 1;

    u8 lshift, rshift, lctrl, rctrl, lalt, ralt, shift, ctrl, alt;
    lshift = rshift = lctrl = rctrl = lalt = ralt = shift = ctrl = alt = 0;
    if((keybind->modifiers & ASC_KEYBIND_MOD_LSHIFT) != 0) lshift = 1;
    if((keybind->modifiers & ASC_KEYBIND_MOD_RSHIFT) != 0) rshift = 1;
    if((keybind->modifiers & ASC_KEYBIND_MOD_LCTRL) != 0) lctrl = 1;
    if((keybind->modifiers & ASC_KEYBIND_MOD_RCTRL) != 0) rctrl = 1;
    if((keybind->modifiers & ASC_KEYBIND_MOD_LALT) != 0) lalt = 1;
    if((keybind->modifiers & ASC_KEYBIND_MOD_RALT) != 0) ralt = 1;
    if((keybind->modifiers & ASC_KEYBIND_MOD_SHIFT) != 0) shift = 1;
    if((keybind->modifiers & ASC_KEYBIND_MOD_CTRL) != 0) ctrl = 1;
    if((keybind->modifiers & ASC_KEYBIND_MOD_ALT) != 0) alt = 1;

    if(lshift && input->keyboard[SDL_SCANCODE_LSHIFT].isDown) lshift++;
    if(rshift && input->keyboard[SDL_SCANCODE_RSHIFT].isDown) rshift++;
    if(lctrl && input->keyboard[SDL_SCANCODE_LCTRL].isDown) lctrl++;
    if(rctrl && input->keyboard[SDL_SCANCODE_RCTRL].isDown) rctrl++;
    if(lalt && input->keyboard[SDL_SCANCODE_LALT].isDown) lalt++;
    if(ralt && input->keyboard[SDL_SCANCODE_RALT].isDown) ralt++;
    if(shift && (input->keyboard[SDL_SCANCODE_LSHIFT].isDown || input->keyboard[SDL_SCANCODE_RSHIFT].isDown)) shift++;
    if(ctrl && (input->keyboard[SDL_SCANCODE_LCTRL].isDown || input->keyboard[SDL_SCANCODE_RCTRL].isDown)) ctrl++;
    if(alt && (input->keyboard[SDL_SCANCODE_LALT].isDown || input->keyboard[SDL_SCANCODE_RALT].isDown)) alt++;

    if(lshift==1 || rshift==1 || lctrl==1 || rctrl==1 || lalt==1 || ralt==1 || shift==1 || ctrl==1 || alt==1) return 0;
    
    return 1;
}

static void ASC_ProcessKeybinds(void)
{
    ASC_Keybinds *kb = &gApp->config.keybinds;

    if(ASC_ProcessKeybind(&kb->AppQuit))
    {
        ASC_DebugLog("Keybind: Quit");
        gApp->running = 0;
    }

    if(ASC_ProcessKeybind(&kb->AppFullscreen[0]) ||
       ASC_ProcessKeybind(&kb->AppFullscreen[1]))
    {
        ASC_DebugLog("Keybind: Fullscreen");
        ASC_WindowToggleFullscreen();
    }
}

static void ASC_UpdateClock(void)
{
    SDL_Time time;
    SDL_GetCurrentTime(&time);
    SDL_TimeToDateTime(time, &gApp->clock, 1);
}

#endif //APPLICATION_H_
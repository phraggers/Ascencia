/* ========================================================================
   $File:    Ascencia [Source] $
   $Project: Ascencia $
   $Date:    08-09-2021 $
   $Author:  Phil Bagshaw $
   $Notice:  (c)Phragware 2021 $
   ======================================================================== */

#include <SDL/SDL.h>
#include "Ascencia.h"
#include "Events.cpp"

internal bool
ASC_Keydown(SDL_Keycode Key)
{
    for(int iKeydown = 0; iKeydown < 10; ++iKeydown)
    {
        if(Application.Keydowns[iKeydown] == Key)
        {
            return true;
        }
    }

    return false;
}

int main(int argc, char **argv)
{
    Application = {};
    Application.Dimension.x = SDL_WINDOWPOS_UNDEFINED;
    Application.Dimension.y = SDL_WINDOWPOS_UNDEFINED;
    Application.Dimension.w = 1280;
    Application.Dimension.h = 720;
    
    if(!SDL_Init(SDL_INIT_EVERYTHING))
    {
        SDL_version Version;
        SDL_GetVersion(&Version);
        SDL_Log("SDL Version: %d.%d.%d", Version.major, Version.minor, Version.patch);

        if(Version.major < 2 || Version.minor < 0 || Version.patch < 16)
        {
            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING, "Warning!",
                                     "This Application is using an older version of sdl2.dll!\nConsider reinstalling, it may behave unexpectedly!", 0);
        }

        Application.Window = SDL_CreateWindow("Ascencia",
                                              Application.Dimension.x,
                                              Application.Dimension.y,
                                              Application.Dimension.w,
                                              Application.Dimension.h,
                                              SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN);

        if(Application.Window)
        {
            SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
            Application.Renderer = SDL_CreateRenderer(Application.Window, -1,
                                                      SDL_RENDERER_ACCELERATED);

            if(Application.Renderer)
            {
                Memory = {};
                Memory.PersistantSize = Megabytes(64);
                Memory.TransientSize = Gigabytes(1);
                Memory.Persistant = SDL_malloc((size_t)(Memory.PersistantSize +
                                                        Memory.TransientSize));
                Memory.Transient = ((uint8*)Memory.Persistant + Memory.PersistantSize);

                if(Memory.Persistant && Memory.Transient)
                {
                    Memory.Initialized = true;
                    Application.Running = true;

                    //TEST:
                    SDL_Texture *Texture = SDL_CreateTexture(Application.Renderer,
                                                             SDL_PIXELFORMAT_RGBA8888,
                                                             SDL_TEXTUREACCESS_TARGET,
                                                             Application.Dimension.w,
                                                             Application.Dimension.h);
                    SDL_Rect Rect = {};
                    Rect.w = 100;
                    Rect.h = 50;

                    while(Application.Running)
                    {
                        //SDL_SetRenderDrawColor(Application.Renderer, 96, 128, 255, 255);
                        //SDL_RenderClear(Application.Renderer);

                        ASC_HandleEvents();

                        if(ASC_Keydown('w')) Rect.y-=5;
                        if(ASC_Keydown('s')) Rect.y+=5;
                        if(ASC_Keydown('a')) Rect.x-=5;
                        if(ASC_Keydown('d')) Rect.x+=5;

                        //TEST:
                        SDL_SetRenderTarget(Application.Renderer, Texture);
                        SDL_SetRenderDrawColor(Application.Renderer, 0,0,0,0);
                        SDL_RenderClear(Application.Renderer);
                        
                        SDL_RenderDrawRect(Application.Renderer, &Rect);
                        SDL_SetRenderDrawColor(Application.Renderer, 0xff,0,0,0);
                        SDL_RenderFillRect(Application.Renderer, &Rect);

                        SDL_SetRenderTarget(Application.Renderer, 0);
                        SDL_RenderCopy(Application.Renderer, Texture, 0,0);
                    
                        SDL_RenderPresent(Application.Renderer);
                        SDL_Delay(16);
                    }
                }
                
                else //NOTE: !Memory.Persistant || !Memory.Transient
                {
                    SDL_SetError("Failed to reserve Application Memory! (PersistantSize: %d, TransientSize: %d)", Memory.PersistantSize, Memory.TransientSize);
                    SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, SDL_GetError());
                    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Fatal Error!",
                                             SDL_GetError(), 0);
                    return 1;
                }
            }

            else //NOTE: SDL_CreateRenderer error
            {
                SDL_SetError("Failed to create Renderer! %s", SDL_GetError());
                SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, SDL_GetError());
                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Fatal Error!",
                                         SDL_GetError(), 0);
            }
        }

        else //NOTE: SDL_CreateWindow error
        {
            SDL_SetError("Failed to create Window! %s", SDL_GetError());
            SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, SDL_GetError());
            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Fatal Error!",
                                     SDL_GetError(), 0);
        }
    }

    else //NOTE: SDL_Init error
    {
        SDL_SetError("Failed to Init SDL! %s", SDL_GetError());
        SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, SDL_GetError());
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Fatal Error!",
                                 SDL_GetError(), 0);
    }
    
    
    return 0;
}

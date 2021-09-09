/* ========================================================================
   $File:    Ascencia [Source] $
   $Project: Ascencia $
   $Date:    08-09-2021 $
   $Author:  Phil Bagshaw $
   $Notice:  (c)Phragware 2021 $
   ======================================================================== */

#include <SDL/SDL.h>
#include "Ascencia.h"

int main(int argc, char **argv)
{
    Memory = {};
    Memory.PersistantSize = Megabytes(64);
    Memory.TransientSize = Gigabytes(1);
    Memory.Persistant = SDL_malloc((size_t)(Memory.PersistantSize + Memory.TransientSize));
    Memory.Transient = ((uint8*)Memory.Persistant + Memory.PersistantSize);

    if(!Memory.Persistant || !Memory.Transient)
    {
        //TODO: Logging: failed to get memory
        Assert(0);
    }
    
    Application = {};
    Application.ScreenWidth = 1280;
    Application.ScreenHeight = 720;
    
    if(!SDL_Init(SDL_INIT_EVERYTHING))
    {
        Application.Window = SDL_CreateWindow("Ascencia",
                                              SDL_WINDOWPOS_UNDEFINED,
                                              SDL_WINDOWPOS_UNDEFINED,
                                              Application.ScreenWidth,
                                              Application.ScreenHeight,
                                              0);

        if(Application.Window)
        {
            SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
            Application.Renderer = SDL_CreateRenderer(Application.Window, -1,
                                                      SDL_RENDERER_ACCELERATED);

            if(Application.Renderer)
            {
                Application.Running = true;

                while(Application.Running)
                {
                    SDL_SetRenderDrawColor(Application.Renderer, 96, 128, 255, 255);
                    SDL_RenderClear(Application.Renderer);

                    SDL_Event Event;
                    while(SDL_PollEvent(&Event))
                    {
                        switch(Event.type)
                        {
                          case SDL_QUIT:
                          {
                              Application.Running = false;
                          } break;

                          default:
                          {
                              break;
                          }
                        }
                    }
                    
                    SDL_RenderPresent(Application.Renderer);
                    SDL_Delay(16);
                }
            }

            else
            {
                //TODO: Logging: SDL_CreateRenderer failed
                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "SDL_CreateRenderer Failed!",
                                         SDL_GetError(), 0);
            }
        }

        else
        {
            //TODO: Logging: SDL_CreateWindow failed
            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "SDL_CreateWindow Failed!",
                                         SDL_GetError(), 0);
        }
    }

    else
    {
        //TODO: Logging: SDL init failed
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "SDL_Init Failed!",
                                         SDL_GetError(), 0);
    }
    
    
    return 0;
}

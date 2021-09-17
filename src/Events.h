#ifndef EVENTS_H
/* ========================================================================
   $File:    Events [Header] $
   $Project: Ascencia $
   $Date:    09-09-2021 $
   $Author:  Phil Bagshaw $
   $Notice:  (c)Phragware 2021 $
   ======================================================================== */

static void
ASC_SetKeyUp(SDL_Keycode Key)
{
    for(int iKeydown = 0; iKeydown < KEYDOWN_COUNT; ++iKeydown)
    {
        if(Application.Keydowns[iKeydown] == Key)
        {
            Application.Keydowns[iKeydown] = 0;
            break;
        }
    }

}

static void
ASC_SetKeyDown(SDL_Keycode Key)
{
    int8 KeydownExists = 0;

    for(int iKeydown = 0; iKeydown < KEYDOWN_COUNT; ++iKeydown)
    {
        if(Application.Keydowns[iKeydown] == Key)
        {
            KeydownExists = 1;
            break;
        }
    }

    if(!KeydownExists)
    {
        for(int iKeydown = 0; iKeydown < KEYDOWN_COUNT; ++iKeydown)
        {
            if(Application.Keydowns[iKeydown] == 0)
            {
                Application.Keydowns[iKeydown] = Key;
                break;
            }
        }
    }

}

static int8
ASC_KeySingle(SDL_Keycode Key)
{
    for(int iKeydown = 0; iKeydown < KEYDOWN_COUNT; ++iKeydown)
    {
        if(Application.Keydowns[iKeydown] == Key)
        {
            ASC_SetKeyUp(Key);
            return 1;
        }
    }

    return 0;
}

static int8
ASC_Keydown(SDL_Keycode Key)
{
    for(int iKeydown = 0; iKeydown < KEYDOWN_COUNT; ++iKeydown)
    {
        if(Application.Keydowns[iKeydown] == Key)
        {
            return 1;
        }
    }

    return 0;
}

static void
ASC_HandleEvents()
{
    SDL_Event Event;
    while(SDL_PollEvent(&Event))
    {
        switch(Event.type)
        {
          case SDL_QUIT:
          {
              Application.Running = 0;
          } break;

          case SDL_WINDOWEVENT:
          {
              switch(Event.window.event)
              {
                case SDL_WINDOWEVENT_SHOWN:
                {
                    //NOTE: Should only happen at startup.
                    // This is NOT the same as EXPOSED
                    SDL_Log("Event: Window Shown");
                } break;

                case SDL_WINDOWEVENT_HIDDEN:
                {
                    //NOTE: This should never happen, just quit if it does.
                    SDL_Log("Unexpected Event: Window Hidden");
                    Application.Running = 0;
                } break;

                case SDL_WINDOWEVENT_EXPOSED:
                {
                    Application.Exposed = 1;
                    Application.Minimized = 0;
                } break;

                case SDL_WINDOWEVENT_MOVED:
                {
                    SDL_GetWindowPosition(Application.Window, &Application.Dimension.x, &Application.Dimension.y);
                } break;

                case SDL_WINDOWEVENT_SIZE_CHANGED:
                {
                    SDL_GetWindowSize(Application.Window, &Application.Dimension.w, &Application.Dimension.h);
                    glViewport(0,0, Application.Dimension.w, Application.Dimension.h);
                } break;

                case SDL_WINDOWEVENT_MINIMIZED:
                {
                    Application.Exposed = 0;
                    Application.Minimized = 1;
                } break;

                case SDL_WINDOWEVENT_MAXIMIZED:
                {
                    Application.Maximized = 1;
                } break;

                case SDL_WINDOWEVENT_RESTORED:
                {
                    Application.Minimized = 0;
                    Application.Maximized = 0;
                    SDL_GetWindowPosition(Application.Window, &Application.Dimension.x, &Application.Dimension.y);
                    SDL_GetWindowSize(Application.Window, &Application.Dimension.w, &Application.Dimension.h);
                    glViewport(0,0, Application.Dimension.w, Application.Dimension.h);
                } break;

                case SDL_WINDOWEVENT_ENTER:
                {
                    Application.MouseInWindow = 1;
                } break;

                case SDL_WINDOWEVENT_LEAVE:
                {
                    Application.MouseInWindow = 0;
                } break;

                case SDL_WINDOWEVENT_FOCUS_GAINED:
                {
                    Application.KeyboardFocus = 1;
                } break;

                case SDL_WINDOWEVENT_FOCUS_LOST:
                {
                    Application.KeyboardFocus = 0;
                } break;

                case SDL_WINDOWEVENT_CLOSE:
                {
                    Application.Running = 0;
                } break;
              }
          } break;

          case SDL_KEYDOWN:
          {
              ASC_SetKeyDown(Event.key.keysym.sym);
          } break;

          case SDL_KEYUP:
          {
              ASC_SetKeyUp(Event.key.keysym.sym);
          } break;

          case SDL_MOUSEMOTION:
          case SDL_MOUSEWHEEL:
          {
              if(Application.MouseInWindow)
              {
                  SDL_GetMouseState(&Application.MouseX, &Application.MouseY);
              }
          } break;

          case SDL_MOUSEBUTTONDOWN:
          {
              if(Event.button.button == SDL_BUTTON_LEFT) Application.MouseLeft = 1;
              if(Event.button.button == SDL_BUTTON_RIGHT) Application.MouseRight = 1;
          } break;

          case SDL_MOUSEBUTTONUP:
          {
              if(Event.button.button == SDL_BUTTON_LEFT) Application.MouseLeft = 0;
              if(Event.button.button == SDL_BUTTON_RIGHT) Application.MouseRight = 0;
          } break;

          default:
          {
              break;
          }
        }
    } // while PollEvent
}

#define EVENTS_H
#endif

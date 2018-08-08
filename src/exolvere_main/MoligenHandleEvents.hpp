//==========================
// Exolvere
// MoligenHandleEvents.hpp
//==========================
void Moligen::handleEvents()
{
    // Handle events
    while(SDL_PollEvent(&gl.mEvent))
    {
        if(gl.mEvent.type == SDL_QUIT)
        {
            gl.appQuit = true;
        }

        // Keydown Events
        if(gl.mEvent.type == SDL_KEYDOWN)
        {

            // Global Keydown Events
            //================================================
            if(gl.mEvent.key.keysym.sym == SDLK_ESCAPE)
            {
                gl.appQuit = true;
            }

            if(gl.mEvent.key.keysym.sym == SDLK_F9)
            {
                SDL_SetWindowSize(gl.mWindow, 960, 540);
            }

            if(gl.mEvent.key.keysym.sym == SDLK_F10)
            {
                SDL_SetWindowSize(gl.mWindow, 1280, 720);
            }

            if(gl.mEvent.key.keysym.sym == SDLK_F11)
            {
                SDL_SetWindowSize(gl.mWindow, 1920, 1080);
            }

            if(gl.mEvent.key.keysym.sym == SDLK_F12)
            {
                if(!gl.appFullscreen)
                {
                    SDL_SetWindowFullscreen(gl.mWindow, SDL_WINDOW_FULLSCREEN);
                    gl.appFullscreen = true;
                }
                else
                {
                    SDL_SetWindowFullscreen(gl.mWindow, 0);
                    gl.appFullscreen = false;
                }
            }

			if(gl.mEvent.key.keysym.sym == SDLK_LEFT)
			{
        SDL_SetWindowPosition(gl.mWindow, gl.appXPos-10, gl.appYPos);
				gl.appXPos -= 10;
			}

			if(gl.mEvent.key.keysym.sym == SDLK_RIGHT)
			{
				SDL_SetWindowPosition(gl.mWindow, gl.appXPos+10, gl.appYPos);
				gl.appXPos += 10;
			}

			if(gl.mEvent.key.keysym.sym == SDLK_UP)
			{
        SDL_SetWindowPosition(gl.mWindow, gl.appXPos, gl.appYPos-10);
				gl.appYPos -= 10;
			}

			if(gl.mEvent.key.keysym.sym == SDLK_DOWN)
			{
        SDL_SetWindowPosition(gl.mWindow, gl.appXPos, gl.appYPos+10);
				gl.appYPos += 10;
			}

			//===================================================

        }
    }
}

// Ascencia
// video/renderer.h

#pragma once

struct sRendererState
{
	bool Running;
	SDL_Thread* Thread;
	SDL_Semaphore* Lock;
	SDL_GLContext GLRC;
	SDL_Window** Window;
};

class cRenderer
{
	private:

	sRendererState State;

	void SetRunning(bool Running);
	bool GetRunning(void);

	public:

	// Main Thread
	cRenderer(void);
	bool Setup(void);
	bool Start(void);
	void Quit(void);

	// Renderer Thread
	bool Run(void);
	bool Frame(void);
	void SetVSync(bool VSync);
};
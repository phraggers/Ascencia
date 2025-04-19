// Ascencia
// platform/window.h

#pragma once

struct sWindowState
{
	int WindowedWH[2]; // saved non-fullscreen width/height
	bool IsFullscreen;
	bool IsMaximized; // when not fullscreen
	bool IsMinimized;
	bool IsFocussed;
	bool VSync;
	int UpdateHz; // when not VSync
};

class cWindow
{
	private:


	public:

	sWindowState State;
	SDL_Window* Handle;
	cRenderer Renderer;

	std::string Title;
	SDL_Rect Dimension; // actual xpos, ypos, width, height

	cWindow();
	bool Init(void);
	void Quit(void);
	bool Frame(void);
	void SetFullscreen(bool Fullscreen);
	void ToggleFullscreen(void);
	void ApplyState(void); // set current SDL window state to State
	void UpdateState(void);
};
// Ascencia
// ascencia/platform/window.h

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

struct sWindow
{
	sWindowState State;
	SDL_Window* Handle;
	SDL_GLContext GLRC;

	std::string Title;
	SDL_Rect Dimension; // actual xpos, ypos, width, height

	sWindow();
	bool Init(void);
	bool Frame(void);
	void SetFullscreen(bool Fullscreen);
	void ToggleFullscreen(void);
	void ApplyState(void); // set current SDL window state to State
	void UpdateState(void);
};
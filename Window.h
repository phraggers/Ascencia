
#ifndef _ASC_WINDOW_H_
#define _ASC_WINDOW_H_

typedef struct
{
	char Title[64];
	SDL_Window* Handle;
	SDL_GLContext GLContext;
	SDL_Rect Dimensions;
	bool Exposed;
	bool Minimized;
	bool Maximized;
	bool MouseFocus;
	bool KeyboardFocus;
	bool Fullscreen;
	bool MouseShown;
} ASC_Window;

bool Window_Init();
void Window_Resize();
bool Window_ToggleFullscreen();
bool Window_Screenshot(char* _Path); // can pass 0 if no path needed
bool Window_ToggleMouse();

#endif //_ASC_WINDOW_H_
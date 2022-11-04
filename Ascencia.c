
/* -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
		  ___   _____  _____  _____  _   _  _____  _____   ___
		 / _ \ /  ___|/  __ \|  ___|| \ | |/  __ \|_   _| / _ \
		/ /_\ \\ `--. | /  \/| |__  |  \| || /  \/  | |  / /_\ \
		|  _  | `--. \| |    |  __| | . ` || |      | |  |  _  |
		| | | |/\__/ /| \__/\| |___ | |\  || \__/\ _| |_ | | | |
		\_| |_/\____/  \____/\____/ \_| \_/ \____/ \___/ \_| |_/

   -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
	  ___   _  _   ___     _      ___  __      __    _     ___   ___
	 | _ \ | || | | _ \   /_\    / __| \ \    / /   /_\   | _ \ | __|
	 |  _/ | __ | |   /  / _ \  | (_ |  \ \/\/ /   / _ \  |   / | _|
	 |_|   |_||_| |_|_\ /_/ \_\  \___|   \_/\_/   /_/ \_\ |_|_\ |___|
			   __      __    ___    __    ___   ___
			  / /  __  \ \  |_  )  /  \  |_  ) |_  )
			 | |  / _|  | |  / /  | () |  / /   / /
			 | |  \__|  | | /___|  \__/  /___| /___|
			  \_\      /_/
 -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+- */

 /*-----------------------
  =-=-= IMPORTANT!!! =-=-=
  ------------------------
 - MUST ensure STACK SIZE at least 1KB (1024 bytes) pref 1 MB (1048576 bytes)
 (maybe gonna fix that, default c stack is 256 bytes) */

/*
* 
=-=-= TODO =-=-=

- threads and timers, need multiple timers, perhaps a thread struct & funcs
- GameState (logic for where the game is, main menu, new game, etc) Need to somehow have accessible variables for running game loop, init state and run state?
- FileIO
- Audio
- Textures
- Networking
- Data (dat files)

normalized types (screen coords, textures, controller axis, mouse coords, audio levels etc)


CURRENT CONTROLS
-----------------

F12 : Screenshot (Alt+P when in Debug)
F11 : Fullscreen
F10 : Toggle Cursor (Windows/OGL)

Up : Increase random shape transform speed
Down : Decrease random shape transform speed
Shift + Up : Increase random shape transform speed faster
Shift + Down : Decrease random shape transform speed faster
*/


#define ASC_MAIN
#include "Ascencia.h"

u64 ASC_GetCurrentStackSize(void)
{
	int x = 0;
	return ASC_TopOfTheStack - (u64)&x;
}

static inline void Main_SetSDLGLAttribute(SDL_GLattr _attr, int _value)
{
	if (SDL_GL_SetAttribute(_attr, _value))
	{
		ASC_Log(LOGLEVEL_ERROR, "MAIN: Error setting SDLGLAttribute[%d]:[%d], %s", _attr, _value, SDL_GetError());
	}
	else
	{
		ASC_Log(LOGLEVEL_DEBUG, "MAIN: Set SDLGLAttribute[%d]:[%d]", _attr, _value);
	}
}

static bool Main_Init(int _argc, char** _argv)
{
	State = (ASC_AppState*)State_Init(0);

	if (!State)
	{
		return 0;
	}

	if (!Log_Init())
	{
		return 0;
	}

	Timer_Init(60);
	int StartupTimer = Timer_Add();

	//char* env = SDL_getenv("USERNAME");
	//ASC_Log(LOGLEVEL_INFO, "MAIN: Login[%s]", ((env) ? env : "None"));
	//if (env) SDL_free(env);

	State->AppVersion.major = DEF_APPVER_MAJ;
	State->AppVersion.minor = DEF_APPVER_MIN;
	State->AppVersion.patch = DEF_APPVER_PAT;
	State->PrefPath = SDL_GetPrefPath(DEF_APPORG, DEF_APPNAME);
	State->BasePath = SDL_GetBasePath();
	State->ArgC = _argc;
	State->ArgV = _argv;
	State->GameState = GS_NONE;

	ASC_Log(LOGLEVEL_INFO, "MAIN: %s %d.%d.%d",
			DEF_APPNAME, State->AppVersion.major, State->AppVersion.minor, State->AppVersion.patch);
	ASC_Log(LOGLEVEL_INFO, "MAIN: PrefPath: %s", State->PrefPath);
	ASC_Log(LOGLEVEL_INFO, "MAIN: BasePath: %s", State->BasePath);
	Log_SetLogFilePath();

	// SDL 2
	{
		if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
		{
			ASC_Log(LOGLEVEL_FATAL, "MAIN: Failed to start SDL2: %s", SDL_GetError());
			return 0;
		}

		SDL_VERSION(&State->SDLVersion);
		ASC_Log(LOGLEVEL_INFO, "MAIN: SDL %d.%d.%d", (int)State->SDLVersion.major,
				(int)State->SDLVersion.minor, (int)State->SDLVersion.patch);
	}

	// SDL Mixer
	{
		if (!Mix_Init(MIX_INIT_OGG))
		{
			ASC_Log(LOGLEVEL_FATAL, "MAIN: SDLMixer Init Error: %s", Mix_GetError());
			return 0;
		}

		MIX_VERSION(&State->MixVersion);
		ASC_Log(LOGLEVEL_INFO, "MAIN: SDLMixer %d.%d.%d", (int)State->MixVersion.major,
				(int)State->MixVersion.minor, (int)State->MixVersion.patch);

		if (Mix_OpenAudio(DEF_AUDIO_FREQ, DEF_AUDIO_FORMAT, DEF_AUDIO_CHANNELS, DEF_AUDIO_CHUNKSIZE) < 0)
		{
			ASC_Log(LOGLEVEL_FATAL, "MAIN: Failed to open Audio Device: %s", Mix_GetError());
			return 0;
		}

		ASC_Log(LOGLEVEL_INFO, "MAIN: AudioDevice Opened: Freq[%d] Format[%d] Channels[%d] Chunksize[%d]",
				DEF_AUDIO_FREQ, DEF_AUDIO_FORMAT, DEF_AUDIO_CHANNELS, DEF_AUDIO_CHUNKSIZE);
	}

	// SDL Net
	{
		if (SDLNet_Init() < 0)
		{
			ASC_Log(LOGLEVEL_FATAL, "MAIN: SDLNet Init Error: %s", SDLNet_GetError());
			return 0;
		}

		SDL_NET_VERSION(&State->NetVersion);
		ASC_Log(LOGLEVEL_INFO, "MAIN: SDLNet %d.%d.%d", (int)State->NetVersion.major,
				(int)State->NetVersion.minor, (int)State->NetVersion.patch);

	}

	// SDL Config
	if (SDL_JoystickEventState(SDL_ENABLE) != 1)
	{
		ASC_Log(LOGLEVEL_ERROR, "MAIN: Failed to enable SDL_JoystickEventState: %s", SDL_GetError());
	}
	else
	{
		ASC_Log(LOGLEVEL_DEBUG, "MAIN: JoystickEventState[%d]", SDL_JoystickEventState(SDL_QUERY));
	}

	Main_SetSDLGLAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, DEF_GFX_GLMAJ);
	Main_SetSDLGLAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, DEF_GFX_GLMIN);
	Main_SetSDLGLAttribute(SDL_GL_CONTEXT_PROFILE_MASK, 
						   ((DEF_GFX_GLCOMPAT) ? SDL_GL_CONTEXT_PROFILE_COMPATIBILITY : 
							SDL_GL_CONTEXT_PROFILE_CORE));
	Main_SetSDLGLAttribute(SDL_GL_DOUBLEBUFFER, DEF_GFX_DOUBLEBUFFER);
	Main_SetSDLGLAttribute(SDL_GL_MULTISAMPLEBUFFERS, DEF_GFX_MULTISAMPLEBUFFERS);
	Main_SetSDLGLAttribute(SDL_GL_MULTISAMPLESAMPLES, DEF_GFX_MULTISAMPLESAMPLES);
	Main_SetSDLGLAttribute(SDL_GL_ACCELERATED_VISUAL, DEF_GFX_ACCELERATED);
	Main_SetSDLGLAttribute(SDL_GL_STENCIL_SIZE, DEF_GFX_STENCILSIZE);

	// Window
	State->Window.Dimensions.w = DEF_WINDOW_WIDTH;
	State->Window.Dimensions.h = DEF_WINDOW_HEIGHT;

	if (!Window_Init())
	{
		ASC_Log(LOGLEVEL_FATAL, "MAIN: Failed to create Window!");
		return 0;
	}

	// GLAD OpenGL
	{
		if (!gladLoadGLLoader(SDL_GL_GetProcAddress))
		{
			ASC_Log(LOGLEVEL_FATAL, "MAIN: Failed to load GLAD.");
			return 0;
		}

		ASC_Log(LOGLEVEL_INFO, "MAIN: GLAD Loaded: OpenGL %s", glGetString(GL_VERSION));
	}

	State->Running = 1;

	ASC_Log(LOGLEVEL_INFO, "MAIN: Startup took %.02f ms", Timer_End(StartupTimer));

	return 1;
}

static void Main_Quit(void)
{
	ASC_Log(LOGLEVEL_DEBUG, "MAIN: Stack Size: %u", ASC_GetCurrentStackSize());

	if (State->Window.GLContext)
	{
		SDL_GL_DeleteContext(State->Window.GLContext);
		ASC_Log(LOGLEVEL_DEBUG, "MAIN: GLContext Deleted");
	}

	if (State->Window.Handle)
	{
		SDL_DestroyWindow(State->Window.Handle);
		State->Window.Handle = 0;
		ASC_Log(LOGLEVEL_DEBUG, "MAIN: Window Destroyed");
	}

	SDLNet_Quit();
	ASC_Log(LOGLEVEL_DEBUG, "MAIN: SDLNet Quit");
	Mix_CloseAudio();
	ASC_Log(LOGLEVEL_INFO, "MAIN: Audio Device Closed");
	Mix_Quit();
	ASC_Log(LOGLEVEL_DEBUG, "MAIN: SDLMixer Quit");

	if ((SDL_WasInit(SDL_INIT_EVERYTHING)))
	{
		SDL_Quit();
		ASC_Log(LOGLEVEL_INFO, "MAIN: SDL Quit");
	}

	Log_Quit();
	State_Quit();
}

typedef struct { r32 X; r32 Y; } NormalizedCoordinates;
static NormalizedCoordinates GetNormalizedCoordinates(int _X, int _Y)
{
	NormalizedCoordinates Result = { 0 };
	Result.X = (((r32)((r32)_X / (r32)State->Window.Dimensions.w) * 2.0f) - 1.0f);
	Result.Y = -(((r32)((r32)_Y / (r32)State->Window.Dimensions.h) * 2.0f) - 1.0f);
	return Result;
}

static void DrawCursor(void)
{
	if (State->Window.MouseShown) return;
	if (!State->Window.MouseFocus) return;

	r32 Scale = 0.10f; // 0.05f - 0.20f, 0.10f default
	r32 Rot = 25.0f;

	NormalizedCoordinates Mouse = GetNormalizedCoordinates(State->Input.Mouse.X, State->Input.Mouse.Y);

	typedef struct { r32 x, y, r, g, b; } Vertex;
	Vertex Outer[4] = { 0 };
	Vertex Inner[4] = { 0 };

	if (Input_MouseDown(SDL_BUTTON_LEFT))
	{
		Outer[0].r = 0.6f; Outer[0].g = 0.8f; Outer[0].b = 0.2f;
		Outer[1].r = 0.4f; Outer[1].g = 0.6f; Outer[1].b = 0.2f;
		Outer[2].r = 0.4f; Outer[2].g = 0.6f; Outer[2].b = 0.2f;
		Outer[3].r = 0.4f; Outer[3].g = 0.6f; Outer[3].b = 0.2f;
	}

	else if (Input_MouseDown(SDL_BUTTON_RIGHT))
	{
		Outer[0].r = 0.4f; Outer[0].g = 0.6f; Outer[0].b = 0.8f;
		Outer[1].r = 0.3f; Outer[1].g = 0.5f; Outer[1].b = 0.6f;
		Outer[2].r = 0.3f; Outer[2].g = 0.5f; Outer[2].b = 0.6f;
		Outer[3].r = 0.3f; Outer[3].g = 0.5f; Outer[3].b = 0.6f;
	}

	else
	{
		Outer[0].r = 0.8f; Outer[0].g = 0.8f; Outer[0].b = 0.8f;
		Outer[1].r = 0.6f; Outer[1].g = 0.7f; Outer[1].b = 0.8f;
		Outer[2].r = 0.6f; Outer[2].g = 0.7f; Outer[2].b = 0.8f;
		Outer[3].r = 0.6f; Outer[3].g = 0.7f; Outer[3].b = 0.8f;
	}

	Inner[0].r = 0.4f; Inner[0].g = 0.40f; Inner[0].b = 0.4f;
	Inner[1].r = 0.3f; Inner[1].g = 0.35f; Inner[1].b = 0.4f;
	Inner[2].r = 0.3f; Inner[2].g = 0.35f; Inner[2].b = 0.4f;
	Inner[3].r = 0.3f; Inner[3].g = 0.35f; Inner[3].b = 0.4f;

	Outer[0].x = 0.0f;
	Outer[0].y = 0.0f;

	Outer[1].x = -(0.1f * Scale);
	Outer[1].y = -(0.35f * Scale);

	Outer[2].x = 0.0f * Scale;
	Outer[2].y = -(0.25f * Scale);

	Outer[3].x = 0.1f * Scale;
	Outer[3].y = -(0.35f * Scale);

	Inner[0].x = 0.0f * Scale;
	Inner[0].y = -(0.06f * Scale);

	Inner[1].x = -(0.08f * Scale);
	Inner[1].y = -(0.32f * Scale);

	Inner[2].x = 0.00f * Scale;
	Inner[2].y = -(0.21f * Scale);

	Inner[3].x = 0.08f * Scale;
	Inner[3].y = -(0.32f * Scale);

	glDisable(GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(Mouse.X, Mouse.Y, 0);
	glRotatef(Rot, 0.0f, 0.0f, 1.0f);
	glBegin(GL_QUADS);

	for (int i = 0; i < 4; i++)
	{
		glColor3f(Outer[i].r, Outer[i].g, Outer[i].b);
		glVertex2f(Outer[i].x, Outer[i].y);
	}
	for (int i = 0; i < 4; i++)
	{
		glColor3f(Inner[i].r, Inner[i].g, Inner[i].b);
		glVertex2f(Inner[i].x, Inner[i].y);
	}

	glEnd();

	glEnable(GL_DEPTH_TEST);
}

size_t write_data(void* buffer, size_t size, size_t nmemb, void* userp)
{
	size_t bytes = size * nmemb;
	ASC_Log(LOGLEVEL_DEBUG, "CURL: New chunk (%u bytes)", bytes);

	return bytes;
}

int main(int argc, char** argv)
{
	{ int x = 0; ASC_TopOfTheStack = (u64)&x; }
	if (!Main_Init(argc, argv)) return 1;
	ASC_Log(LOGLEVEL_DEBUG, "MAIN: Stack Size: %u", ASC_GetCurrentStackSize());
	
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_STENCIL_TEST); //TODO: test these
	glEnable(GL_DEPTH_TEST); //TODO: test these

	GameState_Set(GS_INIT);
	void* Q1 = Game_InitRandomQuads();

	{
		int curltime = Timer_Add();
		CURL* curl = curl_easy_init();
		if (curl)
		{
			curl_easy_setopt(curl, CURLOPT_URL, "http://example.com");
			curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
			curl_easy_setopt(curl, CURLOPT_HEADER, 1);
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);

			CURLcode res = curl_easy_perform(curl);

			if (res != CURLE_OK)
			{
				ASC_Log(LOGLEVEL_ERROR, "CURL: Perform error: %s", curl_easy_strerror(res));
			}

			curl_easy_cleanup(curl);
		}
		ASC_Log(LOGLEVEL_INFO, "MAIN: CurlOp took %.02f ms", Timer_End(curltime));
	}

	while (State->Running)
	{
		Timer_FrameStart();
		Input_HandleEvents();
		if (Input_KeySingle(SDLK_F11)) Window_ToggleFullscreen();
		if (Input_KeySingle(SDLK_F10)) Window_ToggleMouse();
	#if _DEBUG
		if (Input_KeySingle('p')) if (Input_KeyDown(SDLK_LALT)) Window_Screenshot(0);
	#endif
		if (Input_KeySingle(SDLK_F12)) Window_Screenshot(0);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		Game_DrawRandomQuads(Q1);

		DrawCursor();
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		SDL_GL_SwapWindow(State->Window.Handle);
		Timer_FrameEnd();
	}

	Main_Quit();

	return 0;
}
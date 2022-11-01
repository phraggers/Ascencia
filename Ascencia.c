
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
 - MUST ensure at least 1MB STACK SIZE (1048576 BYTES) */

/*
* 
=-=-= TODO =-=-=

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

u64 ASC_GetCurrentStackSize()
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

static bool Main_Init()
{
	if (!Log_Init())
	{
		// this feels pointless, but meh
		SDL_Log("FATAL: Failed to allocate memory for Log!");
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Fatal", "Failed to allocate memory for Log!", 0);
		return 0;
	}

	State = (ASC_AppState*)State_Init(0);

	if (!State)
	{
		ASC_Log(LOGLEVEL_FATAL, "MAIN: Failed to allocate main memory!");
		return 0;
	}

	Timer_Init(60);
	int StartupTimer = Timer_Add();

	SDL_snprintf(State->AppName, 32, "Ascencia");
	SDL_snprintf(State->AppOrg, 32, "Phragware");

	{
		char* env = SDL_getenv("USERNAME");
		ASC_Log(LOGLEVEL_INFO, "MAIN: Login[%s]", ((env) ? env : "None"));
		if(env) SDL_free(env);
	}

	State->AppVersion.major = 0;
	State->AppVersion.minor = 1;
	State->AppVersion.patch = 0;
	State->PrefPath = SDL_GetPrefPath(State->AppOrg, State->AppName);
	State->BasePath = SDL_GetBasePath();

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

		int Freq = 48000;
		Uint16 Format = MIX_DEFAULT_FORMAT;
		int Channels = 2;
		int ChunkSize = 1024;

		if (Mix_OpenAudio(Freq, Format, Channels, ChunkSize) < 0)
		{
			ASC_Log(LOGLEVEL_FATAL, "MAIN: Failed to open Audio Device: %s", Mix_GetError());
			return 0;
		}

		ASC_Log(LOGLEVEL_INFO, "MAIN: AudioDevice Opened: Freq[%d] Format[%d] Channels[%d] Chunksize[%d]",
				Freq, Format, Channels, ChunkSize);
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

	Main_SetSDLGLAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	Main_SetSDLGLAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 1);
	Main_SetSDLGLAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
	Main_SetSDLGLAttribute(SDL_GL_DOUBLEBUFFER, 1);
	Main_SetSDLGLAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	Main_SetSDLGLAttribute(SDL_GL_MULTISAMPLESAMPLES, 8);
	Main_SetSDLGLAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	Main_SetSDLGLAttribute(SDL_GL_STENCIL_SIZE, 8);

	// Window
	State->Window.Dimensions.w = 960;
	State->Window.Dimensions.h = 540;

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

static void Main_Quit()
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

	u32 SDLFlags = SDL_WasInit(SDL_INIT_EVERYTHING);

	if ((SDLFlags |= SDL_INIT_EVERYTHING) > 0)
	{
		SDL_Quit();
		ASC_Log(LOGLEVEL_INFO, "MAIN: SDL Quit");
	}

	Log_Quit();
	State_Quit();
}

typedef struct
{
	r32 X;
	r32 Y;
} NormalizedMouse;
static NormalizedMouse GetNormalizedMouse()
{
	NormalizedMouse Result = { 0 };
	Result.X = (((r32)((r32)State->Input.Mouse.X / (r32)State->Window.Dimensions.w) * 2.0f) - 1.0f);
	Result.Y = -(((r32)((r32)State->Input.Mouse.Y / (r32)State->Window.Dimensions.h) * 2.0f) - 1.0f);
	return Result;
}

static void DrawCursor()
{
	if (State->Window.MouseShown) return;
	if (!State->Window.MouseFocus) return;

	r32 Scale = 0.12f;
	r32 Rot = 25.0f;

	NormalizedMouse Mouse = GetNormalizedMouse();

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
}

void* Main_InitRandomQuads()
{
	typedef struct { r32 x; r32 y; r32 r; r32 g; r32 b; } Vertex;
	typedef struct { Vertex Quad[4]; Vertex QuadTarget[4]; r32 Speed; } S_Quad;
	ASC_Alloc(S_Quad, RandomQuad);

	{
		Vertex Quad[] = { -0.5f, -0.5f, 1.0f, 1.0f, 1.0f };
		SDL_memcpy(&RandomQuad->Quad[0], Quad, sizeof(Vertex));
	}
	{
		Vertex Quad[] = { 0.5f, -0.5f, 1.0f, 1.0f, 1.0f };
		SDL_memcpy(&RandomQuad->Quad[1], Quad, sizeof(Vertex));
	}
	{
		Vertex Quad[] = { 0.5f, 0.5f, 1.0f, 1.0f, 1.0f };
		SDL_memcpy(&RandomQuad->Quad[2], Quad, sizeof(Vertex));
	}
	{
		Vertex Quad[] = { -0.5f, 0.5f, 1.0f, 1.0f, 1.0f };
		SDL_memcpy(&RandomQuad->Quad[3], Quad, sizeof(Vertex));
	}
	{
		Vertex QuadTarget[] = { -0.5f, -0.5f, 1.0f, 1.0f, 1.0f };
		SDL_memcpy(&RandomQuad->QuadTarget[0], QuadTarget, sizeof(Vertex));
	}
	{
		Vertex QuadTarget[] = { 0.5f, -0.5f, 1.0f, 1.0f, 1.0f };
		SDL_memcpy(&RandomQuad->QuadTarget[1], QuadTarget, sizeof(Vertex));
	}
	{
		Vertex QuadTarget[] = { 0.5f, 0.5f, 1.0f, 1.0f, 1.0f };
		SDL_memcpy(&RandomQuad->QuadTarget[2], QuadTarget, sizeof(Vertex));
	}
	{
		Vertex QuadTarget[] = { -0.5f, 0.5f, 1.0f, 1.0f, 1.0f };
		SDL_memcpy(&RandomQuad->QuadTarget[3], QuadTarget, sizeof(Vertex));
	}

	RandomQuad->Speed = 0.002f;

	return RandomQuad;
}

void Main_DrawRandomQuads(void* _RandomQuad)
{
	typedef struct { r32 x; r32 y; r32 r; r32 g; r32 b; } Vertex;
	typedef struct { Vertex Quad[4]; Vertex QuadTarget[4]; r32 Speed; } S_Quad;
	S_Quad* RandomQuad = (S_Quad*)_RandomQuad;

	r32 Speed = RandomQuad->Speed;

	if (Input_KeyRepeat(SDLK_UP))
	{
		if (Input_KeyDown(SDLK_LSHIFT)) Speed += 0.01f;
		else Speed += 0.0001f;
		ASC_Log(LOGLEVEL_INFO, "Speed: %.04f", Speed);
	}

	if (Input_KeyRepeat(SDLK_DOWN))
	{
		if (Input_KeyDown(SDLK_LSHIFT)) Speed -= 0.01f;
		else Speed -= 0.0001f;
		ASC_Log(LOGLEVEL_INFO, "Speed: %.04f", Speed);
	}

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glBegin(GL_QUADS);

	for (int i = 0; i < 4; i++)
	{
		if (RandomQuad->Quad[i].r < RandomQuad->QuadTarget[i].r)
		{
			if (RandomQuad->QuadTarget[i].r - RandomQuad->Quad[i].r < Speed)
			{
				RandomQuad->Quad[i].r = RandomQuad->QuadTarget[i].r;
			}
			else
			{
				RandomQuad->Quad[i].r += Speed;
			}
		}
		else if (RandomQuad->Quad[i].r > RandomQuad->QuadTarget[i].r)
		{
			if (RandomQuad->Quad[i].r - RandomQuad->QuadTarget[i].r < Speed)
			{
				RandomQuad->Quad[i].r = RandomQuad->QuadTarget[i].r;
			}
			else
			{
				RandomQuad->Quad[i].r -= Speed;
			}
		}
		else
		{
			RandomQuad->QuadTarget[i].r = Random_r32m(-1.0f, 1.0f);
		}

		if (RandomQuad->Quad[i].g < RandomQuad->QuadTarget[i].g)
		{
			if (RandomQuad->QuadTarget[i].g - RandomQuad->Quad[i].g < Speed)
			{
				RandomQuad->Quad[i].g = RandomQuad->QuadTarget[i].g;
			}
			else
			{
				RandomQuad->Quad[i].g += Speed;
			}
		}
		else if (RandomQuad->Quad[i].g > RandomQuad->QuadTarget[i].g)
		{
			if (RandomQuad->Quad[i].g - RandomQuad->QuadTarget[i].g < Speed)
			{
				RandomQuad->Quad[i].g = RandomQuad->QuadTarget[i].g;
			}
			else
			{
				RandomQuad->Quad[i].g -= Speed;
			}
		}
		else
		{
			RandomQuad->QuadTarget[i].g = Random_r32m(-1.0f, 1.0f);
		}

		if (RandomQuad->Quad[i].b < RandomQuad->QuadTarget[i].b)
		{
			if (RandomQuad->QuadTarget[i].b - RandomQuad->Quad[i].b < Speed)
			{
				RandomQuad->Quad[i].b = RandomQuad->QuadTarget[i].b;
			}
			else
			{
				RandomQuad->Quad[i].b += Speed;
			}
		}
		else if (RandomQuad->Quad[i].b > RandomQuad->QuadTarget[i].b)
		{
			if (RandomQuad->Quad[i].b - RandomQuad->QuadTarget[i].b < Speed)
			{
				RandomQuad->Quad[i].b = RandomQuad->QuadTarget[i].b;
			}
			else
			{
				RandomQuad->Quad[i].b -= Speed;
			}
		}
		else
		{
			RandomQuad->QuadTarget[i].b = Random_r32m(-1.0f, 1.0f);
		}

		if (RandomQuad->Quad[i].x < RandomQuad->QuadTarget[i].x)
		{
			if (RandomQuad->QuadTarget[i].x - RandomQuad->Quad[i].x < Speed)
			{
				RandomQuad->Quad[i].x = RandomQuad->QuadTarget[i].x;
			}
			else
			{
				RandomQuad->Quad[i].x += Speed;
			}
		}
		else if (RandomQuad->Quad[i].x > RandomQuad->QuadTarget[i].x)
		{
			if (RandomQuad->Quad[i].x - RandomQuad->QuadTarget[i].x < Speed)
			{
				RandomQuad->Quad[i].x = RandomQuad->QuadTarget[i].x;
			}
			else
			{
				RandomQuad->Quad[i].x -= Speed;
			}
		}
		else
		{
			RandomQuad->QuadTarget[i].x = Random_r32m(-1.0f, 1.0f);
		}

		if (RandomQuad->Quad[i].y < RandomQuad->QuadTarget[i].y)
		{
			if (RandomQuad->QuadTarget[i].y - RandomQuad->Quad[i].y < Speed)
			{
				RandomQuad->Quad[i].y = RandomQuad->QuadTarget[i].y;
			}
			else
			{
				RandomQuad->Quad[i].y += Speed;
			}
		}
		else if (RandomQuad->Quad[i].y > RandomQuad->QuadTarget[i].y)
		{
			if (RandomQuad->Quad[i].y - RandomQuad->QuadTarget[i].y < Speed)
			{
				RandomQuad->Quad[i].y = RandomQuad->QuadTarget[i].y;
			}
			else
			{
				RandomQuad->Quad[i].y -= Speed;
			}
		}
		else
		{
			RandomQuad->QuadTarget[i].y = Random_r32m(-1.0f, 1.0f);
		}

		glColor3f(RandomQuad->Quad[i].r, RandomQuad->Quad[i].g, RandomQuad->Quad[i].b);
		glVertex2f(RandomQuad->Quad[i].x, RandomQuad->Quad[i].y);
	}

	glEnd();

	RandomQuad->Speed = Speed;
}


int main(int argc, char** argv)
{
	{ int x = 0; ASC_TopOfTheStack = (u64)&x; }
	ASC_Log(LOGLEVEL_DEBUG, "MAIN: Stack Size: %u", ASC_GetCurrentStackSize());

	if (!Main_Init()) return 1;
	ASC_Log(LOGLEVEL_DEBUG, "MAIN: Stack Size: %u", ASC_GetCurrentStackSize());
	glEnable(GL_MULTISAMPLE);

	void* RndQuad = Main_InitRandomQuads();

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

		Main_DrawRandomQuads(RndQuad);

		DrawCursor();
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		SDL_GL_SwapWindow(State->Window.Handle);
		Timer_FrameEnd();
	}

	Main_Quit();

	return 0;
}
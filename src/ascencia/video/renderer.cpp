// Ascencia
// video/renderer.cpp

#include <ascencia/platform/core.h>

int Renderer_Thread(void* Data)
{
	if (!Core->Window.Renderer.Run())
	{
		return -1;
	}
	else
	{
		return 0;
	}
}

void cRenderer::SetRunning(bool Running)
{
	SDL_WaitSemaphore(State.Lock);
	State.Running = Running;
	SDL_SignalSemaphore(State.Lock);
}

bool cRenderer::GetRunning(void)
{
	SDL_WaitSemaphore(State.Lock);
	bool Result = State.Running;
	SDL_SignalSemaphore(State.Lock);
	return Result;
}

cRenderer::cRenderer(void)
{
	memset(&State, 0, sizeof(sRendererState));
}

bool cRenderer::Setup(void)
{
	// must happen on main thread before Start()
	SDL_assert(State.Thread == 0);

	if (!SDL_GL_LoadLibrary(0))
	{
		LOG_FATAL("Renderer::Setup", std::string("SDL GL failed: ", SDL_GetError()));
		return 0;
	}
	else
	{
		if (!SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1)) LOG_ERROR("Renderer::Setup", std::string("SDLGL Error: ", SDL_GetError()));
		if (!SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE)) LOG_ERROR("Renderer::Setup", std::string("SDLGL Error: ", SDL_GetError()));
		if (!SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, CFG_WINDOW_GL_VERSION_MAJ)) LOG_ERROR("Renderer::Setup", std::string("SDLGL Error: ", SDL_GetError()));
		if (!SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, CFG_WINDOW_GL_VERSION_MIN)) LOG_ERROR("Renderer::Setup", std::string("SDLGL Error: ", SDL_GetError()));
		if (!SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1)) LOG_ERROR("Renderer::Setup", std::string("SDLGL Error: ", SDL_GetError()));
		if (!SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24)) LOG_ERROR("Renderer::Setup", std::string("SDLGL Error: ", SDL_GetError()));
	}

	return 1;
}

bool cRenderer::Start(void)
{
	State.Window = &Core->Window.Handle;
	State.Lock = SDL_CreateSemaphore(1);
	if (!State.Lock)
	{
		LOG_FATAL("Renderer::Start", std::string("Failed to create Renderer Lock Semaphore: ", SDL_GetError()));
		return 0;
	}

	SDL_WaitSemaphore(State.Lock);
	State.Thread = SDL_CreateThread(Renderer_Thread, "RendererThread", 0);
	if (!SDL_WaitSemaphoreTimeout(State.Lock, 1000))
	{
		LOG_FATAL("Renderer::Start", "Error starting RendererThread, Thread stuck");
		return 0;
	}
	SDL_SignalSemaphore(State.Lock);

	if (State.Thread)
	{
		LOG_INFO("Renderer::Start", "RendererThread Started");
		return 1;
	}
	else
	{
		LOG_FATAL("Renderer::Start", std::string("Error starting Renderer Thread: ", SDL_GetError()));
		return 0;
	}
}

void cRenderer::Quit(void)
{
	SetRunning(0);
	int ThreadResult;
	SDL_WaitThread(State.Thread, &ThreadResult);
	if (ThreadResult == -1) LOG_ERROR("Renderer::Quit", "RenderThread Quit with error");
	else  LOG_INFO("Renderer::Quit", "RenderThread Quit");
}

bool cRenderer::Run(void)
{
	State.GLRC = SDL_GL_CreateContext(*State.Window);
	if (!State.GLRC)
	{
		LOG_FATAL("Renderer::Run", std::string("SDLGL CreateContext Error: ", SDL_GetError()));
		return 0;
	}

	gladLoadGL(SDL_GL_GetProcAddress);
	SDL_GL_SetSwapInterval(CFG_WINDOW_VSYNC);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glViewport(0, 0, CFG_WINDOW_W, CFG_WINDOW_H);

	{
		std::stringstream ss;
		ss << "GLAD loaded GL " << CFG_WINDOW_GL_VERSION_MAJ << "." << CFG_WINDOW_GL_VERSION_MIN;
		LOG_PRINT("Renderer::Run", ss.str());
	}
	{
		std::stringstream ss;
		ss << "GL Vendor: " << glGetString(GL_VENDOR);
		LOG_PRINT("Renderer::Run", ss.str());
	}
	{
		std::stringstream ss;
		ss << "GL Renderer: " << glGetString(GL_RENDERER);
		LOG_PRINT("Renderer::Run", ss.str());
	}
	{
		std::stringstream ss;
		ss << "GL: " << glGetString(GL_VERSION);
		LOG_PRINT("Renderer::Run", ss.str());
	}
	{
		std::stringstream ss;
		ss << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION);
		LOG_PRINT("Renderer::Run", ss.str());
	}

	SDL_SignalSemaphore(State.Lock);
	SetRunning(1);

	while (GetRunning())
	{
		Frame();
	}

	return 1;
}

bool cRenderer::Frame(void)
{
	static f32 r = 0.0f;
	static f32 g = 0.0f;
	static f32 b = 0.0f;

	glClearColor(r, g, b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	SDL_GL_SwapWindow(*State.Window);

	r += 0.01f;
	g += 0.02f;
	b += 0.03f;

	if (r >= 1.0f) r = 0.0f;
	if (g >= 1.0f) g = 0.0f;
	if (b >= 1.0f) b = 0.0f;

	return 1;
}

void cRenderer::SetVSync(bool VSync)
{
	SDL_WaitSemaphore(State.Lock);
	if(VSync) SDL_GL_SetSwapInterval(1);
	else SDL_GL_SetSwapInterval(0);
	SDL_SignalSemaphore(State.Lock);
}
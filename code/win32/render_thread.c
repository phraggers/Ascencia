/* ==============================================================
   Project: Ascencia
   File:    render_thread.c
   Author:  Phraggers
   Date:    18-11-2024
   ============================================================== */

#include <win32/platform.h>

local b32 RenderFrameTimerStart(void)
{
    if(timeBeginPeriod(g_state->timer.min_ms_wait) == TIMERR_NOCANDO)
    {
        g_state->running = 0;
        return 0;
    }

    g_state->timer.frames++;
    g_state->timer.frame_timer = PL_QueryTimer();

    if(timeEndPeriod(g_state->timer.min_ms_wait) == TIMERR_NOCANDO)
    {
        g_state->running = 0;
        return 0;
    }

    return 1;
}

local void RenderFrameTimerEnd(void)
{
    if(timeBeginPeriod(g_state->timer.min_ms_wait) == TIMERR_NOCANDO)
    {
        g_state->running = 0;
        return;
    }

    g_state->timer.elapsed_ms = (PL_TimeElapsed(g_state->timer.frame_timer, PL_QueryTimer()) * 1000.0f);
    if(!g_state->window.vsync)
    {
        if((u32)g_state->timer.elapsed_ms < g_state->timer.target_ms)
        {
            Sleep((u32)(g_state->timer.target_ms - g_state->timer.elapsed_ms));
        }
    }

    if(timeEndPeriod(g_state->timer.min_ms_wait) == TIMERR_NOCANDO)
    {
        g_state->running = 0;
        return;
    }
}

local inline void GLErrorCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
                                  GLsizei length, const GLchar *message, const void *user)
{
    const char *source_s;
	switch(source)
    {
		case GL_DEBUG_SOURCE_API: source_s = "API"; break;
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM: source_s = "WINDOW_SYSTEM"; break;
		case GL_DEBUG_SOURCE_SHADER_COMPILER: source_s = "SHADER_COMPILER"; break;
		case GL_DEBUG_SOURCE_THIRD_PARTY: source_s = "THIRD_PARTY"; break;
		case GL_DEBUG_SOURCE_APPLICATION: source_s = "APPLICATION"; break;
		case GL_DEBUG_SOURCE_OTHER: source_s = "OTHER"; break;
		default: source_s = "UNKNOWN_SOURCE"; break;
	}

	const char *type_s;
	switch(type)
    {
		case GL_DEBUG_TYPE_ERROR: type_s = "ERROR"; break;
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: type_s = "DEPRECATED_BEHAVIOR"; break;
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: type_s = "UNDEFINED_BEHAVIOR"; break;
		case GL_DEBUG_TYPE_PORTABILITY: type_s = "PORTABILITY"; break;
		case GL_DEBUG_TYPE_PERFORMANCE: type_s = "PERFORMANCE"; break;
		case GL_DEBUG_TYPE_MARKER: type_s = "MARKER"; break;
		case GL_DEBUG_TYPE_PUSH_GROUP: type_s = "PUSH_GROUP"; break;
		case GL_DEBUG_TYPE_POP_GROUP: type_s = "POP_GROUP"; break;
		case GL_DEBUG_TYPE_OTHER: type_s = "OTHER"; break;
		default: type_s = "UNKNOWN_TYPE"; break;
	}

	const char *severity_s;
	switch(severity)
    {
		case GL_DEBUG_SEVERITY_HIGH: severity_s = "HIGH"; break;
		case GL_DEBUG_SEVERITY_MEDIUM: severity_s = "MEDIUM"; break;
		case GL_DEBUG_SEVERITY_LOW: severity_s = "LOW"; break;
		case GL_DEBUG_SEVERITY_NOTIFICATION: severity_s = "NOTIFICATION"; break;
		default: severity_s = "UNKNOWN_SEVERITY"; break;
	}

	const char *id_s;
	switch(id)
    {
		case GL_NO_ERROR: id_s = "NO_ERROR"; break;
		case GL_INVALID_ENUM: id_s = "INVALID_ENUM"; break;
		case GL_INVALID_VALUE: id_s = "INVALID_VALUE"; break;
		case GL_INVALID_OPERATION: id_s = "INVALID_OPERATION"; break;
		case GL_STACK_OVERFLOW: id_s = "STACK_OVERFLOW"; break;
		case GL_STACK_UNDERFLOW: id_s = "STACK_UNDERFLOW"; break;
		case GL_OUT_OF_MEMORY: id_s = "OUT_OF_MEMORY"; break;
		case GL_INVALID_FRAMEBUFFER_OPERATION: id_s = "INVALID_FRAMEBUFFER_OPERATION"; break;
		case GL_CONTEXT_LOST: id_s = "CONTEXT_LOST"; break;
            //case GL_TABLE_TOO_LARGE: id_s = "TABLE_TOO_LARGE"; break;
		default: id_s = "UNKNOWN_ID"; break;
	}

    char log[1024] = {0};
    snprintf(log, 1024, "OpenGL message caught:\n\t`[source  ] - %s(0x%x)`\n\t`[type    ] - %s(0x%x)`\n\t`[id      ] - %s(0x%x)`\n\t`[severity] - %s(0x%x)`\n\nGot length %d message:\n`%s`\n\n\n", source_s, source, type_s, type, id_s, id, severity_s, severity, length, message);
    OutputDebugStringA(log);
}

DWORD WINAPI RenderThread(LPVOID param)
{
    g_state->render_thread.mutex = CreateMutexA(0, 0, 0);
    WaitForSingleObject(g_state->render_thread.mutex, INFINITE);
    ReleaseMutex(g_state->render_thread.mutex);

    HWND *hwnd = &g_state->window.handle;
    HDC *dc = &g_state->window.dc;
    HGLRC *rc = &g_state->window.rc;

    if(!wglMakeCurrent(*dc, *rc))
    {
        g_state->running = 0;
        return -1;
    }

    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback((GLDEBUGPROC)&GLErrorCallback, 0);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, 0, GL_FALSE);

    GLint profile_mask = 0;
    GLint context_flags = 0;
    glGetIntegerv(GL_CONTEXT_PROFILE_MASK, &profile_mask);
    glGetIntegerv(GL_CONTEXT_FLAGS, &context_flags);

    char log[1024] = {0};
    snprintf(log, 1024, "OpenGL: Renderer[%s] Vendor[%s] Version[%s] ShaderVersion[%s] CoreProfile[%d] CompatabilityProfile[%d] ForwardCompatible[%d] DebugFlag[%d] RobustAccess[%d]\n",
             glGetString(GL_RENDERER), glGetString(GL_VENDOR),
             glGetString(GL_VERSION), glGetString(GL_SHADING_LANGUAGE_VERSION),
             profile_mask & GL_CONTEXT_CORE_PROFILE_BIT ? 1:0,
             profile_mask & GL_CONTEXT_COMPATIBILITY_PROFILE_BIT ? 1:0,
             context_flags & GL_CONTEXT_FLAG_FORWARD_COMPATIBLE_BIT ? 1:0,
             context_flags & GL_CONTEXT_FLAG_DEBUG_BIT ? 1:0,
             context_flags & GL_CONTEXT_FLAG_ROBUST_ACCESS_BIT ? 1:0);
    OutputDebugStringA(log);

    if(PL_ENABLE_VSYNC)
    {
        if(wglSwapIntervalEXT(1))
        {
            g_state->window.vsync = 1;
        }
    }
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glViewport(0, 0, g_state->window.w, g_state->window.h);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    if(!SwapBuffers(*dc))
    {
        g_state->running = 0;
        return -1;
    }
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    if(!SwapBuffers(*dc))
    {
        g_state->running = 0;
        return -1;
    }
    if(!ShowWindow(*hwnd, 1))
    {
        g_state->running = 0;
        return -1;
    }

    g_state->timer.target_ms = 1000.0f / (r32)PL_TARGET_FPS;
    g_state->timer.frame_timer = PL_QueryTimer();
    g_state->timer.elapsed_ms = 0.0f;

    srand(time(0));

    while(g_state->running)
    {
        if(!RenderFrameTimerStart()) return -1;

        //TODO: handle user input

        //TODO: handle game logic

#if 1 //TEST: screensaver simulator
        persist r32 target[3] = {0.0f, 0.0f, 0.0f};
        persist r32 current[3] = {0.0f, 0.0f, 0.0f};
        persist r32 speed = 0.001f;

        for(int i=0;i<3;i++)
        {
            if(current[i] > target[i])
            {
                current[i] -= speed;
                if(current[i] < target[i]) current[i] = target[i];
            }
            else if(current[i] < target[i])
            {
                current[i] += speed;
                if(current[i] > target[i]) current[i] = target[i];
            }
        }

        if(current[0]==target[0] && current[1]==target[1] && current[2]==target[2])
        {
            target[0] = (r32)rand()/(r32)RAND_MAX;
            target[1] = (r32)rand()/(r32)RAND_MAX;
            target[2] = (r32)rand()/(r32)RAND_MAX;
        }

        glClearColor(current[0], current[1], current[2], 0.0f);
#endif

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        //TODO: rendering

        if(!SwapBuffers(*dc))
        {
            g_state->running = 0;
            return -1;
        }

        RenderFrameTimerEnd();
    }

    return 0;
}

b32 PL_StartRenderThread(void)
{
    sThread *thread = &g_state->render_thread;
    thread->handle = CreateThread(0, 0, RenderThread, 0, 0, &thread->id);
    if(!thread->handle) return 0;
    return 1;
}

b32 PL_QuitRenderThread(void)
{
    sThread *thread = &g_state->render_thread;
    WaitForSingleObject(thread->handle, 0);
    DWORD ecode = 0;
    if(!GetExitCodeThread(thread->handle, &ecode)) return 0;

    int attempts = 100;
    while(ecode == 259 && attempts >= 0)
    {
        WaitForSingleObject(thread->handle, 15);
        if(!GetExitCodeThread(thread->handle, &ecode)) return 0;
    }

    if(ecode)
    {
        if(attempts < 0) return 0; //thread hanging
        else return 0; //error
    }

    CloseHandle(g_state->render_thread.mutex);
    return 1;
}

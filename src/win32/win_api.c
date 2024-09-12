
/*============================================================
 * Project: Ascencia [EXE]
 * File: win_api.c
 * Author: Phraggers
 * Date: Wed Sep 11 2024
============================================================*/

#include <util/types.h>
#include <win32/shared.h>
#include <window/opengl.h>

local bool Load_shlwapi(void);
local bool Load_user32(void);
local bool Load_gdi32(void);
local bool Load_opengl32(void);
local bool Load_wgldriver(void);
local bool Load_OpenGL(r32 version);

bool WIN_WinAPIInit(void)
{
    Assert(G_win32_data != 0);

    if(!Load_shlwapi())
    {
        return 0;
    }

    if(!Load_user32())
    {
        return 0;
    }

    if(!Load_gdi32())
    {
        return 0;
    }

    if(!Load_opengl32())
    {
        return 0;
    }

    return 1;
}

bool WIN_OpenGLInit(void)
{
    if(!Load_wgldriver())
    {
        return 0;
    }

    if(!Load_OpenGL((r32)ASC_OPENGL_MAJ + ((r32)ASC_OPENGL_MIN/10.0f)))
    {
        return 0;
    }

    return 1;
}

#define LOAD_WIN_FN(name) {WINAPI.##name = (pWin32_##name)WIN_GetProcAddress(lib, #name); if(!WINAPI.##name) return 0;}

local bool Load_shlwapi(void)
{
    ptr lib = WIN_LoadLibrary("shlwapi.dll");

    if(!lib)
    {
        return 0;
    }

    LOAD_WIN_FN(PathFileExistsA);

    return 1;
}

local bool Load_user32(void)
{
    ptr lib = WIN_LoadLibrary("user32.dll");

    if(!lib)
    {
        return 0;
    }

    LOAD_WIN_FN(TranslateMessage);
    LOAD_WIN_FN(DispatchMessageA);
    LOAD_WIN_FN(PeekMessageA);
    LOAD_WIN_FN(DefWindowProcA);
    LOAD_WIN_FN(RegisterClassA);
    LOAD_WIN_FN(CreateWindowExA);
    LOAD_WIN_FN(DestroyWindow);
    LOAD_WIN_FN(MoveWindow);
    LOAD_WIN_FN(SetWindowPos);
    LOAD_WIN_FN(GetWindowPlacement);
    LOAD_WIN_FN(SetWindowPlacement);
    LOAD_WIN_FN(GetDC);
    LOAD_WIN_FN(ReleaseDC);
    LOAD_WIN_FN(SetWindowTextA);
    LOAD_WIN_FN(MessageBoxA);
    LOAD_WIN_FN(GetCursorPos);
    LOAD_WIN_FN(ScreenToClient);
    LOAD_WIN_FN(GetWindowLongA);
    LOAD_WIN_FN(SetWindowLongA);
    LOAD_WIN_FN(MonitorFromWindow);
    LOAD_WIN_FN(GetMonitorInfoA);
    LOAD_WIN_FN(GetClientRect);
    LOAD_WIN_FN(GetWindowRect);

    return 1;
}

local bool Load_gdi32(void)
{
    ptr lib = WIN_LoadLibrary("gdi32.dll");

    if(!lib)
    {
        return 0;
    }

    LOAD_WIN_FN(ChoosePixelFormat);
    LOAD_WIN_FN(DescribePixelFormat);
    LOAD_WIN_FN(SetPixelFormat);
    LOAD_WIN_FN(SwapBuffers);

    return 1;
}

local bool Load_opengl32(void)
{
    ptr lib = WIN_LoadLibrary("opengl32.dll");

    if(!lib)
    {
        return 0;
    }

    LOAD_WIN_FN(wglCreateContext);
    LOAD_WIN_FN(wglDeleteContext);
    LOAD_WIN_FN(wglGetProcAddress);
    LOAD_WIN_FN(wglMakeCurrent);

    return 1;
}

#define LOAD_WGL_FN(name) {WINAPI.##name = (pWin32_##name)WINAPI.wglGetProcAddress( #name ); if(!WINAPI.##name) return 0;}

local bool Load_wgldriver(void)
{
    {
        G_win32_data->win_api.wglChoosePixelFormatARB = (pWin32_wglChoosePixelFormatARB)
        G_win32_data->win_api.wglGetProcAddress( "wglChoosePixelFormatARB" ); 
        
        if(!G_win32_data->win_api.wglChoosePixelFormatARB) return 0;
    }

    //LOAD_WGL_FN(wglChoosePixelFormatARB);
    LOAD_WGL_FN(wglCreateContextAttribsARB);
    LOAD_WGL_FN(wglSwapIntervalEXT);

    return 1;
}

#define LOAD_GL_FN(name) name = (pfn_##name)WINAPI.wglGetProcAddress( #name );\
if(!name) {name = (pfn_##name)WIN_GetProcAddress(lib, #name );} if(!name) {return 0;}

local bool Load_OpenGL(r32 version)
{
    ptr lib = WIN_LoadLibrary("opengl32.dll");

    if(!lib)
    {
        return 0;
    }

    if(version >= 1.0)
    {
        LOAD_GL_FN(glCullFace);
        LOAD_GL_FN(glFrontFace);
        LOAD_GL_FN(glHint);
        LOAD_GL_FN(glLineWidth);
        LOAD_GL_FN(glPointSize);
        LOAD_GL_FN(glPolygonMode);
        LOAD_GL_FN(glScissor);
        LOAD_GL_FN(glTexParameterf);
        LOAD_GL_FN(glTexParameterfv);
        LOAD_GL_FN(glTexParameteri);
        LOAD_GL_FN(glTexParameteriv);
        LOAD_GL_FN(glTexImage1D);
        LOAD_GL_FN(glTexImage2D);
        LOAD_GL_FN(glDrawBuffer);
        LOAD_GL_FN(glClear);
        LOAD_GL_FN(glClearColor);
        LOAD_GL_FN(glClearStencil);
        LOAD_GL_FN(glClearDepth);
        LOAD_GL_FN(glStencilMask);
        LOAD_GL_FN(glColorMask);
        LOAD_GL_FN(glDepthMask);
        LOAD_GL_FN(glDisable);
        LOAD_GL_FN(glEnable);
        LOAD_GL_FN(glFinish);
        LOAD_GL_FN(glFlush);
        LOAD_GL_FN(glBlendFunc);
        LOAD_GL_FN(glLogicOp);
        LOAD_GL_FN(glStencilFunc);
        LOAD_GL_FN(glStencilOp);
        LOAD_GL_FN(glDepthFunc);
        LOAD_GL_FN(glPixelStoref);
        LOAD_GL_FN(glPixelStorei);
        LOAD_GL_FN(glReadBuffer);
        LOAD_GL_FN(glReadPixels);
        LOAD_GL_FN(glGetBooleanv);
        LOAD_GL_FN(glGetDoublev);
        LOAD_GL_FN(glGetError);
        LOAD_GL_FN(glGetFloatv);
        LOAD_GL_FN(glGetIntegerv);
        LOAD_GL_FN(glGetString);
        LOAD_GL_FN(glGetTexImage);
        LOAD_GL_FN(glGetTexParameterfv);
        LOAD_GL_FN(glGetTexParameteriv);
        LOAD_GL_FN(glGetTexLevelParameterfv);
        LOAD_GL_FN(glGetTexLevelParameteriv);
        LOAD_GL_FN(glIsEnabled);
        LOAD_GL_FN(glDepthRange);
        LOAD_GL_FN(glViewport);
    }
    
    if(version >= 1.1)
    {
        LOAD_GL_FN(glDrawArrays);
        LOAD_GL_FN(glDrawElements);
        LOAD_GL_FN(glPolygonOffset);
        LOAD_GL_FN(glCopyTexImage1D);
        LOAD_GL_FN(glCopyTexImage2D);
        LOAD_GL_FN(glCopyTexSubImage1D);
        LOAD_GL_FN(glCopyTexSubImage2D);
        LOAD_GL_FN(glTexSubImage1D);
        LOAD_GL_FN(glTexSubImage2D);
        LOAD_GL_FN(glBindTexture);
        LOAD_GL_FN(glDeleteTextures);
        LOAD_GL_FN(glGenTextures);
        LOAD_GL_FN(glIsTexture);
    }
    
    if(version >= 1.2)
    {
        LOAD_GL_FN(glDrawRangeElements);
        LOAD_GL_FN(glTexImage3D);
        LOAD_GL_FN(glTexSubImage3D);
        LOAD_GL_FN(glCopyTexSubImage3D);
    }
    
    if(version >= 1.3)
    {
        LOAD_GL_FN(glActiveTexture);
        LOAD_GL_FN(glSampleCoverage);
        LOAD_GL_FN(glCompressedTexImage3D);
        LOAD_GL_FN(glCompressedTexImage2D);
        LOAD_GL_FN(glCompressedTexImage1D);
        LOAD_GL_FN(glCompressedTexSubImage3D);
        LOAD_GL_FN(glCompressedTexSubImage2D);
        LOAD_GL_FN(glCompressedTexSubImage1D);
        LOAD_GL_FN(glGetCompressedTexImage);
    }
    
    if(version >= 1.4)
    {
        LOAD_GL_FN(glBlendFuncSeparate);
        LOAD_GL_FN(glMultiDrawArrays);
        LOAD_GL_FN(glMultiDrawElements);
        LOAD_GL_FN(glPointParameterf);
        LOAD_GL_FN(glPointParameterfv);
        LOAD_GL_FN(glPointParameteri);
        LOAD_GL_FN(glPointParameteriv);
        LOAD_GL_FN(glBlendColor);
        LOAD_GL_FN(glBlendEquation);
    }
    
    if(version >= 1.5)
    {
        LOAD_GL_FN(glGenQueries);
        LOAD_GL_FN(glDeleteQueries);
        LOAD_GL_FN(glIsQuery);
        LOAD_GL_FN(glBeginQuery);
        LOAD_GL_FN(glEndQuery);
        LOAD_GL_FN(glGetQueryiv);
        LOAD_GL_FN(glGetQueryObjectiv);
        LOAD_GL_FN(glGetQueryObjectuiv);
        LOAD_GL_FN(glBindBuffer);
        LOAD_GL_FN(glDeleteBuffers);
        LOAD_GL_FN(glGenBuffers);
        LOAD_GL_FN(glIsBuffer);
        LOAD_GL_FN(glBufferData);
        LOAD_GL_FN(glBufferSubData);
        LOAD_GL_FN(glGetBufferSubData);
        LOAD_GL_FN(glMapBuffer);
        LOAD_GL_FN(glUnmapBuffer);
        LOAD_GL_FN(glGetBufferParameteriv);
        LOAD_GL_FN(glGetBufferPointerv);
    }
    
    if(version >= 2.0)
    {
        LOAD_GL_FN(glBlendEquationSeparate);
        LOAD_GL_FN(glDrawBuffers);
        LOAD_GL_FN(glStencilOpSeparate);
        LOAD_GL_FN(glStencilFuncSeparate);
        LOAD_GL_FN(glStencilMaskSeparate);
        LOAD_GL_FN(glAttachShader);
        LOAD_GL_FN(glBindAttribLocation);
        LOAD_GL_FN(glCompileShader);
        LOAD_GL_FN(glCreateProgram);
        LOAD_GL_FN(glCreateShader);
        LOAD_GL_FN(glDeleteProgram);
        LOAD_GL_FN(glDeleteShader);
        LOAD_GL_FN(glDetachShader);
        LOAD_GL_FN(glDisableVertexAttribArray);
        LOAD_GL_FN(glEnableVertexAttribArray);
        LOAD_GL_FN(glGetActiveAttrib);
        LOAD_GL_FN(glGetActiveUniform);
        LOAD_GL_FN(glGetAttachedShaders);
        LOAD_GL_FN(glGetAttribLocation);
        LOAD_GL_FN(glGetProgramiv);
        LOAD_GL_FN(glGetProgramInfoLog);
        LOAD_GL_FN(glGetShaderiv);
        LOAD_GL_FN(glGetShaderInfoLog);
        LOAD_GL_FN(glGetShaderSource);
        LOAD_GL_FN(glGetUniformLocation);
        LOAD_GL_FN(glGetUniformfv);
        LOAD_GL_FN(glGetUniformiv);
        LOAD_GL_FN(glGetVertexAttribdv);
        LOAD_GL_FN(glGetVertexAttribfv);
        LOAD_GL_FN(glGetVertexAttribiv);
        LOAD_GL_FN(glGetVertexAttribPointerv);
        LOAD_GL_FN(glIsProgram);
        LOAD_GL_FN(glIsShader);
        LOAD_GL_FN(glLinkProgram);
        LOAD_GL_FN(glShaderSource);
        LOAD_GL_FN(glUseProgram);
        LOAD_GL_FN(glUniform1f);
        LOAD_GL_FN(glUniform2f);
        LOAD_GL_FN(glUniform3f);
        LOAD_GL_FN(glUniform4f);
        LOAD_GL_FN(glUniform1i);
        LOAD_GL_FN(glUniform2i);
        LOAD_GL_FN(glUniform3i);
        LOAD_GL_FN(glUniform4i);
        LOAD_GL_FN(glUniform1fv);
        LOAD_GL_FN(glUniform2fv);
        LOAD_GL_FN(glUniform3fv);
        LOAD_GL_FN(glUniform4fv);
        LOAD_GL_FN(glUniform1iv);
        LOAD_GL_FN(glUniform2iv);
        LOAD_GL_FN(glUniform3iv);
        LOAD_GL_FN(glUniform4iv);
        LOAD_GL_FN(glUniformMatrix2fv);
        LOAD_GL_FN(glUniformMatrix3fv);
        LOAD_GL_FN(glUniformMatrix4fv);
        LOAD_GL_FN(glValidateProgram);
        LOAD_GL_FN(glVertexAttrib1d);
        LOAD_GL_FN(glVertexAttrib1dv);
        LOAD_GL_FN(glVertexAttrib1f);
        LOAD_GL_FN(glVertexAttrib1s);
        LOAD_GL_FN(glVertexAttrib1sv);
        LOAD_GL_FN(glVertexAttrib2d);
        LOAD_GL_FN(glVertexAttrib2dv);
        LOAD_GL_FN(glVertexAttrib2f);
        LOAD_GL_FN(glVertexAttrib2fv);
        LOAD_GL_FN(glVertexAttrib2s);
        LOAD_GL_FN(glVertexAttrib2sv);
        LOAD_GL_FN(glVertexAttrib3d);
        LOAD_GL_FN(glVertexAttrib3dv);
        LOAD_GL_FN(glVertexAttrib3f);
        LOAD_GL_FN(glVertexAttrib3fv);
        LOAD_GL_FN(glVertexAttrib3s);
        LOAD_GL_FN(glVertexAttrib3sv);
        LOAD_GL_FN(glVertexAttrib4Nbv);
        LOAD_GL_FN(glVertexAttrib4Niv);
        LOAD_GL_FN(glVertexAttrib4Nsv);
        LOAD_GL_FN(glVertexAttrib4Nub);
        LOAD_GL_FN(glVertexAttrib4Nubv);
        LOAD_GL_FN(glVertexAttrib4Nuiv);
        LOAD_GL_FN(glVertexAttrib4Nusv);
        LOAD_GL_FN(glVertexAttrib4bv);
        LOAD_GL_FN(glVertexAttrib4d);
        LOAD_GL_FN(glVertexAttrib4dv);
        LOAD_GL_FN(glVertexAttrib4f);
        LOAD_GL_FN(glVertexAttrib4fv);
        LOAD_GL_FN(glVertexAttrib4iv);
        LOAD_GL_FN(glVertexAttrib4s);
        LOAD_GL_FN(glVertexAttrib4sv);
        LOAD_GL_FN(glVertexAttrib4ubv);
        LOAD_GL_FN(glVertexAttrib4uiv);
        LOAD_GL_FN(glVertexAttrib4usv);
        LOAD_GL_FN(glVertexAttribPointer);
    }
    
    if(version >= 2.1)
    {
        LOAD_GL_FN(glUniformMatrix2x3fv);
        LOAD_GL_FN(glUniformMatrix3x2fv);
        LOAD_GL_FN(glUniformMatrix2x4fv);
        LOAD_GL_FN(glUniformMatrix4x2fv);
        LOAD_GL_FN(glUniformMatrix3x4fv);
        LOAD_GL_FN(glUniformMatrix4x3fv);
    }
    
    if(version >= 3.0)
    {
        LOAD_GL_FN(glColorMaski);
        LOAD_GL_FN(glGetBooleani_v);
        LOAD_GL_FN(glGetIntegeri_v);
        LOAD_GL_FN(glEnablei);
        LOAD_GL_FN(glDisablei);
        LOAD_GL_FN(glIsEnabledi);
        LOAD_GL_FN(glBeginTransformFeedback);
        LOAD_GL_FN(glEndTransformFeedback);
        LOAD_GL_FN(glBindBufferRange);
        LOAD_GL_FN(glBindBufferBase);
        LOAD_GL_FN(glTransformFeedbackVaryings);
        LOAD_GL_FN(glGetTransformFeedbackVarying);
        LOAD_GL_FN(glClampColor);
        LOAD_GL_FN(glBeginConditionalRender);
        LOAD_GL_FN(glEndConditionalRender);
        LOAD_GL_FN(glVertexAttribIPointer);
        LOAD_GL_FN(glGetVertexAttribIiv);
        LOAD_GL_FN(glGetVertexAttribIuiv);
        LOAD_GL_FN(glVertexAttribI1i);
        LOAD_GL_FN(glVertexAttribI2i);
        LOAD_GL_FN(glVertexAttribI3i);
        LOAD_GL_FN(glVertexAttribI4i);
        LOAD_GL_FN(glVertexAttribI1ui);
        LOAD_GL_FN(glVertexAttribI2ui);
        LOAD_GL_FN(glVertexAttribI3ui);
        LOAD_GL_FN(glVertexAttribI4ui);
        LOAD_GL_FN(glVertexAttribI1iv);
        LOAD_GL_FN(glVertexAttribI2iv);
        LOAD_GL_FN(glVertexAttribI3iv);
        LOAD_GL_FN(glVertexAttribI4iv);
        LOAD_GL_FN(glVertexAttribI1uiv);
        LOAD_GL_FN(glVertexAttribI2uiv);
        LOAD_GL_FN(glVertexAttribI3uiv);
        LOAD_GL_FN(glVertexAttribI4uiv);
        LOAD_GL_FN(glVertexAttribI4bv);
        LOAD_GL_FN(glVertexAttribI4sv);
        LOAD_GL_FN(glVertexAttribI4ubv);
        LOAD_GL_FN(glVertexAttribI4usv);
        LOAD_GL_FN(glGetUniformuiv);
        LOAD_GL_FN(glBindFragDataLocation);
        LOAD_GL_FN(glGetFragDataLocation);
        LOAD_GL_FN(glUniform1ui);
        LOAD_GL_FN(glUniform2ui);
        LOAD_GL_FN(glUniform3ui);
        LOAD_GL_FN(glUniform4ui);
        LOAD_GL_FN(glUniform1uiv);
        LOAD_GL_FN(glUniform2uiv);
        LOAD_GL_FN(glUniform3uiv);
        LOAD_GL_FN(glUniform4uiv);
        LOAD_GL_FN(glTexParameterIiv);
        LOAD_GL_FN(glTexParameterIuiv);
        LOAD_GL_FN(glGetTexParameterIiv);
        LOAD_GL_FN(glGetTexParameterIuiv);
        LOAD_GL_FN(glClearBufferiv);
        LOAD_GL_FN(glClearBufferuiv);
        LOAD_GL_FN(glClearBufferfv);
        LOAD_GL_FN(glClearBufferfi);
        LOAD_GL_FN(glGetStringi);
        LOAD_GL_FN(glIsRenderbuffer);
        LOAD_GL_FN(glBindRenderbuffer);
        LOAD_GL_FN(glDeleteRenderbuffers);
        LOAD_GL_FN(glGenRenderbuffers);
        LOAD_GL_FN(glRenderbufferStorage);
        LOAD_GL_FN(glGetRenderbufferParameteriv);
        LOAD_GL_FN(glIsFramebuffer);
        LOAD_GL_FN(glBindFramebuffer);
        LOAD_GL_FN(glDeleteFramebuffers);
        LOAD_GL_FN(glGenFramebuffers);
        LOAD_GL_FN(glCheckFramebufferStatus);
        LOAD_GL_FN(glFramebufferTexture1D);
        LOAD_GL_FN(glFramebufferTexture2D);
        LOAD_GL_FN(glFramebufferTexture3D);
        LOAD_GL_FN(glFramebufferRenderbuffer);
        LOAD_GL_FN(glGetFramebufferAttachmentParameteriv);
        LOAD_GL_FN(glGenerateMipmap);
        LOAD_GL_FN(glBlitFramebuffer);
        LOAD_GL_FN(glRenderbufferStorageMultisample);
        LOAD_GL_FN(glFramebufferTextureLayer);
        LOAD_GL_FN(glMapBufferRange);
        LOAD_GL_FN(glFlushMappedBufferRange);
        LOAD_GL_FN(glBindVertexArray);
        LOAD_GL_FN(glDeleteVertexArrays);
        LOAD_GL_FN(glGenVertexArrays);
        LOAD_GL_FN(glIsVertexArray);
    }
    
    if(version >= 3.1)
    {
        LOAD_GL_FN(glDrawArraysInstanced);
        LOAD_GL_FN(glDrawElementsInstanced);
        LOAD_GL_FN(glTexBuffer);
        LOAD_GL_FN(glPrimitiveRestartIndex);
        LOAD_GL_FN(glCopyBufferSubData);
        LOAD_GL_FN(glGetUniformIndices);
        LOAD_GL_FN(glGetActiveUniformsiv);
        LOAD_GL_FN(glGetActiveUniformName);
        LOAD_GL_FN(glGetUniformBlockIndex);
        LOAD_GL_FN(glGetActiveUniformBlockiv);
        LOAD_GL_FN(glGetActiveUniformBlockName);
        LOAD_GL_FN(glUniformBlockBinding);
    }
    
    if(version >= 3.2)
    {
        LOAD_GL_FN(glDrawElementsBaseVertex);
        LOAD_GL_FN(glDrawRangeElementsBaseVertex);
        LOAD_GL_FN(glDrawElementsInstancedBaseVertex);
        LOAD_GL_FN(glMultiDrawElementsBaseVertex);
        LOAD_GL_FN(glProvokingVertex);
        LOAD_GL_FN(glFenceSync);
        LOAD_GL_FN(glIsSync);
        LOAD_GL_FN(glDeleteSync);
        LOAD_GL_FN(glClientWaitSync);
        LOAD_GL_FN(glWaitSync);
        LOAD_GL_FN(glGetInteger64v);
        LOAD_GL_FN(glGetSynciv);
        LOAD_GL_FN(glGetInteger64i_v);
        LOAD_GL_FN(glGetBufferParameteri64v);
        LOAD_GL_FN(glFramebufferTexture);
        LOAD_GL_FN(glTexImage2DMultisample);
        LOAD_GL_FN(glTexImage3DMultisample);
        LOAD_GL_FN(glGetMultisamplefv);
        LOAD_GL_FN(glSampleMaski);
    }
    
    if(version >= 3.3)
    {
        LOAD_GL_FN(glBindFragDataLocationIndexed);
        LOAD_GL_FN(glGetFragDataIndex);
        LOAD_GL_FN(glGenSamplers);
        LOAD_GL_FN(glDeleteSamplers);
        LOAD_GL_FN(glIsSampler);
        LOAD_GL_FN(glBindSampler);
        LOAD_GL_FN(glSamplerParameteri);
        LOAD_GL_FN(glSamplerParameteriv);
        LOAD_GL_FN(glSamplerParameterf);
        LOAD_GL_FN(glSamplerParameterfv);
        LOAD_GL_FN(glSamplerParameterIiv);
        LOAD_GL_FN(glSamplerParameterIuiv);
        LOAD_GL_FN(glGetSamplerParameteriv);
        LOAD_GL_FN(glGetSamplerParameterIiv);
        LOAD_GL_FN(glGetSamplerParameterfv);
        LOAD_GL_FN(glGetSamplerParameterIuiv);
        LOAD_GL_FN(glQueryCounter);
        LOAD_GL_FN(glGetQueryObjecti64v);
        LOAD_GL_FN(glGetQueryObjectui64v);
        LOAD_GL_FN(glVertexAttribDivisor);
        LOAD_GL_FN(glVertexAttribP1ui);
        LOAD_GL_FN(glVertexAttribP1uiv);
        LOAD_GL_FN(glVertexAttribP2ui);
        LOAD_GL_FN(glVertexAttribP2uiv);
        LOAD_GL_FN(glVertexAttribP3ui);
        LOAD_GL_FN(glVertexAttribP3uiv);
        LOAD_GL_FN(glVertexAttribP4ui);
        LOAD_GL_FN(glVertexAttribP4uiv);
        LOAD_GL_FN(glVertexP2ui);
        LOAD_GL_FN(glVertexP2uiv);
        LOAD_GL_FN(glVertexP3ui);
        LOAD_GL_FN(glVertexP3uiv);
        LOAD_GL_FN(glVertexP4ui);
        LOAD_GL_FN(glVertexP4uiv);
        LOAD_GL_FN(glTexCoordP1ui);
        LOAD_GL_FN(glTexCoordP1uiv);
        LOAD_GL_FN(glTexCoordP2ui);
        LOAD_GL_FN(glTexCoordP2uiv);
        LOAD_GL_FN(glTexCoordP3ui);
        LOAD_GL_FN(glTexCoordP3uiv);
        LOAD_GL_FN(glTexCoordP4ui);
        LOAD_GL_FN(glTexCoordP4uiv);
        LOAD_GL_FN(glMultiTexCoordP1ui);
        LOAD_GL_FN(glMultiTexCoordP1uiv);
        LOAD_GL_FN(glMultiTexCoordP2ui);
        LOAD_GL_FN(glMultiTexCoordP2uiv);
        LOAD_GL_FN(glMultiTexCoordP3ui);
        LOAD_GL_FN(glMultiTexCoordP3uiv);
        LOAD_GL_FN(glMultiTexCoordP4ui);
        LOAD_GL_FN(glMultiTexCoordP4uiv);
        LOAD_GL_FN(glNormalP3ui);
        LOAD_GL_FN(glNormalP3uiv);
        LOAD_GL_FN(glColorP3ui);
        LOAD_GL_FN(glColorP3uiv);
        LOAD_GL_FN(glColorP4ui);
        LOAD_GL_FN(glColorP4uiv);
        LOAD_GL_FN(glSecondaryColorP3ui);
        LOAD_GL_FN(glSecondaryColorP3uiv);
    }
    
    if(version >= 4.0)
    {
        LOAD_GL_FN(glMinSampleShading);
        LOAD_GL_FN(glBlendEquationi);
        LOAD_GL_FN(glBlendEquationSeparatei);
        LOAD_GL_FN(glBlendFunci);
        LOAD_GL_FN(glBlendFuncSeparatei);
        LOAD_GL_FN(glDrawArraysIndirect);
        LOAD_GL_FN(glDrawElementsIndirect);
        LOAD_GL_FN(glUniform1d);
        LOAD_GL_FN(glUniform2d);
        LOAD_GL_FN(glUniform3d);
        LOAD_GL_FN(glUniform4d);
        LOAD_GL_FN(glUniform1dv);
        LOAD_GL_FN(glUniform2dv);
        LOAD_GL_FN(glUniform3dv);
        LOAD_GL_FN(glUniform4dv);
        LOAD_GL_FN(glUniformMatrix2dv);
        LOAD_GL_FN(glUniformMatrix3dv);
        LOAD_GL_FN(glUniformMatrix4dv);
        LOAD_GL_FN(glUniformMatrix2x3dv);
        LOAD_GL_FN(glUniformMatrix2x4dv);
        LOAD_GL_FN(glUniformMatrix3x2dv);
        LOAD_GL_FN(glUniformMatrix3x4dv);
        LOAD_GL_FN(glUniformMatrix4x2dv);
        LOAD_GL_FN(glUniformMatrix4x3dv);
        LOAD_GL_FN(glGetUniformdv);
        LOAD_GL_FN(glGetSubroutineUniformLocation);
        LOAD_GL_FN(glGetSubroutineIndex);
        LOAD_GL_FN(glGetActiveSubroutineUniformiv);
        LOAD_GL_FN(glGetActiveSubroutineUniformName);
        LOAD_GL_FN(glGetActiveSubroutineName);
        LOAD_GL_FN(glUniformSubroutinesuiv);
        LOAD_GL_FN(glGetUniformSubroutineuiv);
        LOAD_GL_FN(glGetProgramStageiv);
        LOAD_GL_FN(glPatchParameteri);
        LOAD_GL_FN(glPatchParameterfv);
        LOAD_GL_FN(glBindTransformFeedback);
        LOAD_GL_FN(glDeleteTransformFeedbacks);
        LOAD_GL_FN(glGenTransformFeedbacks);
        LOAD_GL_FN(glIsTransformFeedback);
        LOAD_GL_FN(glPauseTransformFeedback);
        LOAD_GL_FN(glResumeTransformFeedback);
        LOAD_GL_FN(glDrawTransformFeedback);
        LOAD_GL_FN(glDrawTransformFeedbackStream);
        LOAD_GL_FN(glBeginQueryIndexed);
        LOAD_GL_FN(glEndQueryIndexed);
        LOAD_GL_FN(glGetQueryIndexediv);
    }
    
    if(version >= 4.1)
    {
        LOAD_GL_FN(glReleaseShaderCompiler);
        LOAD_GL_FN(glShaderBinary);
        LOAD_GL_FN(glGetShaderPrecisionFormat);
        LOAD_GL_FN(glDepthRangef);
        LOAD_GL_FN(glClearDepthf);
        LOAD_GL_FN(glGetProgramBinary);
        LOAD_GL_FN(glProgramBinary);
        LOAD_GL_FN(glProgramParameteri);
        LOAD_GL_FN(glUseProgramStages);
        LOAD_GL_FN(glActiveShaderProgram);
        LOAD_GL_FN(glCreateShaderProgramv);
        LOAD_GL_FN(glBindProgramPipeline);
        LOAD_GL_FN(glDeleteProgramPipelines);
        LOAD_GL_FN(glGenProgramPipelines);
        LOAD_GL_FN(glIsProgramPipeline);
        LOAD_GL_FN(glGetProgramPipelineiv);
        LOAD_GL_FN(glProgramUniform1i);
        LOAD_GL_FN(glProgramUniform1iv);
        LOAD_GL_FN(glProgramUniform1f);
        LOAD_GL_FN(glProgramUniform1fv);
        LOAD_GL_FN(glProgramUniform1d);
        LOAD_GL_FN(glProgramUniform1dv);
        LOAD_GL_FN(glProgramUniform1ui);
        LOAD_GL_FN(glProgramUniform1uiv);
        LOAD_GL_FN(glProgramUniform2i);
        LOAD_GL_FN(glProgramUniform2iv);
        LOAD_GL_FN(glProgramUniform2f);
        LOAD_GL_FN(glProgramUniform2fv);
        LOAD_GL_FN(glProgramUniform2d);
        LOAD_GL_FN(glProgramUniform2dv);
        LOAD_GL_FN(glProgramUniform2ui);
        LOAD_GL_FN(glProgramUniform2uiv);
        LOAD_GL_FN(glProgramUniform3i);
        LOAD_GL_FN(glProgramUniform3iv);
        LOAD_GL_FN(glProgramUniform3f);
        LOAD_GL_FN(glProgramUniform3fv);
        LOAD_GL_FN(glProgramUniform3d);
        LOAD_GL_FN(glProgramUniform3dv);
        LOAD_GL_FN(glProgramUniform3ui);
        LOAD_GL_FN(glProgramUniform3uiv);
        LOAD_GL_FN(glProgramUniform4i);
        LOAD_GL_FN(glProgramUniform4iv);
        LOAD_GL_FN(glProgramUniform4f);
        LOAD_GL_FN(glProgramUniform4fv);
        LOAD_GL_FN(glProgramUniform4d);
        LOAD_GL_FN(glProgramUniform4dv);
        LOAD_GL_FN(glProgramUniform4ui);
        LOAD_GL_FN(glProgramUniform4uiv);
        LOAD_GL_FN(glProgramUniformMatrix2fv);
        LOAD_GL_FN(glProgramUniformMatrix3fv);
        LOAD_GL_FN(glProgramUniformMatrix4fv);
        LOAD_GL_FN(glProgramUniformMatrix2dv);
        LOAD_GL_FN(glProgramUniformMatrix3dv);
        LOAD_GL_FN(glProgramUniformMatrix4dv);
        LOAD_GL_FN(glProgramUniformMatrix2x3fv);
        LOAD_GL_FN(glProgramUniformMatrix3x2fv);
        LOAD_GL_FN(glProgramUniformMatrix2x4fv);
        LOAD_GL_FN(glProgramUniformMatrix4x2fv);
        LOAD_GL_FN(glProgramUniformMatrix3x4fv);
        LOAD_GL_FN(glProgramUniformMatrix4x3fv);
        LOAD_GL_FN(glProgramUniformMatrix2x3dv);
        LOAD_GL_FN(glProgramUniformMatrix3x2dv);
        LOAD_GL_FN(glProgramUniformMatrix2x4dv);
        LOAD_GL_FN(glProgramUniformMatrix4x2dv);
        LOAD_GL_FN(glProgramUniformMatrix3x4dv);
        LOAD_GL_FN(glProgramUniformMatrix4x3dv);
        LOAD_GL_FN(glValidateProgramPipeline);
        LOAD_GL_FN(glGetProgramPipelineInfoLog);
        LOAD_GL_FN(glVertexAttribL1d);
        LOAD_GL_FN(glVertexAttribL2d);
        LOAD_GL_FN(glVertexAttribL3d);
        LOAD_GL_FN(glVertexAttribL4d);
        LOAD_GL_FN(glVertexAttribL1dv);
        LOAD_GL_FN(glVertexAttribL2dv);
        LOAD_GL_FN(glVertexAttribL3dv);
        LOAD_GL_FN(glVertexAttribL4dv);
        LOAD_GL_FN(glVertexAttribLPointer);
        LOAD_GL_FN(glGetVertexAttribLdv);
        LOAD_GL_FN(glViewportArrayv);
        LOAD_GL_FN(glViewportIndexedf);
        LOAD_GL_FN(glViewportIndexedfv);
        LOAD_GL_FN(glScissorArrayv);
        LOAD_GL_FN(glScissorIndexed);
        LOAD_GL_FN(glScissorIndexedv);
        LOAD_GL_FN(glDepthRangeArrayv);
        LOAD_GL_FN(glDepthRangeIndexed);
        LOAD_GL_FN(glGetFloati_v);
        LOAD_GL_FN(glGetDoublei_v);
    }
    
    if(version >= 4.2)
    {
        LOAD_GL_FN(glDrawArraysInstancedBaseInstance);
        LOAD_GL_FN(glDrawElementsInstancedBaseInstance);
        LOAD_GL_FN(glDrawElementsInstancedBaseVertexBaseInstance);
        LOAD_GL_FN(glGetInternalformativ);
        LOAD_GL_FN(glGetActiveAtomicCounterBufferiv);
        LOAD_GL_FN(glBindImageTexture);
        LOAD_GL_FN(glMemoryBarrier);
        LOAD_GL_FN(glTexStorage1D);
        LOAD_GL_FN(glTexStorage2D);
        LOAD_GL_FN(glTexStorage3D);
        LOAD_GL_FN(glDrawTransformFeedbackInstanced);
        LOAD_GL_FN(glDrawTransformFeedbackStreamInstanced);
    }
    
    if(version >= 4.3)
    {
        LOAD_GL_FN(glClearBufferData);
        LOAD_GL_FN(glClearBufferSubData);
        LOAD_GL_FN(glDispatchCompute);
        LOAD_GL_FN(glDispatchComputeIndirect);
        LOAD_GL_FN(glCopyImageSubData);
        LOAD_GL_FN(glFramebufferParameteri);
        LOAD_GL_FN(glGetFramebufferParameteriv);
        LOAD_GL_FN(glGetInternalformati64v);
        LOAD_GL_FN(glInvalidateTexSubImage);
        LOAD_GL_FN(glInvalidateTexImage);
        LOAD_GL_FN(glInvalidateBufferSubData);
        LOAD_GL_FN(glInvalidateBufferData);
        LOAD_GL_FN(glInvalidateFramebuffer);
        LOAD_GL_FN(glInvalidateSubFramebuffer);
        LOAD_GL_FN(glMultiDrawArraysIndirect);
        LOAD_GL_FN(glMultiDrawElementsIndirect);
        LOAD_GL_FN(glGetProgramInterfaceiv);
        LOAD_GL_FN(glGetProgramResourceIndex);
        LOAD_GL_FN(glGetProgramResourceName);
        LOAD_GL_FN(glGetProgramResourceiv);
        LOAD_GL_FN(glGetProgramResourceLocation);
        LOAD_GL_FN(glGetProgramResourceLocationIndex);
        LOAD_GL_FN(glShaderStorageBlockBinding);
        LOAD_GL_FN(glTexBufferRange);
        LOAD_GL_FN(glTexStorage2DMultisample);
        LOAD_GL_FN(glTexStorage3DMultisample);
        LOAD_GL_FN(glTextureView);
        LOAD_GL_FN(glBindVertexBuffer);
        LOAD_GL_FN(glVertexAttribFormat);
        LOAD_GL_FN(glVertexAttribIFormat);
        LOAD_GL_FN(glVertexAttribLFormat);
        LOAD_GL_FN(glVertexAttribBinding);
        LOAD_GL_FN(glVertexBindingDivisor);
        LOAD_GL_FN(glDebugMessageControl);
        LOAD_GL_FN(glDebugMessageInsert);
        LOAD_GL_FN(glDebugMessageCallback);
        LOAD_GL_FN(glGetDebugMessageLog);
        LOAD_GL_FN(glPushDebugGroup);
        LOAD_GL_FN(glPopDebugGroup);
        LOAD_GL_FN(glObjectLabel);
        LOAD_GL_FN(glGetObjectLabel);
        LOAD_GL_FN(glObjectPtrLabel);
        LOAD_GL_FN(glGetObjectPtrLabel);
    }
    
    if(version >= 4.4)
    {
        LOAD_GL_FN(glBufferStorage);
        LOAD_GL_FN(glClearTexImage);
        LOAD_GL_FN(glClearTexSubImage);
        LOAD_GL_FN(glBindBuffersBase);
        LOAD_GL_FN(glBindBuffersRange);
        LOAD_GL_FN(glBindTextures);
        LOAD_GL_FN(glBindSamplers);
        LOAD_GL_FN(glBindImageTextures);
        LOAD_GL_FN(glBindVertexBuffers);
    }
    
    if(version >= 4.5)
    {
        LOAD_GL_FN(glClipControl);
        LOAD_GL_FN(glCreateTransformFeedbacks);
        LOAD_GL_FN(glTransformFeedbackBufferBase);
        LOAD_GL_FN(glTransformFeedbackBufferRange);
        LOAD_GL_FN(glGetTransformFeedbackiv);
        LOAD_GL_FN(glGetTransformFeedbacki_v);
        LOAD_GL_FN(glGetTransformFeedbacki64_v);
        LOAD_GL_FN(glCreateBuffers);
        LOAD_GL_FN(glNamedBufferStorage);
        LOAD_GL_FN(glNamedBufferData);
        LOAD_GL_FN(glNamedBufferSubData);
        LOAD_GL_FN(glCopyNamedBufferSubData);
        LOAD_GL_FN(glClearNamedBufferData);
        LOAD_GL_FN(glClearNamedBufferSubData);
        LOAD_GL_FN(glMapNamedBuffer);
        LOAD_GL_FN(glMapNamedBufferRange);
        LOAD_GL_FN(glUnmapNamedBuffer);
        LOAD_GL_FN(glFlushMappedNamedBufferRange);
        LOAD_GL_FN(glGetNamedBufferParameteriv);
        LOAD_GL_FN(glGetNamedBufferParameteri64v);
        LOAD_GL_FN(glGetNamedBufferPointerv);
        LOAD_GL_FN(glGetNamedBufferSubData);
        LOAD_GL_FN(glCreateFramebuffers);
        LOAD_GL_FN(glNamedFramebufferRenderbuffer);
        LOAD_GL_FN(glNamedFramebufferParameteri);
        LOAD_GL_FN(glNamedFramebufferTexture);
        LOAD_GL_FN(glNamedFramebufferTextureLayer);
        LOAD_GL_FN(glNamedFramebufferDrawBuffer);
        LOAD_GL_FN(glNamedFramebufferDrawBuffers);
        LOAD_GL_FN(glNamedFramebufferReadBuffer);
        LOAD_GL_FN(glInvalidateNamedFramebufferData);
        LOAD_GL_FN(glInvalidateNamedFramebufferSubData);
        LOAD_GL_FN(glClearNamedFramebufferiv);
        LOAD_GL_FN(glClearNamedFramebufferuiv);
        LOAD_GL_FN(glClearNamedFramebufferfv);
        LOAD_GL_FN(glClearNamedFramebufferfi);
        LOAD_GL_FN(glBlitNamedFramebuffer);
        LOAD_GL_FN(glCheckNamedFramebufferStatus);
        LOAD_GL_FN(glGetNamedFramebufferParameteriv);
        LOAD_GL_FN(glGetNamedFramebufferAttachmentParameteriv);
        LOAD_GL_FN(glCreateRenderbuffers);
        LOAD_GL_FN(glNamedRenderbufferStorage);
        LOAD_GL_FN(glNamedRenderbufferStorageMultisample);
        LOAD_GL_FN(glGetNamedRenderbufferParameteriv);
        LOAD_GL_FN(glCreateTextures);
        LOAD_GL_FN(glTextureBuffer);
        LOAD_GL_FN(glTextureBufferRange);
        LOAD_GL_FN(glTextureStorage1D);
        LOAD_GL_FN(glTextureStorage2D);
        LOAD_GL_FN(glTextureStorage3D);
        LOAD_GL_FN(glTextureStorage2DMultisample);
        LOAD_GL_FN(glTextureStorage3DMultisample);
        LOAD_GL_FN(glTextureSubImage1D);
        LOAD_GL_FN(glTextureSubImage2D);
        LOAD_GL_FN(glTextureSubImage3D);
        LOAD_GL_FN(glCompressedTextureSubImage1D);
        LOAD_GL_FN(glCompressedTextureSubImage2D);
        LOAD_GL_FN(glCompressedTextureSubImage3D);
        LOAD_GL_FN(glCopyTextureSubImage1D);
        LOAD_GL_FN(glCopyTextureSubImage2D);
        LOAD_GL_FN(glCopyTextureSubImage3D);
        LOAD_GL_FN(glTextureParameterf);
        LOAD_GL_FN(glTextureParameterfv);
        LOAD_GL_FN(glTextureParameteri);
        LOAD_GL_FN(glTextureParameterIiv);
        LOAD_GL_FN(glTextureParameterIuiv);
        LOAD_GL_FN(glTextureParameteriv);
        LOAD_GL_FN(glGenerateTextureMipmap);
        LOAD_GL_FN(glBindTextureUnit);
        LOAD_GL_FN(glGetTextureImage);
        LOAD_GL_FN(glGetCompressedTextureImage);
        LOAD_GL_FN(glGetTextureLevelParameterfv);
        LOAD_GL_FN(glGetTextureLevelParameteriv);
        LOAD_GL_FN(glGetTextureParameterfv);
        LOAD_GL_FN(glGetTextureParameterIiv);
        LOAD_GL_FN(glGetTextureParameterIuiv);
        LOAD_GL_FN(glGetTextureParameteriv);
        LOAD_GL_FN(glCreateVertexArrays);
        LOAD_GL_FN(glDisableVertexArrayAttrib);
        LOAD_GL_FN(glEnableVertexArrayAttrib);
        LOAD_GL_FN(glVertexArrayElementBuffer);
        LOAD_GL_FN(glVertexArrayVertexBuffer);
        LOAD_GL_FN(glVertexArrayVertexBuffers);
        LOAD_GL_FN(glVertexArrayAttribBinding);
        LOAD_GL_FN(glVertexArrayAttribFormat);
        LOAD_GL_FN(glVertexArrayAttribIFormat);
        LOAD_GL_FN(glVertexArrayAttribLFormat);
        LOAD_GL_FN(glVertexArrayBindingDivisor);
        LOAD_GL_FN(glGetVertexArrayiv);
        LOAD_GL_FN(glGetVertexArrayIndexediv);
        LOAD_GL_FN(glGetVertexArrayIndexed64iv);
        LOAD_GL_FN(glCreateSamplers);
        LOAD_GL_FN(glCreateProgramPipelines);
        LOAD_GL_FN(glCreateQueries);
        LOAD_GL_FN(glGetQueryBufferObjecti64v);
        LOAD_GL_FN(glGetQueryBufferObjectiv);
        LOAD_GL_FN(glGetQueryBufferObjectui64v);
        LOAD_GL_FN(glGetQueryBufferObjectuiv);
        LOAD_GL_FN(glMemoryBarrierByRegion);
        LOAD_GL_FN(glGetTextureSubImage);
        LOAD_GL_FN(glGetCompressedTextureSubImage);
        LOAD_GL_FN(glGetGraphicsResetStatus);
        LOAD_GL_FN(glGetnCompressedTexImage);
        LOAD_GL_FN(glGetnTexImage);
        LOAD_GL_FN(glGetnUniformdv);
        LOAD_GL_FN(glGetnUniformfv);
        LOAD_GL_FN(glGetnUniformiv);
        LOAD_GL_FN(glGetnUniformuiv);
        LOAD_GL_FN(glReadnPixels);
        LOAD_GL_FN(glGetnMapdv);
        LOAD_GL_FN(glGetnMapfv);
        LOAD_GL_FN(glGetnMapiv);
        LOAD_GL_FN(glGetnPixelMapfv);
        LOAD_GL_FN(glGetnPixelMapuiv);
        LOAD_GL_FN(glGetnPixelMapusv);
        LOAD_GL_FN(glGetnPolygonStipple);
        LOAD_GL_FN(glGetnColorTable);
        LOAD_GL_FN(glGetnConvolutionFilter);
        LOAD_GL_FN(glGetnSeparableFilter);
        LOAD_GL_FN(glGetnHistogram);
        LOAD_GL_FN(glGetnMinmax);
        LOAD_GL_FN(glTextureBarrier);
    }

    return 1;
}
/* ==============================================================
   Project: Ascencia
   File:    platform.c
   Author:  Phraggers
   Date:    17-11-2024
   ============================================================== */

#include "platform.h"

local LRESULT WndProc(HWND handle, UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch(msg)
    {
        case WM_CREATE:
        {
            return 0;
        } break;
        case WM_CLOSE:
        case WM_DESTROY:
        {
            g_state->running = 0;
            return 0;
        } break;
    }

    return DefWindowProcA(handle, msg, wparam, lparam);
}

#define LOAD_GL_FUNCTION(name) name = (pfn_##name)wglGetProcAddress( #name ); \
if(!name) {name = (pfn_##name)GetProcAddress(lib, #name );} if(!name) {return 0;}

local b32 LoadGLFunctions(r32 version)
{
    HMODULE lib = LoadLibraryA("opengl32.dll");

    if(version >= 1.0)
    {
        LOAD_GL_FUNCTION(glCullFace);
        LOAD_GL_FUNCTION(glFrontFace);
        LOAD_GL_FUNCTION(glHint);
        LOAD_GL_FUNCTION(glLineWidth);
        LOAD_GL_FUNCTION(glPointSize);
        LOAD_GL_FUNCTION(glPolygonMode);
        LOAD_GL_FUNCTION(glScissor);
        LOAD_GL_FUNCTION(glTexParameterf);
        LOAD_GL_FUNCTION(glTexParameterfv);
        LOAD_GL_FUNCTION(glTexParameteri);
        LOAD_GL_FUNCTION(glTexParameteriv);
        LOAD_GL_FUNCTION(glTexImage1D);
        LOAD_GL_FUNCTION(glTexImage2D);
        LOAD_GL_FUNCTION(glDrawBuffer);
        LOAD_GL_FUNCTION(glClear);
        LOAD_GL_FUNCTION(glClearColor);
        LOAD_GL_FUNCTION(glClearStencil);
        LOAD_GL_FUNCTION(glClearDepth);
        LOAD_GL_FUNCTION(glStencilMask);
        LOAD_GL_FUNCTION(glColorMask);
        LOAD_GL_FUNCTION(glDepthMask);
        LOAD_GL_FUNCTION(glDisable);
        LOAD_GL_FUNCTION(glEnable);
        LOAD_GL_FUNCTION(glFinish);
        LOAD_GL_FUNCTION(glFlush);
        LOAD_GL_FUNCTION(glBlendFunc);
        LOAD_GL_FUNCTION(glLogicOp);
        LOAD_GL_FUNCTION(glStencilFunc);
        LOAD_GL_FUNCTION(glStencilOp);
        LOAD_GL_FUNCTION(glDepthFunc);
        LOAD_GL_FUNCTION(glPixelStoref);
        LOAD_GL_FUNCTION(glPixelStorei);
        LOAD_GL_FUNCTION(glReadBuffer);
        LOAD_GL_FUNCTION(glReadPixels);
        LOAD_GL_FUNCTION(glGetBooleanv);
        LOAD_GL_FUNCTION(glGetDoublev);
        LOAD_GL_FUNCTION(glGetError);
        LOAD_GL_FUNCTION(glGetFloatv);
        LOAD_GL_FUNCTION(glGetIntegerv);
        LOAD_GL_FUNCTION(glGetString);
        LOAD_GL_FUNCTION(glGetTexImage);
        LOAD_GL_FUNCTION(glGetTexParameterfv);
        LOAD_GL_FUNCTION(glGetTexParameteriv);
        LOAD_GL_FUNCTION(glGetTexLevelParameterfv);
        LOAD_GL_FUNCTION(glGetTexLevelParameteriv);
        LOAD_GL_FUNCTION(glIsEnabled);
        LOAD_GL_FUNCTION(glDepthRange);
        LOAD_GL_FUNCTION(glViewport);
    }

    if(version >= 1.1)
    {
        LOAD_GL_FUNCTION(glDrawArrays);
        LOAD_GL_FUNCTION(glDrawElements);
        LOAD_GL_FUNCTION(glPolygonOffset);
        LOAD_GL_FUNCTION(glCopyTexImage1D);
        LOAD_GL_FUNCTION(glCopyTexImage2D);
        LOAD_GL_FUNCTION(glCopyTexSubImage1D);
        LOAD_GL_FUNCTION(glCopyTexSubImage2D);
        LOAD_GL_FUNCTION(glTexSubImage1D);
        LOAD_GL_FUNCTION(glTexSubImage2D);
        LOAD_GL_FUNCTION(glBindTexture);
        LOAD_GL_FUNCTION(glDeleteTextures);
        LOAD_GL_FUNCTION(glGenTextures);
        LOAD_GL_FUNCTION(glIsTexture);
    }

    if(version >= 1.2)
    {
        LOAD_GL_FUNCTION(glDrawRangeElements);
        LOAD_GL_FUNCTION(glTexImage3D);
        LOAD_GL_FUNCTION(glTexSubImage3D);
        LOAD_GL_FUNCTION(glCopyTexSubImage3D);
    }

    if(version >= 1.3)
    {
        LOAD_GL_FUNCTION(glActiveTexture);
        LOAD_GL_FUNCTION(glSampleCoverage);
        LOAD_GL_FUNCTION(glCompressedTexImage3D);
        LOAD_GL_FUNCTION(glCompressedTexImage2D);
        LOAD_GL_FUNCTION(glCompressedTexImage1D);
        LOAD_GL_FUNCTION(glCompressedTexSubImage3D);
        LOAD_GL_FUNCTION(glCompressedTexSubImage2D);
        LOAD_GL_FUNCTION(glCompressedTexSubImage1D);
        LOAD_GL_FUNCTION(glGetCompressedTexImage);
    }

    if(version >= 1.4)
    {
        LOAD_GL_FUNCTION(glBlendFuncSeparate);
        LOAD_GL_FUNCTION(glMultiDrawArrays);
        LOAD_GL_FUNCTION(glMultiDrawElements);
        LOAD_GL_FUNCTION(glPointParameterf);
        LOAD_GL_FUNCTION(glPointParameterfv);
        LOAD_GL_FUNCTION(glPointParameteri);
        LOAD_GL_FUNCTION(glPointParameteriv);
        LOAD_GL_FUNCTION(glBlendColor);
        LOAD_GL_FUNCTION(glBlendEquation);
    }

    if(version >= 1.5)
    {
        LOAD_GL_FUNCTION(glGenQueries);
        LOAD_GL_FUNCTION(glDeleteQueries);
        LOAD_GL_FUNCTION(glIsQuery);
        LOAD_GL_FUNCTION(glBeginQuery);
        LOAD_GL_FUNCTION(glEndQuery);
        LOAD_GL_FUNCTION(glGetQueryiv);
        LOAD_GL_FUNCTION(glGetQueryObjectiv);
        LOAD_GL_FUNCTION(glGetQueryObjectuiv);
        LOAD_GL_FUNCTION(glBindBuffer);
        LOAD_GL_FUNCTION(glDeleteBuffers);
        LOAD_GL_FUNCTION(glGenBuffers);
        LOAD_GL_FUNCTION(glIsBuffer);
        LOAD_GL_FUNCTION(glBufferData);
        LOAD_GL_FUNCTION(glBufferSubData);
        LOAD_GL_FUNCTION(glGetBufferSubData);
        LOAD_GL_FUNCTION(glMapBuffer);
        LOAD_GL_FUNCTION(glUnmapBuffer);
        LOAD_GL_FUNCTION(glGetBufferParameteriv);
        LOAD_GL_FUNCTION(glGetBufferPointerv);
    }

    if(version >= 2.0)
    {
        LOAD_GL_FUNCTION(glBlendEquationSeparate);
        LOAD_GL_FUNCTION(glDrawBuffers);
        LOAD_GL_FUNCTION(glStencilOpSeparate);
        LOAD_GL_FUNCTION(glStencilFuncSeparate);
        LOAD_GL_FUNCTION(glStencilMaskSeparate);
        LOAD_GL_FUNCTION(glAttachShader);
        LOAD_GL_FUNCTION(glBindAttribLocation);
        LOAD_GL_FUNCTION(glCompileShader);
        LOAD_GL_FUNCTION(glCreateProgram);
        LOAD_GL_FUNCTION(glCreateShader);
        LOAD_GL_FUNCTION(glDeleteProgram);
        LOAD_GL_FUNCTION(glDeleteShader);
        LOAD_GL_FUNCTION(glDetachShader);
        LOAD_GL_FUNCTION(glDisableVertexAttribArray);
        LOAD_GL_FUNCTION(glEnableVertexAttribArray);
        LOAD_GL_FUNCTION(glGetActiveAttrib);
        LOAD_GL_FUNCTION(glGetActiveUniform);
        LOAD_GL_FUNCTION(glGetAttachedShaders);
        LOAD_GL_FUNCTION(glGetAttribLocation);
        LOAD_GL_FUNCTION(glGetProgramiv);
        LOAD_GL_FUNCTION(glGetProgramInfoLog);
        LOAD_GL_FUNCTION(glGetShaderiv);
        LOAD_GL_FUNCTION(glGetShaderInfoLog);
        LOAD_GL_FUNCTION(glGetShaderSource);
        LOAD_GL_FUNCTION(glGetUniformLocation);
        LOAD_GL_FUNCTION(glGetUniformfv);
        LOAD_GL_FUNCTION(glGetUniformiv);
        LOAD_GL_FUNCTION(glGetVertexAttribdv);
        LOAD_GL_FUNCTION(glGetVertexAttribfv);
        LOAD_GL_FUNCTION(glGetVertexAttribiv);
        LOAD_GL_FUNCTION(glGetVertexAttribPointerv);
        LOAD_GL_FUNCTION(glIsProgram);
        LOAD_GL_FUNCTION(glIsShader);
        LOAD_GL_FUNCTION(glLinkProgram);
        LOAD_GL_FUNCTION(glShaderSource);
        LOAD_GL_FUNCTION(glUseProgram);
        LOAD_GL_FUNCTION(glUniform1f);
        LOAD_GL_FUNCTION(glUniform2f);
        LOAD_GL_FUNCTION(glUniform3f);
        LOAD_GL_FUNCTION(glUniform4f);
        LOAD_GL_FUNCTION(glUniform1i);
        LOAD_GL_FUNCTION(glUniform2i);
        LOAD_GL_FUNCTION(glUniform3i);
        LOAD_GL_FUNCTION(glUniform4i);
        LOAD_GL_FUNCTION(glUniform1fv);
        LOAD_GL_FUNCTION(glUniform2fv);
        LOAD_GL_FUNCTION(glUniform3fv);
        LOAD_GL_FUNCTION(glUniform4fv);
        LOAD_GL_FUNCTION(glUniform1iv);
        LOAD_GL_FUNCTION(glUniform2iv);
        LOAD_GL_FUNCTION(glUniform3iv);
        LOAD_GL_FUNCTION(glUniform4iv);
        LOAD_GL_FUNCTION(glUniformMatrix2fv);
        LOAD_GL_FUNCTION(glUniformMatrix3fv);
        LOAD_GL_FUNCTION(glUniformMatrix4fv);
        LOAD_GL_FUNCTION(glValidateProgram);
        LOAD_GL_FUNCTION(glVertexAttrib1d);
        LOAD_GL_FUNCTION(glVertexAttrib1dv);
        LOAD_GL_FUNCTION(glVertexAttrib1f);
        LOAD_GL_FUNCTION(glVertexAttrib1s);
        LOAD_GL_FUNCTION(glVertexAttrib1sv);
        LOAD_GL_FUNCTION(glVertexAttrib2d);
        LOAD_GL_FUNCTION(glVertexAttrib2dv);
        LOAD_GL_FUNCTION(glVertexAttrib2f);
        LOAD_GL_FUNCTION(glVertexAttrib2fv);
        LOAD_GL_FUNCTION(glVertexAttrib2s);
        LOAD_GL_FUNCTION(glVertexAttrib2sv);
        LOAD_GL_FUNCTION(glVertexAttrib3d);
        LOAD_GL_FUNCTION(glVertexAttrib3dv);
        LOAD_GL_FUNCTION(glVertexAttrib3f);
        LOAD_GL_FUNCTION(glVertexAttrib3fv);
        LOAD_GL_FUNCTION(glVertexAttrib3s);
        LOAD_GL_FUNCTION(glVertexAttrib3sv);
        LOAD_GL_FUNCTION(glVertexAttrib4Nbv);
        LOAD_GL_FUNCTION(glVertexAttrib4Niv);
        LOAD_GL_FUNCTION(glVertexAttrib4Nsv);
        LOAD_GL_FUNCTION(glVertexAttrib4Nub);
        LOAD_GL_FUNCTION(glVertexAttrib4Nubv);
        LOAD_GL_FUNCTION(glVertexAttrib4Nuiv);
        LOAD_GL_FUNCTION(glVertexAttrib4Nusv);
        LOAD_GL_FUNCTION(glVertexAttrib4bv);
        LOAD_GL_FUNCTION(glVertexAttrib4d);
        LOAD_GL_FUNCTION(glVertexAttrib4dv);
        LOAD_GL_FUNCTION(glVertexAttrib4f);
        LOAD_GL_FUNCTION(glVertexAttrib4fv);
        LOAD_GL_FUNCTION(glVertexAttrib4iv);
        LOAD_GL_FUNCTION(glVertexAttrib4s);
        LOAD_GL_FUNCTION(glVertexAttrib4sv);
        LOAD_GL_FUNCTION(glVertexAttrib4ubv);
        LOAD_GL_FUNCTION(glVertexAttrib4uiv);
        LOAD_GL_FUNCTION(glVertexAttrib4usv);
        LOAD_GL_FUNCTION(glVertexAttribPointer);
    }

    if(version >= 2.1)
    {
        LOAD_GL_FUNCTION(glUniformMatrix2x3fv);
        LOAD_GL_FUNCTION(glUniformMatrix3x2fv);
        LOAD_GL_FUNCTION(glUniformMatrix2x4fv);
        LOAD_GL_FUNCTION(glUniformMatrix4x2fv);
        LOAD_GL_FUNCTION(glUniformMatrix3x4fv);
        LOAD_GL_FUNCTION(glUniformMatrix4x3fv);
    }

    if(version >= 3.0)
    {
        LOAD_GL_FUNCTION(glColorMaski);
        LOAD_GL_FUNCTION(glGetBooleani_v);
        LOAD_GL_FUNCTION(glGetIntegeri_v);
        LOAD_GL_FUNCTION(glEnablei);
        LOAD_GL_FUNCTION(glDisablei);
        LOAD_GL_FUNCTION(glIsEnabledi);
        LOAD_GL_FUNCTION(glBeginTransformFeedback);
        LOAD_GL_FUNCTION(glEndTransformFeedback);
        LOAD_GL_FUNCTION(glBindBufferRange);
        LOAD_GL_FUNCTION(glBindBufferBase);
        LOAD_GL_FUNCTION(glTransformFeedbackVaryings);
        LOAD_GL_FUNCTION(glGetTransformFeedbackVarying);
        LOAD_GL_FUNCTION(glClampColor);
        LOAD_GL_FUNCTION(glBeginConditionalRender);
        LOAD_GL_FUNCTION(glEndConditionalRender);
        LOAD_GL_FUNCTION(glVertexAttribIPointer);
        LOAD_GL_FUNCTION(glGetVertexAttribIiv);
        LOAD_GL_FUNCTION(glGetVertexAttribIuiv);
        LOAD_GL_FUNCTION(glVertexAttribI1i);
        LOAD_GL_FUNCTION(glVertexAttribI2i);
        LOAD_GL_FUNCTION(glVertexAttribI3i);
        LOAD_GL_FUNCTION(glVertexAttribI4i);
        LOAD_GL_FUNCTION(glVertexAttribI1ui);
        LOAD_GL_FUNCTION(glVertexAttribI2ui);
        LOAD_GL_FUNCTION(glVertexAttribI3ui);
        LOAD_GL_FUNCTION(glVertexAttribI4ui);
        LOAD_GL_FUNCTION(glVertexAttribI1iv);
        LOAD_GL_FUNCTION(glVertexAttribI2iv);
        LOAD_GL_FUNCTION(glVertexAttribI3iv);
        LOAD_GL_FUNCTION(glVertexAttribI4iv);
        LOAD_GL_FUNCTION(glVertexAttribI1uiv);
        LOAD_GL_FUNCTION(glVertexAttribI2uiv);
        LOAD_GL_FUNCTION(glVertexAttribI3uiv);
        LOAD_GL_FUNCTION(glVertexAttribI4uiv);
        LOAD_GL_FUNCTION(glVertexAttribI4bv);
        LOAD_GL_FUNCTION(glVertexAttribI4sv);
        LOAD_GL_FUNCTION(glVertexAttribI4ubv);
        LOAD_GL_FUNCTION(glVertexAttribI4usv);
        LOAD_GL_FUNCTION(glGetUniformuiv);
        LOAD_GL_FUNCTION(glBindFragDataLocation);
        LOAD_GL_FUNCTION(glGetFragDataLocation);
        LOAD_GL_FUNCTION(glUniform1ui);
        LOAD_GL_FUNCTION(glUniform2ui);
        LOAD_GL_FUNCTION(glUniform3ui);
        LOAD_GL_FUNCTION(glUniform4ui);
        LOAD_GL_FUNCTION(glUniform1uiv);
        LOAD_GL_FUNCTION(glUniform2uiv);
        LOAD_GL_FUNCTION(glUniform3uiv);
        LOAD_GL_FUNCTION(glUniform4uiv);
        LOAD_GL_FUNCTION(glTexParameterIiv);
        LOAD_GL_FUNCTION(glTexParameterIuiv);
        LOAD_GL_FUNCTION(glGetTexParameterIiv);
        LOAD_GL_FUNCTION(glGetTexParameterIuiv);
        LOAD_GL_FUNCTION(glClearBufferiv);
        LOAD_GL_FUNCTION(glClearBufferuiv);
        LOAD_GL_FUNCTION(glClearBufferfv);
        LOAD_GL_FUNCTION(glClearBufferfi);
        LOAD_GL_FUNCTION(glGetStringi);
        LOAD_GL_FUNCTION(glIsRenderbuffer);
        LOAD_GL_FUNCTION(glBindRenderbuffer);
        LOAD_GL_FUNCTION(glDeleteRenderbuffers);
        LOAD_GL_FUNCTION(glGenRenderbuffers);
        LOAD_GL_FUNCTION(glRenderbufferStorage);
        LOAD_GL_FUNCTION(glGetRenderbufferParameteriv);
        LOAD_GL_FUNCTION(glIsFramebuffer);
        LOAD_GL_FUNCTION(glBindFramebuffer);
        LOAD_GL_FUNCTION(glDeleteFramebuffers);
        LOAD_GL_FUNCTION(glGenFramebuffers);
        LOAD_GL_FUNCTION(glCheckFramebufferStatus);
        LOAD_GL_FUNCTION(glFramebufferTexture1D);
        LOAD_GL_FUNCTION(glFramebufferTexture2D);
        LOAD_GL_FUNCTION(glFramebufferTexture3D);
        LOAD_GL_FUNCTION(glFramebufferRenderbuffer);
        LOAD_GL_FUNCTION(glGetFramebufferAttachmentParameteriv);
        LOAD_GL_FUNCTION(glGenerateMipmap);
        LOAD_GL_FUNCTION(glBlitFramebuffer);
        LOAD_GL_FUNCTION(glRenderbufferStorageMultisample);
        LOAD_GL_FUNCTION(glFramebufferTextureLayer);
        LOAD_GL_FUNCTION(glMapBufferRange);
        LOAD_GL_FUNCTION(glFlushMappedBufferRange);
        LOAD_GL_FUNCTION(glBindVertexArray);
        LOAD_GL_FUNCTION(glDeleteVertexArrays);
        LOAD_GL_FUNCTION(glGenVertexArrays);
        LOAD_GL_FUNCTION(glIsVertexArray);
    }

    if(version >= 3.1)
    {
        LOAD_GL_FUNCTION(glDrawArraysInstanced);
        LOAD_GL_FUNCTION(glDrawElementsInstanced);
        LOAD_GL_FUNCTION(glTexBuffer);
        LOAD_GL_FUNCTION(glPrimitiveRestartIndex);
        LOAD_GL_FUNCTION(glCopyBufferSubData);
        LOAD_GL_FUNCTION(glGetUniformIndices);
        LOAD_GL_FUNCTION(glGetActiveUniformsiv);
        LOAD_GL_FUNCTION(glGetActiveUniformName);
        LOAD_GL_FUNCTION(glGetUniformBlockIndex);
        LOAD_GL_FUNCTION(glGetActiveUniformBlockiv);
        LOAD_GL_FUNCTION(glGetActiveUniformBlockName);
        LOAD_GL_FUNCTION(glUniformBlockBinding);
    }

    if(version >= 3.2)
    {
        LOAD_GL_FUNCTION(glDrawElementsBaseVertex);
        LOAD_GL_FUNCTION(glDrawRangeElementsBaseVertex);
        LOAD_GL_FUNCTION(glDrawElementsInstancedBaseVertex);
        LOAD_GL_FUNCTION(glMultiDrawElementsBaseVertex);
        LOAD_GL_FUNCTION(glProvokingVertex);
        LOAD_GL_FUNCTION(glFenceSync);
        LOAD_GL_FUNCTION(glIsSync);
        LOAD_GL_FUNCTION(glDeleteSync);
        LOAD_GL_FUNCTION(glClientWaitSync);
        LOAD_GL_FUNCTION(glWaitSync);
        LOAD_GL_FUNCTION(glGetInteger64v);
        LOAD_GL_FUNCTION(glGetSynciv);
        LOAD_GL_FUNCTION(glGetInteger64i_v);
        LOAD_GL_FUNCTION(glGetBufferParameteri64v);
        LOAD_GL_FUNCTION(glFramebufferTexture);
        LOAD_GL_FUNCTION(glTexImage2DMultisample);
        LOAD_GL_FUNCTION(glTexImage3DMultisample);
        LOAD_GL_FUNCTION(glGetMultisamplefv);
        LOAD_GL_FUNCTION(glSampleMaski);
    }

    if(version >= 3.3)
    {
        LOAD_GL_FUNCTION(glBindFragDataLocationIndexed);
        LOAD_GL_FUNCTION(glGetFragDataIndex);
        LOAD_GL_FUNCTION(glGenSamplers);
        LOAD_GL_FUNCTION(glDeleteSamplers);
        LOAD_GL_FUNCTION(glIsSampler);
        LOAD_GL_FUNCTION(glBindSampler);
        LOAD_GL_FUNCTION(glSamplerParameteri);
        LOAD_GL_FUNCTION(glSamplerParameteriv);
        LOAD_GL_FUNCTION(glSamplerParameterf);
        LOAD_GL_FUNCTION(glSamplerParameterfv);
        LOAD_GL_FUNCTION(glSamplerParameterIiv);
        LOAD_GL_FUNCTION(glSamplerParameterIuiv);
        LOAD_GL_FUNCTION(glGetSamplerParameteriv);
        LOAD_GL_FUNCTION(glGetSamplerParameterIiv);
        LOAD_GL_FUNCTION(glGetSamplerParameterfv);
        LOAD_GL_FUNCTION(glGetSamplerParameterIuiv);
        LOAD_GL_FUNCTION(glQueryCounter);
        LOAD_GL_FUNCTION(glGetQueryObjecti64v);
        LOAD_GL_FUNCTION(glGetQueryObjectui64v);
        LOAD_GL_FUNCTION(glVertexAttribDivisor);
        LOAD_GL_FUNCTION(glVertexAttribP1ui);
        LOAD_GL_FUNCTION(glVertexAttribP1uiv);
        LOAD_GL_FUNCTION(glVertexAttribP2ui);
        LOAD_GL_FUNCTION(glVertexAttribP2uiv);
        LOAD_GL_FUNCTION(glVertexAttribP3ui);
        LOAD_GL_FUNCTION(glVertexAttribP3uiv);
        LOAD_GL_FUNCTION(glVertexAttribP4ui);
        LOAD_GL_FUNCTION(glVertexAttribP4uiv);
        LOAD_GL_FUNCTION(glVertexP2ui);
        LOAD_GL_FUNCTION(glVertexP2uiv);
        LOAD_GL_FUNCTION(glVertexP3ui);
        LOAD_GL_FUNCTION(glVertexP3uiv);
        LOAD_GL_FUNCTION(glVertexP4ui);
        LOAD_GL_FUNCTION(glVertexP4uiv);
        LOAD_GL_FUNCTION(glTexCoordP1ui);
        LOAD_GL_FUNCTION(glTexCoordP1uiv);
        LOAD_GL_FUNCTION(glTexCoordP2ui);
        LOAD_GL_FUNCTION(glTexCoordP2uiv);
        LOAD_GL_FUNCTION(glTexCoordP3ui);
        LOAD_GL_FUNCTION(glTexCoordP3uiv);
        LOAD_GL_FUNCTION(glTexCoordP4ui);
        LOAD_GL_FUNCTION(glTexCoordP4uiv);
        LOAD_GL_FUNCTION(glMultiTexCoordP1ui);
        LOAD_GL_FUNCTION(glMultiTexCoordP1uiv);
        LOAD_GL_FUNCTION(glMultiTexCoordP2ui);
        LOAD_GL_FUNCTION(glMultiTexCoordP2uiv);
        LOAD_GL_FUNCTION(glMultiTexCoordP3ui);
        LOAD_GL_FUNCTION(glMultiTexCoordP3uiv);
        LOAD_GL_FUNCTION(glMultiTexCoordP4ui);
        LOAD_GL_FUNCTION(glMultiTexCoordP4uiv);
        LOAD_GL_FUNCTION(glNormalP3ui);
        LOAD_GL_FUNCTION(glNormalP3uiv);
        LOAD_GL_FUNCTION(glColorP3ui);
        LOAD_GL_FUNCTION(glColorP3uiv);
        LOAD_GL_FUNCTION(glColorP4ui);
        LOAD_GL_FUNCTION(glColorP4uiv);
        LOAD_GL_FUNCTION(glSecondaryColorP3ui);
        LOAD_GL_FUNCTION(glSecondaryColorP3uiv);
    }

    if(version >= 4.0)
    {
        LOAD_GL_FUNCTION(glMinSampleShading);
        LOAD_GL_FUNCTION(glBlendEquationi);
        LOAD_GL_FUNCTION(glBlendEquationSeparatei);
        LOAD_GL_FUNCTION(glBlendFunci);
        LOAD_GL_FUNCTION(glBlendFuncSeparatei);
        LOAD_GL_FUNCTION(glDrawArraysIndirect);
        LOAD_GL_FUNCTION(glDrawElementsIndirect);
        LOAD_GL_FUNCTION(glUniform1d);
        LOAD_GL_FUNCTION(glUniform2d);
        LOAD_GL_FUNCTION(glUniform3d);
        LOAD_GL_FUNCTION(glUniform4d);
        LOAD_GL_FUNCTION(glUniform1dv);
        LOAD_GL_FUNCTION(glUniform2dv);
        LOAD_GL_FUNCTION(glUniform3dv);
        LOAD_GL_FUNCTION(glUniform4dv);
        LOAD_GL_FUNCTION(glUniformMatrix2dv);
        LOAD_GL_FUNCTION(glUniformMatrix3dv);
        LOAD_GL_FUNCTION(glUniformMatrix4dv);
        LOAD_GL_FUNCTION(glUniformMatrix2x3dv);
        LOAD_GL_FUNCTION(glUniformMatrix2x4dv);
        LOAD_GL_FUNCTION(glUniformMatrix3x2dv);
        LOAD_GL_FUNCTION(glUniformMatrix3x4dv);
        LOAD_GL_FUNCTION(glUniformMatrix4x2dv);
        LOAD_GL_FUNCTION(glUniformMatrix4x3dv);
        LOAD_GL_FUNCTION(glGetUniformdv);
        LOAD_GL_FUNCTION(glGetSubroutineUniformLocation);
        LOAD_GL_FUNCTION(glGetSubroutineIndex);
        LOAD_GL_FUNCTION(glGetActiveSubroutineUniformiv);
        LOAD_GL_FUNCTION(glGetActiveSubroutineUniformName);
        LOAD_GL_FUNCTION(glGetActiveSubroutineName);
        LOAD_GL_FUNCTION(glUniformSubroutinesuiv);
        LOAD_GL_FUNCTION(glGetUniformSubroutineuiv);
        LOAD_GL_FUNCTION(glGetProgramStageiv);
        LOAD_GL_FUNCTION(glPatchParameteri);
        LOAD_GL_FUNCTION(glPatchParameterfv);
        LOAD_GL_FUNCTION(glBindTransformFeedback);
        LOAD_GL_FUNCTION(glDeleteTransformFeedbacks);
        LOAD_GL_FUNCTION(glGenTransformFeedbacks);
        LOAD_GL_FUNCTION(glIsTransformFeedback);
        LOAD_GL_FUNCTION(glPauseTransformFeedback);
        LOAD_GL_FUNCTION(glResumeTransformFeedback);
        LOAD_GL_FUNCTION(glDrawTransformFeedback);
        LOAD_GL_FUNCTION(glDrawTransformFeedbackStream);
        LOAD_GL_FUNCTION(glBeginQueryIndexed);
        LOAD_GL_FUNCTION(glEndQueryIndexed);
        LOAD_GL_FUNCTION(glGetQueryIndexediv);
    }

    if(version >= 4.1)
    {
        LOAD_GL_FUNCTION(glReleaseShaderCompiler);
        LOAD_GL_FUNCTION(glShaderBinary);
        LOAD_GL_FUNCTION(glGetShaderPrecisionFormat);
        LOAD_GL_FUNCTION(glDepthRangef);
        LOAD_GL_FUNCTION(glClearDepthf);
        LOAD_GL_FUNCTION(glGetProgramBinary);
        LOAD_GL_FUNCTION(glProgramBinary);
        LOAD_GL_FUNCTION(glProgramParameteri);
        LOAD_GL_FUNCTION(glUseProgramStages);
        LOAD_GL_FUNCTION(glActiveShaderProgram);
        LOAD_GL_FUNCTION(glCreateShaderProgramv);
        LOAD_GL_FUNCTION(glBindProgramPipeline);
        LOAD_GL_FUNCTION(glDeleteProgramPipelines);
        LOAD_GL_FUNCTION(glGenProgramPipelines);
        LOAD_GL_FUNCTION(glIsProgramPipeline);
        LOAD_GL_FUNCTION(glGetProgramPipelineiv);
        LOAD_GL_FUNCTION(glProgramUniform1i);
        LOAD_GL_FUNCTION(glProgramUniform1iv);
        LOAD_GL_FUNCTION(glProgramUniform1f);
        LOAD_GL_FUNCTION(glProgramUniform1fv);
        LOAD_GL_FUNCTION(glProgramUniform1d);
        LOAD_GL_FUNCTION(glProgramUniform1dv);
        LOAD_GL_FUNCTION(glProgramUniform1ui);
        LOAD_GL_FUNCTION(glProgramUniform1uiv);
        LOAD_GL_FUNCTION(glProgramUniform2i);
        LOAD_GL_FUNCTION(glProgramUniform2iv);
        LOAD_GL_FUNCTION(glProgramUniform2f);
        LOAD_GL_FUNCTION(glProgramUniform2fv);
        LOAD_GL_FUNCTION(glProgramUniform2d);
        LOAD_GL_FUNCTION(glProgramUniform2dv);
        LOAD_GL_FUNCTION(glProgramUniform2ui);
        LOAD_GL_FUNCTION(glProgramUniform2uiv);
        LOAD_GL_FUNCTION(glProgramUniform3i);
        LOAD_GL_FUNCTION(glProgramUniform3iv);
        LOAD_GL_FUNCTION(glProgramUniform3f);
        LOAD_GL_FUNCTION(glProgramUniform3fv);
        LOAD_GL_FUNCTION(glProgramUniform3d);
        LOAD_GL_FUNCTION(glProgramUniform3dv);
        LOAD_GL_FUNCTION(glProgramUniform3ui);
        LOAD_GL_FUNCTION(glProgramUniform3uiv);
        LOAD_GL_FUNCTION(glProgramUniform4i);
        LOAD_GL_FUNCTION(glProgramUniform4iv);
        LOAD_GL_FUNCTION(glProgramUniform4f);
        LOAD_GL_FUNCTION(glProgramUniform4fv);
        LOAD_GL_FUNCTION(glProgramUniform4d);
        LOAD_GL_FUNCTION(glProgramUniform4dv);
        LOAD_GL_FUNCTION(glProgramUniform4ui);
        LOAD_GL_FUNCTION(glProgramUniform4uiv);
        LOAD_GL_FUNCTION(glProgramUniformMatrix2fv);
        LOAD_GL_FUNCTION(glProgramUniformMatrix3fv);
        LOAD_GL_FUNCTION(glProgramUniformMatrix4fv);
        LOAD_GL_FUNCTION(glProgramUniformMatrix2dv);
        LOAD_GL_FUNCTION(glProgramUniformMatrix3dv);
        LOAD_GL_FUNCTION(glProgramUniformMatrix4dv);
        LOAD_GL_FUNCTION(glProgramUniformMatrix2x3fv);
        LOAD_GL_FUNCTION(glProgramUniformMatrix3x2fv);
        LOAD_GL_FUNCTION(glProgramUniformMatrix2x4fv);
        LOAD_GL_FUNCTION(glProgramUniformMatrix4x2fv);
        LOAD_GL_FUNCTION(glProgramUniformMatrix3x4fv);
        LOAD_GL_FUNCTION(glProgramUniformMatrix4x3fv);
        LOAD_GL_FUNCTION(glProgramUniformMatrix2x3dv);
        LOAD_GL_FUNCTION(glProgramUniformMatrix3x2dv);
        LOAD_GL_FUNCTION(glProgramUniformMatrix2x4dv);
        LOAD_GL_FUNCTION(glProgramUniformMatrix4x2dv);
        LOAD_GL_FUNCTION(glProgramUniformMatrix3x4dv);
        LOAD_GL_FUNCTION(glProgramUniformMatrix4x3dv);
        LOAD_GL_FUNCTION(glValidateProgramPipeline);
        LOAD_GL_FUNCTION(glGetProgramPipelineInfoLog);
        LOAD_GL_FUNCTION(glVertexAttribL1d);
        LOAD_GL_FUNCTION(glVertexAttribL2d);
        LOAD_GL_FUNCTION(glVertexAttribL3d);
        LOAD_GL_FUNCTION(glVertexAttribL4d);
        LOAD_GL_FUNCTION(glVertexAttribL1dv);
        LOAD_GL_FUNCTION(glVertexAttribL2dv);
        LOAD_GL_FUNCTION(glVertexAttribL3dv);
        LOAD_GL_FUNCTION(glVertexAttribL4dv);
        LOAD_GL_FUNCTION(glVertexAttribLPointer);
        LOAD_GL_FUNCTION(glGetVertexAttribLdv);
        LOAD_GL_FUNCTION(glViewportArrayv);
        LOAD_GL_FUNCTION(glViewportIndexedf);
        LOAD_GL_FUNCTION(glViewportIndexedfv);
        LOAD_GL_FUNCTION(glScissorArrayv);
        LOAD_GL_FUNCTION(glScissorIndexed);
        LOAD_GL_FUNCTION(glScissorIndexedv);
        LOAD_GL_FUNCTION(glDepthRangeArrayv);
        LOAD_GL_FUNCTION(glDepthRangeIndexed);
        LOAD_GL_FUNCTION(glGetFloati_v);
        LOAD_GL_FUNCTION(glGetDoublei_v);
    }

    if(version >= 4.2)
    {
        LOAD_GL_FUNCTION(glDrawArraysInstancedBaseInstance);
        LOAD_GL_FUNCTION(glDrawElementsInstancedBaseInstance);
        LOAD_GL_FUNCTION(glDrawElementsInstancedBaseVertexBaseInstance);
        LOAD_GL_FUNCTION(glGetInternalformativ);
        LOAD_GL_FUNCTION(glGetActiveAtomicCounterBufferiv);
        LOAD_GL_FUNCTION(glBindImageTexture);
        LOAD_GL_FUNCTION(glMemoryBarrier);
        LOAD_GL_FUNCTION(glTexStorage1D);
        LOAD_GL_FUNCTION(glTexStorage2D);
        LOAD_GL_FUNCTION(glTexStorage3D);
        LOAD_GL_FUNCTION(glDrawTransformFeedbackInstanced);
        LOAD_GL_FUNCTION(glDrawTransformFeedbackStreamInstanced);
    }

    if(version >= 4.3)
    {
        LOAD_GL_FUNCTION(glClearBufferData);
        LOAD_GL_FUNCTION(glClearBufferSubData);
        LOAD_GL_FUNCTION(glDispatchCompute);
        LOAD_GL_FUNCTION(glDispatchComputeIndirect);
        LOAD_GL_FUNCTION(glCopyImageSubData);
        LOAD_GL_FUNCTION(glFramebufferParameteri);
        LOAD_GL_FUNCTION(glGetFramebufferParameteriv);
        LOAD_GL_FUNCTION(glGetInternalformati64v);
        LOAD_GL_FUNCTION(glInvalidateTexSubImage);
        LOAD_GL_FUNCTION(glInvalidateTexImage);
        LOAD_GL_FUNCTION(glInvalidateBufferSubData);
        LOAD_GL_FUNCTION(glInvalidateBufferData);
        LOAD_GL_FUNCTION(glInvalidateFramebuffer);
        LOAD_GL_FUNCTION(glInvalidateSubFramebuffer);
        LOAD_GL_FUNCTION(glMultiDrawArraysIndirect);
        LOAD_GL_FUNCTION(glMultiDrawElementsIndirect);
        LOAD_GL_FUNCTION(glGetProgramInterfaceiv);
        LOAD_GL_FUNCTION(glGetProgramResourceIndex);
        LOAD_GL_FUNCTION(glGetProgramResourceName);
        LOAD_GL_FUNCTION(glGetProgramResourceiv);
        LOAD_GL_FUNCTION(glGetProgramResourceLocation);
        LOAD_GL_FUNCTION(glGetProgramResourceLocationIndex);
        LOAD_GL_FUNCTION(glShaderStorageBlockBinding);
        LOAD_GL_FUNCTION(glTexBufferRange);
        LOAD_GL_FUNCTION(glTexStorage2DMultisample);
        LOAD_GL_FUNCTION(glTexStorage3DMultisample);
        LOAD_GL_FUNCTION(glTextureView);
        LOAD_GL_FUNCTION(glBindVertexBuffer);
        LOAD_GL_FUNCTION(glVertexAttribFormat);
        LOAD_GL_FUNCTION(glVertexAttribIFormat);
        LOAD_GL_FUNCTION(glVertexAttribLFormat);
        LOAD_GL_FUNCTION(glVertexAttribBinding);
        LOAD_GL_FUNCTION(glVertexBindingDivisor);
        LOAD_GL_FUNCTION(glDebugMessageControl);
        LOAD_GL_FUNCTION(glDebugMessageInsert);
        LOAD_GL_FUNCTION(glDebugMessageCallback);
        LOAD_GL_FUNCTION(glGetDebugMessageLog);
        LOAD_GL_FUNCTION(glPushDebugGroup);
        LOAD_GL_FUNCTION(glPopDebugGroup);
        LOAD_GL_FUNCTION(glObjectLabel);
        LOAD_GL_FUNCTION(glGetObjectLabel);
        LOAD_GL_FUNCTION(glObjectPtrLabel);
        LOAD_GL_FUNCTION(glGetObjectPtrLabel);
    }

    if(version >= 4.4)
    {
        LOAD_GL_FUNCTION(glBufferStorage);
        LOAD_GL_FUNCTION(glClearTexImage);
        LOAD_GL_FUNCTION(glClearTexSubImage);
        LOAD_GL_FUNCTION(glBindBuffersBase);
        LOAD_GL_FUNCTION(glBindBuffersRange);
        LOAD_GL_FUNCTION(glBindTextures);
        LOAD_GL_FUNCTION(glBindSamplers);
        LOAD_GL_FUNCTION(glBindImageTextures);
        LOAD_GL_FUNCTION(glBindVertexBuffers);
    }

    if(version >= 4.5)
    {
        LOAD_GL_FUNCTION(glClipControl);
        LOAD_GL_FUNCTION(glCreateTransformFeedbacks);
        LOAD_GL_FUNCTION(glTransformFeedbackBufferBase);
        LOAD_GL_FUNCTION(glTransformFeedbackBufferRange);
        LOAD_GL_FUNCTION(glGetTransformFeedbackiv);
        LOAD_GL_FUNCTION(glGetTransformFeedbacki_v);
        LOAD_GL_FUNCTION(glGetTransformFeedbacki64_v);
        LOAD_GL_FUNCTION(glCreateBuffers);
        LOAD_GL_FUNCTION(glNamedBufferStorage);
        LOAD_GL_FUNCTION(glNamedBufferData);
        LOAD_GL_FUNCTION(glNamedBufferSubData);
        LOAD_GL_FUNCTION(glCopyNamedBufferSubData);
        LOAD_GL_FUNCTION(glClearNamedBufferData);
        LOAD_GL_FUNCTION(glClearNamedBufferSubData);
        LOAD_GL_FUNCTION(glMapNamedBuffer);
        LOAD_GL_FUNCTION(glMapNamedBufferRange);
        LOAD_GL_FUNCTION(glUnmapNamedBuffer);
        LOAD_GL_FUNCTION(glFlushMappedNamedBufferRange);
        LOAD_GL_FUNCTION(glGetNamedBufferParameteriv);
        LOAD_GL_FUNCTION(glGetNamedBufferParameteri64v);
        LOAD_GL_FUNCTION(glGetNamedBufferPointerv);
        LOAD_GL_FUNCTION(glGetNamedBufferSubData);
        LOAD_GL_FUNCTION(glCreateFramebuffers);
        LOAD_GL_FUNCTION(glNamedFramebufferRenderbuffer);
        LOAD_GL_FUNCTION(glNamedFramebufferParameteri);
        LOAD_GL_FUNCTION(glNamedFramebufferTexture);
        LOAD_GL_FUNCTION(glNamedFramebufferTextureLayer);
        LOAD_GL_FUNCTION(glNamedFramebufferDrawBuffer);
        LOAD_GL_FUNCTION(glNamedFramebufferDrawBuffers);
        LOAD_GL_FUNCTION(glNamedFramebufferReadBuffer);
        LOAD_GL_FUNCTION(glInvalidateNamedFramebufferData);
        LOAD_GL_FUNCTION(glInvalidateNamedFramebufferSubData);
        LOAD_GL_FUNCTION(glClearNamedFramebufferiv);
        LOAD_GL_FUNCTION(glClearNamedFramebufferuiv);
        LOAD_GL_FUNCTION(glClearNamedFramebufferfv);
        LOAD_GL_FUNCTION(glClearNamedFramebufferfi);
        LOAD_GL_FUNCTION(glBlitNamedFramebuffer);
        LOAD_GL_FUNCTION(glCheckNamedFramebufferStatus);
        LOAD_GL_FUNCTION(glGetNamedFramebufferParameteriv);
        LOAD_GL_FUNCTION(glGetNamedFramebufferAttachmentParameteriv);
        LOAD_GL_FUNCTION(glCreateRenderbuffers);
        LOAD_GL_FUNCTION(glNamedRenderbufferStorage);
        LOAD_GL_FUNCTION(glNamedRenderbufferStorageMultisample);
        LOAD_GL_FUNCTION(glGetNamedRenderbufferParameteriv);
        LOAD_GL_FUNCTION(glCreateTextures);
        LOAD_GL_FUNCTION(glTextureBuffer);
        LOAD_GL_FUNCTION(glTextureBufferRange);
        LOAD_GL_FUNCTION(glTextureStorage1D);
        LOAD_GL_FUNCTION(glTextureStorage2D);
        LOAD_GL_FUNCTION(glTextureStorage3D);
        LOAD_GL_FUNCTION(glTextureStorage2DMultisample);
        LOAD_GL_FUNCTION(glTextureStorage3DMultisample);
        LOAD_GL_FUNCTION(glTextureSubImage1D);
        LOAD_GL_FUNCTION(glTextureSubImage2D);
        LOAD_GL_FUNCTION(glTextureSubImage3D);
        LOAD_GL_FUNCTION(glCompressedTextureSubImage1D);
        LOAD_GL_FUNCTION(glCompressedTextureSubImage2D);
        LOAD_GL_FUNCTION(glCompressedTextureSubImage3D);
        LOAD_GL_FUNCTION(glCopyTextureSubImage1D);
        LOAD_GL_FUNCTION(glCopyTextureSubImage2D);
        LOAD_GL_FUNCTION(glCopyTextureSubImage3D);
        LOAD_GL_FUNCTION(glTextureParameterf);
        LOAD_GL_FUNCTION(glTextureParameterfv);
        LOAD_GL_FUNCTION(glTextureParameteri);
        LOAD_GL_FUNCTION(glTextureParameterIiv);
        LOAD_GL_FUNCTION(glTextureParameterIuiv);
        LOAD_GL_FUNCTION(glTextureParameteriv);
        LOAD_GL_FUNCTION(glGenerateTextureMipmap);
        LOAD_GL_FUNCTION(glBindTextureUnit);
        LOAD_GL_FUNCTION(glGetTextureImage);
        LOAD_GL_FUNCTION(glGetCompressedTextureImage);
        LOAD_GL_FUNCTION(glGetTextureLevelParameterfv);
        LOAD_GL_FUNCTION(glGetTextureLevelParameteriv);
        LOAD_GL_FUNCTION(glGetTextureParameterfv);
        LOAD_GL_FUNCTION(glGetTextureParameterIiv);
        LOAD_GL_FUNCTION(glGetTextureParameterIuiv);
        LOAD_GL_FUNCTION(glGetTextureParameteriv);
        LOAD_GL_FUNCTION(glCreateVertexArrays);
        LOAD_GL_FUNCTION(glDisableVertexArrayAttrib);
        LOAD_GL_FUNCTION(glEnableVertexArrayAttrib);
        LOAD_GL_FUNCTION(glVertexArrayElementBuffer);
        LOAD_GL_FUNCTION(glVertexArrayVertexBuffer);
        LOAD_GL_FUNCTION(glVertexArrayVertexBuffers);
        LOAD_GL_FUNCTION(glVertexArrayAttribBinding);
        LOAD_GL_FUNCTION(glVertexArrayAttribFormat);
        LOAD_GL_FUNCTION(glVertexArrayAttribIFormat);
        LOAD_GL_FUNCTION(glVertexArrayAttribLFormat);
        LOAD_GL_FUNCTION(glVertexArrayBindingDivisor);
        LOAD_GL_FUNCTION(glGetVertexArrayiv);
        LOAD_GL_FUNCTION(glGetVertexArrayIndexediv);
        LOAD_GL_FUNCTION(glGetVertexArrayIndexed64iv);
        LOAD_GL_FUNCTION(glCreateSamplers);
        LOAD_GL_FUNCTION(glCreateProgramPipelines);
        LOAD_GL_FUNCTION(glCreateQueries);
        LOAD_GL_FUNCTION(glGetQueryBufferObjecti64v);
        LOAD_GL_FUNCTION(glGetQueryBufferObjectiv);
        LOAD_GL_FUNCTION(glGetQueryBufferObjectui64v);
        LOAD_GL_FUNCTION(glGetQueryBufferObjectuiv);
        LOAD_GL_FUNCTION(glMemoryBarrierByRegion);
        LOAD_GL_FUNCTION(glGetTextureSubImage);
        LOAD_GL_FUNCTION(glGetCompressedTextureSubImage);
        LOAD_GL_FUNCTION(glGetGraphicsResetStatus);
        LOAD_GL_FUNCTION(glGetnCompressedTexImage);
        LOAD_GL_FUNCTION(glGetnTexImage);
        LOAD_GL_FUNCTION(glGetnUniformdv);
        LOAD_GL_FUNCTION(glGetnUniformfv);
        LOAD_GL_FUNCTION(glGetnUniformiv);
        LOAD_GL_FUNCTION(glGetnUniformuiv);
        LOAD_GL_FUNCTION(glReadnPixels);
        LOAD_GL_FUNCTION(glGetnMapdv);
        LOAD_GL_FUNCTION(glGetnMapfv);
        LOAD_GL_FUNCTION(glGetnMapiv);
        LOAD_GL_FUNCTION(glGetnPixelMapfv);
        LOAD_GL_FUNCTION(glGetnPixelMapuiv);
        LOAD_GL_FUNCTION(glGetnPixelMapusv);
        LOAD_GL_FUNCTION(glGetnPolygonStipple);
        LOAD_GL_FUNCTION(glGetnColorTable);
        LOAD_GL_FUNCTION(glGetnConvolutionFilter);
        LOAD_GL_FUNCTION(glGetnSeparableFilter);
        LOAD_GL_FUNCTION(glGetnHistogram);
        LOAD_GL_FUNCTION(glGetnMinmax);
        LOAD_GL_FUNCTION(glTextureBarrier);
    }

    return 1;
}

#define LOAD_WGL_FUNCTION(name) {name = (pfn_##name)wglGetProcAddress( #name ); if(!name) return 0;}
local b32 LoadWGLFunctions(void)
{
    LOAD_WGL_FUNCTION(wglChoosePixelFormatARB);
    LOAD_WGL_FUNCTION(wglCreateContextAttribsARB);
    LOAD_WGL_FUNCTION(wglSwapIntervalEXT);

    return 1;
}

void *PL_Alloc(u64 size)
{
    void *result = 0;
    result = (void*)HeapAlloc(GetProcessHeap(), 0, size);
    return result;
}

void *PL_Alloc0(u64 size)
{
    void *result = 0;
    result = (void*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, size);
    return result;
}

void *PL_ReAlloc(void *old, u64 size)
{
    void *result = 0;
    result = (void*)HeapReAlloc(GetProcessHeap(), 0, old, size);
    return result;
}

void *PL_ReAlloc0(void *old, u64 size)
{
    void *result = 0;
    result = (void*)HeapReAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, old, size);
    return result;
}

b32 PL_Free(void *mem)
{
    BOOL win32_result = HeapFree(GetProcessHeap(), 0, mem);
    b32 result = (b32)((win32_result) ? 1 : 0);
    return result;
}

b32 PL_TimerInit(void)
{
    LARGE_INTEGER freq = {0};
    if(!QueryPerformanceFrequency(&freq)) return 0;
    g_state->timer.perf_freq = (u64)freq.QuadPart;

    TIMECAPS caps;
    if(timeGetDevCaps(&caps, sizeof(TIMECAPS)) != MMSYSERR_NOERROR) return 0;
    g_state->timer.min_ms_wait = caps.wPeriodMin;

    return 1;
}

inline u64 PL_QueryTimer(void)
{
    LARGE_INTEGER counter;
    if(!QueryPerformanceCounter(&counter)) return 0;
    u64 result = counter.QuadPart;
    return result;
}

inline r32 PL_TimeElapsed(u64 start, u64 end)
{
    r32 result = ((r32)(end-start) / (r32)g_state->timer.perf_freq);
    return result;
}

b32 PL_RenderFrameTimerStart(void)
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

void PL_RenderFrameTimerEnd(void)
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

b32 PL_CreateWindow(const char *title, int w, int h)
{
    WNDCLASSEXA tmp_wndclass = {0};
    tmp_wndclass.cbSize = sizeof(WNDCLASSEXA);
    tmp_wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC | CS_DBLCLKS;
    tmp_wndclass.lpfnWndProc = &WndProc;
    tmp_wndclass.hInstance = GetModuleHandle(0);
    tmp_wndclass.hCursor = LoadCursor(0, IDC_ARROW);
    tmp_wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    tmp_wndclass.lpszClassName = "TMPPLWINDOWCLASS";
    if(!RegisterClassExA(&tmp_wndclass)) return 0;

    HWND tmp_hwnd = CreateWindowExA(0, tmp_wndclass.lpszClassName, "TMPWINDOW", WS_OVERLAPPEDWINDOW,
                                    CW_USEDEFAULT, CW_USEDEFAULT, 960, 540,
                                    0, 0, tmp_wndclass.hInstance, 0);
    if(!tmp_hwnd) return 0;

    HDC tmp_dc = GetDC(tmp_hwnd);
    if(!tmp_dc) return 0;

    PIXELFORMATDESCRIPTOR tmp_pfd = {0};
	tmp_pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	tmp_pfd.nVersion = 1;
	tmp_pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	tmp_pfd.iPixelType = PFD_TYPE_RGBA;
	tmp_pfd.cColorBits = 32;
	tmp_pfd.cDepthBits = 24;
	tmp_pfd.cStencilBits = 8;
	tmp_pfd.iLayerType = PFD_MAIN_PLANE;

    int tmp_pixel_format = ChoosePixelFormat(tmp_dc, &tmp_pfd);
    if(!tmp_pixel_format) return 0;
    if(!SetPixelFormat(tmp_dc, tmp_pixel_format, &tmp_pfd)) return 0;

    HGLRC tmp_rc = wglCreateContext(tmp_dc);
    if(!tmp_rc) return 0;
    if(!wglMakeCurrent(tmp_dc, tmp_rc)) return 0;

    if(!LoadGLFunctions(4.5f)) return 0;
    if(!LoadWGLFunctions()) return 0;

    WNDCLASSEXA *wndclass = &g_state->window.wndclass;
    wndclass->cbSize = sizeof(WNDCLASSEXA);
    wndclass->style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC | CS_DBLCLKS;
    wndclass->lpfnWndProc = &WndProc;
    wndclass->hInstance = GetModuleHandle(0);
    wndclass->hCursor = LoadCursor(0, IDC_ARROW);
    wndclass->hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wndclass->lpszClassName = "PLWINDOWCLASS";
    if(!RegisterClassExA(wndclass)) return 0;

    g_state->window.handle = CreateWindowExA(0, wndclass->lpszClassName, title, WS_OVERLAPPEDWINDOW,
                                             CW_USEDEFAULT, CW_USEDEFAULT, w, h,
                                             0, 0, wndclass->hInstance, 0);
    if(!g_state->window.handle) return 0;
    ShowWindow(g_state->window.handle, SW_SHOW);

    g_state->window.dc = GetDC(g_state->window.handle);
    if(!g_state->window.dc) return 0;

    int pixel_format = 0;
    u32 pixel_format_count = 0;
    persist const int pixel_attribs[] =
    {
        WGL_SUPPORT_OPENGL_ARB,	GL_TRUE,
		WGL_ACCELERATION_ARB,	WGL_FULL_ACCELERATION_ARB,
		WGL_DRAW_TO_WINDOW_ARB,	GL_TRUE,
		WGL_DOUBLE_BUFFER_ARB,	GL_TRUE,
		WGL_PIXEL_TYPE_ARB,		WGL_TYPE_RGBA_ARB,
		WGL_COLOR_BITS_ARB,		24,
		WGL_DEPTH_BITS_ARB,		24,
		WGL_STENCIL_BITS_ARB,	8,
		WGL_SAMPLE_BUFFERS_ARB,	GL_TRUE,
		WGL_SAMPLES_ARB,		4,
		0
    };

    if(!wglChoosePixelFormatARB(g_state->window.dc, pixel_attribs, 0, 1,
                                &pixel_format, &pixel_format_count)) return 0;
    PIXELFORMATDESCRIPTOR pfd = {0};
    if(!DescribePixelFormat(g_state->window.dc, pixel_format, sizeof(pfd), &pfd)) return 0;
    if(!SetPixelFormat(g_state->window.dc, pixel_format, &pfd)) return 0;

    const int attribs[] =
    {
        WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
		WGL_CONTEXT_MINOR_VERSION_ARB, 5,
		WGL_CONTEXT_LAYER_PLANE_ARB,   0,
		WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB | WGL_CONTEXT_DEBUG_BIT_ARB,
		WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
		0
    };

    g_state->window.rc = wglCreateContextAttribsARB(g_state->window.dc, 0, attribs);
    if(!g_state->window.rc) return 0;

    if(!wglDeleteContext(tmp_rc)) return 0;
    if(!ReleaseDC(tmp_hwnd, tmp_dc)) return 0;
    if(!DestroyWindow(tmp_hwnd)) return 0;


    g_state->window.w = w;
    g_state->window.h = h;
    g_state->window.target_fps = PL_TARGET_FPS;
    g_state->running = 1;

    return 1;
}

typedef HRESULT WINAPI tfn_DirectSoundCreate(LPCGUID pcGuidDevice, LPDIRECTSOUND *ppDS, LPUNKNOWN pUnkOuter);
global tfn_DirectSoundCreate *pfn_DirectSoundCreate;
b32 PL_InitDSound(void)
{
    g_state->audio.sample_rate = PL_AUDIO_SAMPLERATE;
    g_state->audio.bytes_per_sample = ((PL_AUDIO_BITSPERSAMPLE/8)*PL_AUDIO_CHANNELS);
    g_state->audio.dsbuffer_size = g_state->audio.bytes_per_sample * g_state->audio.sample_rate;
    g_state->audio.margin_bytes = (int)(((r32)g_state->audio.sample_rate *
                                         (r32)g_state->audio.bytes_per_sample / (r32)PL_TARGET_FPS) / 3.0f);

    HMODULE dsound_lib = LoadLibraryA("dsound.dll");
    if(!dsound_lib) return 0;
    pfn_DirectSoundCreate = (tfn_DirectSoundCreate*)GetProcAddress(dsound_lib, "DirectSoundCreate");
    if(!pfn_DirectSoundCreate) return 0;

    LPDIRECTSOUND dsound;
    HRESULT result = pfn_DirectSoundCreate(0, &dsound, 0);
    if(!SUCCEEDED(result)) return 0;

    WAVEFORMATEX fmt = {0};
    fmt.wFormatTag = WAVE_FORMAT_PCM;
    fmt.nChannels = PL_AUDIO_CHANNELS;
    fmt.nSamplesPerSec = g_state->audio.sample_rate;
    fmt.wBitsPerSample = PL_AUDIO_BITSPERSAMPLE;
    fmt.nBlockAlign = (fmt.nChannels * fmt.wBitsPerSample) / 8;
    fmt.nAvgBytesPerSec = fmt.nSamplesPerSec * fmt.nBlockAlign;

    result = IDirectSound_SetCooperativeLevel(dsound, g_state->window.handle, DSSCL_PRIORITY);
    if(!SUCCEEDED(result)) return 0;
    DSBUFFERDESC primary_buffer_desc = {0};
    primary_buffer_desc.dwSize = sizeof(primary_buffer_desc);
    primary_buffer_desc.dwFlags = DSBCAPS_PRIMARYBUFFER;

    LPDIRECTSOUNDBUFFER primary_buffer;
    result = IDirectSound_CreateSoundBuffer(dsound, &primary_buffer_desc, &primary_buffer, 0);
    if(!SUCCEEDED(result)) return 0;

    result = IDirectSoundBuffer_SetFormat(primary_buffer, &fmt);
    if(!SUCCEEDED(result)) return 0;

    DSBUFFERDESC secondary_buffer_desc = {0};
    secondary_buffer_desc.dwSize = sizeof(secondary_buffer_desc);
    secondary_buffer_desc.dwFlags = DSBCAPS_GETCURRENTPOSITION2;
    secondary_buffer_desc.dwBufferBytes = g_state->audio.dsbuffer_size;
    secondary_buffer_desc.lpwfxFormat = &fmt;

    result = IDirectSound_CreateSoundBuffer(dsound, &secondary_buffer_desc, &g_state->audio.dsbuffer, 0);
    if(!SUCCEEDED(result)) return 0;

    g_state->audio.is_init = 1;
    return 1;
}

void PL_ClearSoundBuffer(void)
{
    if(!g_state->audio.is_init) return;

    VOID *region[2];
    DWORD region_size[2];

    HRESULT result = IDirectSoundBuffer_Lock(g_state->audio.dsbuffer, 0, g_state->audio.dsbuffer_size,
                                             &region[0], &region_size[0], &region[1], &region_size[1], 0);
    if(!SUCCEEDED(result)) return;

    for(int region_index = 0;
        region_index < 2;
        region_index++)
    {
        u8 *sample = (u8*)region[region_index];
        for(DWORD byte_index = 0;
            byte_index < region_size[region_index];
            byte_index++)
        {
            *sample++ = 0;
        }
    }

    IDirectSoundBuffer_Unlock(g_state->audio.dsbuffer, region[0], region_size[0], region[1], region_size[1]);
}

void PL_FillSoundBuffer(DWORD byte_to_lock, DWORD bytes_to_write,
                        int sample_count, i16 *samples)
{
    if(!g_state->audio.is_init) return;

    VOID *region[2];
    DWORD region_size[2];

    HRESULT result = IDirectSoundBuffer_Lock(g_state->audio.dsbuffer, 0, g_state->audio.dsbuffer_size,
                                             &region[0], &region_size[0], &region[1], &region_size[1], 0);
    if(!SUCCEEDED(result)) return;

    i16 *src_sample = samples;

    for(int region_index = 0;
        region_index < 2;
        region_index++)
    {
        DWORD region_sample_count = region_size[region_index] / g_state->audio.bytes_per_sample;
        i16 *dst_sample = (i16*)region[region_index];
        for(DWORD sample_index = 0;
            sample_index < region_sample_count;
            sample_index++)
        {
            *dst_sample++ = *src_sample++;
            *dst_sample++ = *src_sample++;
            g_state->audio.sample_index++;
        }
    }

    IDirectSoundBuffer_Unlock(g_state->audio.dsbuffer, region[0], region_size[0], region[1], region_size[1]);
}

void PL_AudioPlay(void)
{
    if(!g_state->audio.is_init) return;
    IDirectSoundBuffer_Play(g_state->audio.dsbuffer, 0, 0, DSBPLAY_LOOPING);
}

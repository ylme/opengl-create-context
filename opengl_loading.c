#include "opengl_loading.h"

#include <windows.h>

PFNGLVIEWPORTPROC glViewport;

PFNGLCREATESHADERPROC glCreateShader;
PFNGLSHADERSOURCEPROC glShaderSource;
PFNGLCOMPILESHADERPROC glCompileShader;
PFNGLGETSHADERIVPROC glGetShaderiv;
PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
PFNGLCREATEPROGRAMPROC glCreateProgram;
PFNGLATTACHSHADERPROC glAttachShader;
PFNGLLINKPROGRAMPROC glLinkProgram;
PFNGLGETPROGRAMIVPROC glGetProgramiv;
PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;
PFNGLUSEPROGRAMPROC glUseProgram;

PFNGLCLEARBUFFERFVPROC glClearBufferfv;
PFNGLDRAWARRAYSPROC glDrawArrays;
PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;
PFNGLBINDVERTEXARRAYPROC glBindVertexArray;
PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays;
PFNGLVERTEXATTRIB4FVPROC glVertexAttrib4fv;

PFNGLPOINTSIZEPROC glPointSize;
PFNGLGETINTEGERVPROC glGetIntegerv;
PFNGLGETERRORPROC glGetError;

PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB;
PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB;

#define LoadFunc(T, val) val = (T)GetAnyGLFuncAddress(#val)

static void*
GetAnyGLFuncAddress(const char *name)
{
    void *p = (void *)wglGetProcAddress(name);
    if(p == 0 ||
      (p == (void*)0x1) || (p == (void*)0x2) || (p == (void*)0x3) ||
      (p == (void*)-1)) {
        static HMODULE module = NULL;
        if (!module)
            module = LoadLibraryW(L"opengl32.dll");
        p = (void*)GetProcAddress(module, name);
    }

  return p;
}

void
glloading_prev_init()
{
    LoadFunc(PFNWGLCREATECONTEXTATTRIBSARBPROC, wglCreateContextAttribsARB);
    LoadFunc(PFNWGLCHOOSEPIXELFORMATARBPROC, wglChoosePixelFormatARB);
}

void
glloading_init()
{
    LoadFunc(PFNGLVIEWPORTPROC, glViewport);

    LoadFunc(PFNGLCREATESHADERPROC, glCreateShader);
    LoadFunc(PFNGLSHADERSOURCEPROC, glShaderSource);
    LoadFunc(PFNGLCOMPILESHADERPROC, glCompileShader);
    LoadFunc(PFNGLGETSHADERIVPROC, glGetShaderiv);
    LoadFunc(PFNGLGETSHADERINFOLOGPROC, glGetShaderInfoLog);
    LoadFunc(PFNGLCREATEPROGRAMPROC, glCreateProgram);
    LoadFunc(PFNGLATTACHSHADERPROC, glAttachShader);
    LoadFunc(PFNGLLINKPROGRAMPROC, glLinkProgram);
    LoadFunc(PFNGLGETPROGRAMIVPROC, glGetProgramiv);
    LoadFunc(PFNGLGETPROGRAMINFOLOGPROC, glGetProgramInfoLog);
    LoadFunc(PFNGLUSEPROGRAMPROC, glUseProgram);
    
    LoadFunc(PFNGLCLEARBUFFERFVPROC, glClearBufferfv);
    LoadFunc(PFNGLDRAWARRAYSPROC, glDrawArrays);
    LoadFunc(PFNGLGENVERTEXARRAYSPROC, glGenVertexArrays);
    LoadFunc(PFNGLBINDVERTEXARRAYPROC, glBindVertexArray);
    LoadFunc(PFNGLDELETEVERTEXARRAYSPROC, glDeleteVertexArrays);
    LoadFunc(PFNGLVERTEXATTRIB4FVPROC, glVertexAttrib4fv);

    LoadFunc(PFNGLGETINTEGERVPROC, glGetIntegerv);
    LoadFunc(PFNGLGETERRORPROC, glGetError);
}

void*
glloading_pointer(const char *name)
{
    return GetAnyGLFuncAddress(name);
}
#ifndef _OPENGL_LOADING_H_
#define _OPENGL_LOADING_H_
#include <GL/glcorearb.h>
#include <GL/wglext.h>

void glloading_prev_init();
void glloading_init();
void* glloading_pointer(const char *name);

extern PFNGLVIEWPORTPROC glViewport;

extern PFNGLCREATESHADERPROC glCreateShader;
extern PFNGLSHADERSOURCEPROC glShaderSource;
extern PFNGLCOMPILESHADERPROC glCompileShader;
extern PFNGLGETSHADERIVPROC glGetShaderiv;
extern PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
extern PFNGLCREATEPROGRAMPROC glCreateProgram;
extern PFNGLATTACHSHADERPROC glAttachShader;
extern PFNGLLINKPROGRAMPROC glLinkProgram;
extern PFNGLGETPROGRAMIVPROC glGetProgramiv;
extern PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;
extern PFNGLUSEPROGRAMPROC glUseProgram;

extern PFNGLCLEARBUFFERFVPROC glClearBufferfv;
extern PFNGLDRAWARRAYSPROC glDrawArrays;
extern PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;
extern PFNGLBINDVERTEXARRAYPROC glBindVertexArray;
extern PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays;
extern PFNGLVERTEXATTRIB4FVPROC glVertexAttrib4fv;


extern PFNGLGETINTEGERVPROC glGetIntegerv;
extern PFNGLGETERRORPROC glGetError;

extern PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB;
extern PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB;

#endif // _OPENGL_LOADING_H_

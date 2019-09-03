#include "render_mgr.h"
#include "opengl_loading.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static GLuint
compile(GLenum type, const GLchar *str)
{
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &str, NULL);
    glCompileShader(shader);

    GLint result, len;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);

    if (len > 1) {
        char *log = (char*)malloc(len + 1);
        memset(log, 0, len + 1);
        glGetShaderInfoLog(shader, len, NULL, log);

        fprintf(stderr, "compile shader(%d) err:%s\n", type, log);
        free(log);
        exit(1);
    }
    
    return shader;
}

GLuint
render_mgr_link(const char *vstr, const char *fstr)
{
    GLuint vs = compile(GL_VERTEX_SHADER, vstr);
    GLuint fs = compile(GL_FRAGMENT_SHADER, fstr);

    GLuint program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);

    GLint result, len;
    glGetProgramiv(program, GL_LINK_STATUS, &result);
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);
    if (len > 1) {
        char *log = (char*)malloc(len + 1);
        memset(log, 0, len + 1);
        glGetProgramInfoLog(program, len, NULL, log);

        fprintf(stderr, "init program err:%s\n", log);
        free(log);
        exit(1);
    }

    return program;
}

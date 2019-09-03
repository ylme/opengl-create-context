#include "win_platform.h"
#include "opengl_loading.h"
#include "render_mgr.h"

#include <stdio.h>
#include <assert.h>

#define WIDTH 1280
#define HEIGHT 720

static const char *vstr =
"#version 430 core\n"
"void main()\n"
"{\n"
"    const vec4 vertices[] = vec4[](vec4(0.25, -0.25, 0.5, 1.0), vec4(-0.25, -0.25, 0.5, 1.0), vec4(0.25, 0.25, 0.5, 1.0));\n"
"    gl_Position = vertices[gl_VertexID];\n"
"}\n";

static const char *fstr =
"#version 430 core\n"
"layout (location = 0) out vec4 o_color;\n"
"void main()\n"
"{\n"
"    o_color = vec4(1.0, 0.0, 0.0, 1.0);\n"
"}\n";

struct UserData {
    GLuint vao;
    GLuint program;
};

static void
_init(void *pud)
{
    int major, minor;
    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);
    printf("OpenGL info major:%d minor:%d\n", major, minor);

    struct UserData *ud = pud;
    glGenVertexArrays(1, &ud->vao);
    glBindVertexArray(ud->vao);

    ud->program = render_mgr_link(vstr, fstr);
    assert(ud->program);
}

static void
_render(void *pud)
{
    static const GLfloat green[] = { 0.0f, 0.25f, 0.0f, 1.0f };
    glClearBufferfv(GL_COLOR, 0, green);

    struct UserData *ud = pud;
    glUseProgram(ud->program);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

static void
_shutdown(void *pud)
{
    struct UserData *ud = pud;
    glDeleteVertexArrays(1, &ud->vao);
}

int main()
{
    struct UserData data;
    PlatformContext pc;
    pc.ud = &data;
    pc.init_cb = _init;
    pc.render_cb = _render;
    pc.shutdown_cb = _shutdown;

    winplat_run(WIDTH, HEIGHT, &pc);
    return 0;
}

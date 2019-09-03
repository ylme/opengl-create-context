#include "win_platform.h"
#include "opengl_loading.h"

#include <windows.h>

#include <assert.h>
#include <string.h>

#define CLASS_NAME L"OpenGL window class"
#define WINDOW_NAME L"OpenGL create context"
#define WINDOW_STYLE (WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX)

static struct {
    HWND real_wnd;
    HDC real_dc;
    HGLRC real_ctx;
} G;

static LRESULT CALLBACK
_wnd_proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {
        case WM_CLOSE:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProcW(hWnd, message, wParam, lParam);
    }
    return 0;
}

static void
_register_class(HINSTANCE inst)
{
    WNDCLASSEXW wndclass;
    memset(&wndclass, 0, sizeof(wndclass));
    wndclass.cbSize = sizeof(wndclass);
    wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wndclass.lpfnWndProc = _wnd_proc;
    wndclass.hInstance = inst;
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndclass.lpszClassName = CLASS_NAME;

    RegisterClassExW(&wndclass);
}

static HWND
_create_fake_wnd(HINSTANCE inst)
{
    HWND wnd = CreateWindowExW(0, CLASS_NAME, L"",
        WINDOW_STYLE,
        0, 0, 1, 1,  // x, y, width, height
        NULL, NULL,  // parent window, menu
        inst, NULL); // instance, param
    return wnd;
}

static HGLRC
_create_fake_context(HDC dc)
{
    PIXELFORMATDESCRIPTOR pfd;
    memset(&pfd, 0, sizeof(pfd));
    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cAlphaBits = 8;
    pfd.cDepthBits = 24;

    int format = ChoosePixelFormat(dc, &pfd);
    assert(format);

    assert(SetPixelFormat(dc, format, &pfd));
    HGLRC ctx = wglCreateContext(dc);
    assert(ctx);
    return ctx;
}

static HWND
_create_real_wnd(HINSTANCE inst, int width, int height)
{
    RECT rect;
    rect.left = 0;
    rect.right = width;
    rect.top = 0;
    rect.bottom = height;

    AdjustWindowRect(&rect, WINDOW_STYLE, 0);

    HWND wnd = CreateWindowExW(0, CLASS_NAME, WINDOW_NAME,
        WINDOW_STYLE,
        CW_USEDEFAULT, 0, rect.right-rect.left, rect.bottom-rect.top,
        NULL, NULL,
        inst, NULL);
    return wnd;
}

static HGLRC
_create_real_context(HDC dc)
{
    const int pixel_attribs[] = {
        WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
        WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
        WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
        WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
        WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
        WGL_COLOR_BITS_ARB, 32,
        WGL_ALPHA_BITS_ARB, 8,
        WGL_DEPTH_BITS_ARB, 24,
        WGL_STENCIL_BITS_ARB, 8,
        WGL_SAMPLE_BUFFERS_ARB, GL_TRUE,
        WGL_SAMPLES_ARB, 4,
        0
    };

    int format; UINT num_formats;
    int status = wglChoosePixelFormatARB(dc, pixel_attribs, NULL, 1, &format, &num_formats);
    assert(status && num_formats > 0);

    // see MSDN
    // An application can only set the pixel format of a window one time.
    // Once a window's pixel format is set, it cannot be changed.
    PIXELFORMATDESCRIPTOR pfd;
    DescribePixelFormat(dc, format, sizeof(pfd), &pfd);
    SetPixelFormat(dc, format, &pfd);

    const int major_min = 4, minor_min = 5;
    const int ctx_attribs[] = {
        WGL_CONTEXT_MAJOR_VERSION_ARB, major_min,
        WGL_CONTEXT_MINOR_VERSION_ARB, minor_min,
        WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
        WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
//        WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_DEBUG_BIT_ARB,
        0
    };

    HGLRC ctx = wglCreateContextAttribsARB(dc, 0, ctx_attribs);
    assert(ctx);
    return ctx;
}

static void
_destroy_context(HWND wnd, HDC dc, HGLRC ctx)
{
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(ctx);
    ReleaseDC(wnd, dc);
    DestroyWindow(wnd);
}

static void
_create_window(HINSTANCE inst, int width, int height)
{
    HWND fake_wnd = _create_fake_wnd(inst);
    HDC fake_dc = GetDC(fake_wnd);
    HGLRC fake_ctx = _create_fake_context(fake_dc);

    assert(wglMakeCurrent(fake_dc, fake_ctx));
    glloading_prev_init();

    HWND real_wnd = _create_real_wnd(inst, width, height);
    HDC real_dc = GetDC(real_wnd);
    HGLRC real_ctx = _create_real_context(real_dc);

    _destroy_context(fake_wnd, fake_dc, fake_ctx);

    assert(wglMakeCurrent(real_dc, real_ctx));
    glloading_init();
    glViewport(0, 0, width, height);

    G.real_wnd = real_wnd;
    G.real_dc = real_dc;
    G.real_ctx = real_ctx;
}

static void
_destroy_window()
{
    _destroy_context(G.real_wnd, G.real_dc, G.real_ctx);
}

static void
_on_init(const PlatformContext *pc)
{
    if (pc->init_cb)
        pc->init_cb(pc->ud);
}

static void
_on_render(const PlatformContext *pc)
{
    pc->render_cb(pc->ud);
}

static void
_on_shutdown(const PlatformContext *pc)
{
    if (pc->shutdown_cb)
        pc->shutdown_cb(pc->ud);
}

void
winplat_run(int width, int height, const PlatformContext *platform_ctx)
{
    HINSTANCE inst = GetModuleHandleW(NULL);
    _register_class(inst);
    _create_window(inst, width, height);
    _on_init(platform_ctx);

    ShowWindow(G.real_wnd, SW_SHOWDEFAULT);

    int active = 1;
    MSG msg;
    while (active) {
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT)
                active = 0;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        _on_render(platform_ctx);
        SwapBuffers(G.real_dc);
    }

    _on_shutdown(platform_ctx);
    _destroy_window();
}

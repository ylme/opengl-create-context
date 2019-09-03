#ifndef _WIN_PLATFORM_H
#define _WIN_PLATFORM_H

typedef void (*InitCb)(void*);
typedef void (*RenderCb)(void*);
typedef void (*ShutdownCb)(void*);

typedef struct {
    void *ud;
    InitCb init_cb;
    RenderCb render_cb;
    ShutdownCb shutdown_cb;
} PlatformContext;

void winplat_run(int with, int height, const PlatformContext *platform_ctx);

#endif // _WIN_PLATFORM_H

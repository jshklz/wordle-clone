
#include "gfx_opengl.h"

#ifdef TORN_OPENGL
#include "../../third-party/glad/glad.h"
#endif

typedef struct GFXGL_SDL2Handle GFXGL_SDL2Handle;
struct GFXGL_SDL2Handle
{
   SDL_GLContext context;
   SDL_Window* window;
};


global GFXGL_SDL2Handle gl_handle;

torn_function i32
GFXGL_LoadOpenGL(OS_App* app)
{
 
    OS_SDL2Handle* handle = (OS_SDL2Handle*)OS_GetHandle(app);
    
    gl_handle.context = SDL_GL_CreateContext(handle->window);
    gl_handle.window = handle->window;
    if (!gladLoadGL())
        return 1;
    SDL_GL_SetSwapInterval(1);
    

    return 0;
    
}

torn_function void
GFXGL_Present(void)
{
    SDL_GL_SwapWindow(gl_handle.window);
}
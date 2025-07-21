// SDL2 Pipeline (Tues. Jan 6. 2023)

#include "../os.h"
#include <SDL2/SDL.h>



#ifdef TORN_OPENGL
#include "../../third-party/glad/glad.h"
#define GL_GLEXT_PROTOTYPES
#include <SDL2/SDL_opengl.h>  
#endif
global OS_App* active_app;

internal OS_SDL2Handle*
OS_GetSDL2Handle(OS_App* app)
{
    return (OS_SDL2Handle*)app->handle;
}

torn_function OS_App*
 OS_ConstructApp(const u8* name, V2F size, V2F pos, OS_AppStyle style)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        return 0;
    }

    OS_App* application = (OS_App*)MEM_ArenaAlloc(MEM_GetDefaultArena(), sizeof(OS_App));
    
    application->name  = name;
    application->style = style;
    application->handle = (OS_SDL2Handle*)MEM_ArenaAlloc(MEM_GetDefaultArena(), sizeof(OS_SDL2Handle));
    application->running = 1;
   active_app = application;
    OS_SDL2Handle* handle = OS_GetSDL2Handle(application);
    /*
    u32 flags = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME;
        
    if (style & OS_AppStyleVisible)
            flags |= WS_VISIBLE;
    if (style & OS_AppStyleMinimize)
            flags |= WS_MINIMIZEBOX;
    if (style & OS_AppStyleMaximize)
            flags |= WS_MAXIMIZEBOX;
    */
    #ifdef TORN_OPENGL
    u32 flags = SDL_WINDOW_OPENGL;
    #else
    u32 flags = 0;
    #endif
    if (size.x == OS_AppDefault.x) size.x = 1500;
    if (size.y == OS_AppDefault.y) size.y = 900;

    if (pos.x == OS_AppDefault.x) pos.x = SDL_WINDOWPOS_UNDEFINED;
    if (pos.y == OS_AppDefault.y) pos.y = SDL_WINDOWPOS_UNDEFINED;
#ifdef TORN_OPENGL
    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
    SDL_GL_SetAttribute( SDL_GL_ACCELERATED_VISUAL, 1 );
    SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 8 );
    SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 8 );
    SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 8 );
    SDL_GL_SetAttribute( SDL_GL_ALPHA_SIZE, 8 );

    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 2 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 1);
    
#endif

    handle->window = SDL_CreateWindow(name, 
                                      pos.x, pos.y,
                                      size.x, size.y,
                                      SDL_WINDOW_OPENGL);
    application->size.w = size.w;
    application->size.h = size.h;
    if (handle->window == 0) 
        return 0;
    
    u64 freq = 0;
    u64 time = 0;
    
    freq = SDL_GetPerformanceFrequency();
    time = SDL_GetPerformanceCounter();
    handle->time_start = (1000 * time) / freq;

    return application;
}
torn_function void OS_PollEvents(OS_App* app)
{
    OS_SDL2Handle* handle = OS_GetSDL2Handle(app);

    if (handle->current_keys == NULL)
        handle->current_keys = SDL_GetKeyboardState(NULL); 
    memcpy(handle->previous_keys, handle->current_keys, SDL_NUM_SCANCODES);
    SDL_PumpEvents(); 
    handle->current_keys = SDL_GetKeyboardState(NULL);

    while (SDL_PollEvent(&handle->event))
    {
        switch (handle->event.type)
        {
            case SDL_QUIT:
                app->running = 0;
                break;
        }
    }
}

torn_function void 
OS_WaitEvents(OS_App* app)
{
    OS_SDL2Handle* handle = OS_GetSDL2Handle(app);
    while (SDL_WaitEvent(&handle->event))
    {
        if (handle->event.type == SDL_QUIT) app->running = 0;
    }
}
torn_function V2F
OS_GetMousePosition(OS_App* app)
{
    V2I mouse_position = {0};
    SDL_GetMouseState(&mouse_position.x, &mouse_position.y);
    
    return v2(mouse_position.x, mouse_position.y);
}
 

torn_function void
OS_MessageBox(OS_App* app, const u8* contents, const u8* titlebar, OS_MessageBoxFlags flags)
{
    
    
    TORN_Log("OS: SDL2: MessageBox(%s, %s)\n", contents, titlebar);
    switch (flags)
    {
        case (OS_MessageBox_Error):
        {
            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, titlebar, contents, 0);
        } break;
        
        case (OS_MessageBox_Warning):
        {
            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING, titlebar, contents, 0);
        } break;
        
        case (OS_MessageBox_Info):
        {
            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, titlebar, contents, 0);
        } break;
        default:
        {
            SDL_ShowSimpleMessageBox(0, titlebar, contents, 0);
        } break;
    }
    
}

torn_function void OS_SetWindowTitle(OS_App* app, const char* title)
{

}
torn_function V2F  OS_GetPrimaryMonitorSize(void)
{

}
torn_function void OS_SetWindowBorderedFullscreen(OS_App* app)
{

}
torn_function void OS_SetWindowBorderlessFullscreen(OS_App* app)
{

}
torn_function r32
OS_GetTime(OS_App* app)
{

    u64 freq = 0;
    u64 time = 0;
    
    freq = SDL_GetPerformanceFrequency();
    time = SDL_GetPerformanceCounter();
    OS_SDL2Handle* handle = OS_GetSDL2Handle(app);
    
    return ((( 1000* time) / freq) - handle->time_start)/1000 ;
    

}
torn_function u8* 
OS_ReadFile(const u8* path)
{
    return (u8*)SDL_LoadFile(path, 0);
}
torn_function u8 OS_SaveFile(const u8* path, const u8* contents)
{

}
torn_function OS_App* OS_GetActiveApp(void)
{
    return active_app;
}
/*
* Events:
*/
torn_function OS_KeyState
OS_GetKeyState(i8 key)
{
    u8* keyboard_state = SDL_GetKeyboardState(0);
    OS_KeyState state;
    OS_App* app = OS_GetActiveApp();
    if (!app) {
        OS_KeyState empty = {0};
        return empty;
    }
    OS_SDL2Handle* handle = OS_GetSDL2Handle(app);
    i32 scancode = SDL_GetScancodeFromKey(key);

    state.pressed = keyboard_state[SDL_GetScancodeFromKey(scancode)];
    state.released = !state.pressed;
    state.clicked = handle->current_keys[scancode] && !handle->previous_keys[scancode];

    return state;
}
torn_function OS_MouseState 
OS_GetLeftMouseState(void)
{
    OS_MouseState state = {0};
    state.clicked = (SDL_BUTTON(SDL_GetMouseState(0,0)) == 1);
    state.released = !state.clicked;
    return state;
}
torn_function OS_MouseState OS_GetRightMouseState(void)
{

    OS_MouseState state = {0};
    state.clicked = (SDL_BUTTON(SDL_GetMouseState(0,0)) == 0);
    state.released = !state.clicked;
    return state;
}



torn_function void* OS_Allocate(i32 size)
{   

    return SDL_malloc(size);
}
torn_function void  OS_Free(void* address, u32 size)
{
    SDL_free(address);
}




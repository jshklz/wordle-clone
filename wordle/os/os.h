#ifndef OS_APP_H
#define OS_APP_H

#include "../base/base.h"
#include "../memory/arena.h"
#ifndef OS_AppDefault
#define OS_AppDefault v2(TORN_DEFAULT, TORN_DEFAULT)
#endif

typedef enum OS_AppStyle OS_AppStyle;
enum OS_AppStyle
{
    OS_AppStyleVisible = 1,
    OS_AppStyleMinimize = 2
        ,
    OS_AppStyleViewportResize = 3,
    OS_AppStyleMaximize = 4,
    OS_AppStyleDefault = (OS_AppStyleMaximize | OS_AppStyleViewportResize| OS_AppStyleMinimize | OS_AppStyleVisible),
};

typedef enum OS_EventType OS_EventType;
enum OS_EventType
{
    OS_EventNothing,
    OS_EventKeyPressed = 1,
    OS_EventKeyReleased = 2,
    OS_EventLMousePressed = 3,
    OS_EventRMousePressed = 4
        
};

typedef struct OS_Event OS_Event;
struct OS_Event
{
    OS_EventType type;
    const u8 key;
    V2F mouse_position;
};

typedef struct OS_App OS_App;
struct OS_App
{
    const u8* name;
    OS_Event event;
    V2I size;
    V2I pos;
    OS_AppStyle style;
    void* handle;
    b32 running;
};

typedef enum OS_MessageBoxFlags OS_MessageBoxFlags;
enum OS_MessageBoxFlags
{
    OS_MessageBox_Error,
    OS_MessageBox_Warning,
    OS_MessageBox_Info,
    OS_MessageBox_Nothing
};

#ifdef _WIN32
#include <windows.h>
typedef struct OS_Win32Handle OS_Win32Handle;
struct OS_Win32Handle
{
    HWND window;
    r32 time_start;
};

#else
#include <SDL2/SDL.h>

typedef struct OS_SDL2Handle OS_SDL2Handle;
struct OS_SDL2Handle
{
    SDL_Window* window;
    SDL_Event event;
    r32 time_start;
    u8 key_event_queue[256];
    const u8* current_keys;

    u8 previous_keys[SDL_NUM_SCANCODES];

};


#endif




torn_function OS_App* OS_ConstructApp(const u8* name, V2F size, V2F pos, OS_AppStyle style);

torn_function void OS_PollEvents(OS_App* app);

torn_function void OS_WaitEvents(OS_App* app);

torn_function V2F OS_GetMousePosition(OS_App* app);

// (OS_App* app) can be null.
torn_function void OS_MessageBox(OS_App* app, const u8* contents, const u8* titlebar, OS_MessageBoxFlags flags);


// This returns the handle to OS-specific stuff.
// This is to add upon the functionality of your program, without being limited to the API.
torn_function void* OS_GetHandle(OS_App* app);

torn_function void OS_SetWindowTitle(OS_App* app, const char* title);

torn_function V2F  OS_GetPrimaryMonitorSize(void);
torn_function void OS_SetWindowBorderedFullscreen(OS_App* app);
torn_function void OS_SetWindowBorderlessFullscreen(OS_App* app);
torn_function r32 OS_GetTime(OS_App* app);
torn_function u8* OS_ReadFile(const u8* path);
torn_function u8 OS_SaveFile(const u8* path, const u8* contents);

torn_function OS_App* OS_GetActiveApp(void);


/*
* Events:
*/


typedef struct OS_KeyState OS_KeyState;
struct OS_KeyState
{
    b32 pressed;
    b32 released;
    b32 clicked;
};

torn_function OS_KeyState OS_GetKeyState(i8 key);



typedef struct OS_MouseState OS_MouseState;
struct OS_MouseState
{
    b32 clicked;
    b32 released;
};


torn_function OS_MouseState OS_GetLeftMouseState(void);
torn_function OS_MouseState OS_GetRightMouseState(void);





#endif
#include "ModuleWindow.h"
#include "App.h"
#include "Log.h"
#include <glad/glad.h>

ModuleWindow::ModuleWindow(App* app)
    : Module(app, "Window")
{
}

bool ModuleWindow::Init()
{
    LOG_INFO("[Window] Initializing SDL window...");

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER) < 0)
    {
        LOG_ERROR("[Window] SDL_Init failed: %s", SDL_GetError());
        return false;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

  
    Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE;
    if (fullscreen)
        flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;

    window = SDL_CreateWindow(
        "YourEngine",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        width, height,
        flags
    );

    if (!window)
    {
        LOG_ERROR("[Window] SDL_CreateWindow failed: %s", SDL_GetError());
        return false;
    }

    
    gl_context = SDL_GL_CreateContext(window);
    if (!gl_context)
    {
        LOG_ERROR("[Window] Failed to create OpenGL context: %s", SDL_GetError());
        return false;
    }

    if (SDL_GL_SetSwapInterval(1) < 0)
        LOG_WARN("[Window] Unable to set VSync: %s", SDL_GetError());
    else
        LOG_INFO("[Window] VSync enabled.");

    
    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
    {
        LOG_ERROR("[Window] Failed to initialize GLAD.");
        return false;
    }

    LOG_INFO("[Window] OpenGL initialized: %s | %s",
        glGetString(GL_VENDOR),
        glGetString(GL_VERSION));


    glViewport(0, 0, width, height);

    LOG_INFO("[Window] SDL window created successfully (%dx%d).", width, height);
    return true;
}

bool ModuleWindow::CleanUp()
{
    LOG_INFO("[Window] Cleaning up window...");

    if (gl_context)
    {
        SDL_GL_DeleteContext(gl_context);
        gl_context = nullptr;
    }

    if (window)
    {
        SDL_DestroyWindow(window);
        window = nullptr;
    }

    SDL_QuitSubSystem(SDL_INIT_VIDEO);
    return true;
}

void ModuleWindow::SetTitle(const std::string& title)
{
    if (window)
        SDL_SetWindowTitle(window, title.c_str());
}

void ModuleWindow::SetSize(int w, int h)
{
    width = w;
    height = h;
    if (window)
        SDL_SetWindowSize(window, w, h);
    glViewport(0, 0, w, h);
}

void ModuleWindow::ToggleFullscreen()
{
    fullscreen = !fullscreen;
    if (window)
    {
        Uint32 mode = fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0;
        SDL_SetWindowFullscreen(window, mode);
    }
}

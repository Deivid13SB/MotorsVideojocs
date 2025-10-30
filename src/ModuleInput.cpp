#include "ModuleInput.h"
#include "App.h"
#include "ModuleEditor.h"
#include "Log.h"
#include <SDL.h>
#include <imgui.h>
#include <backends/imgui_impl_sdl2.h>


ModuleInput::ModuleInput(App* app)
    : Module(app, "Input")
{
}


bool ModuleInput::Init()
{
    SDL_InitSubSystem(SDL_INIT_EVENTS);
    LOG_INFO("[Input] Initialized.");
    return true;
}


bool ModuleInput::PreUpdate(float)
{
    SDL_Event e;
    wheel = 0;
    mouse_dx = mouse_dy = 0;

    while (SDL_PollEvent(&e))
    {
  
        ImGui_ImplSDL2_ProcessEvent(&e);

        switch (e.type)
        {
     
        case SDL_QUIT:
            quit = true;
            break;

          
        case SDL_KEYDOWN:
        case SDL_KEYUP:
        {
            bool pressed = (e.type == SDL_KEYDOWN);
            SDL_Keycode key = e.key.keysym.sym;

            switch (key)
            {
            case SDLK_w: keyW = pressed; break;
            case SDLK_a: keyA = pressed; break;
            case SDLK_s: keyS = pressed; break;
            case SDLK_d: keyD = pressed; break;
            case SDLK_q: keyQ = pressed; break;
            case SDLK_e: keyE = pressed; break;
            case SDLK_LSHIFT:
            case SDLK_RSHIFT: keyShift = pressed; break;
            case SDLK_LCTRL:
            case SDLK_RCTRL: keyCtrl = pressed; break;
            case SDLK_LALT:
            case SDLK_RALT: keyAlt = pressed; break;
            case SDLK_f: keyF = pressed; break;
            default: break;
            }
            break;
        }

        case SDL_MOUSEMOTION:
            mouse_x = e.motion.x;
            mouse_y = e.motion.y;
            mouse_dx = e.motion.xrel;
            mouse_dy = e.motion.yrel;
            break;

        
        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP:
        {
            bool pressed = (e.type == SDL_MOUSEBUTTONDOWN);
            switch (e.button.button)
            {
            case SDL_BUTTON_LEFT:   lmb = pressed; break;
            case SDL_BUTTON_RIGHT:  rmb = pressed; break;
            case SDL_BUTTON_MIDDLE: mmb = pressed; break;
            }
            break;
        }

        case SDL_MOUSEWHEEL:
            wheel = e.wheel.y;
            break;

        case SDL_DROPFILE:
        {
            char* dropped_filedir = e.drop.file;
            if (dropped_filedir)
            {
                pending_drop.emplace_back(dropped_filedir);
                LOG_INFO("[Input] File dropped: %s", dropped_filedir);
                SDL_free(dropped_filedir);
            }
            break;
        }

        default:
            break;
        }
    }

    return !quit;
}

bool ModuleInput::CleanUp()
{
    SDL_QuitSubSystem(SDL_INIT_EVENTS);
    LOG_INFO("[Input] CleanUp complete.");
    return true;
}

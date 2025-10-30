#pragma once

#include "Module.h"
#include <string>
#include <SDL.h>


class ModuleWindow : public Module
{
public:
    explicit ModuleWindow(App* app);
    ~ModuleWindow() override = default;

    bool Init() override;
    bool CleanUp() override;


    void SetTitle(const std::string& title);

    void SetSize(int width, int height);

    void ToggleFullscreen();

public:
    SDL_Window* window = nullptr;
    SDL_GLContext gl_context = nullptr;

    int width = 1280;
    int height = 720;
    bool fullscreen = false;
};


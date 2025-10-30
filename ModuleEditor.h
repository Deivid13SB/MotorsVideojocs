#pragma once

#include "Module.h"
#include <string>
#include <vector>


struct SDL_Window;
struct ImGuiIO;

class ModuleEditor : public Module
{
public:
    explicit ModuleEditor(App* app);
    ~ModuleEditor() override = default;


    bool Init() override;
    bool Start() override;
    bool PreUpdate(float dt) override;  
    bool Update(float dt) override;      
    bool PostUpdate(float dt) override;  
    bool CleanUp() override;


    bool show_menu_bar = true;
    bool show_console = true;
    bool show_config = true;
    bool show_hierarchy = true;
    bool show_inspector = true;
    bool show_about = false;


    bool request_quit = false;

private:

    void DrawMainMenu();
    void DrawConsole();
    void DrawConfig(float dt);
    void DrawHierarchy();
    void DrawInspector();
    void DrawAbout();


    void DrawDockspace();
    void OpenURL(const std::string& url);

private:
    static constexpr int FPS_SAMPLES = 120;
    float fps_buffer[FPS_SAMPLES] = { 0.0f };
    int   fps_head = 0;

    std::string gl_vendor, gl_renderer, gl_version, glsl_version;
    std::string sdl_version;

    void SetupStyle();

    ImGuiIO* io = nullptr;
};


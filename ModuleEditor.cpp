#include "ModuleEditor.h"
#include "App.h"
#include "ModuleWindow.h"
#include "ModuleRenderer.h"
#include "ModuleInput.h"
#include "ModuleCamera.h"
#include "ModuleScene.h"
#include "GameObject.h"
#include "Log.h"
#include <SDL.h>
#include <glad/glad.h>
#include <imgui.h>
#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_opengl3.h>
#include <string>
#include <functional>

ModuleEditor::ModuleEditor(App* app)
    : Module(app, "Editor")
{
}

bool ModuleEditor::Init()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    io = &ImGui::GetIO();
    io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    SetupStyle();

    
    ImGui_ImplSDL2_InitForOpenGL(app->window->window, app->window->gl_context);
    ImGui_ImplOpenGL3_Init("#version 460");

   
    const char* v = reinterpret_cast<const char*>(glGetString(GL_VENDOR));
    const char* r = reinterpret_cast<const char*>(glGetString(GL_RENDERER));
    const char* gv = reinterpret_cast<const char*>(glGetString(GL_VERSION));
    const char* gs = reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION));
    gl_vendor = v ? v : "";
    gl_renderer = r ? r : "";
    gl_version = gv ? gv : "";
    glsl_version = gs ? gs : "";

    SDL_version sv;
    SDL_GetVersion(&sv);
    sdl_version = std::to_string(sv.major) + "." + std::to_string(sv.minor) + "." + std::to_string(sv.patch);

    LOG_INFO("[Editor] ImGui initialized. Docking: ON");
    return true;
}

bool ModuleEditor::Start()
{

    return true;
}

bool ModuleEditor::PreUpdate(float)
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
    return true;
}

bool ModuleEditor::Update(float dt)
{
    DrawDockspace();

    if (show_menu_bar)   DrawMainMenu();
    if (show_console)    DrawConsole();
    if (show_config)     DrawConfig(dt);
    if (show_hierarchy)  DrawHierarchy();
    if (show_inspector)  DrawInspector();
    if (show_about)      DrawAbout();

    return !request_quit;
}

bool ModuleEditor::PostUpdate(float)
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    return true;
}


bool ModuleEditor::CleanUp()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
    LOG_INFO("[Editor] ImGui shutdown.");
    return true;
}

void ModuleEditor::DrawDockspace()
{
  
    const ImGuiViewport* vp = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(vp->Pos);
    ImGui::SetNextWindowSize(vp->Size);
    ImGui::SetNextWindowViewport(vp->ID);
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus |
        ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_MenuBar;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::Begin("DockSpace", nullptr, flags);
    ImGui::PopStyleVar(2);

    ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
    ImGui::DockSpace(dockspace_id, ImVec2(0, 0), ImGuiDockNodeFlags_None);
    ImGui::End();
}

void ModuleEditor::DrawMainMenu()
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Exit")) request_quit = true;
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("View"))
        {
            ImGui::MenuItem("Console", nullptr, &show_console);
            ImGui::MenuItem("Config", nullptr, &show_config);
            ImGui::MenuItem("Hierarchy", nullptr, &show_hierarchy);
            ImGui::MenuItem("Inspector", nullptr, &show_inspector);
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Help"))
        {
            if (ImGui::MenuItem("About")) show_about = true;
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}

void ModuleEditor::DrawConsole()
{
    ImGui::Begin("Console", &show_console);

    const auto& logs = Log::GetEntries();
    static bool autoScroll = true;
    ImGui::Checkbox("Auto-scroll", &autoScroll);
    ImGui::SameLine();
    if (ImGui::Button("Clear")) Log::Clear();

    ImGui::Separator();

    ImGui::BeginChild("ConsoleScroll", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
    for (const auto& e : logs)
    {
        ImVec4 color = { 0.85f, 0.85f, 0.85f, 1.0f };
        if (e.type == LogType::WARNING) color = { 1.0f, 0.95f, 0.6f, 1.0f };
        if (e.type == LogType::ERROR)   color = { 1.0f, 0.4f, 0.4f, 1.0f };
        ImGui::TextColored(color, "%s", e.message.c_str());
    }
    if (autoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
        ImGui::SetScrollHereY(1.0f);
    ImGui::EndChild();

    ImGui::End();
}

void ModuleEditor::DrawConfig(float dt)
{
    ImGui::Begin("Config", &show_config);

    // FPS plot
    fps_buffer[fps_head] = 1.0f / (dt > 0.00001f ? dt : 0.00001f);
    fps_head = (fps_head + 1) % FPS_SAMPLES;

    ImGui::SeparatorText("Performance");
    ImGui::PlotLines("FPS", fps_buffer, FPS_SAMPLES, fps_head, nullptr, 0.0f, 240.0f, ImVec2(0, 80));
    ImGui::Text("dt: %.3f ms", dt * 1000.0f);

    ImGui::SeparatorText("Hardware / API");
    ImGui::Text("SDL: %s", sdl_version.c_str());
    ImGui::Text("GL Vendor:   %s", gl_vendor.c_str());
    ImGui::Text("GL Renderer: %s", gl_renderer.c_str());
    ImGui::Text("GL Version:  %s", gl_version.c_str());
    ImGui::Text("GLSL:        %s", glsl_version.c_str());

    ImGui::End();
}

void ModuleEditor::DrawHierarchy()
{
    ImGui::Begin("Hierarchy", &show_hierarchy);

    if (!app->scene || !app->scene->root)
    {
        ImGui::TextDisabled("No scene loaded.");
        ImGui::End();
        return;
    }

    std::function<void(GameObject*)> drawNode = [&](GameObject* go)
        {
            ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
            if (app->scene->selected == go) flags |= ImGuiTreeNodeFlags_Selected;

            bool open = ImGui::TreeNodeEx(go, flags, "%s", go->name.c_str());
            if (ImGui::IsItemClicked())
                app->scene->selected = go;

            if (open)
            {
                for (auto& c : go->children)
                    drawNode(c.get());
                ImGui::TreePop();
            }
        };

    drawNode(app->scene->root.get());
    ImGui::End();
}

void ModuleEditor::DrawInspector()
{
    ImGui::Begin("Inspector", &show_inspector);

    if (!app->scene || !app->scene->selected)
    {
        ImGui::TextDisabled("No GameObject selected.");
        ImGui::End();
        return;
    }

    app->scene->selected->OnEditor();

    ImGui::End();
}

void ModuleEditor::DrawAbout()
{
    if (ImGui::Begin("About", &show_about))
    {
        ImGui::Text("YourEngine – Geometry Viewer (E1)");
        ImGui::TextDisabled("Powered by SDL2, GLAD, OpenGL, ImGui, Assimp, GLM");
        if (ImGui::Button("Open repository")) OpenURL("https://example.com/yourengine");
    }
    ImGui::End();
}

void ModuleEditor::SetupStyle()
{
    ImGui::StyleColorsDark();
    auto& s = ImGui::GetStyle();
    s.WindowRounding = 6.0f;
    s.FrameRounding = 6.0f;
    s.GrabRounding = 6.0f;
}

void ModuleEditor::OpenURL(const std::string& url)
{
#if defined(_WIN32)
    std::wstring wurl(url.begin(), url.end());
    ShellExecuteW(nullptr, L"open", wurl.c_str(), nullptr, nullptr, SW_SHOWNORMAL);
#else
    (void)url;
#endif
}

#include "App.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleRenderer.h"
#include "ModuleEditor.h"
#include "ModuleCamera.h"
#include "ModuleAssets.h"
#include "ModuleTextures.h"
#include "ModuleModels.h"
#include "ModuleScene.h"
#include "Log.h"

App::App()
{
    modules.reserve(16);
    modules.emplace_back(window = new ModuleWindow(this));
    modules.emplace_back(renderer = new ModuleRenderer(this));
    modules.emplace_back(input = new ModuleInput(this));
    modules.emplace_back(textures = new ModuleTextures(this));
    modules.emplace_back(models = new ModuleModels(this));
    modules.emplace_back(scene = new ModuleScene(this));
    modules.emplace_back(camera = new ModuleCamera(this));
    modules.emplace_back(assets = new ModuleAssets(this));
    modules.emplace_back(editor = new ModuleEditor(this));
}

App::~App() = default;

bool App::Init()
{
    for (auto& m : modules)
    {
        if (!m->Init())
        {
            return false;
        }
    }
    return true;
}

bool App::Start()
{
    for (auto& m : modules)
    {
        if (!m->Start())
        {
            return false;
        }
    }
    return true;
}

bool App::Update()
{
    time.Tick();
    const float dt = time.DeltaTime();

    if (!PreUpdateAll(dt))  return false;
    if (!UpdateAll(dt))     return false;
    if (!PostUpdateAll(dt)) return false;

    return true;
}

bool App::CleanUp()
{
    for (auto it = modules.rbegin(); it != modules.rend(); ++it)
    {
        if (!(*it)->CleanUp())
        {
            return false;
        }
    }
    return true;
}

bool App::PreUpdateAll(float dt)
{
    if (renderer && !renderer->PreUpdate(dt)) return false;
    for (auto& m : modules)
    {
        if (m.get() == renderer) continue;
        if (!m->PreUpdate(dt)) return false;
    }
    return true;
}

bool App::UpdateAll(float dt)
{
    for (auto& m : modules)
    {
        if (!m->Update(dt)) return false;
    }
    return true;
}

bool App::PostUpdateAll(float dt)
{
    for (auto& m : modules)
    {
        if (m.get() == renderer) continue;
        if (!m->PostUpdate(dt)) return false;
    }

    if (renderer && !renderer->PostUpdate(dt)) return false;

    return true;
}
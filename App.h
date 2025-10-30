#pragma once

#include <vector>
#include <memory>
#include <string>

#include "Module.h"
#include "Time.h"


class ModuleWindow;
class ModuleInput;
class ModuleRenderer;
class ModuleEditor;
class ModuleCamera;
class ModuleAssets;
class ModuleTextures;
class ModuleModels;
class ModuleScene;

class App
{
public:
    App();
    ~App();


    bool Init();   
    bool Start();   
    bool Update();     
    bool CleanUp();   

    float DeltaTime() const { return time.DeltaTime(); }

    ModuleWindow* window = nullptr;
    ModuleRenderer* renderer = nullptr;
    ModuleInput* input = nullptr;
    ModuleEditor* editor = nullptr;
    ModuleCamera* camera = nullptr;
    ModuleAssets* assets = nullptr;
    ModuleTextures* textures = nullptr;
    ModuleModels* models = nullptr;
    ModuleScene* scene = nullptr;


    Time time;

private:

    std::vector<std::unique_ptr<Module>> modules;

    bool PreUpdateAll(float dt);
    bool UpdateAll(float dt);
    bool PostUpdateAll(float dt);
};
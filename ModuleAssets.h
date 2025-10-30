#pragma once

#include "Module.h"
#include <string>
#include <vector>
#include <memory>

class GameObject;
class ModuleModels;
class ModuleTextures;

class ModuleAssets : public Module
{
public:
    explicit ModuleAssets(App* app);
    ~ModuleAssets() override = default;

    bool Start() override;
    bool Update(float dt) override;
    bool CleanUp() override;


    bool LoadAsset(const std::string& path);


    std::string assetsRoot = "Assets/";

private:
    void ProcessDroppedFiles();
};

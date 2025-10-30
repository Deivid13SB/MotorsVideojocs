#pragma once

#include "Module.h"
#include "ecs/C_Texture.h"
#include <string>
#include <unordered_map>

class ModuleTextures : public Module
{
public:
    explicit ModuleTextures(App* app);
    ~ModuleTextures() override = default;

    bool Init() override;
    bool CleanUp() override;


    Tex2D Load(const std::string& path);

 
    const Tex2D& GetChecker() const { return checker; }

private:
    Tex2D CreateCheckerTexture();
    Tex2D LoadFromFile(const std::string& path);

private:
    std::unordered_map<std::string, Tex2D> cache;
    Tex2D checker;
};


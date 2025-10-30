#pragma once

#include "Module.h"
#include <string>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

class GameObject;

class ModuleModels : public Module
{
public:
    explicit ModuleModels(App* app);
    ~ModuleModels() override = default;

    bool Start() override;
    bool CleanUp() override;

   
    bool ImportFBX(const std::string& path, GameObject& outRoot);

private:
    
    void ProcessNode(aiNode* node, const aiScene* scene, GameObject& parent);
    void ProcessMesh(aiMesh* mesh, const aiScene* scene, GameObject& go);
};

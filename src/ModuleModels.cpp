#include "ModuleModels.h"
#include "App.h"
#include "ecs/GameObject.h"
#include "ecs/C_Mesh.h"
#include "ecs/C_Texture.h"
#include "ecs/C_Transform.h"
#include "Log.h"
#include <glad/glad.h>
#include <filesystem>

namespace fs = std::filesystem;

ModuleModels::ModuleModels(App* app)
    : Module(app, "Models")
{
}

bool ModuleModels::Start()
{
    LOG_INFO("[Models] Assimp module ready.");
    return true;
}

bool ModuleModels::CleanUp()
{
    LOG_INFO("[Models] CleanUp.");
    return true;
}

bool ModuleModels::ImportFBX(const std::string& path, GameObject& outRoot)
{
    if (!fs::exists(path))
    {
        LOG_ERROR("[Models] File not found: %s", path.c_str());
        return false;
    }

    LOG_INFO("[Models] Importing model: %s", path.c_str());

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path,
        aiProcess_Triangulate |
        aiProcess_GenNormals |
        aiProcess_CalcTangentSpace |
        aiProcess_JoinIdenticalVertices |
        aiProcess_SortByPType);

    if (!scene || !scene->mRootNode)
    {
        LOG_ERROR("[Models] Assimp import failed: %s", importer.GetErrorString());
        return false;
    }

    outRoot.name = fs::path(path).stem().string();
    outRoot.Add<C_Transform>();

    ProcessNode(scene->mRootNode, scene, outRoot);

    LOG_INFO("[Models] Model imported successfully: %s", outRoot.name.c_str());
    return true;
}

void ModuleModels::ProcessNode(aiNode* node, const aiScene* scene, GameObject& parent)
{
    GameObject* go = parent.CreateChild(node->mName.C_Str());
    auto* tr = go->Add<C_Transform>();


    for (unsigned i = 0; i < node->mNumMeshes; ++i)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        ProcessMesh(mesh, scene, *go);
    }

    for (unsigned i = 0; i < node->mNumChildren; ++i)
    {
        ProcessNode(node->mChildren[i], scene, *go);
    }
}

void ModuleModels::ProcessMesh(aiMesh* mesh, const aiScene* scene, GameObject& go)
{
    auto* compMesh = go.Add<C_Mesh>();

    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    vertices.reserve(mesh->mNumVertices * 8);

    for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
    {
        aiVector3D pos = mesh->mVertices[i];
        aiVector3D norm = mesh->HasNormals() ? mesh->mNormals[i] : aiVector3D(0, 0, 0);
        aiVector3D uv = mesh->HasTextureCoords(0) ? mesh->mTextureCoords[0][i] : aiVector3D(0, 0, 0);

        vertices.push_back(pos.x);
        vertices.push_back(pos.y);
        vertices.push_back(pos.z);

        vertices.push_back(norm.x);
        vertices.push_back(norm.y);
        vertices.push_back(norm.z);

        vertices.push_back(uv.x);
        vertices.push_back(uv.y);

        compMesh->gpu.localBBox.Grow({ pos.x, pos.y, pos.z });
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; ++i)
    {
        const aiFace& face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; ++j)
            indices.push_back(face.mIndices[j]);
    }

    compMesh->gpu.vertexCount = static_cast<int>(mesh->mNumVertices);
    compMesh->gpu.indexCount = static_cast<int>(indices.size());

    GLuint vao, vbo, ebo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    GLsizei stride = sizeof(float) * 8;
    glEnableVertexAttribArray(0); 
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);

    glEnableVertexAttribArray(1); 
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));

    glBindVertexArray(0);

    compMesh->gpu.vao = vao;
    compMesh->gpu.vbo = vbo;
    compMesh->gpu.ebo = ebo;

    LOG_INFO("[Models] Mesh processed: %s (%d vertices, %d indices)",
        go.name.c_str(), compMesh->gpu.vertexCount, compMesh->gpu.indexCount);

    if (mesh->mMaterialIndex >= 0 && scene->mMaterials)
    {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        aiString texPath;
        if (material->GetTexture(aiTextureType_DIFFUSE, 0, &texPath) == AI_SUCCESS)
        {
            std::string fullPath = texPath.C_Str();

            if (!fullPath.empty())
            {
                fs::path textureFile = fs::path(fullPath);
                std::string textureName = textureFile.filename().string();
                fs::path localPath = fs::path("Assets") / textureName;

                auto* texComp = go.Add<C_Texture>();
                texComp->tex.path = localPath.string();
                texComp->useChecker = false;

                LOG_INFO("[Models] Found texture: %s", textureName.c_str());
            }
        }
    }
}

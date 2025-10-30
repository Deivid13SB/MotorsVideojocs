#include "ModuleAssets.h"
#include "App.h"
#include "ModuleInput.h"
#include "ModuleModels.h"
#include "ModuleTextures.h"
#include "ModuleScene.h"
#include "GameObject.h"
#include "C_Texture.h"
#include "Log.h"

#include <filesystem>

namespace fs = std::filesystem;

ModuleAssets::ModuleAssets(App* app)
    : Module(app, "Assets")
{
}

bool ModuleAssets::Start()
{
    if (!fs::exists(assetsRoot))
    {
        fs::create_directory(assetsRoot);
        LOG_INFO("[Assets] Created assets folder: %s", assetsRoot.c_str());
    }
    else
    {
        LOG_INFO("[Assets] Using existing assets folder: %s", assetsRoot.c_str());
    }

    return true;
}

bool ModuleAssets::Update(float dt)
{
    ProcessDroppedFiles();
    return true;
}

bool ModuleAssets::CleanUp()
{
    LOG_INFO("[Assets] CleanUp complete.");
    return true;
}

bool ModuleAssets::LoadAsset(const std::string& path)
{
    std::string ext;
    {
        size_t dot = path.find_last_of('.');
        if (dot != std::string::npos)
            ext = path.substr(dot + 1);
        for (auto& c : ext) c = (char)tolower(c);
    }

    if (ext == "fbx" || ext == "obj")
    {
        GameObject tempRoot;
        if (app->models->ImportFBX(path, tempRoot))
        {
            LOG_INFO("[Assets] Model loaded: %s", path.c_str());
            if (app->scene && app->scene->root)
            {
             
                app->scene->root->children.emplace_back(std::make_unique<GameObject>(std::move(tempRoot)));
                LOG_INFO("[Assets] Added model to scene hierarchy.");
            }
        }
        else
        {
            LOG_ERROR("[Assets] Failed to load FBX: %s", path.c_str());
        }
        return true;
    }

    if (ext == "png" || ext == "jpg" || ext == "jpeg" || ext == "dds" || ext == "tga")
    {
        if (app->scene && app->scene->selected)
        {
            auto* texComp = app->scene->selected->Get<C_Texture>();
            if (!texComp) texComp = app->scene->selected->Add<C_Texture>();
            texComp->tex = app->textures->Load(path);
            LOG_INFO("[Assets] Applied texture to selected GameObject: %s", path.c_str());
        }
        else
        {
            LOG_WARN("[Assets] No GameObject selected for texture: %s", path.c_str());
        }
        return true;
    }

    LOG_WARN("[Assets] Unsupported file type: %s", path.c_str());
    return false;
}

void ModuleAssets::ProcessDroppedFiles()
{
    if (!app->input)
        return;

    auto dropped = app->input->pending_drop;
    if (dropped.empty())
        return;

    for (const auto& file : dropped)
    {
        LOG_INFO("[Assets] File dropped: %s", file.c_str());

   
        fs::path src(file);
        fs::path dst = fs::path(assetsRoot) / src.filename();

        try
        {
            if (!fs::exists(dst))
            {
                fs::copy_file(src, dst, fs::copy_options::overwrite_existing);
                LOG_INFO("[Assets] Copied file to Assets/: %s", dst.string().c_str());
            }
        }
        catch (std::exception& e)
        {
            LOG_WARN("[Assets] Copy failed: %s", e.what());
        }

  
        LoadAsset(dst.string());
    }
    app->input->pending_drop.clear();
}

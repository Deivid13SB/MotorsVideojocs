#include "ModuleTextures.h"
#include "App.h"
#include "Log.h"
#include <glad/glad.h>
#define STB_IMAGE_IMPLEMENTATION
#include "external/stb/stb_image.h"

#include <filesystem>
namespace fs = std::filesystem;

ModuleTextures::ModuleTextures(App* app)
    : Module(app, "Textures")
{
}

bool ModuleTextures::Init()
{
    checker = CreateCheckerTexture();
    LOG_INFO("[Textures] Initialized with checker texture (%dx%d)", checker.w, checker.h);
    return true;
}

bool ModuleTextures::CleanUp()
{
    for (auto& [path, tex] : cache)
    {
        if (tex.id != 0)
            glDeleteTextures(1, &tex.id);
    }
    cache.clear();

    if (checker.id)
        glDeleteTextures(1, &checker.id);

    LOG_INFO("[Textures] CleanUp complete.");
    return true;
}

Tex2D ModuleTextures::Load(const std::string& path)
{
    if (path.empty())
    {
        LOG_WARN("[Textures] Empty path.");
        return checker;
    }

    auto it = cache.find(path);
    if (it != cache.end())
        return it->second;

    Tex2D tex = LoadFromFile(path);
    if (tex.id != 0)
    {
        cache[path] = tex;
        LOG_INFO("[Textures] Loaded: %s (%dx%d)", path.c_str(), tex.w, tex.h);
    }
    else
    {
        LOG_WARN("[Textures] Failed to load: %s (using checker)", path.c_str());
        tex = checker;
    }
    return tex;
}

Tex2D ModuleTextures::LoadFromFile(const std::string& path)
{
    Tex2D result;
    int w = 0, h = 0, comp = 0;

    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path.c_str(), &w, &h, &comp, STBI_rgb_alpha);
    if (!data)
    {
        LOG_ERROR("[Textures] stb_image failed: %s", stbi_failure_reason());
        return result;
    }

    GLuint id;
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);

    result.id = id;
    result.w = w;
    result.h = h;
    result.path = path;
    return result;
}

Tex2D ModuleTextures::CreateCheckerTexture()
{
    constexpr int size = 64;
    constexpr int cell = 8;

    unsigned char data[size * size * 3];
    for (int y = 0; y < size; ++y)
    {
        for (int x = 0; x < size; ++x)
        {
            int i = (y * size + x) * 3;
            bool white = ((x / cell + y / cell) % 2) == 0;
            unsigned char c = white ? 220 : 80;
            data[i + 0] = c;
            data[i + 1] = c;
            data[i + 2] = c;
        }
    }

    GLuint id;
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, size, size, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glBindTexture(GL_TEXTURE_2D, 0);

    Tex2D tex;
    tex.id = id;
    tex.w = size;
    tex.h = size;
    tex.path = "Checker";
    return tex;
}

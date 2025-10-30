#pragma once
#include "Component.h"
#include <string>

struct Tex2D
{
    unsigned    id = 0;    
    int         w = 0;  
    int         h = 0;      
    std::string path;          

    bool IsValid() const { return id != 0 && w > 0 && h > 0; }
};

struct C_Texture : Component
{
    Tex2D      tex;
    bool       useChecker = false;
    bool       linear = true;  
    bool       mipmaps = true;    
    glm::vec2  uvTiling{ 1.0f, 1.0f };
    glm::vec2  uvOffset{ 0.0f, 0.0f };

    C_Texture() : Component(TEXTURE) {}

    void OnEditor() override
    {
    }
};
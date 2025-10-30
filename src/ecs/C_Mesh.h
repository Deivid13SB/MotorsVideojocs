#pragma once

#include "Component.h"
#include <vector>
#include <cfloat>      
#include <glm/glm.hpp> 

struct AABB
{
    glm::vec3 min{ FLT_MAX,  FLT_MAX,  FLT_MAX };
    glm::vec3 max{ -FLT_MAX, -FLT_MAX, -FLT_MAX };

    void Reset()
    {
        min = { FLT_MAX,  FLT_MAX,  FLT_MAX };
        max = { -FLT_MAX, -FLT_MAX, -FLT_MAX };
    }

    void Grow(const glm::vec3& p)
    {
        min = glm::min(min, p);
        max = glm::max(max, p);
    }

    glm::vec3 Center() const { return (min + max) * 0.5f; }

  
    float Radius() const
    {
        const glm::vec3 c = Center();
        const glm::vec3 r = max - c;
        return glm::length(r);
    }
};

struct MeshGPU
{
    unsigned vao = 0;
    unsigned vbo = 0;
    unsigned ebo = 0;

    int  indexCount = 0;   
    int  vertexCount = 0;   
    AABB localBBox;         

    bool IsValid() const { return vao != 0 && indexCount > 0; }

    void ResetGPU()
    {
        vao = vbo = ebo = 0;
        indexCount = vertexCount = 0;
        localBBox.Reset();
    }
};

struct C_Mesh : Component
{
    MeshGPU gpu;

    bool showNormals = false;
    bool perFace = false;
    bool wireframe = false; 

    int  materialIndex = -1;
    bool hasUVs = false;
    bool hasNormals = true;

    C_Mesh() : Component(MESH) {}

    void OnEditor() override
    {
    }
};

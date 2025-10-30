#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

namespace DebugDraw
{
    struct Vertex
    {
        glm::vec3 pos;
        glm::vec3 color;
    };

    static std::vector<Vertex> gLines;

    inline void AddLine(const glm::vec3& a, const glm::vec3& b, const glm::vec3& color)
    {
        gLines.push_back({ a, color });
        gLines.push_back({ b, color });
    }

    inline void AddAABB(const glm::vec3& min, const glm::vec3& max, const glm::vec3& color)
    {
        glm::vec3 v[8] = {
            {min.x, min.y, min.z},
            {max.x, min.y, min.z},
            {max.x, max.y, min.z},
            {min.x, max.y, min.z},
            {min.x, min.y, max.z},
            {max.x, min.y, max.z},
            {max.x, max.y, max.z},
            {min.x, max.y, max.z}
        };

        int edges[12][2] = {
            {0,1},{1,2},{2,3},{3,0},
            {4,5},{5,6},{6,7},{7,4},
            {0,4},{1,5},{2,6},{3,7}
        };

        for (auto& e : edges)
            AddLine(v[e[0]], v[e[1]], color);
    }

    inline void AddAxis(const glm::mat4& transform, float length = 1.0f)
    {
        glm::vec3 origin = glm::vec3(transform[3]);
        glm::vec3 x = glm::normalize(glm::vec3(transform[0])) * length;
        glm::vec3 y = glm::normalize(glm::vec3(transform[1])) * length;
        glm::vec3 z = glm::normalize(glm::vec3(transform[2])) * length;

        AddLine(origin, origin + x, { 1,0,0 }); 
        AddLine(origin, origin + y, { 0,1,0 });
        AddLine(origin, origin + z, { 0,0,1 }); 
    }

    inline void DrawAll(const glm::mat4& viewProj)
    {
        if (gLines.empty()) return;

        glDisable(GL_DEPTH_TEST);
        glUseProgram(0);
        glMatrixMode(GL_PROJECTION);
        glLoadMatrixf(&viewProj[0][0]);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        glBegin(GL_LINES);
        for (const auto& v : gLines)
        {
            glColor3fv(&v.color.x);
            glVertex3fv(&v.pos.x);
        }
        glEnd();

        glEnable(GL_DEPTH_TEST);
        gLines.clear();
    }


    inline void Clear()
    {
        gLines.clear();
    }
}
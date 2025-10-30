#pragma once
#include "Component.h"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp> 
#include <glm/gtx/quaternion.hpp> 
#include <glm/gtc/matrix_transform.hpp> 

class GameObject;

struct C_Transform : Component
{
    glm::vec3 position{ 0.0f, 0.0f, 0.0f };
    glm::quat rotation{ 1.0f, 0.0f, 0.0f, 0.0f };
    glm::vec3 scale{ 1.0f, 1.0f, 1.0f };

    glm::mat4 local{ 1.0f };
    glm::mat4 global{ 1.0f };

    bool dirty = true;

    C_Transform() : Component(TRANSFORM) {}

    void SetPosition(const glm::vec3& p) { position = p; dirty = true; }
    void SetRotation(const glm::quat& q) { rotation = glm::normalize(q); dirty = true; }
    void SetEulerDegrees(const glm::vec3& eulerDeg)
    {
        glm::vec3 eulerRad = glm::radians(eulerDeg);
        rotation = glm::normalize(glm::quat(eulerRad));
        dirty = true;
    }
    void SetScale(const glm::vec3& s) { scale = s; dirty = true; }

    const glm::vec3& GetPosition() const { return position; }
    const glm::quat& GetRotation() const { return rotation; }
    const glm::vec3& GetScale()    const { return scale; }


    static glm::mat4 MakeTRS(const glm::vec3& pos, const glm::quat& rot, const glm::vec3& scl)
    {
        glm::mat4 T = glm::translate(glm::mat4(1.0f), pos);
        glm::mat4 R = glm::toMat4(rot);
        glm::mat4 S = glm::scale(glm::mat4(1.0f), scl);
        return T * R * S;
    }

    void Recalc()
    {
        local = MakeTRS(position, rotation, scale);

        glm::mat4 parentGlobal(1.0f);
        if (owner && owner->parent)
        {

            C_Transform* pT = owner->parent->Transform();
            if (pT)
            {
                if (pT->dirty) pT->Recalc();
                parentGlobal = pT->global;
            }
        }

        global = parentGlobal * local;
        dirty = false;
    }

    const glm::mat4& GetGlobalMatrix()
    {
        if (dirty) Recalc();
        return global;
    }

    glm::vec3 Forward() { return glm::normalize(glm::vec3(GetGlobalMatrix()[2]) * -1.0f); }
    glm::vec3 Right() { return glm::normalize(glm::vec3(GetGlobalMatrix()[0])); }
    glm::vec3 Up() { return glm::normalize(glm::vec3(GetGlobalMatrix()[1])); }

  
    void OnEditor() override
    {
    }
};

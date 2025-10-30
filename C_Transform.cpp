#include "C_Transform.h"
#include "GameObject.h"
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <imgui.h>

void C_Transform::Recalc()
{

    local = MakeTRS(position, rotation, scale);


    glm::mat4 parentGlobal(1.0f);
    if (owner && owner->parent)
    {
        if (auto* parentT = owner->parent->Transform())
        {
  
            if (parentT->dirty)
                parentT->Recalc();

            parentGlobal = parentT->global;
        }
    }

    global = parentGlobal * local;
    dirty = false;
}

glm::mat4 C_Transform::MakeTRS(const glm::vec3& pos, const glm::quat& rot, const glm::vec3& scl)
{
    glm::mat4 T = glm::translate(glm::mat4(1.0f), pos);
    glm::mat4 R = glm::toMat4(rot);
    glm::mat4 S = glm::scale(glm::mat4(1.0f), scl);
    return T * R * S;
}

const glm::mat4& C_Transform::GetGlobalMatrix()
{
    if (dirty)
        Recalc();
    return global;
}

glm::vec3 C_Transform::Forward()
{
    return glm::normalize(-glm::vec3(GetGlobalMatrix()[2]));
}

glm::vec3 C_Transform::Right()
{
    return glm::normalize(glm::vec3(GetGlobalMatrix()[0]));
}

glm::vec3 C_Transform::Up()
{
    return glm::normalize(glm::vec3(GetGlobalMatrix()[1]));
}

void C_Transform::OnEditor()
{
    bool changed = false;

    if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::Indent();


        changed |= ImGui::DragFloat3("Position", &position.x, 0.1f);


        glm::vec3 eulerDeg = glm::degrees(glm::eulerAngles(rotation));
        if (ImGui::DragFloat3("Rotation", &eulerDeg.x, 0.5f))
        {
            rotation = glm::quat(glm::radians(eulerDeg));
            changed = true;
        }


        changed |= ImGui::DragFloat3("Scale", &scale.x, 0.01f, 0.001f, 100.0f);

        if (changed)
            dirty = true;

        ImGui::Unindent();
    }
}
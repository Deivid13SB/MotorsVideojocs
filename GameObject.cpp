#include "GameObject.h"
#include "C_Transform.h"
#include "C_Mesh.h"
#include "C_Texture.h"
#include <imgui.h>
#include <algorithm>

C_Transform* GameObject::Transform() const
{
    for (const auto& c : components)
    {
        if (auto* t = dynamic_cast<C_Transform*>(c.get()))
            return t;
    }
    return nullptr;
}

void GameObject::OnEditor()
{
    if (ImGui::CollapsingHeader(name.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::Indent();
        ImGui::Checkbox("Active", &active);


        for (auto& c : components)
        {
            c->OnEditor();
        }

    
        if (ImGui::Button("Add Component"))
            ImGui::OpenPopup("AddComp");

        if (ImGui::BeginPopup("AddComp"))
        {
            if (ImGui::MenuItem("Transform"))
            {
                if (!Get<C_Transform>())
                    Add<C_Transform>();
            }
            if (ImGui::MenuItem("Mesh"))
            {
                if (!Get<C_Mesh>())
                    Add<C_Mesh>();
            }
            if (ImGui::MenuItem("Texture"))
            {
                if (!Get<C_Texture>())
                    Add<C_Texture>();
            }
            ImGui::EndPopup();
        }

        ImGui::Unindent();
    }
}

GameObject* GameObject::FindByName(const std::string& target)
{
    if (name == target)
        return this;

    for (auto& c : children)
    {
        if (auto* found = c->FindByName(target))
            return found;
    }
    return nullptr;
}

void GameObject::SetActive(bool value)
{
    active = value;
    for (auto& c : components)
        c->active = value;
    for (auto& ch : children)
        ch->SetActive(value);
}

void GameObject::ForEachRecursive(const std::function<void(GameObject&)>& fn)
{
    fn(*this);
    for (auto& ch : children)
        ch->ForEachRecursive(fn);
}
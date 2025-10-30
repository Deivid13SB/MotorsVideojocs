#include "C_Texture.h"
#include <imgui.h>
#include <glad/glad.h>          
#include <sstream>


static void ClearTex(Tex2D& t)
{
    t.id = 0;
    t.w = 0;
    t.h = 0;
    t.path.clear();
}

void C_Texture::OnEditor()
{

    ImGui::Checkbox("Use Checker", &useChecker);

    if (tex.IsValid())
    {
        ImGui::Text("Path: %s", tex.path.c_str());
        ImGui::Text("Size: %d x %d", tex.w, tex.h);
    }
    else
    {
        ImGui::TextDisabled("No texture assigned.");
    }


    ImGui::SeparatorText("Sampling");
    ImGui::Checkbox("Linear Filtering", &linear);
    ImGui::SameLine();
    ImGui::Checkbox("Mipmaps", &mipmaps);

    ImGui::SeparatorText("UV");
    ImGui::DragFloat2("Tiling", &uvTiling.x, 0.01f, 0.01f, 100.0f, "%.2f");
    ImGui::DragFloat2("Offset", &uvOffset.x, 0.01f, -1000.0f, 1000.0f, "%.2f");


    if (!useChecker && tex.IsValid())
    {
        ImGui::SeparatorText("Preview");
 
        ImTextureID img = (ImTextureID)(intptr_t)tex.id;


        const float previewW = 128.0f;
        float ratio = (tex.h > 0) ? (previewW * (float)tex.h / (float)tex.w) : previewW;
        ImGui::Image(img, ImVec2(previewW, ratio));
    }

    if (ImGui::Button("Clear Texture"))
    {
        ClearTex(tex);
    }
}
#include "C_Mesh.h"
#include "C_Transform.h"
#include "GameObject.h"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

static void DrawNormals(const MeshGPU& mesh, bool perFace)
{
    if (!mesh.IsValid()) return;

    glBindVertexArray(mesh.vao);

    glPushAttrib(GL_CURRENT_BIT | GL_ENABLE_BIT);
    glDisable(GL_LIGHTING);

    glColor3f(1.0f, 0.0f, 0.0f);
    glLineWidth(1.0f);

    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
    const size_t stride = sizeof(float) * 8; 

    const float* data = (const float*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_ONLY);
    if (!data)
    {
        glBindVertexArray(0);
        return;
    }

    if (perFace)
    {
        glBegin(GL_LINES);
        for (int i = 0; i < mesh.indexCount; i += 3)
        {
            const float* v0 = data + (mesh.indexCount > i ? mesh.indexCount : 0);
            glm::vec3 p0(v0[0], v0[1], v0[2]);
            glm::vec3 n0(v0[3], v0[4], v0[5]);

            const float* v1 = v0 + stride / sizeof(float);
            glm::vec3 p1(v1[0], v1[1], v1[2]);
            glm::vec3 n1(v1[3], v1[4], v1[5]);

            const float* v2 = v1 + stride / sizeof(float);
            glm::vec3 p2(v2[0], v2[1], v2[2]);
            glm::vec3 n2(v2[3], v2[4], v2[5]);

            glm::vec3 center = (p0 + p1 + p2) / 3.0f;
            glm::vec3 normal = glm::normalize((n0 + n1 + n2) / 3.0f);

            glVertex3fv(glm::value_ptr(center));
            glVertex3fv(glm::value_ptr(center + normal * 0.2f));
        }
        glEnd();
    }
    else
    {
        glBegin(GL_LINES);
        for (int i = 0; i < mesh.vertexCount; ++i)
        {
            const float* v = data + i * 8;
            glm::vec3 pos(v[0], v[1], v[2]);
            glm::vec3 nrm(v[3], v[4], v[5]);
            glVertex3fv(glm::value_ptr(pos));
            glVertex3fv(glm::value_ptr(pos + nrm * 0.2f));
        }
        glEnd();
    }

    glUnmapBuffer(GL_ARRAY_BUFFER);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glPopAttrib();
}

void C_Mesh::OnEditor()
{
    // Si usas ImGui, aquí se dibujan los controles del componente.
    // Por ejemplo (pseudo-código):
    //
    // ImGui::Text("Mesh Info:");
    // ImGui::Text("Vertices: %d", gpu.vertexCount);
    // ImGui::Text("Indices: %d", gpu.indexCount);
    // ImGui::Checkbox("Show Normals", &showNormals);
    // ImGui::Checkbox("Per Face", &perFace);
    // ImGui::Checkbox("Wireframe", &wireframe);
    // ImGui::Separator();
    // ImGui::Text("AABB Min: (%.2f, %.2f, %.2f)", gpu.localBBox.min.x, gpu.localBBox.min.y, gpu.localBBox.min.z);
    // ImGui::Text("AABB Max: (%.2f, %.2f, %.2f)", gpu.localBBox.max.x, gpu.localBBox.max.y, gpu.localBBox.max.z);
}

void C_Mesh::DrawDebugNormals()
{
    if (showNormals)
    {
        DrawNormals(gpu, perFace);
    }
}
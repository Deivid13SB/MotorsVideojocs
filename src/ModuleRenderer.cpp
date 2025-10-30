#include "ModuleRenderer.h"
#include "App.h"
#include "ModuleWindow.h"
#include "ModuleCamera.h"
#include "ModuleScene.h"
#include "ecs/GameObject.h"
#include "ecs/C_Mesh.h"
#include "ecs/C_Texture.h"
#include "ecs/C_Transform.h"
#include "Log.h"
#include "rendering/gl_utils.h"
#include "rendering/DebugDraw.h"
#include <SDL.h>
#include <glm/gtc/type_ptr.hpp>
#include <filesystem>
namespace fs = std::filesystem;

ModuleRenderer::ModuleRenderer(App* app)
    : Module(app, "Renderer")
{
}

bool ModuleRenderer::Init()
{
    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
    {
        LOG_ERROR("[Renderer] Failed to initialize GLAD.");
        return false;
    }

    LOG_INFO("[Renderer] GL initialized. Vendor: %s | Renderer: %s | Version: %s",
        (const char*)glGetString(GL_VENDOR),
        (const char*)glGetString(GL_RENDERER),
        (const char*)glGetString(GL_VERSION));

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    if (!CreateDefaultProgram()) return false;

    SetViewportToWindow();
    return true;
}

bool ModuleRenderer::Start()
{
    return true;
}

bool ModuleRenderer::PreUpdate(float)
{
    SetViewportToWindow();
    glClearColor(clearColor[0], clearColor[1], clearColor[2], clearColor[3]);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    return true;
}

bool ModuleRenderer::Update(float)
{
    DrawScene();
    return true;
}

bool ModuleRenderer::PostUpdate(float)
{

    if (app->camera)
    {
        Mat4 vp = app->camera->GetViewProjMatrix();
        DebugDraw::DrawAll(vp);
    }

    SDL_GL_SwapWindow(app->window->window);
    return true;
}

bool ModuleRenderer::CleanUp()
{
    DestroyProgram();
    LOG_INFO("[Renderer] CleanUp.");
    return true;
}

void ModuleRenderer::SetViewportToWindow()
{
    int w = 0, h = 0;
    SDL_GetWindowSize(app->window->window, &w, &h);
    if (w != vpW || h != vpH)
    {
        vpW = w; vpH = h;
        glViewport(0, 0, vpW, vpH);
        if (app->camera)
        {
            app->camera->aspectRatio = (vpH > 0) ? (float)vpW / (float)vpH : 1.0f;
       
        }
    }
}

bool ModuleRenderer::CreateDefaultProgram()
{
  
    std::string vsSrc = LoadShaderFile("Assets/shaders/basic.vert");
    std::string fsSrc = LoadShaderFile("Assets/shaders/basic.frag");

    if (vsSrc.empty())
    {
        vsSrc = R"(#version 460 core
layout (location=0) in vec3 aPos;
layout (location=1) in vec3 aNormal;
layout (location=2) in vec2 aUV;

uniform mat4 u_MVP;
uniform mat4 u_Model;

out vec3 vNormal;
out vec2 vUV;

void main(){
    gl_Position = u_MVP * vec4(aPos,1.0);
    vNormal = mat3(u_Model) * aNormal;
    vUV = aUV;
}
)";
    }

    if (fsSrc.empty())
    {
        fsSrc = R"(#version 460 core
in vec3 vNormal;
in vec2 vUV;

uniform sampler2D u_Tex0;
uniform int u_UseTex;

out vec4 FragColor;

void main(){
    vec3 N = normalize(vNormal);
    float ndotl = clamp(dot(N, normalize(vec3(0.3,0.7,0.6))), 0.1, 1.0);
    vec3 base = (u_UseTex == 1) ? texture(u_Tex0, vUV).rgb : vec3(0.7,0.75,0.8);
    FragColor = vec4(base * ndotl, 1.0);
}
)";
    }

    GLuint vs = CompileShader(GL_VERTEX_SHADER, vsSrc);
    GLuint fs = CompileShader(GL_FRAGMENT_SHADER, fsSrc);
    if (!vs || !fs) return false;

    program = LinkProgram(vs, fs);
    if (!program) return false;

    u_MVP = glGetUniformLocation(program, "u_MVP");
    u_Model = glGetUniformLocation(program, "u_Model");
    u_UseTex = glGetUniformLocation(program, "u_UseTex");
    u_Tex0 = glGetUniformLocation(program, "u_Tex0");
    return true;
}

void ModuleRenderer::DestroyProgram()
{
    if (program)
    {
        glDeleteProgram(program);
        program = 0;
    }
}

void ModuleRenderer::DrawScene()
{
    if (!app->scene || !app->scene->root || !app->camera || program == 0)
        return;

    const Mat4 view = app->camera->GetViewMatrix();
    const Mat4 proj = app->camera->GetProjectionMatrix();

 
    app->scene->root->ForEach([&](GameObject& go) {
        DrawGameObject(go, view, proj);
        });
}

void ModuleRenderer::DrawGameObject(GameObject& go, const Mat4& view, const Mat4& proj)
{
    if (auto* mesh = go.Get<C_Mesh>())
    {
        C_Texture* tex = go.Get<C_Texture>();
        C_Transform* tr = go.Get<C_Transform>();
        Mat4 model = tr ? tr->GetGlobalMatrix() : Mat4(1.0f);
        DrawMesh(*mesh, tex, model, view, proj);
    }

 
}

void ModuleRenderer::DrawMesh(C_Mesh& mesh, C_Texture* tex, const Mat4& model, const Mat4& view, const Mat4& proj)
{
    if (!mesh.gpu.IsValid()) return;

    glUseProgram(program);


    Mat4 mvp = proj * view * model;
    glUniformMatrix4fv(u_MVP, 1, GL_FALSE, glm::value_ptr(mvp));
    glUniformMatrix4fv(u_Model, 1, GL_FALSE, glm::value_ptr(model));


    int useTex = 0;
    if (tex && tex->tex.IsValid() && !tex->useChecker)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, tex->tex.id);
        if (u_Tex0 >= 0)   glUniform1i(u_Tex0, 0);
        useTex = 1;
    }
    if (u_UseTex >= 0) glUniform1i(u_UseTex, useTex);


    GLboolean wire = mesh.wireframe ? GL_TRUE : GL_FALSE;
    if (wire) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


    glBindVertexArray(mesh.gpu.vao);
    glDrawElements(GL_TRIANGLES, mesh.gpu.indexCount, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);

    if (wire) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


    mesh.DrawDebugNormals();

    
    if (useTex) glBindTexture(GL_TEXTURE_2D, 0);
    glUseProgram(0);
}

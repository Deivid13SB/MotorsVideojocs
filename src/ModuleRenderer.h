#pragma once

#include "Module.h"
#include "Math.h"
#include <glad/glad.h>
#include <string>


class ModuleWindow;
class ModuleCamera;
class ModuleScene;
class GameObject;
struct C_Mesh;
struct C_Texture;
struct C_Transform;

class ModuleRenderer : public Module
{
public:
    explicit ModuleRenderer(App* app);
    ~ModuleRenderer() override = default;

    bool Init() override;
    bool Start() override;
    bool PreUpdate(float dt) override;
    bool Update(float dt) override;
    bool PostUpdate(float dt) override;
    bool CleanUp() override;

  
    float clearColor[4] = { 0.12f, 0.12f, 0.12f, 1.0f };

private:
   
    bool CreateDefaultProgram();
    void DestroyProgram();
    void SetViewportToWindow();

 
    void DrawScene();
    void DrawGameObject(GameObject& go, const Mat4& view, const Mat4& proj);
    void DrawMesh(C_Mesh& mesh, C_Texture* tex, const Mat4& model, const Mat4& view, const Mat4& proj);

private:
  
    GLuint program = 0;
    GLint  u_MVP = -1;
    GLint  u_Model = -1;
    GLint  u_UseTex = -1;
    GLint  u_Tex0 = -1;

    int vpW = 0, vpH = 0;
};


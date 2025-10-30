#pragma once

#include "Module.h"
#include "Math.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

class ModuleCamera : public Module
{
public:
    explicit ModuleCamera(App* app);
    ~ModuleCamera() override = default;

    bool Start() override;
    bool Update(float dt) override;
    bool CleanUp() override;

    const Mat4& GetViewMatrix() const { return view; }
    const Mat4& GetProjectionMatrix() const { return proj; }
    Mat4 GetViewProjMatrix() const { return proj * view; }

    void FocusSelected();

private:
    void RecalculateViewMatrix();
    void RecalculateProjection();

public:
    Vec3 position{ 0.0f, 3.0f, 8.0f };
    Vec3 pivot{ 0.0f, 0.0f, 0.0f };
    float yaw = -90.0f;
    float pitch = -15.0f;

    float fov = 60.0f;      
    float aspectRatio = 16.0f / 9.0f;
    float nearPlane = 0.1f;
    float farPlane = 1000.0f;


    float moveSpeed = 10.0f;
    float mouseSens = 0.15f;
    float orbitSens = 0.3f;
    float zoomStep = 2.0f;

private:
    Mat4 view{ 1.0f };
    Mat4 proj{ 1.0f };
};


#include "ModuleCamera.h"
#include "App.h"
#include "ModuleInput.h"
#include "ModuleScene.h"
#include "GameObject.h"
#include "C_Transform.h"
#include "Log.h"
#include <SDL.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>


ModuleCamera::ModuleCamera(App* app)
    : Module(app, "Camera")
{
}

bool ModuleCamera::Start()
{
    RecalculateProjection();
    RecalculateViewMatrix();
    LOG_INFO("[Camera] Initialized at position (%.2f, %.2f, %.2f)",
        position.x, position.y, position.z);
    return true;
}

bool ModuleCamera::Update(float dt)
{
    if (!app->input)
        return true;


    bool W = app->input->keyW;
    bool A = app->input->keyA;
    bool S = app->input->keyS;
    bool D = app->input->keyD;
    bool Q = app->input->keyQ;
    bool E = app->input->keyE;
    bool F = app->input->keyF;
    bool Shift = app->input->keyShift;
    bool RMB = app->input->rmb;
    bool LMB = app->input->lmb;
    bool ALT = app->input->alt;

    float velocity = moveSpeed * (Shift ? 2.0f : 1.0f) * dt;

  
    Vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(front);
    Vec3 right = glm::normalize(glm::cross(front, Vec3(0, 1, 0)));
    Vec3 up = glm::normalize(glm::cross(right, front));


    if (RMB)
    {
        if (W) position += front * velocity;
        if (S) position -= front * velocity;
        if (A) position -= right * velocity;
        if (D) position += right * velocity;
        if (Q) position -= up * velocity;
        if (E) position += up * velocity;

        yaw -= app->input->mouse_dx * mouseSens;
        pitch -= app->input->mouse_dy * mouseSens;

        pitch = glm::clamp(pitch, -89.0f, 89.0f);
    }

    if (ALT && LMB)
    {
        yaw -= app->input->mouse_dx * orbitSens;
        pitch -= app->input->mouse_dy * orbitSens;
        pitch = glm::clamp(pitch, -89.0f, 89.0f);

        Vec3 dir;
        dir.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        dir.y = sin(glm::radians(pitch));
        dir.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

        float distance = glm::length(position - pivot);
        position = pivot - glm::normalize(dir) * distance;
    }

 
    if (app->input->wheel != 0)
    {
        Vec3 forward = glm::normalize(front);
        position += forward * (app->input->wheel * zoomStep);
    }

    if (F)
    {
        FocusSelected();
    }


    RecalculateViewMatrix();
    return true;
}

bool ModuleCamera::CleanUp()
{
    LOG_INFO("[Camera] CleanUp.");
    return true;
}

void ModuleCamera::FocusSelected()
{
    if (!app->scene || !app->scene->selected)
    {
        LOG_WARN("[Camera] No selected GameObject to focus.");
        return;
    }

    auto* tr = app->scene->selected->Get<C_Transform>();
    if (!tr)
    {
        LOG_WARN("[Camera] Selected GameObject has no Transform.");
        return;
    }

    Vec3 center = Vec3(tr->GetGlobalMatrix()[3]); 
    float distance = 5.0f;

    position = center + Vec3(0, 2, distance);
    pivot = center;

    yaw = -90.0f;
    pitch = -15.0f;

    RecalculateViewMatrix();

}

void ModuleCamera::RecalculateViewMatrix()
{
    Vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    Vec3 front = glm::normalize(direction);

    view = glm::lookAt(position, position + front, Vec3(0, 1, 0));
}


void ModuleCamera::RecalculateProjection()
{
    proj = glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
}

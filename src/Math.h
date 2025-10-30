#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp>         
#include <glm/gtx/quaternion.hpp>       
#include <glm/gtc/constants.hpp>      

using Vec2 = glm::vec2;
using Vec3 = glm::vec3;
using Vec4 = glm::vec4;
using Quat = glm::quat;
using Mat3 = glm::mat3;
using Mat4 = glm::mat4;

namespace Math
{
    constexpr float PI = glm::pi<float>();

    inline float DegToRad(float deg) { return glm::radians(deg); }
    inline float RadToDeg(float rad) { return glm::degrees(rad); }

    inline Vec3 Lerp(const Vec3& a, const Vec3& b, float t)
    {
        return a + t * (b - a);
    }

    inline float Clamp(float v, float minV, float maxV)
    {
        return glm::clamp(v, minV, maxV);
    }

    inline float Length(const Vec3& v)
    {
        return glm::length(v);
    }

    inline Vec3 Normalize(const Vec3& v)
    {
        return glm::normalize(v);
    }

    inline float Dot(const Vec3& a, const Vec3& b)
    {
        return glm::dot(a, b);
    }

    inline Vec3 Cross(const Vec3& a, const Vec3& b)
    {
        return glm::cross(a, b);
    }

    inline Mat4 Identity()
    {
        return glm::mat4(1.0f);
    }

    inline Mat4 Translate(const Vec3& v)
    {
        return glm::translate(Mat4(1.0f), v);
    }

    inline Mat4 Scale(const Vec3& s)
    {
        return glm::scale(Mat4(1.0f), s);
    }

    inline Mat4 Rotate(const Quat& q)
    {
        return glm::toMat4(q);
    }

    inline Quat EulerToQuat(const Vec3& eulerDegrees)
    {
        return glm::quat(glm::radians(eulerDegrees));
    }

    inline Vec3 QuatToEuler(const Quat& q)
    {
        return glm::degrees(glm::eulerAngles(q));
    }
}
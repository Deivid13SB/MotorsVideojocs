#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glad/glad.h>
#include "Log.h"

inline void GLClearErrors()
{
    while (glGetError() != GL_NO_ERROR);
}

inline bool GLCheckErrors(const char* context)
{
    GLenum error;
    bool ok = true;
    while ((error = glGetError()) != GL_NO_ERROR)
    {
        const char* errStr = "";
        switch (error)
        {
        case GL_INVALID_ENUM:                  errStr = "GL_INVALID_ENUM"; break;
        case GL_INVALID_VALUE:                 errStr = "GL_INVALID_VALUE"; break;
        case GL_INVALID_OPERATION:             errStr = "GL_INVALID_OPERATION"; break;
        case GL_STACK_OVERFLOW:                errStr = "GL_STACK_OVERFLOW"; break;
        case GL_STACK_UNDERFLOW:               errStr = "GL_STACK_UNDERFLOW"; break;
        case GL_OUT_OF_MEMORY:                 errStr = "GL_OUT_OF_MEMORY"; break;
        case GL_INVALID_FRAMEBUFFER_OPERATION: errStr = "GL_INVALID_FRAMEBUFFER_OPERATION"; break;
        default:                               errStr = "Unknown error"; break;
        }
        LOG_ERROR("[OpenGL Error] (%s) at %s", errStr, context);
        ok = false;
    }
    return ok;
}

inline std::string LoadShaderFile(const std::string& path)
{
    std::ifstream file(path);
    if (!file.is_open())
    {
        LOG_ERROR("Cannot open shader file: %s", path.c_str());
        return "";
    }

    std::stringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

inline GLuint CompileShader(GLenum type, const std::string& source)
{
    if (source.empty())
    {
        LOG_ERROR("Empty shader source!");
        return 0;
    }

    GLuint shader = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    GLint success = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char infoLog[1024];
        glGetShaderInfoLog(shader, sizeof(infoLog), nullptr, infoLog);
        const char* t = (type == GL_VERTEX_SHADER) ? "VERTEX" :
            (type == GL_FRAGMENT_SHADER) ? "FRAGMENT" :
            (type == GL_GEOMETRY_SHADER) ? "GEOMETRY" : "UNKNOWN";
        LOG_ERROR("Shader compile error (%s): %s", t, infoLog);
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

inline GLuint LinkProgram(GLuint vertexShader, GLuint fragmentShader)
{
    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    GLint success = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success)
    {
        char infoLog[1024];
        glGetProgramInfoLog(program, sizeof(infoLog), nullptr, infoLog);
        LOG_ERROR("Shader linking error: %s", infoLog);
        glDeleteProgram(program);
        return 0;
    }

    glDetachShader(program, vertexShader);
    glDetachShader(program, fragmentShader);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
}


inline GLint GetUniform(GLuint program, const char* name)
{
    GLint loc = glGetUniformLocation(program, name);
    if (loc == -1)
        LOG_WARN("Uniform not found: %s", name);
    return loc;
}

inline void SetUniform(GLuint program, const char* name, int value)
{
    glUniform1i(GetUniform(program, name), value);
}

inline void SetUniform(GLuint program, const char* name, float value)
{
    glUniform1f(GetUniform(program, name), value);
}

inline void SetUniformMat4(GLuint program, const char* name, const float* data)
{
    glUniformMatrix4fv(GetUniform(program, name), 1, GL_FALSE, data);
}
#pragma once

#include <string>
#include <unordered_map>
#include <filesystem>
#include <glad/glad.h>
#include "Log.h"


class Shader
{
public:
    Shader() = default;
    ~Shader();

    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;

   
    bool LoadFromFiles(const std::string& vertPath, const std::string& fragPath);


    bool LoadFromSource(const std::string& vertSrc, const std::string& fragSrc);

    
    bool ReloadIfChanged();

  
    void Use() const { glUseProgram(program); }
    GLuint Id() const { return program; }

   
    GLint GetUniformLocation(const char* name);

    void SetInt(const char* name, int v);
    void SetFloat(const char* name, float v);
    void SetVec2(const char* name, const float* v); 
    void SetVec3(const char* name, const float* v); 
    void SetVec4(const char* name, const float* v);
    void SetMat4(const char* name, const float* m4, bool transpose = false);

private:

    bool Build(const std::string& vertSrc, const std::string& fragSrc);


    void Destroy();

private:
    GLuint program = 0;

 
    std::unordered_map<std::string, GLint> uniformCache;


    std::string vsPath, fsPath;
    std::filesystem::file_time_type vsTime{}, fsTime{};
};


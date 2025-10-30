#include "rendering/Shader.h"
#include "rendering/gl_utils.h"   
#include <utility>
#include <fstream>
#include <sstream>

Shader::~Shader()
{
    Destroy();
}

bool Shader::LoadFromFiles(const std::string& vertPath, const std::string& fragPath)
{
 
    std::string vs = LoadShaderFile(vertPath);
    std::string fs = LoadShaderFile(fragPath);
    if (vs.empty() || fs.empty())
    {
        LOG_ERROR("[Shader] Empty source when loading '%s' / '%s'", vertPath.c_str(), fragPath.c_str());
        return false;
    }

    if (!Build(vs, fs))
        return false;

  
    vsPath = vertPath;
    fsPath = fragPath;

    try {
        vsTime = std::filesystem::last_write_time(vsPath);
        fsTime = std::filesystem::last_write_time(fsPath);
    }
    catch (...) {
        
        vsTime = fsTime = std::filesystem::file_time_type::min();
    }

    LOG_INFO("[Shader] Loaded from files: VS='%s'  FS='%s'", vsPath.c_str(), fsPath.c_str());
    return true;
}

bool Shader::LoadFromSource(const std::string& vertSrc, const std::string& fragSrc)
{
    vsPath.clear(); fsPath.clear();
    vsTime = fsTime = std::filesystem::file_time_type::min();
    return Build(vertSrc, fragSrc);
}

bool Shader::Build(const std::string& vertSrc, const std::string& fragSrc)
{
    GLuint vs = CompileShader(GL_VERTEX_SHADER, vertSrc);
    GLuint fs = CompileShader(GL_FRAGMENT_SHADER, fragSrc);
    if (!vs || !fs)
    {
        if (vs) glDeleteShader(vs);
        if (fs) glDeleteShader(fs);
        return false;
    }

    GLuint prog = LinkProgram(vs, fs);
    if (!prog)
        return false;

    
    Destroy();
    program = prog;
    uniformCache.clear();
    return true;
}

void Shader::Destroy()
{
    if (program)
    {
        glDeleteProgram(program);
        program = 0;
    }
    uniformCache.clear();
}

bool Shader::ReloadIfChanged()
{
    if (vsPath.empty() || fsPath.empty())
        return false; 

 
    std::filesystem::file_time_type nVS{}, nFS{};
    try {
        nVS = std::filesystem::last_write_time(vsPath);
        nFS = std::filesystem::last_write_time(fsPath);
    }
    catch (...) {
        return false;
    }

    if (nVS == vsTime && nFS == fsTime)
        return false; 


    std::string vs = LoadShaderFile(vsPath);
    std::string fs = LoadShaderFile(fsPath);
    if (vs.empty() || fs.empty())
        return false;

    if (!Build(vs, fs))
        return false;

    vsTime = nVS;
    fsTime = nFS;

    LOG_INFO("[Shader] Reloaded: VS='%s'  FS='%s'", vsPath.c_str(), fsPath.c_str());
    return true;
}

GLint Shader::GetUniformLocation(const char* name)
{
    auto it = uniformCache.find(name);
    if (it != uniformCache.end())
        return it->second;

    GLint loc = glGetUniformLocation(program, name);
    if (loc == -1)
        LOG_WARN("[Shader] Uniform not found: %s", name);

    uniformCache.emplace(name, loc);
    return loc;
}

void Shader::SetInt(const char* name, int v)
{
    glUniform1i(GetUniformLocation(name), v);
}

void Shader::SetFloat(const char* name, float v)
{
    glUniform1f(GetUniformLocation(name), v);
}

void Shader::SetVec2(const char* name, const float* v)
{
    glUniform2fv(GetUniformLocation(name), 1, v);
}

void Shader::SetVec3(const char* name, const float* v)
{
    glUniform3fv(GetUniformLocation(name), 1, v);
}

void Shader::SetVec4(const char* name, const float* v)
{
    glUniform4fv(GetUniformLocation(name), 1, v);
}

void Shader::SetMat4(const char* name, const float* m4, bool transpose)
{
    glUniformMatrix4fv(GetUniformLocation(name), 1, transpose ? GL_TRUE : GL_FALSE, m4);
}

//
// Created by Danmeng on 2026/3/24.
//

#ifndef SIMPLEVOXEL_SHADER_H
#define SIMPLEVOXEL_SHADER_H
#pragma once
#include <unordered_map>
#include "Utils.h"
#include "glm/glm.hpp"

struct ShaderProgramSource
{
    std::string VertexSource;
    std::string FragmentSource;
};

enum class ShaderType
{
    NONE = -1,
    VERTEX = 0,
    FRAGMENT = 1
};

class Shader
{
private:
    unsigned int m_programID;
    std::string m_fileName;
    mutable std::unordered_map<std::string, int> m_uniformLocationCache;

public:
    Shader(const std::string& fileName);
    ~Shader();
    void bind() const;
    void unbind() const;
    void setUniform1f(const std::string& name, float v0);
    void setUniform2f(const std::string& name, float v0, float v1);
    void setUniform3f(const std::string& name, float v0, float v1, float v2);
    void setUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
    void setUniformMat3f(const std::string& name, const glm::mat3& matrix);
    void setUniformMat4f(const std::string& name, const glm::mat4& matrix);
    void setUniform1i(const std::string& name, int v0);

private:
    ShaderProgramSource parseShader(const std::string& fileName);
    unsigned int compileShader(unsigned int shaderType, const std::string& source);
    unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader );
    GLint getUniformLocation(const std::string& name) const;

};


#endif //SIMPLEVOXEL_SHADER_H
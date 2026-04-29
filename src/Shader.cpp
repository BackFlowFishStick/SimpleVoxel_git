//
// Created by Danmeng on 2026/3/24.
//

#include "Shader.h"
#include "glm/gtc/type_ptr.inl"
#include <fstream>
#include <sstream>

Shader::Shader(const std::string& fileName) :
m_fileName(fileName),
m_programID(0)
{
    auto shaderSource = parseShader(fileName);
    m_programID = createShader(shaderSource.VertexSource, shaderSource.FragmentSource);
}

Shader::~Shader()
{
    GLCall(glDeleteProgram(m_programID));
}

void Shader::bind() const
{
    GLCall(glUseProgram(m_programID));
}

void Shader::unbind() const
{
    GLCall(glUseProgram(0));
}

void Shader::setUniform1f(const std::string& name, float v0)
{
    GLCall(glUniform1f(getUniformLocation(name), v0));
}

void Shader::setUniform2f(const std::string& name, float v0, float v1)
{
    GLCall(glUniform2f(getUniformLocation(name), v0, v1));
}

void Shader::setUniform3f(const std::string& name, float v0, float v1, float v2)
{
    GLCall(glUniform3f(getUniformLocation(name), v0, v1, v2));
}

void Shader::setUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    GLCall(glUniform4f(getUniformLocation(name), v0, v1, v2, v3));
}

void Shader::setUniformMat3f(const std::string& name, const glm::mat3& matrix)
{
    GLCall(glUniformMatrix3fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(matrix)));
}

void Shader::setUniformMat4f(const std::string& name, const glm::mat4& matrix)
{
    GLCall(glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(matrix)));
}

void Shader::setUniform1i(const std::string& name, int v0)
{
    GLCall(glUniform1i(getUniformLocation(name), v0));
}

ShaderProgramSource Shader::parseShader(const std::string& fileName)
{
    std::ifstream stream(fileName);

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    while (getline(stream, line))
    {
        // Find shader type tag. If the result is not "not found".
        if (line.find("#shader") != std::string::npos)
        {
            // Set mode to vertex
            if (line.find("vertex") != std::string::npos)
            {
                type = ShaderType::VERTEX;
            }
            // Set mode to fragment
            else if (line.find("fragment") != std::string::npos)
            {
                type = ShaderType::FRAGMENT;
            }
        }
        else
        {
            ss[static_cast<int>(type)] << line << '\n';
        }
    }

    return { ss[0].str(), ss[1].str() };
}

unsigned int Shader::compileShader(unsigned int shaderType, const std::string& source)
{
    unsigned int id = glCreateShader(shaderType);
    const char* src = source.c_str(); // c_str() gives pointer to internal string data which equals to source[0]
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    // Error handling
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (!result) // == GL_FALSE
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char)); // allocate on stack
        glGetShaderInfoLog(id, length, &length, message);
        std::cerr << "Failed to compiled "
            << (shaderType == GL_VERTEX_SHADER ? "vertext" : "fragment")
            << " shader!" << std::endl;

        std::cerr << message << std::endl;

        glDeleteShader(id);

        return 0;
    }

    return id;
}

unsigned int Shader::createShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    GLCall(GLuint program = glCreateProgram());
    unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

    GLCall(glAttachShader(program, vs));
    GLCall(glAttachShader(program, fs));
    GLCall(glLinkProgram(program));
    GLCall(glValidateProgram(program));

    GLCall(glDeleteShader(vs));
    GLCall(glDeleteShader(fs));

    return program;
}

GLint Shader::getUniformLocation(const std::string& name) const
{
    if (m_uniformLocationCache.find(name) != m_uniformLocationCache.end())
    {
        return m_uniformLocationCache[name];
    }

    auto location = glGetUniformLocation(m_programID, name.c_str());
    if (location == -1)
    {
        std::cerr << "Uniform " << name << " not found!" << std::endl;
        return -1;
    }

    m_uniformLocationCache[name] = location;

    return location;
}
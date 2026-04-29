//
// Created by Danmeng on 2026/3/24.
//

#include "Utils.h"

void Utils::glClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

bool Utils::glLogCall(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError())
    {
        std::cout << "[OperGL Error] (" << error << "): " << function
            << " " << ": " << line << std::endl;
        return false;
    }
    return true;
}

void Utils::checkShaderCompileStatus(GLuint shader)
{
    GLint len = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
    if (len > 0)
    {
        auto infoLog = new char[len];
        int chWritten = 0;
        glGetShaderInfoLog(shader, len, &chWritten, infoLog);
        std::cerr << "Shader compilation failed(" << chWritten << "): " << infoLog << std::endl;
        delete[] infoLog;
    }
}

void Utils::checkProgramLinkStatus(GLuint program)
{
    GLint len = 0;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);
    if (len > 0)
    {
        auto infoLog = static_cast<char*>(malloc(len));
        int chWritten = 0;
        glGetProgramInfoLog(program, len, &chWritten, infoLog);
        std::cerr << "Program linking failed(" << chWritten << "): " << infoLog << std::endl;
        free(infoLog);
    }
}

glm::vec2 Utils::worldToScreen(const glm::vec3& worldPos, const glm::mat4& projMat, const glm::mat4& viewMat,float screenWidth, float screenHeight)
{
    auto modelMat = glm::mat4(1.0f);

    auto mvp = projMat * viewMat * modelMat;

    auto clipPos = mvp * glm::vec4(worldPos, 1.0f);

    if (clipPos.w <= 0.0f)
    {
        return glm::vec2(-1.f, -1.f);
    }

    auto ndcPos = glm::vec3(clipPos) / clipPos.w;

    float screenX = (ndcPos.x + 1.0f) * 0.5 * screenWidth;
    float screenY = (1.0f - ndcPos.y) * 0.5 * screenHeight;

    return glm::vec2(screenX, screenY);
}


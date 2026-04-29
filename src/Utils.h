//
// Created by Danmeng on 2026/3/24.
//

#ifndef SIMPLEVOXEL_UTILS_H
#define SIMPLEVOXEL_UTILS_H

#pragma once
#include "glad/glad.h"
#include "string"
#include <iostream>
#include "glm/glm.hpp"

#define ASSERT(x) if(!(x)) __debugbreak();
#define GLCall(x) Utils::glClearError();\
    x;\
    ASSERT(Utils::glLogCall(#x, __FILE__, __LINE__))

class Utils
{
public:
    static void glClearError();
    static bool glLogCall(const char* function, const char* file, int line);
    static void checkShaderCompileStatus(GLuint shader);
    static void checkProgramLinkStatus(GLuint program);
    static glm::vec2 worldToScreen(const glm::vec3& worldPos, const glm::mat4& projMat, const glm::mat4& viewMat, float screenWidth, float screenHeight);
};


#endif //SIMPLEVOXEL_UTILS_H
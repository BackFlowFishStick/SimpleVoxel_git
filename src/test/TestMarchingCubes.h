//
// Created by Danmeng on 2026/4/3.
//

#ifndef SIMPLEVOXEL_TESTMARCHINGCUBES_H
#define SIMPLEVOXEL_TESTMARCHINGCUBES_H
#pragma once
#include <stack>
#include "Test.h"
#include "Utils.h"
#include "Shader.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "MarchingCube.h"
#include "VertexArray.h"
#include "GLFW/glfw3.h"
#include "imgui_impl_glfw.h"
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#define GLM_ENABLE_EXPERIMENTAL


#include "glm/ext/matrix_transform.hpp"

class TestMarchingCubes : public test::Test
{
public:
    TestMarchingCubes(GLFWwindow* window);
    ~TestMarchingCubes() override;
    void onUpdate(float deltaTime) override;
    void onRender() override;
    void onImGuiRender() override;
private:
    MarchingCube* m_mc;
    Shader* m_shader;
    IndexBuffer** m_ibArr;
    VertexBuffer** m_vbArr;
    VertexBufferLayout** m_vblArr;
    VertexArray** m_vaArr;
    glm::mat4 m_viewMat, m_modelMat;
    float m_cameraSpeed;
    unsigned int* m_numIndicesArr;
    GLFWwindow* m_window;

private:
    void onKeyPress(float deltaTime);
    void onMouseClick(const double& x, const double& y);
    void performRayPicking(const double& xpos, const double& ypos);
    bool detectRayTriangleIntersect(const glm::vec3& origin, const glm::vec3& dir, const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2, float& t) const;
    static double m_yaw;
    static double m_pitch;
    static float m_fov;
    static glm::vec3 m_cameraPos;
    static glm::vec3 m_cameraFront;
    static glm::vec3 m_cameraUp;
    static bool m_firstMouse;
    static double m_lastX, m_lastY;
    static glm::mat4 m_projMat;
    std::stack<glm::mat4> m_matStack;
    glm::mat4 m_mat0, m_mat1;
    float currentRad;
    bool m_lineMode;
    bool m_isMouseClicked, m_isModelRotate;
    ImVec2 m_leftTextSize, m_rightTextSize;
    double m_clickX, m_clickY;
    int m_selectedTriangle = -1;
};

#endif //SIMPLEVOXEL_TESTMARCHINGCUBES_H
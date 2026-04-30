//
// Created by Danmeng on 2026/4/3.
//

#include "TestMarchingCubes.h"
#include "glm/ext/matrix_clip_space.hpp"

double TestMarchingCubes::m_yaw = 0.0;
double TestMarchingCubes::m_pitch = 0.0;
double TestMarchingCubes::m_lastX = 0.0;
double TestMarchingCubes::m_lastY = 0.0;

float TestMarchingCubes::m_fov = 0.0f;

glm::vec3 TestMarchingCubes::m_cameraPos = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 TestMarchingCubes::m_cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 TestMarchingCubes::m_cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

bool TestMarchingCubes::m_firstMouse = true;

glm::mat4 TestMarchingCubes::m_projMat = glm::mat4();

const char* leftBarText = "MarchingCubes直接生成的粗模";
const char* rightBarText = "Isotropic Remeshing后的模型";

TestMarchingCubes::TestMarchingCubes(GLFWwindow* window)
{
    currentRad = 0.0f;

    m_mc = new MarchingCube();

    auto targetEdgeLength = m_mc->getAverageEdgeLength();

    std::vector<float> vertices = {};
    for (auto& v : m_mc->getVertices())
    {
        vertices.push_back(v.position.x);
        vertices.push_back(v.position.y);
        vertices.push_back(v.position.z);
        vertices.push_back(v.normal.x);
        vertices.push_back(v.normal.y);
        vertices.push_back(v.normal.z);
    }
    std::vector<unsigned int> indices = {};
    for (auto i : m_mc->getIndices())
    {
        indices.push_back(i);
    }

    m_numIndicesArr = new unsigned int[2];

    m_numIndicesArr[0] = static_cast<unsigned int>(indices.size());

    m_vaArr = new VertexArray*[2];

    m_vaArr[0] = new VertexArray();
    m_vaArr[0]->bind();

    m_vbArr = new VertexBuffer*[2];
    m_vbArr[0] = new VertexBuffer(vertices.data(), vertices.size() * sizeof(float));

    m_ibArr = new IndexBuffer*[2];
    m_ibArr[0] = new IndexBuffer(indices.data(), indices.size());

    m_vblArr = new VertexBufferLayout*[2];
    m_vblArr[0] = new VertexBufferLayout();
    m_vblArr[0]->push<float>(3);
    m_vblArr[0]->push<float>(3);

    m_vaArr[0]->AddBuffer(*m_vbArr[0], *m_vblArr[0]);

    m_cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    m_cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    m_cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

    m_viewMat = glm::lookAt(m_cameraPos, m_cameraPos + m_cameraFront, m_cameraUp);

    auto modelMat = glm::mat4(1.0f);
    m_matStack.push(modelMat);
    // 平移变换
    auto currentModelMat = m_matStack.top();
    currentModelMat = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 0.f, 0.f));
    m_matStack.push(currentModelMat);
    // 旋转变换
    currentModelMat = glm::rotate(m_matStack.top(), glm::radians(0.f), glm::vec3(0.f, 1.f, 0.f));
    m_matStack.push(currentModelMat);
    // 缩放变换
    currentModelMat = glm::scale(m_matStack.top(), glm::vec3(1.0f, 1.0f, 1.0f));
    m_matStack.push(currentModelMat);
    // 法线变换
    auto normalMatrix = glm::transpose(glm::inverse(glm::mat3(m_matStack.top())));
    m_matStack.push(m_matStack.top());
    // mv变换
    currentModelMat = m_viewMat * m_matStack.top();
    m_matStack.push(currentModelMat);

    // auto mvMat = m_viewMat * m_modelMat;

    auto aspect = 1080.f / 720.f;
    m_projMat = glm::perspective(glm::radians(60.0f), aspect, 0.1f, 1000.0f);

    m_mc->isotropicalRemesh(targetEdgeLength, 10);

    vertices.clear();
    indices.clear();

    for (auto& v : m_mc->getVertices())
    {
        vertices.push_back(v.position.x);
        vertices.push_back(v.position.y);
        vertices.push_back(v.position.z);
        vertices.push_back(v.normal.x);
        vertices.push_back(v.normal.y);
        vertices.push_back(v.normal.z);
    }

    for (auto i : m_mc->getIndices())
    {
        indices.push_back(i);
    }

    m_numIndicesArr[1] = static_cast<unsigned int>(indices.size());

    m_vaArr[1] = new VertexArray();
    m_vaArr[1]->bind();

    m_vbArr[1] = new VertexBuffer(vertices.data(), vertices.size() * sizeof(float));

    m_ibArr[1] = new IndexBuffer(indices.data(), indices.size());

    m_vblArr[1] = new VertexBufferLayout();
    m_vblArr[1]->push<float>(3);
    m_vblArr[1]->push<float>(3);

    m_vaArr[1]->AddBuffer(*m_vbArr[1], *m_vblArr[1]);

    m_shader = new Shader("resources/shaders/SimpleTestShader.glsl");

    m_shader->bind();
    m_shader->setUniformMat4f("proj_Matrix", m_projMat);
    // m_shader->setUniformMat4f("mv_Matrix", mvMat);
    m_shader->setUniformMat4f("mv_Matrix", m_matStack.top());
    m_shader->setUniform3f("lightColor", 1.f, 1.f, 1.f);
    m_shader->setUniform3f("objectColor", 1.0f, 0.0f, 0.0f);
    m_shader->setUniform3f("viewPos", 0.f, 0.f, 0.f);
    m_shader->setUniform3f("lightPos", 100.0f, 200.0f, 112.f);

    while (m_matStack.size() > 1)
    {
        m_matStack.pop();
    }

    m_window = window;

    // 隐藏光标并捕捉它
    // glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    m_yaw = 0;
    m_pitch = 0;

    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.Fonts->AddFontFromFileTTF("../resources/fonts/msyhl.ttc", 18.0f, nullptr, io.Fonts->GetGlyphRangesChineseFull());

    m_lineMode = false;
    m_isModelRotate = true;
    // GLCall(glfwSetCursorPosCallback(m_window, mouseCallback));
    // GLCall(glfwSetScrollCallback(m_window, scrollCallback));
}

TestMarchingCubes::~TestMarchingCubes()
{
    for (int i = 0; i < 2; ++i)
    {
        delete m_vaArr[i];
        delete m_vbArr[i];
        delete m_ibArr[i];
        delete m_vblArr[i];
    }
    delete []m_vaArr;
    delete []m_vbArr;
    delete []m_ibArr;
    delete []m_vblArr;
    delete m_shader;
    delete m_mc;
    m_window = nullptr;
}

void TestMarchingCubes::onKeyPress(float deltaTime)
{
    if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(m_window, GL_TRUE);
    }
}

void TestMarchingCubes::onMouseClick(const double& x, const double& y)
{
    ImGuiIO& io = ImGui::GetIO();
    bool leftBtnDown = glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;

    if (leftBtnDown && !io.WantCaptureMouse)
    {
        performRayPicking(x, y);
    }
    else
    {
        m_selectedTriangle = -1;
    }
}

void TestMarchingCubes::performRayPicking(const double& xpos, const double& ypos)
{
    if (m_mc == nullptr) return;

    int width, height;
    glfwGetWindowSize(m_window, &width, &height);

    float ndcX = (2.0f * xpos) / width - 1.0f;
    float ndcY = 1.0f - (2.0f * ypos) / height;

    glm::vec4 nearPoint(ndcX, ndcY, -1.0f, 1.0f);
    glm::vec4 farPoint(ndcX, ndcY, 1.0f, 1.0f);

    glm::mat4 invProj = glm::inverse(m_projMat);
    glm::vec4 nearView = invProj * nearPoint;
    nearView /= nearView.w;
    glm::vec4 farView = invProj * farPoint;
    farView /= farView.w;

    glm::vec3 rayOriginView(0.0f);
    glm::vec3 rayDirView   = glm::normalize(glm::vec3(farView - nearView));

    glm::mat4 invMV = glm::inverse(m_mat1);

    glm::vec4 localOrigin4 = invMV * glm::vec4(rayOriginView, 1.0f);
    localOrigin4 /= localOrigin4.w;
    glm::vec3 localRayOrigin(localOrigin4);
    glm::vec3 localRayDir   = glm::normalize(glm::vec3(invMV * glm::vec4(rayDirView, 0.0f)));

    const auto& vertices = m_mc->getVertices();
    const auto& indices  = m_mc->getIndices();

    float minT = INFINITY;
    int   hitTri = -1;
    int   triCount = indices.size() / 3;

    for (int i = 0; i < triCount; ++i)
    {
        auto v0 = vertices[indices[3 * i    ]].position;
        auto v1 = vertices[indices[3 * i + 1]].position;
        auto v2 = vertices[indices[3 * i + 2]].position;

        float t;
        if (detectRayTriangleIntersect(localRayOrigin, localRayDir, v0, v1, v2, t))
        {
            if (t < minT)
            {
                minT   = t;
                hitTri = i;
            }
        }
    }

    m_selectedTriangle = hitTri;
}

bool TestMarchingCubes::detectRayTriangleIntersect(const glm::vec3& origin, const glm::vec3& dir, const glm::vec3& v0,
    const glm::vec3& v1, const glm::vec3& v2, float& t) const
{
    constexpr float EPSILON = 1e-6f;
    glm::vec3 edge1 = v1 - v0;
    glm::vec3 edge2 = v2 - v0;
    glm::vec3 h = glm::cross(dir, edge2);
    float a = glm::dot(edge1, h);
    if (fabs(a) < EPSILON) return false;  
    float f = 1.0f / a;
    glm::vec3 s = origin - v0;
    float u = f * glm::dot(s, h);
    if (u < 0.0f || u > 1.0f) return false;
    glm::vec3 q = glm::cross(s, edge1);
    float v = f * glm::dot(dir, q);
    if (v < 0.0f || u + v > 1.0f) return false;
    t = f * glm::dot(edge2, q);
    return t > 0.0f;
}

void TestMarchingCubes::onUpdate(float deltaTime)
{
    if (m_window == nullptr) return;

    onKeyPress(deltaTime);

    if (m_isModelRotate)
    {
        currentRad += 0.5f * deltaTime;
    }

    if (currentRad > 360.f) currentRad = 0.f;
    m_viewMat = glm::lookAt(m_cameraPos, m_cameraPos + m_cameraFront, m_cameraUp);

    m_matStack.push(m_matStack.top());
    m_matStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(-2.f, 0.5f, -2.f));
    m_matStack.push(m_matStack.top());
    m_matStack.top() *= glm::rotate(glm::mat4(1.0f), currentRad, glm::vec3(0.f, 1.f, 0.f));
    m_matStack.push(m_matStack.top());
    m_matStack.top() *= glm::scale(glm::mat4(1.0f), glm::vec3(1.f, 1.f, 1.f));
    m_matStack.push(m_matStack.top());
    m_matStack.top() = m_viewMat * m_matStack.top();
    m_matStack.push(m_matStack.top());
    m_mat0 = m_matStack.top();
    // auto mvMat = m_viewMat * m_modelMat;
    while (m_matStack.size() > 1)
    {
        m_matStack.pop();
    }

    m_matStack.push(m_matStack.top());
    m_matStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(2.f, 0.5f, -2.f));
    m_matStack.push(m_matStack.top());
    m_matStack.top() *= glm::rotate(glm::mat4(1.0f), currentRad, glm::vec3(0.f, 1.f, 0.f));
    m_matStack.push(m_matStack.top());
    m_matStack.top() *= glm::scale(glm::mat4(1.0f), glm::vec3(1.f, 1.f, 1.f));
    m_matStack.push(m_matStack.top());
    m_matStack.top() = m_viewMat * m_matStack.top();
    m_matStack.push(m_matStack.top());
    m_mat1 = m_matStack.top();

    glfwGetCursorPos(m_window, &m_clickX, &m_clickY);
    onMouseClick(m_clickX, m_clickY);

    while (m_matStack.size() > 1)
    {
        m_matStack.pop();
    }
}

void TestMarchingCubes::onRender()
{
    if (m_lineMode)
    {
        GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
    }
    else
    {
        GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
    }

    // 设置光源位置，由 viewMat X lightPos得到相对的观察空间的光源位置
    glm::vec3 lightPosWorld = glm::vec3(100.0f, 200.0f, 112.0f);
    glm::vec4 lightPosView = m_viewMat * glm::vec4(lightPosWorld, 1.0f);
    m_shader->setUniform3f("lightPos", lightPosView.x, lightPosView.y, lightPosView.z);

    m_vaArr[0]->bind();
    m_vbArr[0]->bind();
    m_ibArr[0]->bind();
    m_shader->setUniformMat4f("proj_Matrix", m_projMat);
    m_shader->setUniformMat4f("mv_Matrix", m_mat0);
    m_shader->setUniform1i("SelectedPrimitive", -1);
    GLCall(glDrawElements(GL_TRIANGLES, m_numIndicesArr[0], GL_UNSIGNED_INT, nullptr));

    m_vaArr[1]->bind();
    m_vbArr[1]->bind();
    m_ibArr[1]->bind();
    m_shader->setUniformMat4f("proj_Matrix", m_projMat);
    m_shader->setUniformMat4f("mv_Matrix", m_mat1);
    m_shader->setUniform1i("SelectedPrimitive", m_selectedTriangle);
    GLCall(glDrawElements(GL_TRIANGLES, m_numIndicesArr[1], GL_UNSIGNED_INT, nullptr));
}

void TestMarchingCubes::onImGuiRender()
{
    auto textBarFlag =
        ImGuiWindowFlags_NoMove
    |   ImGuiWindowFlags_NoTitleBar
    |   ImGuiWindowFlags_NoResize
    |   ImGuiWindowFlags_NoCollapse
    |   ImGuiWindowFlags_NoBackground;

    auto controlPanelFlag =
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoCollapse;
    {
        ImGui::Begin("TextBlock_0", nullptr, textBarFlag);

        auto viewport = ImGui::GetMainViewport();
        float screenWidth = viewport->Size.x;
        float screenHeight = viewport->Size.y;

        auto windowPos = Utils::worldToScreen(glm::vec3(-3.f, 1.75f, -2.f) , m_projMat, m_viewMat, screenWidth, screenHeight);

        ImGui::SetWindowPos(ImVec2(windowPos.x, windowPos.y - 10));

        ImGui::SetWindowSize(ImVec2(230, 40));

        m_leftTextSize = ImGui::CalcTextSize(leftBarText);
        // rightTextSize = ImGui::CalcTextSize(rightBarText);

        ImGui::SetCursorPosX((230.f - m_leftTextSize.x) * 0.5f);
        // ImGui::Text("The model is directly generated by MarchingCubes");

        ImGui::Text("%s", leftBarText);

        ImGui::End();

        ImGui::Begin("TextBlock_1", nullptr, textBarFlag);

        windowPos = Utils::worldToScreen(glm::vec3(1.25f, 1.75f, -2.f), m_projMat, m_viewMat, screenWidth, screenHeight);

        ImGui::SetWindowPos(ImVec2(windowPos.x, windowPos.y - 10));

        ImGui::SetWindowSize(ImVec2(230, 40));

        m_rightTextSize = ImGui::CalcTextSize(rightBarText);

        ImGui::SetCursorPosX((230.f - m_rightTextSize.x) * 0.5f);

        ImGui::Text("%s", rightBarText);

        ImGui::End();

        float controlWidth = 500.f;

        ImGui::SetNextWindowPos(
            ImVec2(controlWidth * 0.5f, screenHeight),
            ImGuiCond_Always,
            ImVec2(0.5f, 1.0f)
            );

        ImGui::SetNextWindowSize(ImVec2(controlWidth, 120));

        ImGui::Begin("控制台", nullptr, controlPanelFlag);

        ImGui::Checkbox("启用线框模式", &m_lineMode);

        ImGui::Checkbox("模型旋转", &m_isModelRotate);
        ImGui::End();

    }
}


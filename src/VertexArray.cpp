//
// Created by Danmeng on 2026/3/24.
//

#include "VertexArray.h"

VertexArray::VertexArray() : m_VAO(0)
{
    GLCall(glGenVertexArrays(1, &m_VAO));
}

VertexArray::~VertexArray()
{
    GLCall(glDeleteVertexArrays(1, &m_VAO));
}

void VertexArray::bind() const
{
    GLCall(glBindVertexArray(m_VAO));
}

void VertexArray::unbind() const
{
    GLCall(glBindVertexArray(0));
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
    vb.bind();
    const auto& elements = layout.getElements();

    // 调试输出
    // std::cout << "Adding buffer with " << elements.size() << " elements" << std::endl;

    GLint maxAttribs;

    unsigned int offset = 0;
    for (unsigned int i = 0; i < elements.size(); ++i)
    {
        auto& element = elements[i];

        // glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxAttribs);
        // if (i >= static_cast<unsigned int>(maxAttribs)) {
        //     std::cerr << "Error: Vertex attribute index " << i
        //               << " exceeds maximum (" << maxAttribs << ")" << std::endl;
        //     break;
        // }
        //
        // // 调试输出每个元素的信息
        // std::cout << "Element " << i << ": count=" << element.count
        //           << ", type=" << element.type
        //           << ", offset=" << offset << std::endl;

        GLCall(glEnableVertexAttribArray(i));
        GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.getStride(), (const void*)offset));
        offset += element.count * VertexBufferElement::getSizeOfType(element.type);
    }
}
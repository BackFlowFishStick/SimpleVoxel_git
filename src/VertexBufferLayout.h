//
// Created by Danmeng on 2026/3/24.
//

#ifndef SIMPLEVOXEL_VERTEXBUFFERLAYOUT_H
#define SIMPLEVOXEL_VERTEXBUFFERLAYOUT_H

#pragma once

#include <vector>
#include "Utils.h"
#include "glad/glad.h"

struct VertexBufferElement
{
    unsigned int type;
    unsigned int count;
    unsigned char normalized;

    static unsigned int getSizeOfType(unsigned int type)
    {
        switch (type)
        {
        case GL_FLOAT:
            return sizeof(GLfloat);

        case GL_UNSIGNED_INT:
            return sizeof(GLuint);

        case GL_UNSIGNED_BYTE:
            return sizeof(GLubyte);
        }

        ASSERT(false);
        return 0;
    }
};

class VertexBufferLayout
{
public:
    VertexBufferLayout()
    : m_Stride(0) {}
    ~VertexBufferLayout() = default;

    template<typename T>
    void push(int count)
    {
        static_assert(false);
    }

    [[nodiscard]] inline std::vector<VertexBufferElement> getElements() const { return m_Elements; }
    [[nodiscard]] inline unsigned int getStride() const { return m_Stride;}


private:
    std::vector<VertexBufferElement> m_Elements;
    unsigned int m_Stride;
};

    template<>
    inline void VertexBufferLayout::push<float>(int count)
    {
        m_Elements.push_back( {GL_FLOAT, static_cast<unsigned int>(count), GL_FALSE});
        m_Stride += count * VertexBufferElement::getSizeOfType(GL_FLOAT);
    }

    template<>
    inline void VertexBufferLayout::push<unsigned int>(int count)
    {
        m_Elements.push_back({GL_UNSIGNED_INT, static_cast<unsigned int>(count), GL_FALSE});
        m_Stride += count * VertexBufferElement::getSizeOfType(GL_UNSIGNED_INT);
    }

    template<>
    inline void VertexBufferLayout::push<unsigned char>(int count)
    {
        m_Elements.push_back({GL_UNSIGNED_BYTE, static_cast<unsigned int>(count), GL_TRUE });
        m_Stride += count * VertexBufferElement::getSizeOfType(GL_UNSIGNED_BYTE);
    }




#endif //SIMPLEVOXEL_VERTEXBUFFERLAYOUT_H
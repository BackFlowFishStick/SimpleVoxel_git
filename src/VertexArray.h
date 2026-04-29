//
// Created by Danmeng on 2026/3/24.
//

#ifndef SIMPLEVOXEL_VERTEXARRAY_H
#define SIMPLEVOXEL_VERTEXARRAY_H

#pragma once
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

class VertexArray
{
public:
    VertexArray();
    ~VertexArray();
    void bind() const;
    void unbind() const;
    void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

private:
    unsigned int m_VAO;
};


#endif //SIMPLEVOXEL_VERTEXARRAY_H
//
// Created by Danmeng on 2026/3/24.
//

#ifndef SIMPLEVOXEL_VERTEXBUFFER_H
#define SIMPLEVOXEL_VERTEXBUFFER_H

#pragma once
#include "Utils.h"

class VertexBuffer
{
private:
private:
    unsigned int m_RendererID;

public:
    VertexBuffer(const void* data, unsigned int size);
    ~VertexBuffer();
    void bind() const;
    void unbind() const;
};


#endif //SIMPLEVOXEL_VERTEXBUFFER_H
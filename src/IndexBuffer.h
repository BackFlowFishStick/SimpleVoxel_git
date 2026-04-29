//
// Created by Danmeng on 2026/3/24.
//

#ifndef SIMPLEVOXEL_INDEXBUFFER_H
#define SIMPLEVOXEL_INDEXBUFFER_H

#pragma once
#include "Utils.h"


class IndexBuffer
{
private:
    unsigned int m_RendererID;
    unsigned int m_Count;

public:
    IndexBuffer(const void* data, unsigned int count);
    ~IndexBuffer();
    void bind() const;
    void unbind() const;
    [[nodiscard]]unsigned int getCount() const;
};


#endif //SIMPLEVOXEL_INDEXBUFFER_H
//
// Created by Danmeng on 2026/3/24.
//
#ifndef SIMPLEVOXEL_TEXTURE_H
#define SIMPLEVOXEL_TEXTURE_H

#pragma once
#include "Utils.h"
#include <iostream>
#include "stb/stb_image.h"

class Texture
{
private:
    unsigned int m_textureID;
    std::string m_fileName;
    unsigned char* m_localBuffer;
    int m_width, m_height;
    int m_bpp;

public:
    Texture(const std::string & fileName);
    ~Texture();

    void bind(unsigned int slot = 0) const;
    void unbind();
    [[nodiscard]] inline unsigned int getWidth() const { return m_width; }
    [[nodiscard]] inline unsigned int getHeight() const { return m_height; }
};


#endif //SIMPLEVOXEL_TEXTURE_H
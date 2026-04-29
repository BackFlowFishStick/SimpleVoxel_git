//
// Created by Danmeng on 2026/3/24.
//

#include "Texture.h"

Texture::Texture(const std::string& fileName) :
m_fileName(fileName),
m_localBuffer(nullptr),
m_width(0),
m_height(0),
m_textureID(0)
{
    stbi_set_flip_vertically_on_load(1);
    m_localBuffer = stbi_load(fileName.c_str(), &m_width, &m_height, &m_bpp, 4);

    GLCall(glGenTextures(1, &m_textureID));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_textureID));

    GLCall(glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
    GLCall(glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

    GLCall(glTexImage2D(GL_TEXTURE_2D, 0 , GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_localBuffer));
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));

    if (m_localBuffer != nullptr)
    {
        stbi_image_free(m_localBuffer);
    }
}

Texture::~Texture()
{
    GLCall(glDeleteTextures(1, &m_textureID));
}

void Texture::bind(unsigned int slot) const
{
    GLCall(glActiveTexture(GL_TEXTURE0 + slot));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_textureID));
}

void Texture::unbind()
{
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

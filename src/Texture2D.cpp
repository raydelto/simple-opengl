#include "Texture2D.h"
#include <iostream>
#include <cassert>
#include "TgaLoader.h"

Texture2D::Texture2D()
    : mTexture(0)
{
}

Texture2D::~Texture2D()
{
    glDeleteTextures(1, &mTexture);
}

bool Texture2D::loadTexture(const string &fileName, bool generateMipMaps)
{
    unsigned short width, height;
    unsigned char *imageData = nullptr;
    
    if (!LoadTGA(fileName.c_str(), imageData, width, height))
    {
        std::cerr << "Error loading texture '" << fileName << "'" << std::endl;
        return false;
    }

    glGenTextures(1, &mTexture);
    glBindTexture(GL_TEXTURE_2D, mTexture);

    // Set the texture wrapping/filtering options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // TGA files are in BGR format
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, imageData);

    if (generateMipMaps)
    {
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    free(imageData);
    glBindTexture(GL_TEXTURE_2D, 0);

    return true;
}

void Texture2D::bind(GLuint texUnit)
{
    assert(texUnit >= 0 && texUnit < 32);

    glActiveTexture(GL_TEXTURE0 + texUnit);
    glBindTexture(GL_TEXTURE_2D, mTexture);
}

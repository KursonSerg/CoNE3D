#include <CTexture.h>

#include <CLogger.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

CTexture::CTexture(const std::string &filename)
    : _target(GL_TEXTURE_2D)
    , _texture(0)
    , _format(GL_RGB)
{
    unsigned char *image = stbi_load(filename.c_str(), &_width, &_height, &_components, STBI_default);

    if (!image)
        throw std::runtime_error( utils::ws2s(utils::CFormat(L"Failed to load texture '%%'") << filename) );

    glGenTextures(1, &_texture);
    bind();

    glTexParameteri(_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(_target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    switch (_components)
    {
    case STBI_rgb:
        _format = GL_RGB;
        break;
    case STBI_rgb_alpha:
        _format = GL_RGBA;
        break;
    default:
        break;
    }

    glTexImage2D(_target, 0, _format, _width, _height, 0, _format, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(_target);

    stbi_image_free(image);
    unbind();
}

CTexture::~CTexture()
{
    glDeleteTextures(1, &_texture);
}
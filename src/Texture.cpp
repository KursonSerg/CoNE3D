#include <Texture.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

CTexture::CTexture(const std::string &filename)
    : _target(GL_TEXTURE_2D)
    , _texture(0)
    , _internalformat(GL_SRGB)
    , _format(GL_RGB)
    , _width(0)
    , _height(0)
    , _components(0)
{
    stbi_set_flip_vertically_on_load(true);
    unsigned char *image = stbi_load(filename.c_str(), &_width, &_height, &_components, STBI_default);

    if (!image)
        throw std::runtime_error( utils::ws2s(utils::CFormat(L"Failed to load texture '%%'") << filename) );

    glGenTextures(1, &_texture);
    bind();

    switch (_components)
    {
    case STBI_grey:
        _internalformat = GL_SLUMINANCE;
        _format = GL_LUMINANCE;
        break;
    case STBI_grey_alpha:
        _internalformat = GL_SLUMINANCE_ALPHA;
        _format = GL_LUMINANCE_ALPHA;
        break;
    case STBI_rgb:
        _internalformat = GL_SRGB;
        _format = GL_RGB;
        break;
    case STBI_rgb_alpha:
        _internalformat = GL_SRGB_ALPHA;
        _format = GL_RGBA;
        break;
    default:
        break;
    }

    glTexImage2D(_target, 0, _internalformat, _width, _height, 0, _format, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(_target);

    glTexParameteri(_target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    stbi_image_free(image);
    unbind();
}

CTexture::~CTexture()
{
    glDeleteTextures(1, &_texture);
}

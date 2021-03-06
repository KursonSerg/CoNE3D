#ifndef TEXTURE_H
#define TEXTURE_H

#include <Common.h>

class CTexture
{
public:
    CTexture(const std::string &filename);
    ~CTexture();

    void bind()
    {
        glBindTexture(_target, _texture);
    }

    void unbind()
    {
        glBindTexture(_target, 0);
    }

    int width() const
    {
        return _width;
    }

    int height() const
    {
        return _height;
    }

private:
    GLenum _target;
    GLuint _texture;
    GLint  _internalformat;
    GLenum _format;

    int _width;
    int _height;
    int _components;
};

#endif // TEXTURE_H

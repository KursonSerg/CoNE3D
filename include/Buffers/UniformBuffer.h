#ifndef UNIFORMBUFFER_H
#define UNIFORMBUFFER_H

#include <Common.h>
#include <Bindings.glsl>

class CUniformBuffer
{
public:
    CUniformBuffer(GLsizeiptr size, GLuint binding);
    virtual ~CUniformBuffer();

    void bind() const;
    void unbind() const;

    void setData(const void *data) const;
    void setSubData(GLintptr offset, GLsizeiptr size, const void *data) const;

private:
    GLuint _ubo;
    GLuint _binding;
    GLsizeiptr _size;

    void reset() const;
};

#endif // UNIFORMBUFFER_H

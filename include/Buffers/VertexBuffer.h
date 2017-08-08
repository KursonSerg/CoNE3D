#ifndef VERTEXBUFFER_H
#define VERTEXBUFFER_H

#include <Common.h>

class CVertexBuffer final
{
public:
    CVertexBuffer();
    ~CVertexBuffer();

    void bind() const;
    void unbind() const;

    void updateData(GLsizeiptr size, const void *data, GLenum usage);
    void updateAttrib(GLuint index, GLint size, GLsizei stride, GLuint offset);

private:
    GLuint _vbo;
};

#endif // VERTEXBUFFER_H
